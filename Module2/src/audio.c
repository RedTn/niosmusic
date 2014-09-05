#include "audio.h"
#include "altera_up_avalon_rs232.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

int sound_file = 0;
alt_up_audio_dev * audio;
alt_up_char_buffer_dev* char_buffer;
int size_of_sound = 0;
int audiosize[MAXFILES] = {0};
int sizeisr[MAXFILES] = {0};
int audiosecs[MAXFILES] = {0};
alt_up_rs232_dev* uart;

int turn = 0;
int arrow_index;
int arrow_y;
int arrow_x = 0;
int VOL_SET = 10;
int onetime = 0;
int emulate_temp = 0;

int fifo_isr = -1;
unsigned char fifo_buffer [129] = {0};

unsigned int ***arr;

char * bgm;

unsigned char data;
unsigned char parity;

int isr_index = 0;
int oldvolume;

char * fname;
bool rs_flag = false;
unsigned char * queue[MAXFILES] = {0};
audlog audio_log;
bool logonce = false;

unsigned int **copyarr;
unsigned int **mixedcopy;
int copyonce = 0;
bool songloop = true;
int printfcounter = 0;
bool ampflag = false;
int ampcounter = 0;
bool visualflag = false;

int seek = 0;
int tempcounter = 0;

int few = 0;

//Converts two char numbers into unsigned int of size 24 bits, based on volume coefficient
static unsigned int convert (unsigned int high, unsigned int low) {
	unsigned int temp;
	int negative;

	temp = ((high * pow(16,4)) + (low * pow(16,2)));

	if (VOL_SET != 10) {
		if (temp > 8388607) {
			negative = temp - (8388608 * 2);
			negative = ((negative * VOL_SET) / 10) + (16777216);
			temp = (unsigned int)negative;
			if(temp > 16777215)
				temp = 16777215;
		}
		else if (temp <= 8388607) {
			temp = (unsigned int)((temp * VOL_SET) / 10);
			if (temp > 8388607) {
				temp = 8388607;
			}
		}

	}
	return temp;
}

//Initializes audio & video core, will not return until succuessful
void av_config_setup() {
	bool once = true;
	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev(AV_CONFIG);
	while(!alt_up_av_config_read_ready(av_config)) {
		if(once) {
			printf("Error, audio not configured.\n");
			once = false;
		}
	}
	audio = alt_up_audio_open_dev(AUDIO_DEV);
	alt_up_audio_reset_audio_core(audio);
}

//Initializes SD card, outputs filenames on sd card
int init_sd(char * filenames[]){
	int j;
	int i = 0;
	char thepath[] = AUD_DIRECT;	//Directory for filenames we want to read
	short int check_file_name;

	for (j = 0; j < MAXFILES; j++) {
		filenames[j] = (char *)malloc(sizeof(char) * MAX_NAME_LEN);
		if (filenames[j] == NULL)
			printf("Error, out of memory (2)\n");
	}

	alt_up_sd_card_dev * sd_card = NULL;
	int connected = 0;
	sd_card = alt_up_sd_card_open_dev("/dev/Interface");

	if (sd_card != NULL) {
		while(1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())) {
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					check_file_name = alt_up_sd_card_find_first(thepath,filenames[i]);
					if (check_file_name == 1)
						printf("Invalid Directory.\n");
					else if (check_file_name == 2)
						printf("Invalid SD Card.\n");
					else if (check_file_name == -1)
						printf("No files found.\n");
					else {
						i++;
					}
					while(alt_up_sd_card_find_next(filenames[i]) != -1){
						i++;
					}
					arr = (unsigned int ***)malloc(sizeof(unsigned int) * i);
					if (arr == NULL){
						printf("Error, arr null\n");
					}
					return i;		//If card connected, returns amount of files
				} else {
					printf("Unknown file system.\n");
				}
				connected = 1;
			} else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
				printf("Card disconnected.\n");
				connected = 0;
			}
		}
	}
	else {
		printf("Error.\n");
		return -1;
	}
}

