/*********************************************************************************
 * @file	main.c
 * @Author	Jacek
 * @Created on: 25 sie 2022
 * @brief	This app is counting up or counting down on a 7-segment display
 * 			steering by BeagleBone Black
 ********************************************************************************/


#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define GPIO_PATH 		"/sys/class/gpio"
#define BUF_SIZE 		100

#define HIGH_STATE		1
#define LOW_STATE		0

#define GPIO_DIR_IN 	LOW_STATE
#define GPIO_DIR_OUT 	HIGH_STATE

#define SEG_ON 			HIGH_STATE
#define SEG_OFF 		LOW_STATE

#define GPIO_66     66
#define GPIO_67     67
#define GPIO_69     69
//#define GPIO_68     68
#define GPIO_45     45
#define GPIO_44     44
#define GPIO_26     26
#define GPIO_46     46



#define SEG_A	GPIO_66
#define SEG_B	GPIO_67
#define SEG_C	GPIO_69
#define SEG_D	GPIO_45
#define SEG_E	GPIO_44
#define SEG_F	GPIO_26
#define SEG_G	GPIO_46

/*====================================================================================
 * BBB_expansion_header_P8_pins           GPIO number            7Seg Display segment
 * ===================================================================================
 * P8.7                                   GPIO_66                     a
 * P8.8                                   GPIO_67                     b
 * P8.9                                   GPIO_69                     c
 * P8.10                                  GPIO_68                     h //decimal point
 * P8.11                                  GPIO_45                     d
 * P8.12                                  GPIO_44                     e
 * P8.14                                  GPIO_26                     f
 * P8.16                                  GPIO_46                     g
 * =================================================================================== */

void up_counting(char * delay);
void gpios_init(void);
void write_number_to_disp(uint8_t number);
void gpio_set_value(uint32_t gpio_num, uint8_t value);
void gpio_set_dir(uint8_t gpio_num, uint8_t direction);
void down_counting(char * delay);

//direction -> in or out
//value -> 0 or 1

uint16_t segment_array[] = {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G };
uint16_t number_array[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};


int main(int argc, char *argv[])
{

	if( argc != 3)
	{
        printf( "usage: %s <direction> <delay>\n", argv[0] );
        printf( "valid direction : up, down\n");
        printf ("recommended delay range in ms : 0 to 1000\n");
	}
	else
	{

		if(! strcmp(argv[1],"up"))
		{
			gpios_init();
			up_counting(argv[2]);
		}
		else if(! strcmp(argv[1],"down"))
		{
			gpios_init();
			down_counting(argv[2]);
		}
		else
		{
			printf("Invalid <direction> argument\n");
		}

	}

return 0;
}



void write_number_to_disp(uint8_t number)
{
	uint8_t x = 0;

	while(x <= 9)
	{
		if(number_array[number] & (1<<x) )
		{
			gpio_set_value(segment_array[x],SEG_ON);
		}
		else
		{
			gpio_set_value(segment_array[x],SEG_OFF);
		}
		x++;
	}

	/*
	switch(number)
	{
	case 0:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_ON);
			gpio_set_value(SEG_E,SEG_ON);
			gpio_set_value(SEG_F,SEG_ON);
			gpio_set_value(SEG_G,SEG_OFF);
			break;
	case 1:	gpio_set_value(SEG_A,SEG_OFF);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_OFF);
			gpio_set_value(SEG_E,SEG_OFF);
			gpio_set_value(SEG_F,SEG_OFF);
			gpio_set_value(SEG_G,SEG_OFF);
			break;
	case 2:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_OFF);
			gpio_set_value(SEG_D,SEG_ON);
			gpio_set_value(SEG_E,SEG_ON);
			gpio_set_value(SEG_F,SEG_OFF);
			gpio_set_value(SEG_G,SEG_ON);
			break;
	case 3:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_ON);
			gpio_set_value(SEG_E,SEG_OFF);
			gpio_set_value(SEG_F,SEG_OFF);
			gpio_set_value(SEG_G,SEG_ON);
			break;
	case 4:	gpio_set_value(SEG_A,SEG_OFF);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_OFF);
			gpio_set_value(SEG_E,SEG_OFF);
			gpio_set_value(SEG_F,SEG_ON);
			gpio_set_value(SEG_G,SEG_ON);
			break;
	case 5:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_OFF);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_ON);
			gpio_set_value(SEG_E,SEG_OFF);
			gpio_set_value(SEG_F,SEG_ON);
			gpio_set_value(SEG_G,SEG_ON);
			break;
	case 6:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_OFF);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_ON);
			gpio_set_value(SEG_E,SEG_ON);
			gpio_set_value(SEG_F,SEG_ON);
			gpio_set_value(SEG_G,SEG_ON);
			break;
	case 7:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_OFF);
			gpio_set_value(SEG_E,SEG_OFF);
			gpio_set_value(SEG_F,SEG_OFF);
			gpio_set_value(SEG_G,SEG_OFF);
			break;
	case 8:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_ON);
			gpio_set_value(SEG_E,SEG_ON);
			gpio_set_value(SEG_F,SEG_ON);
			gpio_set_value(SEG_G,SEG_ON);
			break;
	case 9:	gpio_set_value(SEG_A,SEG_ON);
			gpio_set_value(SEG_B,SEG_ON);
			gpio_set_value(SEG_C,SEG_ON);
			gpio_set_value(SEG_D,SEG_ON);
			gpio_set_value(SEG_E,SEG_OFF);
			gpio_set_value(SEG_F,SEG_ON);
			gpio_set_value(SEG_G,SEG_ON);
	}*/
}

