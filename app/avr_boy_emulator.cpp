#include "avr_boy_emulator.hpp"
#include "spdlog/spdlog.h"
#include <thread>
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
}

void avr_boy_emulator_c::do_work()
{
	m_handheld->waitfor_instructions();
}