//Initializes background music, input requires file directory
void init_wav(char * bgmfile, int volume, int id)
{
	fname = (char *)malloc(sizeof(char) * (MAX_NAME_LEN + direct_len));
	if (fname == NULL) {
		printf("Error, fname null\n");
	}
	strcpy(fname, AUD_DIRECT);
	strcat(fname, bgmfile);

	printf("Initializing %s, Id: %d\n", fname, id);

	int temp1 = 0;
	int temp2 = 0;
	int i,j,k;
	int location = 0;
	int size_of_wav = 0;
	unsigned int buffer[96] = {0};
	int t1;
	int t2;
	int t3;
	int t4;

	VOL_SET = volume;
	printf("Volume Setting: %d\n", VOL_SET);

	sound_file = alt_up_sd_card_fopen(fname, false);

	if (sound_file != -1)
	{
		while(alt_up_sd_card_read(sound_file) != -1) {
			size_of_wav++;		//Get size of file
		}

		alt_up_sd_card_fclose(sound_file);
	}

	sound_file = alt_up_sd_card_fopen(fname, false);		//Reopen so we start from index 0

	if (sound_file != -1) {
		printf("%s opened.\n", fname);

		for (i = 0; i < size_of_wav; i++) {
			t1 = alt_up_sd_card_read(sound_file);
			if (t1 == 'd') {
				t1 = alt_up_sd_card_read(sound_file);
				if (t1 == 'a') {
					t1 = alt_up_sd_card_read(sound_file);
					if(t1 == 't') {
						t1 = alt_up_sd_card_read(sound_file);
						if(t1 == 'a') {
							location = i;	//skip header values to data chunk
							break;
						}
					}
				}
			}

		}
		i = location + 4;
		t1 = alt_up_sd_card_read(sound_file);
		t2 = alt_up_sd_card_read(sound_file);
		t3 = alt_up_sd_card_read(sound_file);
		t4 = alt_up_sd_card_read(sound_file);

		//printf("Size: %d\n", (int)(((t4 * pow(16,6) + t3 * pow(16,4) + t2 * pow(16,2) + t1))));
		size_of_sound = (int)(((t4 * pow(16,6) + t3 * pow(16,4) + t2 * pow(16,2) + t1) / 192));

		audiosize[id] = size_of_sound;
		audiosecs[id] = (int)(size_of_sound / 333);
		//printf("Audiosecs: %d\n", audiosecs[id]);

		arr[id] = (unsigned int**)malloc(size_of_sound * sizeof(unsigned int));
		if (arr[id] == NULL)
		{
			printf("Error, out of memory (3).\n");
		}

		//Skip subchunk block
		i += 4;

		unsigned int ** temp;
		temp = arr[id];
		for (k = 0; k<size_of_sound; k++) {
			for (j = 0; j<96; j++){
				temp1 = alt_up_sd_card_read(sound_file);
				temp2 = alt_up_sd_card_read(sound_file);
				buffer[j] = convert(temp2, temp1);		//converting values into arrays of size 96, for quick feed into buffer
			}
			temp[k] = (unsigned int*)malloc(sizeof(buffer));
			if (temp[k] == NULL) {
				printf("Error, no more memory space (4).\n");
			}
			memcpy((void*)temp[k], (void*)buffer, sizeof(buffer));
			if (k == (int)(size_of_sound / 4))
				printf("25 Percent converting Wav\n");
			if (k == (int)((size_of_sound * 3)/4))
				printf("75 Percent converting Wav\n");
		}

	}
	if(alt_up_sd_card_fclose(sound_file))
		printf("%s closed.\n", fname);
	else
		printf("Error closing %s.\n", fname);

	free(fname);
}

