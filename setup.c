/* mipslabmain.c
	Contains configuration of timers for bird, pipes, etc. Latest update 
	by Karim H and Lukas N was configuration of bird and pipe timers, the rest was produced by
	Axel I and F Lundevall.
	
	This file written 2015 by Axel Isaksson,
	modified 2015, 2017 by F Lundevall
	modified 2023 by K Haque, L Nyström

	Latest update 2023-10-11 by K Haque

	For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */



int config(void) {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;
}

/*
Timer for the pipe obsticles
*/
int configPipes(void) {
	T4CON = 0x0; // Stop timer and clear control register,
	T4CONSET = 0x0070; // set prescaler at 1:256
	TMR4 = 0x0; // Clear timer register
	T4CONSET = 0x8000; // Start timer  
}

/*
Timer for bird gravity 
So that it falls 
*/
int configBird(void) {
	T3CON = 0x0; // Stop timer and clear control register,
	T3CONSET = 0x0070; // set prescaler at 1:256
	TMR3 = 0x0; // Clear timer register
	PR3 = 3125; // Load period register for gravity
	T3CONSET = 0x8000; // Start timer
	
}

/*
// to configure btns and switches that will be used 
configBTNnSW() {
	TRISFSET = 0x2; // Button 1 to PORT F because it is the main BTN and will be used a lot so it has a single PORT. 
	TRISDSET = 0xFE0; // sw 1-4 buttons 2-4
	PORTE = 0x0;
}*/

/* Interrupt Service Routine */
void user_isr(void) {
  return;
}

