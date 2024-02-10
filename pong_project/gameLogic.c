
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

int frame = 0;

/*
  Game logic initialization
*/

void gameLogicStep ( void ) {
  displayPixel(64, 16);
  displayPixel(64, 16);

  if (btn1pressed()) {
    *displayBuffer = *clearedBuffer;
  }
  
  display_image(0, displayBuffer);   
}

void gameLoop ( void ) {
  int timeoutcount = 0;

  struct Ball gameBall;

  gameBall.posX = 64.0;
  gameBall.posY = 16.0;
  gameBall.speedX = 0.5;
  gameBall.speedY = 0.5;
  
  
  while (getsw() != 0xf) {

    if (IFS(0) & 0x100) {
      timeoutcount++;
      IFSCLR(0) = 0x100; // answer to question 1
    }

    if (timeoutcount == 10) {
      displayClr();
      displayPixel(gameBall.posX, gameBall.posY);
      displayPixel(gameBall.posX + 1, gameBall.posY + 1);
      display_image(0, displayBuffer);
      timeoutcount = 0;
    }
    
    
  }
}

