//Audio interrupt function, plays sound effects and/or loops audio
//REQ: audmode->id must point to correct id
void audio_isr(audisr * audmode, alt_u32 irq_id) {
	int i;
	unsigned int ** temp;
	unsigned int * second;

	if (audmode->resetmix == true) {
		audmode->resetmix = false;
		audio_isr_2 = 0;
	}
	//if (audmode->mode == 0) {
	if (audmode->id != audmode->oldid) {
		audio_isr_k = 0;
		seek = 0;
		init_copy(audmode->id, audmode->oldid);
		audmode->oldid = audmode->id;
	}
	if (audmode->id2 != audmode->oldid2) {
		audio_isr_2 = 0;
		audmode->oldid2 = audmode->id2;
	}
	if (audio_isr_k == 0) {
		copy_bgm(audmode->id);
		alt_up_rs232_write_data(uart, 0x6);
		alt_up_rs232_write_data(uart, (int)audiosecs[audmode->id]);
	}
	audio_log.bgmin = copyarr[audio_isr_k];

	//Mode 0, loop music
	if (audmode->mode == 0) {
		for (i = 0; i < 96; i++) {
			audio_log.bgmin[i] = volume_adjust(audio_log.bgmin[i], audmode->newvolume);
		}
	}
	else if (audmode->mode == 1) {
		temp = arr[audmode->id2];
		second = temp[audio_isr_2];
		for (i = 0; i < 96; i++) {
			unsigned int tempmix = audio_log.bgmin[i];
			audio_log.bgmin[i] = mix_adjust(tempmix,second[i],5);
		}
		audio_isr_2++;
		if (audio_isr_2 > audiosize[audmode->id2]) {
			audio_isr_2 = 0;
			audmode->mode = 0;
		}
	}
	if (alt_up_audio_write_interrupt_pending(audio) == 1) {
		alt_up_audio_write_fifo(audio, audio_log.bgmin, 96, ALT_UP_AUDIO_LEFT);
		alt_up_audio_write_fifo(audio, audio_log.bgmin, 96, ALT_UP_AUDIO_RIGHT);
		if (audmode->loop == true){
			audio_isr_k = (audio_isr_k + 1) % audiosize[audmode->id];
			seek = (seek + 1) % 333;
			if (seek == 0) {
				//alt_up_rs232_write_data(uart, 0x0A);
			}
			if (audio_isr_k == 0) {
				alt_up_rs232_write_data(uart, 0x3);
			}
		}
		else
		{
			if(audio_isr_k <= audiosize[audmode->id])
			{
				audio_isr_k += 1;
				seek = (seek + 1) % 333;
				if (seek == 0) {
					//alt_up_rs232_write_data(uart, 0x0A);
				}
			}
			else
			{
				seek = 0;
				audmode->mode = 0;
				audio_isr_2 = 0;
				audio_isr_k = 0;
				//alt_up_rs232_write_data(uart, 0x3);
				alt_up_rs232_write_data(uart, 0x4);
				wait();
				if (audmode->shuffle == false) {
					audmode->id += 1;
					if ((audmode->id >= audmode->files) && (audmode->listloop == true)) {
						audmode->id = 0;
						//printf("Restarting songs\n");
					}
					else if((audmode->id >= audmode->files) && (audmode->listloop == false)) {
						printf("End of Playlist\n");
						audmode->id = 0;
						alt_up_audio_disable_write_interrupt(audio);
					}
				}
				else {
					/*
					do {
						while(rs_flag == false);
						alt_up_rs232_read_data(uart, &data, &parity);
					}while((int)data == 0);
					 */
					wait();
					alt_up_rs232_read_data(uart, &data, &parity);
					int nextindex = (int)data;
					nextindex -= 4;
					//printf("Next Index: %d\n", nextindex);
					if ((nextindex < 0) || (nextindex > audmode->files)) {
						nextindex = 0;
						printf("Error, next Index: %d\n", nextindex);
					}
					audmode->id = nextindex;
				}
			}
		}
	}
	//}
	/*
	else if (audmode->mode == 1) {

		if (audmode->id != audmode->oldid) {
			//TEMP
			audmode->id2 = audmode->id++;
			if (audmode->id2 >= audmode->files) {
				audmode->id2 = 0;
			}
			if (audiosize[audmode->id] > audiosize[audmode->id2]) {
				audmode->large = audmode->id;
			}
			else {
				audmode->large = audmode->id2;
			}
			//
			audio_isr_k = 0;
			init_copy(audmode->large, audmode->oldid);
			audmode->oldid = audmode->id;
		}
		if (audio_isr_k == 0) {
			copy_bgm(audmode->large);
			alt_up_rs232_write_data(uart, 0x6);
		}
		audio_log.bgmin = copyarr[audio_isr_k];

		for (i = 0; i < 96; i++) {
			//audio_log.bgmin[i] = mix_adjust(audio_log.bgmin[i],  ,audmode->newvolume);
		}

		if (alt_up_audio_write_interrupt_pending(audio) == 1) {


			audio_isr_k = 0;
			audmode->id += 1;
			alt_up_rs232_write_data(uart, 0x3);
			if ((audmode->id >= audmode->files) && (audmode->listloop == true)) {
				audmode->id = 0;
				printf("Restarting songs\n");
			}
			else if((audmode->id >= audmode->files) && (audmode->listloop == false)) {
				printf("End of Playlist\n");
				audmode->id = 0;
				alt_up_audio_disable_write_interrupt(audio);
			}
		}

	}
	 */
}


int selectwavs(char * filenames[], char * dest[], int numfiles)
{
	int i;
	char * ptr;
	int current_len = 0;
	int tempindex = 0;

	for (i = 0; i < numfiles; i++) {
		ptr = strrchr(filenames[i], '.');
		if (ptr != NULL) {
			if ((*(ptr + 1) == 'W') && (*(ptr + 2) == 'A') && (*(ptr + 3) == 'V')) {
				current_len = ((int)strlen(filenames[i])) + 1;
				dest[tempindex] = (char *)malloc(sizeof(char) * current_len);
				if (dest[tempindex] == NULL)
					printf("Error, out of memory (5)\n");
				strcpy(dest[tempindex],filenames[i]);
				printf("%s\n", dest[tempindex]);
				tempindex++;
			}
		}

	}

	return tempindex;
}

