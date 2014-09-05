/*
 * audio.h
 *
 *  Created on: 2013-01-28
 *      Author: kidax
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include <altera_up_sd_card_avalon_interface.h>
#include <string.h>
#include <math.h>
#include "global.h"
#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_irq.h"
#include "system.h"
#include "alt_types.h"
#include <stddef.h>
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "VGA.h"
#include "altera_up_avalon_rs232.h"
//#include <limits.h>

/*

#include "ps_controller.h"
#include "tank_controls.h"
*/

#define AUD_DIRECT "AUDIO/"
#define MAX_NAME_LEN 13
#define MAXFILES 20
#define AV_CONFIG "/dev/audio_and_video_config"
#define AUDIO_DEV "/dev/audio"
#define keys (volatile char *) 0x0002470
#define switches (volatile char *) 0x002480
#define direct_len 7
#define ENQ 0x5
#define ACK 0x6
#define EOT 0x4
#define ETX 0x3
#define HEAD 0x1
#define SRT 0x2
#define ETB	0x17

#define QUEUE 3
#define PLAY 4
#define STOP 5
#define PAUSE 6
#define VOLUME 7
#define SELECT 8
#define S_LOOP 9
#define Q_LOOP 11
#define C_LOOP 10
#define SEND 12
#define VOL_UP 15
#define VOL_DOWN 16
#define SHUFFLE 17
#define NEXT 13
#define PREVIOUS 14
#define MIX 17
#define VISUAL 7
#define VISUALON 18

typedef struct
{
	int mode;
	int newvolume;
	bool loop;
	int id;
	int id2;
	int large;
	int oldid;
	int oldid2;
	int files;
	bool listloop;
	int * queue;
	bool resetmix;
	bool shuffle;
}audisr;

typedef struct
{
	unsigned int * bgmin;

	//bool volinit[MAXFILES];
}audlog;

void send_fnames(char * filenames[], int numfiles);
void audio_isr(audisr * audmode, alt_u32 irq_id);
void av_config_setup();
int init_sd(char * filenames[]);
void init_wav(char * bgmfile, int volume, int id);
int init_sound_fx(char * SHOT_FX, char * IMPACT_FX);
int selectwavs(char * filenames[], char * dest[], int numfiles);
void select_bgm(char ** soundfiles, int numfiles);
char * select_shot(char ** soundfiles, int numfiles);
unsigned int volume_adjust(unsigned int pcm, int volume);
unsigned int volume_revert(unsigned int pcm, int volume);
alt_u32 my_alarm_callback(void* context);
void start_clock(int time);
void init_sound_fx_troll(char * SHOT_FX, char * IMPACT_FX);
void init_sound_fx_laser(char * SHOT_FX, char * IMPACT_FX);
void change_bgm(int input, char ** soundfiles, int numfiles);
void init_rs232();
void send_filenames(char * filenames[], int numfiles);
//void receive_songs(unsigned char * queue[]);
void emulate_ack();
void clear_fifo();
void wait();
void init_timer(double period);
void timer_isr(void * context, alt_u32 irq_id);
void start_timer();
void stop_timer();
void emulate_ack2();
void listen(char * soundfiles[], int numwavfiles, audisr * audmode);
void get_filenames();
void check_ack();
void copy_bgm(int id);
void init_copy(int id, int oldid);
int get_fnames(char * soundfiles[],int numwavfiles);
unsigned int mix_adjust(unsigned int pcm, unsigned int pcm2, int volume);
unsigned int mix_adjust2(unsigned int pcm, unsigned int pcm2);
//void init_VGA_Char();
//void manual(int input, psController ps);

#endif /* AUDIO_H_ */
