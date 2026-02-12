/**************************************************************
* Class:: CSC-615-01 Spring 2026
* Name:: Ian Ayala
* Student ID:: 923102528
* Github-Name:: ianayala01
* Project:: Assignment 1 - Traffic Light
*
* File:: Ayala_Ian_HW1_dra.c
*
* Description:: 
*
**************************************************************/


#define BCM2708_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0X200000)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>	//for open()
#include <sys/mman.h>	//for mmap()
#include <unistd.h>	//for close()

// 4kb of memory
#define BLOCK_SIZE (4*1024)

// file descriptor for /dev/mem
int mem_fd;
void *gpio_map;
// i/o access
volatile unsigned *gpio;
void setup_io();
void hangOn(double seconds);

// input must always precede OUT, input looks like this: 000
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
// output bits: 001
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)
#define GPIO_CLR *(gpio+10)

const int redPin = 23;
const int yellowPin = 24;
const int greenPin = 25;

int main(){

	setup_io();
	// initialize int to correspond to gpio pins
	int g;

	// for pins 23-25
	for (g = 23; g <= 25; g++){
		// technically would be fine if only called once
		// documentation says always to call this first to prevent accidental 011
		// basically never assuming gpio pin bits are 000
		INP_GPIO(g);
		// then set pin to output
		OUT_GPIO(g);
	}

	// cycle colors 3 times
	for (int i = 0; i < 3; i++){
		printf("GREEN\n");
		// set green to be on
		GPIO_SET = 1<<greenPin;
		// wait 6 seconds
		hangOn(6);

		printf("YELLOW\n");
		// turn green off
		GPIO_CLR = 1<<greenPin;
		// turn yellow on
		GPIO_SET = 1<<yellowPin;
		// wait 1.5 seconds
		hangOn(1.5);

		printf("RED\n");
		// turn yellow off
		GPIO_CLR = 1<<yellowPin;
		// turn red on
		GPIO_SET = 1<<redPin;
		hangOn(5);
		// turn red off
		GPIO_CLR = 1<<redPin;
	}
}

// converts seconds to microseconds for usleep() call
void hangOn(double seconds){
	int microSecs = seconds * 1000000;
	usleep(microSecs);
}


void setup_io(){

	// opens /dev/mem
	if((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0){
		printf("couldn't open /dev/mem\n");
		exit(-1);
	}

	// memory map for GPIO
	gpio_map = mmap(
		NULL,
		BLOCK_SIZE,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		mem_fd,
		GPIO_BASE
	);

	close(mem_fd);

	if (gpio_map == MAP_FAILED){
		printf("mmap error %d\n");
		exit(-1);
	}

	gpio = (volatile unsigned *)gpio_map;
}