//Audio selection screen and volume control
void select_bgm(char ** soundfiles, int numfiles)
{
	int init_y;
	int i;
	int y_index = 4;
	int temp,temp2;
	char ptr[] = "->";
	int cross;
	int crossptr = 36;

	if (onetime > 0)
	{
		free(bgm);	//If called again, clean up original file name for audio file
	}

	bgm = (char *)malloc(sizeof(char) * (MAX_NAME_LEN + direct_len));
	char vol_display[3];
	temp2 = sprintf(vol_display, "%d", ((int)(VOL_SET * 10)));

	alt_up_char_buffer_string(char_buffer, "Select BGM file",33, y_index);
	alt_up_char_buffer_string(char_buffer, "Use D-pad to scroll",31, 6);
	alt_up_char_buffer_string(char_buffer, "Use Left & Right to scroll to volume",25, 7);
	alt_up_char_buffer_string(char_buffer, "Press 'X' to select",31, 8);
	alt_up_char_buffer_string(char_buffer, "Volume Control",62, 21);
	alt_up_char_buffer_string(char_buffer, "+", 68, 26);
	alt_up_char_buffer_string(char_buffer, vol_display, 68, 30);
	alt_up_char_buffer_string(char_buffer, "-", 68, 34);

	y_index = 27;
	temp = numfiles;
	while (temp > 0) {
		y_index -= 2;
		temp--;
	}
	for(i = 0; i < numfiles; i++) {
		if (soundfiles[i] != NULL) {
			y_index += 3;					//Display soundfiles
			alt_up_char_buffer_string(char_buffer, soundfiles[i],36, y_index);
			if (i == 0) {
				arrow_y = y_index;
				init_y = arrow_y;
				if (arrow_x == 0) {
					alt_up_char_buffer_string(char_buffer, ptr,33, arrow_y);
				}
				else if (arrow_x == 1){
					arrow_y = 30;
					alt_up_char_buffer_string(char_buffer,"->",65, arrow_y);


				}
			}
		}
	}

	arrow_index = 0;

	while(1) {
		if(*keys == 7)
		{
			usleep(150000);
			if (((arrow_index) > 0) && (arrow_x == 0)) {
				alt_up_char_buffer_string(char_buffer,"  ",33, arrow_y);
				arrow_y -= 3;
				alt_up_char_buffer_string(char_buffer,ptr,33, arrow_y);
				arrow_index--;
			}
			else if (arrow_x == 1) {
				alt_up_char_buffer_string(char_buffer, " ", 68, 30);
				VOL_SET = VOL_SET + 0.1;		//Volume precision
				if (VOL_SET >= 2.0)
					VOL_SET = 2.0;	//Max volume
				temp2 = sprintf(vol_display, "%d", ((int)(VOL_SET * 10)));
				alt_up_char_buffer_string(char_buffer, vol_display, 68, 30);
			}
		}
		if(*keys == 11)
		{
			usleep(150000);
			if (((arrow_index) < (numfiles - 1)) && (arrow_x == 0)) {
				alt_up_char_buffer_string(char_buffer,"  ",33, arrow_y);
				arrow_y += 3;
				alt_up_char_buffer_string(char_buffer,ptr,33, arrow_y);
				arrow_index++;
			}
			else if (arrow_x == 1) {
				alt_up_char_buffer_string(char_buffer, "  ", 68, 30);
				VOL_SET = VOL_SET - 0.1;
				if (VOL_SET <= 0.0)
					VOL_SET = 0.0;
				temp2 = sprintf(vol_display, "%d", ((int)(VOL_SET * 10)));
				alt_up_char_buffer_string(char_buffer, vol_display, 68, 30);
			}
		}

		//Select button
		if(*switches == 1)
		{
			usleep(150000);
			if (arrow_x == 0) {
				cross = strlen(soundfiles[arrow_index]);
				for (i = 0; i < cross; i++) {
					alt_up_char_buffer_string(char_buffer,"-",crossptr, arrow_y);
					crossptr++;
				}
				strcpy(bgm, AUD_DIRECT);
				strcat(bgm, soundfiles[arrow_index]);
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer,"Loading, Please Wait",31, 25);
				init_wav(bgm, VOL_SET, 0);
				alt_up_audio_enable_write_interrupt(audio);		//Resume audio
				alt_up_char_buffer_clear(char_buffer);
				return;
			}
		}
		if(*keys == 13)
		{
			usleep(150000);
			if (arrow_x == 0) {
				arrow_x = 1;
				alt_up_char_buffer_string(char_buffer,"  ",33, arrow_y);
				arrow_y = 30;
				alt_up_char_buffer_string(char_buffer,"->",65, arrow_y);
			}
		}
		if (arrow_x == 1) {
			arrow_x = 0;
			alt_up_char_buffer_string(char_buffer,"  ",65, arrow_y);
			arrow_y = init_y;
			alt_up_char_buffer_string(char_buffer,"->",33, arrow_y);
			arrow_index = 0;
		}

		/*
		else if (ps.b.sel == 1) {
			usleep(150000);
			alt_up_audio_enable_write_interrupt(audio);
			alt_up_char_buffer_clear(char_buffer);
			return;

		}
		 */
		usleep(150000);
	}

}

void init_rs232()
{
	//printf("UART Initialization\n");
	uart = alt_up_rs232_open_dev("/dev/rs232");

	clear_fifo();
}

void send_filenames(char * filenames[], int numfiles)
{
	int i,j;
	int size;
	char * ptr;

	clear_fifo();

	printf("Sending the message to the Middleman\n");

	//Start with header
	alt_up_rs232_write_data(uart, (unsigned char) HEAD);

	//Function protocol
	alt_up_rs232_write_data(uart, (unsigned char) 3);

	//NumFiles
	alt_up_rs232_write_data(uart, (unsigned char) numfiles);

	/*
	wait();
	alt_up_rs232_read_data(uart, &data, &parity);
	printf("%d files.\n", (int)data);
	 */

	for (j = 0; j < numfiles; j++) {

		size = strlen(filenames[j]);
		ptr = filenames[j];

		//Make sure there is space
		while(alt_up_rs232_get_available_space_in_write_FIFO(uart) < ((size * 2) + 2));

		// Start with the number of bytes in our message
		alt_up_rs232_write_data(uart, (unsigned char)size);

		// Now send the actual message to the Middleman
		for (i = 0; i < size; i++) {
			alt_up_rs232_write_data(uart, *(ptr + i));

			//Send ETX
			alt_up_rs232_write_data(uart, (unsigned char) ETX);

		}

		alt_up_rs232_write_data(uart, (unsigned char) ETB);

		wait();
		//emulate_ack();
		emulate_ack2();

		wait();
		alt_up_rs232_read_data(uart, &data, &parity);
		//printf("%d\n", (int)data);
		if (data != ACK) {
			printf("Error, Acknowledge was not received.\n");
		}
		else
			printf("Acknowledge received.\n");

		clear_fifo();

		if (j == (numfiles - 1)) {
			//End of Transmission
			alt_up_rs232_write_data(uart, (unsigned char) EOT);
		}
	}
	wait();
	alt_up_rs232_read_data(uart, &data, &parity);
	printf("I received %d\n", (int)data);

}

