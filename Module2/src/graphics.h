/*
 * graphics.h
 *
 *  Created on: 2013-01-22
 *      Author: Jae-Hwan Jung
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "altera_up_avalon_video_pixel_buffer_dma.h"

alt_up_pixel_buffer_dma_dev* pixel_buffer;

//Requires: device structure descriptor file is pixel_buffer
//			pixel buffer device name is "pixel_buffer_dma"
//			resolution is 320*240
//			two pixel buffers are used
//Modifies: nothing
//Effects : initializes the VGA system so that altera HAL API can be used to draw
void initVGA();

void clearForeground();

void clearBackground();

void updateVGA();

#endif /* GRAPHICS_H_ */
