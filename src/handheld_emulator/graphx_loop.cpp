#include "avr-boy-core/font5x7.hpp"
#include "avr-boy-core/graphx.hpp"
#include <array>

graphx_c g_fx;

void graphx_init()
{
	g_fx.draw_rect(graphx_c::min_x, graphx_c::max_x, graphx_c::min_y,
	               graphx_c::max_y, graphx_c::PIXEL_ON);

	g_fx.draw_string(font5x7, 5, 5, "Hello World!");

	std::array<uint8_t, 8> tile_block1{0xAA, 0xAA, 0xAA, 0xAA,
	                                   0xAA, 0xAA, 0xAA, 0xAA};

	std::array<uint8_t, 8> tile_block2{0xAA, 0x55, 0xAA, 0x55,
	                                   0xAA, 0x55, 0xAA, 0x55};

	// graphx_c::sprite_data_t sprite1 = {0x00, 0x00, 0xFF, 0x81,
	//                                    0x81, 0xFF, 0x00, 0x00};

	// c.sprite_set_data(0, sprite1);
	// c.sprite_set_coord(0, 10, 10);
	// c.sprite_set_show(0, true);

	g_fx.draw_tile(16, 20, tile_block1.data(), 8, 8);
	g_fx.draw_tile(24, 20, tile_block2.data(), 8, 8);
	g_fx.draw_tile(32, 20, tile_block2.data(), 8, 8);
	g_fx.draw_tile(40, 20, tile_block2.data(), 8, 8);
}

graphx_c &graphx_loop()
{
	// static uint8_t x = 0;
	// static uint8_t y = 0;

	// ++x;

	return g_fx;
}