//Sends the sound file names to android
void send_fnames(char * filenames[], int numfiles)
{
	int i,j;
	int size;
	char * ptr;

	rs_flag = false;

	clear_fifo();

	printf("Sending the message to the Middleman\n");

	for (j = 0; j < numfiles; j++) {
		alt_up_rs232_write_data(uart, 2);
		alt_up_rs232_write_data(uart, j);
		size = strlen(filenames[j]);
		ptr = filenames[j];

		//Make sure there is space
		while(alt_up_rs232_get_available_space_in_write_FIFO(uart) < (size + 2));

		// Now send the actual message to the Middleman
		for (i = 0; i < size; i++) {
			alt_up_rs232_write_data(uart, *(ptr + i));
		}

		wait();
		do {
			if (rs_flag == true) {
				alt_up_rs232_read_data(uart, &data, &parity);
			}

			////////////////////////////

			//TO BE COMMENTED OUT, this is my delay simulation for android
			/*
			testvalue++;
			if (testvalue == 1000000) {
				alt_up_rs232_write_data(uart, (unsigned char)ACK);
				testvalue = 0;
			}
			 */
			/////////////////////////////
		}while(data != ACK);
		clear_fifo();
	}
}

//Gets the index for song to be played
int get_fnames(char * soundfiles[], int numwavfiles)
{
	int index = 0;

	do {
		while(rs_flag == false);
		alt_up_rs232_read_data(uart, &data, &parity);
	}while(((int)data == 3) || ((int)data == 0));
	printf("\n");

	index = (int)data - 4;
	if ((index < 0) || (index > numwavfiles)) {
		printf("Index received out of bounds\n");
		index = 0;
	}

	printf("Song Selected(if detected): %s, Index selected = %d\n", soundfiles[index], index);

	return index;
}

void emulate_ack2()
{
	printf("Emulating ACK\n");
	int i;
	int stop;
	int once = 0;

	start_timer();
	while(1)
	{
		if(rs_flag == true) {
			alt_up_rs232_read_data(uart, &data, &parity);
			if (data == HEAD) {

			}
			else
				printf("Error, Header not received.\n");
		}
	}
	printf("\n");
	stop_timer();
	alt_up_rs232_write_data(uart, (unsigned char) ACK);
}

void clear_fifo()
{
	while (alt_up_rs232_get_used_space_in_read_FIFO(uart)) {
		alt_up_rs232_read_data(uart, &data, &parity);
	}
	//	printf("Fifo cleared.\n");
}

/*
void receive_songs(unsigned char * queue[])
{
	int i,j;
	int numfiles;

	// Now receive the message from the Middleman
	printf("Waiting for data to come back from the Middleman\n");
	wait();

	//First byte is number of files android wants
	alt_up_rs232_read_data(uart, &data, &parity);
	numfiles = data;

	for (j = 0; j < numfiles; j++) {
		while (fifo_isr <= -1);
		// First byte is the number of characters in our message
		alt_up_rs232_read_data(uart, &data, &parity);
		int num_to_receive = (int)data;
		printf("About to receive %d characters:\n", num_to_receive);

		unsigned char * messagebuf = (unsigned char*)malloc(sizeof(char) * num_to_receive);
		queue[j] = (unsigned char *)malloc(sizeof(char) * num_to_receive);
		if ((queue[j] || messagebuf) == NULL)
		{
			printf("Error, out of memory (1)\n");
		}

		for (i = 0; i < num_to_receive; i++) {
			wait();
			alt_up_rs232_read_data(uart, &data, &parity);
			messagebuf[i] = data;
			printf("%c", data);
		}
		printf("\n");
		printf("Message Echo Complete\n");

		strcat(queue[j], AUD_DIRECT);
		strcat(queue[j], messagebuf);

		free(messagebuf);

		clear_fifo();

		//Tell android reading is done
		alt_up_rs232_write_data(uart, (unsigned char) ACK);
		wait();
	}
}
 */

//Waits for fifo space to be empty in rs232
void wait()
{
	while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
}

