#include <memory>
#include <stdio.h>
#include <unistd.h> // sleep()

#include "avr-boy-core/cartridge.hpp"
#include "avr-boy-emulator/communicator.hpp"
#include "avr-boy-emulator/communicator_pipe.hpp"

#include "spdlog/spdlog.h"

namespace {
static std::unique_ptr<communicator_c> m_communicator;
}

int main()
{
	spdlog::set_level(spdlog::level::debug);
	spdlog::info("Hello World!");

	m_communicator = std::make_unique<communicator_pipe_c>(
	    communicator_c::role_e::CARTRIDGE);
	m_communicator->init();

	auto cartridge = std::make_unique<cartridge_c>(
	    [](uint8_t byte) { m_communicator->send_byte(byte); },
	    []() -> uint8_t { return m_communicator->receive_byte(); });

	cartridge->sync_with_handheld();

	spdlog::info("Configuration done, starting with main loop");

	while (1) {
		static bool color_on = true;

		for (uint8_t y = 0; y < 64; ++y) {

			for (uint8_t x = 0; x < 127; ++x) {

				auto result = cartridge->set_pixel(
				    x, y,
				    color_on ? color_dao_e::COLOR_BLACK
				             : color_dao_e::COLOR_WHITE);

				if (result != result_e::RESULT_OK) {
					spdlog::warn(
					    "protocol parsing result is {}",
					    result);
				}

				cartridge->draw_buffer();
				usleep(100);
			}
		}

		color_on = !color_on;
	}
}
