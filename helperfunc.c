/* helperfunc.c
  Helper file with helper methods for mipslabworkfuncs.c. Provides mostly functions to draw out objects on I/O screen. 
  This file written 2023 by Lukas Nyström and Karim Haque
  
  For copyright and licensing, see file COPYING 
*/


#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

//Height and width of the screen
#define HEIGHT_SCREEN 32
#define WIDTH_SCREEN 128

//variables for creating randomness in the pipes and spawn 

extern double xp1pos;
extern double xp2pos;
extern double yp1pos;
extern double yp2pos;

/* defining the bird*/
#define BIRD_WIDTH 6
#define BIRD_HEIGHT 3

/* defining pipe width, height will change */
#define PIPE_WIDTH 4

int randomSeed;
int randomSetterVarPipeOne;
int randomSetterVarPipeTwo;
int randomSeedOne = 0;
int randomSeedTwo = 0;
#define EASY_DIFFICULTY 1
#define HARD_DIFFICULTY 2
#define IMPOSSIBLE_DIFFICULTY 3

uint8_t screen[128*32] = {0}; //Display

int getsw( void ) {
    int switches = (PORTD >> 8) & 0xF;
    return switches;
}

int getbtns( void ) {
    int buttons = (PORTD >> 5) & 0x7;
    return buttons;
}

/*
function to draw pixels
converting x coord and y coord
and storing it in a memory array
*/
void draw_pixel(int x, int y, uint8_t *array) {

    // checks if the x and y coord are outside of the display and return if they are
    if(x <= 0 || x >= WIDTH_SCREEN || y <= 0 || y >= HEIGHT_SCREEN)
        return;
    
    // chipkit display is divided into pages with 8 pixels each vertically. 0 < y < 32
    int page = y / 8;

    /*
    the pixel's information is added to the memory array. 
    array[page * 128 + x] selects the appropriate byte in the array startd on the page and x-coordinate. 
    1 << (y - page * 8) creates a bitmask by shifting a 1-bit to the correct position within the selected byte, 
    determined by the pixel's position within the chosen page.
    The existing information for the selected pixel in the memory array 
    is combined with the new information to add the pixel to the screen. 
    This is accomplished by using the OR operation |=."
    */
    array[page * 128 + x] |= (1 << (y - page * 8));

}

// clears the screen by setting every byte to 0. 
void clearBytes() {
    int i;
    for(i = 0; i < sizeof(screen); i++) {
        screen[i] = 0; 
    }

}

int randGen() {
    if(getbtns() & 2) {
        int a = TMR4;
        randomSeed = a; 
    }

    return randomSeed; 
}

/*
this functions converts a Binary number to a decimal number
in our case, we convert the random seed from randGen() to a decimal value
We have two because we have two pipes 
we save the final value in the variables randomSeedTwo and randomSeedOne 
these variables will later be used to set the seed of the pipes 
*/
void randSeed(int oneOrTwo) {
int decimalRandomSeed = 0;
  int start = 1;
  int moduloWith10;
  int seed = randGen() * oneOrTwo;
  
  while (seed > 0) {
    moduloWith10 = seed % 10;
    decimalRandomSeed = decimalRandomSeed + moduloWith10 * start;
    seed = seed / 10;
    start = start * 2;
  }

  if (oneOrTwo == 1) {
    randomSeedOne = (decimalRandomSeed % 10);
  } else if (oneOrTwo == 2) {
    randomSeedTwo = (decimalRandomSeed % 10); //value from 0 to 9
  }
}

/*
Retrieve the value from randSeed and store it for subsequent use in generating random 
heights for pipes or enemy spawns. This ensures that each time a pipe or enemy exits 
the screen, a new random value is obtained to determine its height or spawn location.
*/
int randSetPipe(int oneOrTwo, int difficulty) {
  if (oneOrTwo == 1) {
    if (xp1pos == 127) {
      randomSetterVarPipeOne = randomSeedOne;
    } 
    if (difficulty == IMPOSSIBLE_DIFFICULTY) {
      return randomSetterVarPipeOne - 1;
    } else if (difficulty == HARD_DIFFICULTY) {
      return randomSetterVarPipeOne;
    } else {
      return randomSetterVarPipeOne + 1;
    }
  }
  
  if (oneOrTwo == 2) {
    if (xp2pos == 127) {
      randomSetterVarPipeTwo = randomSeedTwo;  
    }
    if (difficulty == IMPOSSIBLE_DIFFICULTY) {
      return randomSetterVarPipeTwo - 1;
    } else if (difficulty == HARD_DIFFICULTY) {
      return randomSetterVarPipeTwo;
    } else {
      return randomSetterVarPipeTwo + 1;
    }

  }
}

void draw_pipe_one(int difficulty) {
  int localtemp = 0;
  if (xp1pos == 0) {
    xp1pos = 127;
  }
  int i, j;
  randSeed(1);
  localtemp = randSetPipe(1, difficulty);
  if (localtemp != 0) {
    yp1pos = (2 * localtemp) + 8; // height at which pipe gets drawn
  }
  for (i = 0; i < (31 - yp1pos); i++) {
    for (j = 0; j < PIPE_WIDTH; j++) {
      draw_pixel((int)xp1pos + j, (int)yp1pos + i, (uint8_t *)screen);
    }
  }
}

void draw_pipe_two(int difficulty) {
  int localtemp = 0;
  if (xp2pos == 0) {
    xp2pos = 127;
  }
  int i, j;
  randSeed(2);  // Use a different random seed function for pipe two
  localtemp = randSetPipe(2, difficulty);
  if (localtemp != 0) {
    yp2pos = (2 * localtemp) + 8; // height at which pipe gets drawn
  }
  for (i = 0; i < (31 - yp2pos); i++) {
    for (j = 0; j < PIPE_WIDTH; j++) {
      draw_pixel((int)xp2pos + j, (int)yp2pos + i, (uint8_t *)screen);
    }
  }
}