//Initialize hardware-only timer
void init_timer(double period)
{
	int timer_period,status,control,tp_low,tp_high;
	bool irq,repeat;
	alt_irq_register(TIMER_0_IRQ, NULL, &timer_isr);
	timer_period = period * 50000000;
	IOWR_16DIRECT(TIMER_0_BASE, 8, timer_period & 0xFFFF);
	IOWR_16DIRECT(TIMER_0_BASE, 12, timer_period >> 16);

	tp_low = IORD_16DIRECT(TIMER_0_BASE, 8);
	tp_high = IORD_16DIRECT(TIMER_0_BASE, 12);

	//	printf("Period: %x%x\n", tp_high,tp_low);

	double dec = ((tp_high << 16) + tp_low) / 50000;

	printf("Period (decimal): %lf milliseconds\n", dec);

	//printf("Stopping Timer\n");
	status = IORD_16DIRECT(TIMER_0_BASE, 0);
	//printf("Status: %x\n", status);
	if (status & 0x2) {
		printf("Timer stopped\n");
		IOWR_16DIRECT(TIMER_0_BASE, 4, 1 << 3);
	}
	if (status & 0x1) {
		printf("Reset TO\n");
		IOWR_16DIRECT(TIMER_0_BASE, 0, 0);
	}

	control = IORD_16DIRECT(TIMER_0_BASE, 4);
	//printf("Control: %x\n", control);
	irq = (control & 0x1);
	repeat = (control & 0x2) >> 1;
	if ((!irq) || (!repeat)){
		IOWR_16DIRECT(TIMER_0_BASE, 4, 3);
	}

	control = IORD_16DIRECT(TIMER_0_BASE, 4);
	//printf("New control: %x\n", control);

}

//Timer interrupt function
void timer_isr(void * context, alt_u32 irq_id)
{
	/*
	unsigned int buffer;

	buffer = (unsigned int)alt_up_rs232_get_used_space_in_read_FIFO;
	while(buffer > 0) {
		alt_up_rs232_read_data(uart, &data, &parity);
		fifo_isr++;

		if (fifo_isr > 128)
					printf("Fifo overflow\n");
		else
			fifo_buffer[fifo_isr] = data;

			buffer--;
	}
	 */
	unsigned int space = (unsigned int)alt_up_rs232_get_used_space_in_read_FIFO;
	if (space > 0)
		rs_flag = true;
	else
		rs_flag = false;

	IOWR_16DIRECT(TIMER_0_BASE, 0, 0);
}

//Starts hardware timer
void start_timer()
{
	printf("Starting Timer\n");
	alt_irq_enable(TIMER_0_IRQ);
	IOWR_16DIRECT(TIMER_0_BASE, 4, 7);

	/*
	 int control;
	control = IORD_16DIRECT(TIMER_0_BASE, 4);
	printf("New control (2): %x\n", control);
	 */
}

//Stops hardware timer
void stop_timer()
{
	printf("Stopping Timer\n");
	alt_irq_disable(TIMER_0_IRQ);
	IOWR_16DIRECT(TIMER_0_BASE, 4, 11);
	IOWR_16DIRECT(TIMER_0_BASE, 0, 0);
}
unsigned int volume_adjust(unsigned int pcm, int volume)
{
	int negative;
	if (pcm > 8388607) {
		negative = pcm - (8388608 * 2);
		negative = ((negative * volume) / 10) + (16777216);
		pcm = (unsigned int)negative;
		if(pcm > 16777215) {
			pcm = 16777215;
		}
		if ((pcm <= 16777215) && (pcm > 14680063) && (ampcounter > 1000)) {
			if (visualflag) {
			alt_up_rs232_write_data(uart, VISUAL);
			}
			ampcounter = 0;
		}
		else {
			ampcounter++;
		}


	}
	else if (pcm <= 8388607) {
		pcm = (unsigned int)((pcm * volume) / 10);
		if (pcm > 8388607) {
			pcm = 8388607;

			if ((pcm <= 8388607) && (pcm > 7340031) && (ampcounter > 1000)) {
				if (visualflag) {
					alt_up_rs232_write_data(uart, VISUAL);
				}
				ampcounter = 0;
			}
			else {
				ampcounter++;
			}

		}
	}
	return pcm;
}

//Mixes two audio samples together based on volume variable
unsigned int mix_adjust(unsigned int pcm, unsigned int pcm2, int volume)
{
	int negative = 0;
	int negative2 = 0;
	int temp = 0;
	unsigned int result = 0;
	if ((pcm > 8388607) && (pcm2 > 8388607)) {
		negative = pcm - (16777216);
		negative2 = pcm2 - (16777216);
		negative = (int)((negative * volume) / 10);
		negative2 = (int)((negative2 * volume) / 10);
		temp = negative + negative2;
		temp += 16777216;
		result = (unsigned int)temp;
		if(result > 16777215)
			result = 16777215;
	}
	else if (pcm > 8388607) {
		negative = pcm - (16777216);
		negative = (int)((negative * volume) / 10);
		pcm2 = (unsigned int)((pcm2 * volume) / 10);
		temp = negative + pcm2;
		if (temp < 0) {
			result = (unsigned int)(temp + (16777216));
			if(result > 16777215)
				result = 16777215;
		}
		else {
			result = (unsigned int)(temp);
			if(result > 8388607)
				result = 8388607;
		}
	}
	else if (pcm2 > 8388607) {
		negative2 = pcm2 - (16777216);
		negative2 = (int)((negative2 * volume) / 10);
		pcm = (unsigned int)((pcm * volume) / 10);
		temp = negative2 + pcm;
		if (temp < 0) {
			result = (unsigned int)(temp + (16777216));
			if(result > 16777215)
				result = 16777215;
		}
		else {
			result = (unsigned int)(temp);
			if(result > 8388607)
				result = 8388607;
		}
	}
	else {
		pcm = (unsigned int)((pcm * volume) / 10);
		pcm2 = (unsigned int)((pcm2 * volume) / 10);
		result = pcm + pcm2;
		if (result > 8388607) {
			result = 8388607;
		}
	}
	return result;
}


