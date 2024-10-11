/* runprogram.c
    Main function of program, contains all neccesary methods and sets them together to create
    Flappy Eagle game.
*/

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

/*
Run the program trough this file
*/
uint8_t screen[128*32];

int main() {
    // configs
    config(); // timer config for screen
    configPipes(); // timer config for pipe obsticles
    configBird(); // timer config for Bird gravity
    display_init();  // Initialize the display
    display_image(screen);

    // game functions that will run in an infinite loop
    while(1) {
        openingScreen();
        highScoreMenu();
        modeMenu();
        //configPipes(); // timer config for pipe obsticles. needs to be at this place because we set the Period Timer depending on Mode  
        startGame();
        gameplay();

        // game over 
        gameOverMenu();
    }
 

    
}