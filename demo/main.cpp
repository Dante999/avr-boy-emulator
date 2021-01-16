#include <memory>
#include <stdio.h>
#include <unistd.h> // sleep()
// ---
#include <chrono>
#include <thread>
// --
#include "ball.hpp"

#include "avr-boy-core/cartridge.hpp"
#include "avr-boy-core/graphx.hpp"

#include "avr-boy-emulator/communicator.hpp"
#include "avr-boy-emulator/communicator_pipe.hpp"

#include "spdlog/spdlog.h"

namespace {
static std::unique_ptr<communicator_c> m_communicator;
}

inline static void sleep_for_ms(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static void draw_sequencial(cartridge_c &cartridge, uint8_t x, uint8_t y,
                            const char *text, int delay_ms)
{
	uint8_t i = 0;

	while (*text != 0) {
		cartridge.set_char(x + i, y, *text);
		cartridge.draw_buffer();
		++text;
		i += 8;
		sleep_for_ms(delay_ms);
	}
}

static void demo_text(cartridge_c &cartridge)
{
	constexpr int text_delay_ms = 100;
	cartridge.clear_buffer();
	cartridge.draw_buffer();

	draw_sequencial(cartridge, 5, 10, "Hi there!", text_delay_ms);
	draw_sequencial(cartridge, 5, 20, "This is a text", text_delay_ms);
	draw_sequencial(cartridge, 5, 30, "demonstration", text_delay_ms);
}

static void demo_pixel(cartridge_c &cartridge)
{
	constexpr uint8_t draw_delay_ms = 20;
	cartridge.clear_buffer();
	cartridge.draw_buffer();

	uint8_t x_min = 0;
	uint8_t x_max = graphx_c::max_x;
	uint8_t y_min = 0;
	uint8_t y_max = graphx_c::max_y;

	for (uint8_t x = x_min; x <= x_max; ++x) {

		for (uint8_t y = y_min; y <= y_max; ++y) {

			if (y % 2 == 0) {
				cartridge.set_pixel(
				    x, y, avrboy_payload::color_e::COLOR_BLACK);
			}
		}

		cartridge.draw_buffer();
		sleep_for_ms(draw_delay_ms);
	}

	for (uint8_t y = y_min; y <= y_max; ++y) {

		for (uint8_t x = x_min; x <= x_max; ++x) {

			if (x % 2 == 0) {
				cartridge.set_pixel(
				    x, y, avrboy_payload::color_e::COLOR_BLACK);
			}
		}

		cartridge.draw_buffer();
		sleep_for_ms(draw_delay_ms);
	}
}

static void demo_bouncing_pixel(cartridge_c &cartridge)
{
	cartridge.clear_buffer();
	cartridge.draw_buffer();

	constexpr uint16_t max_bounces   = 300;
	constexpr uint8_t  draw_delay_ms = 20;

	uint8_t x_min = 0;
	uint8_t x_max = graphx_c::max_x;
	uint8_t y_min = 0;
	uint8_t y_max = graphx_c::max_y;

	ball_c ball(ball_c::direction_e::DIR_UPRIGHT, x_min, x_max, y_min,
	            y_max);

	ball.set_coordinates(2, 10);

	for (uint16_t i = 0; i < max_bounces; ++i) {

		cartridge.set_pixel(ball.get_x(), ball.get_y(),
		                    avrboy_payload::color_e::COLOR_WHITE);
		ball.bounce();
		cartridge.set_pixel(ball.get_x(), ball.get_y(),
		                    avrboy_payload::color_e::COLOR_BLACK);

		cartridge.draw_buffer();
		sleep_for_ms(draw_delay_ms);
	}
}

static void demo_buttons(cartridge_c &cartridge)
{
	using namespace avrboy_payload;

	cartridge.clear_buffer();
	cartridge.draw_buffer();

	constexpr uint16_t max_bounces   = 500;
	constexpr uint8_t  draw_delay_ms = 20;

	draw_sequencial(cartridge, 5, 5, "Press buttons", draw_delay_ms);

	cartridge.set_text(5, 20, "up    ");
	cartridge.set_text(5, 30, "down  ");
	cartridge.set_text(5, 40, "left  ");
	cartridge.set_text(5, 50, "right ");

	cartridge.set_text(75, 20, "A      ");
	cartridge.set_text(75, 30, "B      ");
	cartridge.set_text(75, 40, "start  ");
	cartridge.set_text(75, 50, "select ");
	cartridge.draw_buffer();

	auto stc = [](buttons_s buttons, button_e id) -> char {
		return (buttons.states & (1 << id)) ? '1' : '0';
	};

	avrboy_payload::buttons_s btns;

	for (uint16_t i = 0; i < max_bounces; ++i) {
		cartridge.get_buttons(btns);

		cartridge.set_char(40, 20, stc(btns, button_e::BUTTON_UP));
		cartridge.set_char(40, 30, stc(btns, button_e::BUTTON_DOWN));
		cartridge.set_char(40, 40, stc(btns, button_e::BUTTON_LEFT));
		cartridge.set_char(40, 50, stc(btns, button_e::BUTTON_RIGHT));

		cartridge.set_char(120, 20, stc(btns, button_e::BUTTON_A));
		cartridge.set_char(120, 30, stc(btns, button_e::BUTTON_B));
		cartridge.set_char(120, 40, stc(btns, button_e::BUTTON_START));
		cartridge.set_char(120, 50, stc(btns, button_e::BUTTON_SELECT));

		cartridge.draw_buffer();
		sleep_for_ms(draw_delay_ms);
	}
}

int main()
{
	spdlog::set_level(spdlog::level::info);
	spdlog::info("Hello World!");

	m_communicator = std::make_unique<communicator_pipe_c>(
	    communicator_c::role_e::CARTRIDGE);
	m_communicator->init();

	cartridge_c cartridge(
	    [](uint8_t byte) { m_communicator->send_byte(byte); },
	    []() -> uint8_t { return m_communicator->receive_byte(); });

	cartridge.sync_with_handheld();

	spdlog::info("Configuration done, starting with main loop");

	cartridge.set_text(0, 0, "AVR BOY DEMO");

	while (1) {
		demo_buttons(cartridge);
		sleep_for_ms(1000);

		demo_text(cartridge);
		sleep_for_ms(1000);

		demo_pixel(cartridge);
		sleep_for_ms(1000);

		demo_bouncing_pixel(cartridge);
		sleep_for_ms(1000);
	}
}