unsigned int mix_adjust2(unsigned int pcm, unsigned int pcm2)
{
	volatile int negative = 0;
	volatile	int negative2 = 0;
	volatile int temp = 0;
	volatile unsigned int result = 0;
	if ((pcm > 8388607) && (pcm2 > 8388607)) {
		pcm = (unsigned int)(pcm / 4);
		pcm2 = (unsigned int)(pcm2 / 4);
		negative = pcm - (16777216);
		negative2 = pcm2 - (16777216);
		temp = negative + negative2;
		temp += 16777216;
		result = (unsigned int)temp;
		if(result > 16777215)
			result = 16777215;
	}
	else if (pcm > 8388607) {
		pcm = (unsigned int)(pcm / 4);
		pcm2 = (unsigned int)(pcm2 / 4);
		negative = pcm - (16777216);
		temp = negative + pcm2;
		if (temp < 0) {
			result = (unsigned int)(temp + (16777216));
			if(result > 16777215)
				result = 16777215;
		}
		else {
			result = (unsigned int)(temp);
			if(result > 8388607)
				result = 8388607;
		}
	}
	else if (pcm2 > 8388607) {
		pcm = (unsigned int)(pcm / 4);
		pcm2 = (unsigned int)(pcm2 / 4);
		negative2 = pcm2 - (16777216);
		temp = negative2 + pcm;
		if (temp < 0) {
			result = (unsigned int)(temp + (16777216));
			if(result > 16777215)
				result = 16777215;
		}
		else {
			result = (unsigned int)(temp);
			if(result > 8388607)
				result = 8388607;
		}
	}
	else {
		pcm = (unsigned int)(pcm / 4);
		pcm2 = (unsigned int)(pcm2 / 4);
		result = pcm + pcm2;
		if (result > 8388607) {
			result = 8388607;
		}
	}
	return result;
}

