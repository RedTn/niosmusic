/*
 * VGA.c
 *
 *  Created on: Jan 23, 2013
 *      Author: Gary
 */

#include "VGA.h"

void init_VGA_Pixel()
{

	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, PIXEL_BUFFER_BASE);

	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

	// Wait for the swap to complete
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);



	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	// Draw a white line to the foreground buffer
	//alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0xFFFF,0);
	//alt_up_char_buffer_string(char_buffer, "EECE 381", 40, 30);


}

void init_VGA_Char()
{
	alt_up_char_buffer_init(char_buffer);
	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_clear(char_buffer);
}
