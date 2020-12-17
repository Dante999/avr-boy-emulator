#ifndef COMMUNICATOR_PIPE_H
#define COMMUNICATOR_PIPE_H

#include "avr-boy-emulator/communicator.hpp"
#include "avr-boy-emulator/fifo_pipe.hpp"

class communicator_pipe_c : public communicator_c {

private:
	fifo_pipe_c m_handheld_to_cartridge;
	fifo_pipe_c m_cartridge_to_handheld;

protected:
	void    cartridge_init() override;
	void    cartridge_send_byte(uint8_t byte) override;
	uint8_t cartridge_receive_byte() override;

	void    handheld_init() override;
	void    handheld_send_byte(uint8_t byte) override;
	uint8_t handheld_receive_byte() override;

public:
	communicator_pipe_c(role_e role)
	    : communicator_c(role),
	      m_handheld_to_cartridge("/tmp/handheld_to_cartridge.fifo"),
	      m_cartridge_to_handheld("/tmp/cartridge_to_handheld.fifo")
	{
	}
	virtual ~communicator_pipe_c() = 0;
};

#endif /* COMMUNICATOR_PIPE_H */