//DE2 waits for command from Android/middleman
void listen(char * soundfiles[], int numwavfiles, audisr * audmode)
{
	printf("Listening\n");
	int volume_index_up = 10;
	int volume_index_down = 10;
	int func;
	bool headerfound = false;
	int volume;

	clear_fifo();

	//stop_timer();
	start_timer();
	while(1)
	{
		if(rs_flag == true) {
			if (headerfound == false) {

				//Loop check header
				if (rs_flag == true) {
					alt_up_rs232_read_data(uart, &data, &parity);
					if ((int)data == 1) {
						headerfound = true;
						while((int)data == 1) {
							alt_up_rs232_read_data(uart, &data, &parity);
						}
					}
				}
			}

			else {

				//Function get
				alt_up_rs232_read_data(uart, &data, &parity);
				func = (int)data;

				if (func == QUEUE) {
					alt_up_audio_disable_write_interrupt(audio);
					audmode->id = get_fnames(soundfiles,numwavfiles);
					//alt_up_audio_enable_write_interrupt(audio);
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == PLAY) {
					alt_up_audio_enable_write_interrupt(audio);
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == STOP) {
					alt_up_audio_disable_write_interrupt(audio);
					audio_isr_k = 0;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == PAUSE) {
					alt_up_audio_disable_write_interrupt(audio);
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				/*
				else if (func == VOLUME) {
					int volumetemp;

					do {
						while(rs_flag == false);
						alt_up_rs232_read_data(uart, &data, &parity);
						volumetemp = (int)data;
					}while((volumetemp == VOLUME) || (volumetemp == 0));

					//	alt_up_rs232_read_data(uart, &data, &parity);
					volume = volumetemp;

					if (volume == VOL_UP) {
						volume_index_up += 3;
						if (volume_index_up > 127) {
							volume_index_up = 127;
						}
						printf("Up %d\n", volume_index_up);
						audmode->newvolume = volume_index_up;
						volume_index_down = 10;
					}
					else if (volume == VOL_DOWN) {
						volume_index_down--;
						if (volume_index_down < 1) {
							volume_index_down = 1;
						}
						printf("Down %d\n", volume_index_down);
						audmode->newvolume = volume_index_down;
						volume_index_up = 10;
					}
					else {
						printf("Error, unknown volume value %d\n", volume);

					}
					headerfound = false;

					alt_up_rs232_write_data(uart, ACK);

				}
				 */
				else if (func == VOL_UP) {
					volume_index_up += 5;
					if (volume_index_up > 127) {
						volume_index_up = 127;
					}
					printf("Up %d\n", volume_index_up);
					audmode->newvolume = volume_index_up;
					volume_index_down = 10;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == VOL_DOWN) {
					volume_index_down--;
					if (volume_index_down < 1) {
						volume_index_down = 1;
					}
					printf("Down %d\n", volume_index_down);
					audmode->newvolume = volume_index_down;
					volume_index_up = 10;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == SELECT) {
					int index = 0;
					do {
						if (rs_flag == true)
							alt_up_rs232_read_data(uart, &data, &parity);
					}while((int)data == SELECT);
					index = (int)data;
					if ((index < 0) || (index >= numwavfiles)) {
						printf("Error, index out of bounds, playing index 0\n");
						index = 0;
					}
					alt_up_audio_disable_write_interrupt(audio);
					audmode->id = index;
					printf("Changing to song: %s, Id: %d\n", soundfiles[audmode->id], audmode->id);
					alt_up_audio_enable_write_interrupt(audio);
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == S_LOOP) {
					printf("Loop song\n");
					audmode->loop = true;
					audmode->listloop = false;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == Q_LOOP) {
					printf("List loop\n");
					audmode->listloop = true;
					audmode->loop = false;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == C_LOOP) {
					printf("Cancel loops\n");
					audmode->listloop = false;
					audmode->loop = false;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == SEND) {
					send_fnames(soundfiles, numwavfiles);
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == NEXT) {
					alt_up_audio_disable_write_interrupt(audio);
					audmode->id += 1;
					if(audmode->id >= numwavfiles) {
						audmode->id = numwavfiles - 1;
					}
					printf("Changing to song Id: %d\n", audmode->id);
					alt_up_audio_enable_write_interrupt(audio);

					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == PREVIOUS) {
					alt_up_audio_disable_write_interrupt(audio);
					audmode->id -= 1;
					if(audmode->id < 0) {
						audmode->id = 0;
					}
					printf("Changing to song Id: %d\n", audmode->id);
					alt_up_audio_enable_write_interrupt(audio);
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == MIX) {
					audmode->mode = 1;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == SHUFFLE) {
					if(audmode->shuffle == true) {
						audmode->shuffle = false;
					}
					else {
						audmode->shuffle = true;
					}
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if (func == VISUALON) {
					visualflag = !visualflag;
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
				else if ((func != 1) && (func != 0)) {
					printf("Error, Function %d not implemented yet, or invalid\n", (int)data);
					headerfound = false;
					alt_up_rs232_write_data(uart, ACK);
				}
			}
		}
	}
	stop_timer();
}
void get_filenames()
{
	unsigned int oldvalue;
	bool _numfiles = false;
	int numfiles;

	printf("Getting filenames\n");

	do {
		alt_up_rs232_read_data(uart, &data, &parity);
	}while((int)data == (MAXFILES + 1));

	while(1) {
		if (rs_flag == true)
		{
			if (_numfiles == false) {
				alt_up_rs232_read_data(uart, &data, &parity);
				numfiles = (int)data;
				_numfiles = true;
			}
			else {
				do {
					alt_up_rs232_read_data(uart, &data, &parity);
				}while((int)data == numfiles);

			}
		}
	}
}

//Copies the array pointed by copy array for audio playing
void copy_bgm(int id)
{
	int i;
	unsigned int ** temp;
	temp = arr[id];
	unsigned int * outtemp;
	unsigned int * copytemp;

	for (i = 0; i < audiosize[id]; i++) {
		outtemp = temp[i];
		copytemp = copyarr[i];
		memcpy((void*)copytemp, (void*)outtemp, (96 * sizeof(unsigned int)));
	}

}

//Function called before song is played, makes copy of current song so song can be adjusted
void init_copy(int id, int oldid)
{
	int i,j;
	unsigned int ** temp;
	temp = arr[id];
	unsigned int * outtemp;

	//printf("Initializing copy of bgm\n");

	if (copyonce > 0) {
		for (j = 0; j < audiosize[oldid]; j++)
			free(copyarr[j]);
		free(copyarr);
	}

	copyarr = (unsigned int**)malloc(audiosize[id] * sizeof(unsigned int));
	if (copyarr == NULL){
		printf("Error, copyarr null\n");
	}

	for (i = 0; i < audiosize[id]; i++) {
		outtemp = temp[i];
		copyarr[i] = (unsigned int*)malloc(96 * sizeof(unsigned int));
		if(copyarr[i] == NULL) {
			printf("Error, copyarr[i] null \n");
		}
		memcpy((void*)copyarr[i], (void*)outtemp, (96 * sizeof(unsigned int)));
	}

	copyonce++;
}

void init_mix(int id2, int id1, int oldid)
{
	int i,j;
	int small = audiosize[id1];
	int large = audiosize[id2];

	if (audiosize[id2] < audiosize[id1]) {
		small = audiosize[id2];
		large = audiosize[id1];
	}
	mixedcopy = (unsigned**)malloc(large * sizeof(unsigned int));
	if (mixedcopy == NULL){
		printf("Error, mixedcopy null\n");
	}
	for (i = 0; i < small; i++) {
		mixedcopy[i] = (unsigned int*)malloc(96 * sizeof(unsigned int));
		if(mixedcopy[i] == NULL) {
			printf("Error, mixedcopy[i] null \n");
		}
		for(j = 0; j < 96; j++) {
			unsigned * mixed = mixedcopy[i];
			//mixed = mix();
		}
	}

}
