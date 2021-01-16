#include "handheld_emulator.hpp"
#include "spdlog/spdlog.h"
#include <thread>

#include <SFML/Window/Keyboard.hpp>

void handheld_emulator_c::draw_buffer(graphx_c &graphx)
{
	for (uint8_t x = 0; x < graphx_c::width; ++x) {

		for (uint8_t y = 0; y < graphx_c::height; ++y) {
			m_image.setPixel(x, y,
			                 graphx.get_pixel(x, y)
			                     ? sf::Color::Black
			                     : sf::Color::White);
		}
	}
}

void handheld_emulator_c::get_buttons(avrboy_payload::buttons_s &buttons)
{

	buttons.states = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		buttons.states |= (1 << avrboy_payload::button_e::BUTTON_UP);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		buttons.states |= (1 << avrboy_payload::button_e::BUTTON_DOWN);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		buttons.states |= (1 << avrboy_payload::button_e::BUTTON_LEFT);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		buttons.states |= (1 << avrboy_payload::button_e::BUTTON_RIGHT);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		buttons.states |= (1 << avrboy_payload::button_e::BUTTON_B);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
		buttons.states |= (1 << avrboy_payload::button_e::BUTTON_A);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		buttons.states |= (1 << avrboy_payload::button_e::BUTTON_START);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
		buttons.states |=
		    (1 << avrboy_payload::button_e::BUTTON_SELECT);
	}
}

void handheld_emulator_c::do_work()
{
	waitfor_instructions();
}
