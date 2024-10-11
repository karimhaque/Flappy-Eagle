/* namefunc.c
    Contains name input function which is used when achieving a high score.
    This file written 2023 by Lukas Nyström and Karim Haque
    For copyright and licensing, see file COPYING 
*/

#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"
#include <string.h>

#define BTN_DELAY 200000  // Adjust this delay as needed
#define MAX_NAME_LENGTH 7

char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int alphabetIndex = -1;

char playerName1[] = "--------";
char playerName2[] = "--------";
char playerName3[] = "--------";
char tempArray[] = "--------";
/*
Delay function to delay letter toggle between letters
*/
void delay(int cycles) {
    int i;
    for (i = 0; i < cycles; i++) {
        asm("nop");
    }
}

/*
Function to input name when obtaining a high score. BTN4 is to toggle between A-Z,
SW3+BTN4 is used to select letter. BTN2 picks name.
*/
void nameInput(int placement, char *playerName) {
    clear_text();
    display_update();

    display_string(0, "Name?Toggle:BTN4");
    display_string(1, "Select: SW4+BTN4");
    display_string(2, playerName);
    display_update();

    int inputIndex = 0;
    int btnPressed = 0;

    while (1) {
        if (getbtns() & 1 || inputIndex >= MAX_NAME_LENGTH) { // DONE EDITING NAME
            switch (placement) {
                case 1:
                    strncpy(playerName3, playerName2, MAX_NAME_LENGTH);
                    strncpy(playerName2, playerName1, MAX_NAME_LENGTH);
                    strncpy(playerName1, tempArray, MAX_NAME_LENGTH);
                    break;
                case 2:
                    strncpy(playerName3, playerName2, MAX_NAME_LENGTH);
                    strncpy(playerName2, tempArray, MAX_NAME_LENGTH);
                    break;
                case 3:
                    strncpy(playerName3, tempArray, MAX_NAME_LENGTH);
                    break;
            }
            
            // Reset the tempArray
            int i;
            for (i = 0; i < MAX_NAME_LENGTH; i++) {
                tempArray[i] = '-';
            }
            
            alphabetIndex = -1;
            break;
        }
        
        if ((getbtns() & 4) && !btnPressed) { // TOGGLE BETWEEN LETTERS A-Z
            btnPressed = 1;
            delay(BTN_DELAY);

            alphabetIndex = (alphabetIndex + 1) % 26;
            char letter = alphabet[alphabetIndex];
            display_string(3, &letter); // Displays current letter
            display_update();

            if (getsw() & 8) {  // SELECT LETTER SW4+BTN4
                delay(BTN_DELAY);
                if (inputIndex < MAX_NAME_LENGTH && alphabetIndex >= -1) {
                    tempArray[inputIndex] = alphabet[alphabetIndex-1];
                    inputIndex++;
                    display_string(2, tempArray);  // Updates the word for every new letter and displays it
                    display_update();
                }
            }
        } else if (!(getbtns() & 4)) {
            btnPressed = 0;
        }
    }
}
