#ifndef AVR_BOY_EMULATOR_H
#define AVR_BOY_EMULATOR_H

#include <memory>

#include <SFML/Graphics/Image.hpp>

#include "avr-boy-core/handheld.hpp"
#include "avr-boy-emulator/transceiver_pipe.hpp"

class handheld_emulator_c : private handheld_c {

private:
	transceiver_pipe_handheld m_transceiver;
	sf::Image &               m_image;

public:
	handheld_emulator_c(sf::Image &img) : m_image(img){};

	void do_work();

protected:
	void draw_buffer(graphx_c &graphx) override;
	void get_buttons(avrboy_payload::buttons_s &buttons) override;

	void send_byte(uint8_t byte) override
	{
		m_transceiver.send_byte(byte);
	}

	uint8_t receive_byte() override
	{
		return m_transceiver.receive_byte();
	}
};

#endif /* AVR_BOY_EMULATOR_H */
