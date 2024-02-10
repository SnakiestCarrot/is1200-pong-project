
/*  gameLogic.c:
 
  Originally created by Casper Johansson

  As the name implies, handles game logic.




*/


#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/*
 gameLogic.c

 Handles logic of the game, such as movement, positions, collisions etc. 

 Made by Casper Johansson
*/

/*
  struct to track ball parameters
*/ 
struct Ball {
  double posX;
  double posY;
  double speedX;
  double speedY;
};


/*
  struct for the paddles

  pos
*/
struct Paddle {
  double posX;
  double posY;
  double speedX;
  double speedY;
  int height;
};

// set the paddle height here
int defaultPaddleHeight = 10;

void gameLoop ( void ) {
  int timeoutcount = 0;

  struct Ball gameBall1;
  gameBall1.posX = 64.0;
  gameBall1.posY = 16.0;
  gameBall1.speedX = 40.0 / 60.0;
  gameBall1.speedY = 30.0 / 60.0;

  struct Paddle paddleR;
  paddleR.posX = 120;
  paddleR.posY = 16;
  paddleR.speedX = 0;
  paddleR.speedY = 0;
  paddleR.height = defaultPaddleHeight; 
  
  while (getsw() != 0x1) {

    // usage of the timer from lab 3
    if (IFS(0) & 0x100) {
      timeoutcount++;
      IFSCLR(0) = 0x100;
    }

    if (timeoutcount == 1) {
      
      // Clear displayBuffer from previous screen
      displayClr();

      // Ball and wall collision detection
      if (gameBall1.posX >= 127 || gameBall1.posX < 0) {
        gameBall1.speedX *= -1;
      }

      if (gameBall1.posY >= 31 || gameBall1.posY < 0) {
        gameBall1.speedY *= -1;
      }

      // Paddle and wall collison detection
      if (paddleR.posY < 0) {
        paddleR.posY = 0;
      } 
      else if ((paddleR.posY + paddleR.height) >= 32) {
        paddleR.posY = 32 - (paddleR.height + 1);
      }

      // Right paddle and ball collision detection
      int ballPaddleXCollide = (paddleR.posX - 1 <= gameBall1.posX && paddleR.posX + 2 >= gameBall1.posX);
      int ballPaddleYCollide = ((paddleR.posY - 1 <= gameBall1.posY) && (paddleR.posY + paddleR.height + 1) >= gameBall1.posY);
      int ballPaddleCollision = ballPaddleXCollide && ballPaddleYCollide;

      if (ballPaddleCollision) {
        gameBall1.speedX *= -1;
      }

      // Paddle movement
      if (btn4pressed() && paddleR.posY > -1) {
        paddleR.speedY = 45.0 / 60.0;
      } 
      else if (btn3pressed() && (paddleR.posY + 4) < 32) {
        paddleR.speedY = -45.0 / 60.0;
      } 
      else {
        paddleR.speedY = 0;
      }

      // Ball position update
      gameBall1.posX += gameBall1.speedX;
      gameBall1.posY += gameBall1.speedY;

      // Paddle position update
      paddleR.posX += paddleR.speedX;
      paddleR.posY += paddleR.speedY;

      // Paddle pixels being written to displayBuffer
      displayPaddle(paddleR.posX, paddleR.posY);

      // Ball pixels being written to displayBuffer
      displayBall(gameBall1.posX, gameBall1.posY);

      // Sending the display buffer to OLED screen
      display_image(0, displayBuffer);
      timeoutcount = 0;
    }
    
    
  }

  displaySplashMenu();
  menuState = 0;
}

















