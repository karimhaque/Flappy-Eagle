/* mipslabworkfuncs.c
   Contains the structure of the game along with basic Flappy Bird logic.
   This file written 2023 by Lukas Nyström and Karim Haque

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

/* defining the screen */
#define WIDTH_SCREEN 128
#define HEIGHT_SCREEN 32

/* defining the bird*/
#define WIDTH_BIRD 6
#define HEIGHT_BIRD 3

/* defining pipe width, height will change */
#define WIDTH_PIPE 4

//-------------- Global variables ---------------
int leaderboardScreen = 0; // boolean to get to highscore screen
int difficulty = 0;

//scores 
int score = 0; // current score 

// saved high scores
int highScore1 = 0;
int highScore2 = 0;
int highScore3 = 0;
extern char playerName1[];
extern char playerName2[];
extern char playerName3[];

// Coordinates for objects 

// Bird
double xbpos = 10;
double ybpos = 8; 

// Pipes 
double xp1pos = 120;
double yp1pos = 20;

double xp2pos = 200;
double yp2pos = 20;

/* The opening screen frame when you first enter the game.
Lets you choose between entering the difficulty page or leaderboard page.
*/
void openingScreen() {
   score = 0;

   clear_text();
   display_update();

   int active = 1; 

   // display 
   display_string(0, "  Flappy Eagle  ");
   display_string(2, "Difficulty: BTN2");
   display_string(3, "Leaderboard: SW4");
   display_update();

   // if BTN1 is pressed the menu stop
   while(active) {
      if (getbtns() & 1) { // BTN2 MODE
         active = 0;
      }

      if (getsw() & 8) { // SW4 HIGH SCORE
         leaderboardScreen = 1; 
         active = 0;
      }
   }
}

/*
Compares current score to the high scores
to see if the current score is a new high score
*/
void highScoreCheck() {
   if (score >= highScore1) {  //NUMBER 1 RANK
      highScore3 = highScore2;
      highScore2 = highScore1;
      highScore1 = score; 
      nameInput(1, playerName1);
   } else if (score < highScore1 && score > highScore2 && score >= highScore3) { // NUMBER 2 RANK
      highScore3 = highScore2;
      highScore2 = score;
      nameInput(2, playerName2);
   } else if (score < highScore2 && score >= highScore3) { // NUMBER 3 RANK
      highScore3 = score;
      nameInput(3, playerName3);
   }
}

/*
When colliding this function is called as game is over. Resets values as well as checks
if high score achieved.
*/
void gameOverMenu() {
   difficulty = 0;
   //reset the positions to initial position
   xbpos = 10;
   ybpos = 8;
   xp1pos = 120;
   yp1pos = 20;
   xp2pos = 200;
   yp2pos = 20;
   // func for comparing scores
   highScoreCheck();

   // game over display 
   int active = 1;
   display_string(0, "--Game Over--");
   display_string(1, "----score----");
   display_string(2, (char*)itoaconv(score)); // itoaconv -- see mipslabdisplay
   display_string(3, " menu: BTN4 ");
   display_update();

   while(active) {
      if(getbtns() & 4) { // BT4
         active = 0; 
      }
   }
}

/*
Leaderbord page, to check all the high scores
*/
void highScoreMenu() {

   // if we didnt press SW4 in the menu, we will skip this func 
   if(leaderboardScreen == 0) {
      return;
   }

   // to clear the menu text and to update the display 
   clear_text();
   display_update();
   
   char highScoreName1[12] = "";
   char highScoreName2[12] = "";
   char highScoreName3[12] = "";

   strcat(highScoreName1, playerName1);
   strcat(highScoreName1, (char*) itoaconv(highScore1));

   strcat(highScoreName2, playerName2);
   strcat(highScoreName2, (char*) itoaconv(highScore2));

   strcat(highScoreName3, playerName3);
   strcat(highScoreName3, (char*) itoaconv(highScore3));
   
   display_string(0, highScoreName1);
   display_string(1, highScoreName2);
   display_string(2, highScoreName3);
   display_string(3, " Mode: BT2");
   display_update();

   while(1){
      if(getbtns() & 1) {
         break;
      }
   }

   leaderboardScreen = 0;
}

/*
Difficulty chooser. When entering this page you can access which level you want to play.
*/
void modeMenu() {

   // to clear the menu text and to update the display 
   clear_text();
   display_update();

   display_string(0, "   Choose Mode  ");
   display_string(1, "SW3: Ezzz");
   display_string(2, "SW2: Harder");
   display_string(3, "SW1: Impossible");
   display_update();

   while(1) {
      if(getsw() & 4) {
         PR4 = (7500);
         difficulty = 1;
         break;
      }
      if(getsw() & 2) {
         PR4 = (3000);
         difficulty = 2;
         break;
      }
      if(getsw() & 1) {
         PR4 = (10);
         difficulty = 3;
         break;
      }
   }

}

