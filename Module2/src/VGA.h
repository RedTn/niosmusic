/*
 * VGA.h
 *
 *  Created on: Jan 23, 2013
 *      Author: Gary
 */

#ifndef VGA_H_
#define VGA_H_

#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev* char_buffer;


void init_VGA_Pixel(void);
void init_VGA_Char(void);



#endif /* VGA_H_ */
