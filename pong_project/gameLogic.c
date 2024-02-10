
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/*
 gameLogic.c

 Handles logic of the game, such as movement, positions, collisions etc. 

 Made by Casper Johansson
*/

/*
  making struct to track ball parameters
*/ 
struct Ball {
  double posX;
  double posY;
  double speedX;
  double speedY;
};

/*
  Game logic initialization
*/
void gameLogicInit ( void ) {
  struct Ball gameBall;

  gameBall.posX = 64.0;
  gameBall.posY = 16.0;
  gameBall.speedX = 0.5;
  gameBall.speedY = 0.5;
}

void gameLogicStep ( void ) {
  int i = 0;
  for (i = 0; i < 128; i++) {
    displayPixel(i, 15);
  }
}

void gameLoop ( void ) {
  display_image(0, displayBuffer);
  gameLogicInit();
  int timeoutcount = 0;
  while (getsw() != 0xf) {
    
    if (IFS(0) & 0x100) {
      timeoutcount++;
      IFSCLR(0) = 0x100; // answer to question 1
    }

  if (timeoutcount == 10) {
    gameLogicStep();
    display_image(0, displayBuffer);
    display_update();
    timeoutcount = 0;
  }
    
    
  }
}

