/*
Before game starts, a frame is called where you get basic instructions of movement in the game.
BTN4 for left, BTN3 for up, BTN2 for right.
*/
void startGame() {

   // to clear the menu text and to update the display 
   clear_text();
   display_update();

   display_string(0, "BTN4: Left");
   display_string(1, "BTN3: Up");
   display_string(2, "BTN2: Right");
   display_string(3, "BTN3 to Play!");
   display_update();


   while(1){
      if(getbtns() & 2) {
         break;
      }
   }

}

/*
This is the rendering of the "bird". By calling the loop pixles can be generated on certain places
with draw_pixel function.
*/
void draw_bird() {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 10; j++) {
            if (
                ((j == 3 || j == 6) && (i == 0)) || // Eyes
                ((j == 1 || j == 8) && (i == 1)) || // Beak
                ((j >= 0 && j <= 9) && (i == 2)) || // Body
                ((j == 4) && (i == 3)) || // Tail
                ((j == 2 || j == 7) && (i == 4))    // Legs
            ) {
                draw_pixel((int) xbpos + j, (int) ybpos + i, (uint8_t *) screen);
            }
        }
    }
}

/*
 Movement of bird so it can jump in y position and also go in x position.
*/
void birdMovement() {

   // left
   if(getbtns() & 4) {
      if(xbpos >= 0)
         xbpos -= 0.25;
   }

   // right
   if(getbtns() & 1) {
      if(xbpos <= (127-WIDTH_BIRD))
         xbpos += 0.35;
   }
   //  up
   if(getbtns() & 2) {     //FIX SO THE BIRD DOESNT LEAVE THE SCREEN
      if(ybpos >= 0)
         ybpos -= 0.325;
   }
  
}

/*
Collission system that checks if if collides with any of the borders or pipes.
*/
int collisions() {
   int leftScreen = 0;
   int lowerScreen = HEIGHT_SCREEN;
   int upperScreen = 0;
   
   int upperBird = ybpos;
   int lowerBird = ybpos + HEIGHT_BIRD;
   int rightBird = xbpos + WIDTH_BIRD;
   int leftBird = xbpos;

   int upperPipeOne = yp1pos;
   int rightPipeOne = xp1pos + WIDTH_PIPE;
   int leftPipeOne = xp1pos;
   int upperPipeTwo = yp2pos;
   int rightPipeTwo = xp2pos + WIDTH_PIPE;
   int leftPipeTwo = xp2pos;

   // Collisions with the screen borders
    if (upperBird == upperScreen || lowerBird == lowerScreen || leftBird == leftScreen) {
        return 0;
    } else if (((lowerBird == upperPipeOne) && (rightBird >= leftPipeOne) && (leftBird <= rightPipeOne)) || 
               ((rightBird == leftPipeOne) && (lowerBird >= leftPipeOne)) || 
               ((leftBird == rightPipeOne) && (lowerBird >= upperPipeOne))) { // Collisions with the pipes 
      return 0;
    } else if (((lowerBird == upperPipeTwo) && (rightBird >= leftPipeTwo) && (leftBird <= rightPipeTwo)) || 
               ((rightBird == leftPipeTwo) && (lowerBird >= leftPipeTwo)) || 
               ((leftBird == rightPipeTwo) && (lowerBird >= upperPipeTwo))) { // Collisions with the pipes 
      return 0;
    } else {
      return 1;
    }
}

/*
Gameplay method which puts the game logic together which is then ran in the runprogram.c file.
*/
void gameplay() {
   int gameplaySesh = 0;

   int session = 1;
   while(session) {
      
      birdMovement();
      clearBytes();
      
      if (IFS(0) & 0x1000) {
         if (ybpos >= 0) {
            ybpos += 0.25;
         }
         IFSCLR(0) = 0x1000;
      }
      
      if (IFS(0) & 0x10000) {
         gameplaySesh++;
         if (xp1pos > 0) {
            xp1pos -= 1;
         }
         if (xp2pos > 0) {
            xp2pos -= 1;
         }
         IFSCLR(0) = 0x10000; // Reset
      }

      // Score increment
      if (gameplaySesh == 80) {
         score++;
         PORTE += 1;
         gameplaySesh = 0;
      }
      
      clear_text();  // Clear the previous score display

      draw_pipe_one(difficulty);
      draw_pipe_two(difficulty);
      draw_bird();
      display_image(screen);
      session = collisions();
	}
}



