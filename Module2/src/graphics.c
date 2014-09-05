/*
 * graphics.c
 *
 *  Created on: 2013-01-22
 *      Author: Jae-Hwan Jung
 */

#include "graphics.h"


void initVGA()
{

	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (320*240*2);

	/* Set the background buffer address – Although we don’t use the
	background, they only provide a function to change the background
	buffer address, so we must set that, and then swap it to the foreground */
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,pixel_buffer_addr1);

	// Swap background and foreground buffers
	// Wait for the swap to complete
	updateVGA();

	//set the 2nd buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,pixel_buffer_addr2);

	// Clear both buffers (this makes all pixels black)
	clearForeground();
	clearBackground();

}

void clearForeground()
{
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
}

void clearBackground()
{
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);
}

void updateVGA()
{
	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

	// Wait for the swap to complete
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
}
