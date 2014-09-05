/*
 * global.h
 *
 *  Created on: Jan 26, 2013
 *      Author: Gary
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

/*
#define XMAXSCREEN_RESOLUTION 319
#define YMAXSCREEN_RESOLUTION 239
#define PI_ 3.14159
#define EMPTY 0
#define TERRAIN_OCCUPIED 10
#define PLAYER1_OCCUPIED 9
#define PLAYER2_OCCUPIED 8
#define GREEN_COLOR 0x7E0
#define YELLOW_COLOR 0xFFE0
#define WHITE_COLOR 0xFFFF
#define BLACK_COLOR 0x0000
#define BROWN_COLOR 0x79E0
#define RED_COLOR 0xF800
#define DARK_GREY_COLOR 0x7BEF
#define PINK_COLOR 0xF81F
#define MISSILE_SELECT 0
#define MIRV_SELECT 1
#define ICBM_SELECT 2
#define NUKE_SELECT 3
*/

#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"

int time_of_sound;
int audio_isr_k;
int audio_isr_2;
int shot_isr;
int impact_isr;
int pew_isr;
int fart_isr;
int laser_isr;
int dis_isr;
int wind;
extern alt_up_audio_dev * audio;


#endif /* GLOBAL_H_ */
