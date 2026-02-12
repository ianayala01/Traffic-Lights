/**************************************************************
* Class:: CSC-615-01 Spring 2026
* Name:: Ian Ayala
* Student ID:: 923102528
* Github-Name:: ianayala01
* Project:: Assignment 1 - Traffic Light
*
* File:: Ayala_Ian_HW1_pigpio.c
*
* Description:: 
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <pigpio.h>


const int redPin = 23;
const int yellowPin = 24;
const int greenPin = 25;

int main(void){
	// initialize pi-gpio library
	int init = gpioInitialise();
	printf("initialized: %d\n", init);
	// set GPIO pins for each light as OUTPUT
	// 0 is input, 1 is output
	gpioSetMode(redPin, 1);
	gpioSetMode(yellowPin, 1);
	gpioSetMode(greenPin, 1);

	// light cycle should repeat 3 times
	for (int i = 3; i > 0; i--){

		printf("GREEN\n");
		// set green pin to be ON
		gpioWrite(greenPin, 1);
		// wait 6 seconds
		time_sleep(6);

		printf("YELLOW\n");
		// set green pin to be OFF
		gpioWrite(greenPin,0);
		// yellow pin ON
		gpioWrite(yellowPin, 1);
		// wait 1.5 seconds
		time_sleep(1.5);

		printf("RED\n");
		// set yellow to OFF
		gpioWrite(yellowPin, 0);
		// set red to ON
		gpioWrite(redPin, 1);
		// wait 5 seconds
		time_sleep(5);
		// turn red off
		gpioWrite(redPin, 0);
	}

	// Make sure to end gpio before exiting program
	// resets DMA channels, releases memory, terminates running threads
	gpioTerminate();
}

