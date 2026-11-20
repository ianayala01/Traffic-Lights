/**************************************************************
* Class:: CSC-615-01 Spring 2026
* Name:: Ian Ayala
* Student ID::
* Github-Name:: ianayala01
* Project:: Assignment 1 - Traffic Light
*
* File:: Ayala_Ian_HW1_dra.c
*
* Description:: This file initializes 3 LEDs and cycles through
*	them by manipulating the Fuction Select Registers
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

// declare functions
void setup();
void waitSecs(double seconds);
void inpGpio(int pinNum);
void outGpio(int pinNum);

// locations of set and clear registers
#define GPIO_SET *(gpio+7)
#define GPIO_CLR *(gpio+10)

// this way I can easily change gpio pins
const int redPin = 23;
const int yellowPin = 24;
const int greenPin = 25;

int main(){

	setup();
	// initialize int to correspond to gpio pins
	int pin;

	// iterate through each GPIO pin in use
	for (pin = 23; pin <= 25; pin++){
		// the documentation says to clear those bits before setting it to
		// either output or an alt state, never assume they're already 000
		inpGpio(pin);
		// then set pin to output
		outGpio(pin);
	}

	// cycle colors 3 times
	for (int i = 0; i < 3; i++){
		// print statement just makes this more satisfying to me
		printf("GREEN\n");
		// set green to be on by writing a 1 to it
		GPIO_SET = 1<<greenPin;
		// wait 6 seconds
		waitSecs(6);

		printf("YELLOW\n");
		// turn green off by writing 0 to it
		GPIO_CLR = 1<<greenPin;
		// turn yellow on
		GPIO_SET = 1<<yellowPin;
		// wait 1.5 seconds
		waitSecs(1.5);

		printf("RED\n");
		// turn yellow off
		GPIO_CLR = 1<<yellowPin;
		// turn red on
		GPIO_SET = 1<<redPin;
		waitSecs(5);
		// turn red off
		GPIO_CLR = 1<<redPin;
	}
}

// excessively breaking the example macros into steps
// mostly for fun, partially for notes
void inpGpio(int pinNum){
	// figure out which FSEL current GPIO is in
	int registerIndex = pinNum / 10;
	volatile uint32_t *fsel = gpio + registerIndex;

	// now to find which bits within the register
	int pindex = pinNum % 10;
	int bitPos = pindex *3;

	// 000 is input, so simply clear those bits with AND NOT 111
	*fsel = *fsel & ~(7 << bitPos);

	printf("FSEL%d, bits: %d-%d cleared.\n", registerIndex, bitPos, bitPos+2);
}

void outGpio(int pinNum){
	// in which FSEL is the pin?
	int registerIndex = pinNum / 10;
	volatile uint32_t *fsel = gpio + registerIndex;

	// bits within register
	int pindex = pinNum % 10;
	int bitPos = pindex * 3;

	// 001 is output, so bitwise OR 001
	*fsel = *fsel | (1<< bitPos);

	printf("FSEL%d, bits: %d-%d set to 001.\n", registerIndex, bitPos, bitPos+2);
}

// converts seconds to microseconds for usleep() call
void waitSecs(double seconds){
	// this is so I can write fewer zeroes
	int microSecs = seconds * 1000000;
	usleep(microSecs);
}

void setup(){

	// open /dev/mem with read write
	mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (mem_fd < 0) {
		printf("couldn't open /dev/mem\n");
		exit(-1);
	}

	// memory map for GPIO to virtual mem
	gpio_map = mmap(
		NULL,
		BLOCK_SIZE,
		// this lets the program read/write
		PROT_READ|PROT_WRITE,
		// other processes 
		MAP_SHARED,
		mem_fd,
		GPIO_BASE
	);

	// /dev/mem is no longer needed, close it
	close(mem_fd);

	if (gpio_map == MAP_FAILED){
		printf("mmap error\n");
		exit(-2);
	}

	gpio = (volatile unsigned *)gpio_map;
}
