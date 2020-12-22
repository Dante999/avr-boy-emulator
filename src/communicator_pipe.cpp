#include "avr-boy-emulator/communicator_pipe.hpp"

#include "spdlog/spdlog.h"

void communicator_pipe_c::cartridge_init()
{
	spdlog::info("Initializing pipe as cartridge");
	m_handheld_to_cartridge.open_pipe(fifo_pipe_c::mode_e::READ);
	m_cartridge_to_handheld.open_pipe(fifo_pipe_c::mode_e::WRITE);
}

void communicator_pipe_c::handheld_init()
{
	spdlog::info("Initializing pipe as handheld");
	m_handheld_to_cartridge.open_pipe(fifo_pipe_c::mode_e::WRITE);
	m_cartridge_to_handheld.open_pipe(fifo_pipe_c::mode_e::READ);
}

void communicator_pipe_c::cartridge_send_byte(uint8_t byte)
{
	spdlog::debug("Cartridge send byte: {}", byte);
	m_cartridge_to_handheld.write_bytes(&byte, 1);
}

void communicator_pipe_c::handheld_send_byte(uint8_t byte)
{
	spdlog::debug("Handheld send byte: {}", byte);
	m_handheld_to_cartridge.write_bytes(&byte, 1);
}

uint8_t communicator_pipe_c::cartridge_receive_byte()
{
	uint8_t result = 0;
	m_handheld_to_cartridge.read_bytes(&result, 1);
	spdlog::debug("Cartridge received byte: {}", result);
	return result;
}

uint8_t communicator_pipe_c::handheld_receive_byte()
{
	uint8_t result = 0;
	m_cartridge_to_handheld.read_bytes(&result, 1);
	spdlog::debug("Handheld received byte: {}", result);
	return result;
}
