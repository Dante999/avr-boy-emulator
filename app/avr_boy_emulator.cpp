#include "avr_boy_emulator.hpp"
#include "spdlog/spdlog.h"
#include <thread>

#include <SFML/Window/Keyboard.hpp>
/**
 * must be static, otherwise oldschool function pointers did did not
 * work
 */
namespace {
static sf::Image *m_image;

static std::unique_ptr<handheld_c>     m_handheld;
static std::unique_ptr<communicator_c> m_communicator;

static void cb_draw_screen_buffer(graphx_c &gfx)
{
	for (uint8_t x = 0; x < graphx_c::width; ++x) {

		for (uint8_t y = 0; y < graphx_c::height; ++y) {
			m_image->setPixel(x, y,
			                  gfx.get_pixel(x, y)
			                      ? sf::Color::Black
			                      : sf::Color::White);
		}
	}
}

static void cb_get_buttons(avrboy_payload::buttons_s &buttons)
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

} // namespace

avr_boy_emulator_c::avr_boy_emulator_c(sf::Image *img)
{
	m_image = img;

	m_communicator = std::make_unique<communicator_pipe_c>(
	    communicator_c::role_e::HANDHELD);
	m_communicator->init();

	m_handheld = std::make_unique<handheld_c>(
	    [](uint8_t byte) { m_communicator->send_byte(byte); },
	    []() -> uint8_t { return m_communicator->receive_byte(); });

	m_handheld->set_draw_buffer_callback(
	    [](graphx_c &gfx) { cb_draw_screen_buffer(gfx); });
	m_handheld->set_get_buttons_callback(
	    [](avrboy_payload::buttons_s &btn) { cb_get_buttons(btn); });
}

void avr_boy_emulator_c::do_work()
{
	m_handheld->waitfor_instructions();
}
