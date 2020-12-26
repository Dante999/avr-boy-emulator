#include <memory>
#include <stdio.h>
#include <unistd.h> // sleep()
// ---
#include <chrono>
#include <thread>
// --
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

	for (uint8_t y = y_min; y <= x_max; ++y) {

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
		demo_text(cartridge);
		sleep_for_ms(1000);

		demo_pixel(cartridge);
		sleep_for_ms(1000);
	}
}