void gpio_set_value(uint32_t gpio_num, uint8_t value)
{
	int fd = 0;
	char buf[BUF_SIZE] = {0};

	snprintf(buf, sizeof(buf),GPIO_PATH "/gpio%d/value", gpio_num );

	fd = open(buf,O_WRONLY );
	if(fd < 0)
	{
		perror("value awario\n");

	}
	if(value == 0)
		 write(fd, "0", 2);
	else if ( value == 1)
		 write(fd, "1", 2);

	close(fd);
}

void down_counting(char * delay)
{
	int delay_cnt = atoi(delay);
	for(int counter = 9; counter >=0 ; counter--)
	{
		write_number_to_disp(counter);
		usleep(delay_cnt*1000);
	}
	gpio_set_value(SEG_A,SEG_OFF);
	gpio_set_value(SEG_B,SEG_OFF);
	gpio_set_value(SEG_C,SEG_OFF);
	gpio_set_value(SEG_D,SEG_OFF);
	gpio_set_value(SEG_E,SEG_OFF);
	gpio_set_value(SEG_F,SEG_OFF);
	gpio_set_value(SEG_G,SEG_OFF);
}


void up_counting(char * delay)
{
	int delay_cnt = atoi(delay);
	for(uint8_t counter = 0; counter <=9 ; counter++)
	{
		write_number_to_disp(counter);
		usleep(delay_cnt*1000);
	}
	gpio_set_value(SEG_A,SEG_OFF);
	gpio_set_value(SEG_B,SEG_OFF);
	gpio_set_value(SEG_C,SEG_OFF);
	gpio_set_value(SEG_D,SEG_OFF);
	gpio_set_value(SEG_E,SEG_OFF);
	gpio_set_value(SEG_F,SEG_OFF);
	gpio_set_value(SEG_G,SEG_OFF);
}

void gpio_set_dir(uint8_t gpio_num, uint8_t direction)
{
	int fd;
	char buf[BUF_SIZE] = {0};

	snprintf(buf, sizeof(buf),GPIO_PATH "/gpio%d/direction", gpio_num );

	fd = open(buf,O_WRONLY );
	if(fd<0)
	{
		perror("direkszyn awario\n");
	}
	if(direction == 0)
		write(fd, "in", 3);
	else if ( direction == 1)
		write(fd, "out", 4);

	close(fd);

}

void gpios_init(void)
{
	uint8_t cnt =0;
	while( cnt <7)
	{
		gpio_set_dir(segment_array[cnt], GPIO_DIR_OUT);
	}

	cnt = 0;

	while( cnt <7)
	{
		gpio_set_value(segment_array[cnt], SEG_OFF);
	}

	/*
	gpio_set_dir(SEG_A, GPIO_DIR_OUT);
	gpio_set_dir(SEG_B, GPIO_DIR_OUT);
	gpio_set_dir(SEG_C, GPIO_DIR_OUT);
	gpio_set_dir(SEG_D, GPIO_DIR_OUT);
	gpio_set_dir(SEG_E, GPIO_DIR_OUT);
	gpio_set_dir(SEG_F, GPIO_DIR_OUT);
	gpio_set_dir(SEG_G, GPIO_DIR_OUT);

	gpio_set_value(SEG_A,SEG_OFF);
	gpio_set_value(SEG_B,SEG_OFF);
	gpio_set_value(SEG_C,SEG_OFF);
	gpio_set_value(SEG_D,SEG_OFF);
	gpio_set_value(SEG_E,SEG_OFF);
	gpio_set_value(SEG_F,SEG_OFF);
	gpio_set_value(SEG_G,SEG_OFF);
	*/
}

