/*
 * test_led.c
 *
 *  Created on: 23 sie 2022
 *      Author: Jacek
 */


#include	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include	<stdint.h>
#include 	<errno.h>
#include 	<unistd.h>
#include 	<fcntl.h>

#define USR_LED_NUMBER		3
#define BUFF_SIZE			100


#define LEDS_PATH 	"/sys/class/leds"

void write_brightness_data(char * argumn,uint8_t led_no);
void process_trigger_mode( char * text);
void process_brightness_mode( char * value);
void write_trigger_data(uint8_t led_no , char* text);


int main(int argc, char *argv[] )
{
	printf("This Application controls the USER LED%d\n", USR_LED_NUMBER);


	/*must be 3 arguments:
	 * 1 - file_name
	 * 2 - brightness/trigger
	 * 3 - brightness or trigger value
	 */
	if(argc != 3)
	{
		printf("usage: <brightness> or <trigger>\n");
		printf("valid brightness values: <0> or <1>\n");
		printf("valid trigger values: timer, heartbeat, none,\n oneshot, default\n");
	}
	else
	{
		if(strcmp(argv[1], "trigger" ) == 0)
		{
			process_trigger_mode(argv[2]);
		}
		else if(strcmp(argv[1], "brightness" ) == 0 )
		{
			process_brightness_mode(argv[2]);
		}
		else
		{
			printf("Invalid value! Please choose \"brightness\" or \"trigger\"\n");
		}
	}
	return 0;
}

void process_trigger_mode( char *text)
{

	if(! (strcmp(text, "timer") && strcmp(text, "heartbeat") && \
			strcmp(text, "none") && strcmp(text, "oneshot") && \
			strcmp(text, "default-on") ))
	{
		write_trigger_data(USR_LED_NUMBER, text);
	}
	else
	{
		printf("Invalid trigger value\n");
		printf("Valid trigger values: heartbeat,timer,none,oneshot,default-on\n");

	}
}


void process_brightness_mode( char *value)
{
	if((strcmp(value, "0") == 0 ) || (strcmp(value, "1") == 0 ))
	{
		write_brightness_data(value, USR_LED_NUMBER);
	}
	else
	{
		printf("Invalid brightness value!\n");
		printf("Valid brightness values: 0 or 1 \n");
	}

}


void write_trigger_data(uint8_t led_no , char* text)
{
	int fd, ret =0;
	char buf[BUFF_SIZE];

	snprintf(buf,sizeof(buf),LEDS_PATH "/beaglebone:green:usr%d/trigger",led_no);
	fd = open(buf, O_WRONLY);
	if (fd <= 0) {
		perror(" write trigger error\n");
	}
	ret = write(fd, (void*)text, strlen(text));
	if(ret <= 0)
	{
		printf("Trigger value write failed!\n");
		printf("error code: %d \n",ret);
	}

}


void write_brightness_data(char * argumn,uint8_t led_no)
{
	int fd, ret =0;
	char buf[BUFF_SIZE];

	snprintf(buf,sizeof(buf),LEDS_PATH "/beaglebone:green:usr%d/brightness",led_no);
	fd = open(buf, O_WRONLY);
	if (fd <= 0) {
		perror(" write trigger error\n");
	}
	ret = write(fd, (void*)argumn, strlen(argumn));
	if(ret <= 0)
	{
		printf("Trigger value write failed!\n");
	}
}



