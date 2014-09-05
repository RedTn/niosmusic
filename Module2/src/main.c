#include "global.h"
#include <stdio.h>
#include "altera_up_sd_card_avalon_interface.h"
#include "audio.h"
#include <stddef.h>
#include <string.h>
#include "VGA.h"
#include "graphics.h"


//WORKSPACE: C:\Users\red\workspace_de2
int main()
{
	int numfiles;
	int numwavfiles;
	char * filenames[MAXFILES] = {0};
	char * soundfiles[MAXFILES] = {0};
	unsigned char * queue[MAXFILES] = {0};
	char * bgm;
	int once = 0;
	int temp_id = 3;
	int i;

	av_config_setup();
	numfiles = init_sd(filenames);
	numwavfiles = selectwavs(filenames, soundfiles, numfiles);

	init_rs232();
	init_timer(0.001);

	stop_timer();


	//
	numwavfiles = 3;
	//
	int tempindex = 0;
	for(i = 0; i < numwavfiles; i++) {
		alt_timestamp_start();
		init_wav(soundfiles[i], 2, tempindex);
		tempindex++;
		printf("Timestamp[%d] = %d\n", i, alt_timestamp());
	}

	//====AUDIO LOOP ======================================================//
	audisr audmode;
	audmode.shuffle = true;
	audmode.mode = 0;
	audmode.newvolume = 10;
	audmode.loop = false;
	audmode.id = 0;
	audmode.id2 = 1;
	audmode.oldid = 0;
	audmode.oldid2 = 0;
	audmode.listloop = true;
	audmode.files = numwavfiles;
	audmode.resetmix = false;
	audio_isr_2 = 0;

	alt_up_audio_disable_write_interrupt(audio);

	init_copy(audmode.id, audmode.oldid);
	audio_isr_k = 0;
	alt_irq_register(AUDIO_IRQ, &audmode, &audio_isr);

	int volume_index_up = 10;
	int volume_index_down = 10;

	listen(soundfiles, numwavfiles, &audmode);

	printf("Out of listen loop\n");

	//DE2 side
	alt_up_audio_enable_write_interrupt(audio);

	printf("Starting Debug loop\n");
	while(1)
	{
		while(*keys == 11) {
			volume_index_down--;
			if (volume_index_down < 1) {
				volume_index_down = 1;
			}
			printf("Down %d\n", volume_index_down);
			audmode.newvolume = volume_index_down;
			volume_index_up = 10;
			while(*keys == 11);
		}
		while(*keys == 7) {
			volume_index_up += 2;
			if (volume_index_up > 127) {
				volume_index_up = 127;
			}
			printf("Up %d\n", volume_index_up);
			audmode.newvolume = volume_index_up;
			volume_index_down = 10;
			while(*keys == 7);
		}
		while(*keys == 13) {
			/*
			printf("Now setting volume to x10\n");
			audmode.newvolume = 100;
			 */
			/*
			printf("Testing loop off:\n");
			audmode.loop = false;
			 */
			alt_up_audio_disable_write_interrupt(audio);
			audmode.id += 1;
			if (audmode.id >= numwavfiles) {
				audmode.id = 0;
			}
			/*
			audio_isr_k = 0;
			init_copy(audmode.id);
			audmode.oldid = audmode.id;
			 */
			printf("Changing to song Id: %d\n", audmode.id);
			alt_up_audio_enable_write_interrupt(audio);
			while(*keys == 13);
		}
		while (*switches == 2) {
			printf("Resume\n");
			alt_up_audio_enable_write_interrupt(audio);
			while(*switches == 2);
		}
		while (*switches == 1) {
			printf("Paused.\n");
			alt_up_audio_disable_write_interrupt(audio);
			while(*switches == 1);
		}

		while(*switches == 4) {
			printf("Stop\n");
			alt_up_audio_disable_write_interrupt(audio);
			audio_isr_k = 0;
			while(*switches == 4);
		}
		while(*switches == 8) {
			if(audmode.loop == true) {
				printf("Loop off\n");
				audmode.loop = false;
			}
			else if (audmode.loop == false) {
				printf("Loop on\n");
				audmode.loop = true;
			}
			while(*switches == 8);
		}
	}

	printf("Why am I out of loop?\n");
	return 0;
}
