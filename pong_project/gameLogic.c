

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

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
*/
struct Paddle {
  double posX;
  double posY;
  double speedX;
  double speedY;
  int height;
};

// set the default paddle height here
int defaultPaddleHeight = 8;

// creating ball and paddles
struct Ball gameBall1;
struct Paddle paddleR;
struct Paddle paddleL;

// Tracks the score for the left side paddle (Player)
int scoreLeft;

// Tracks the score for the right side paddle (Player or AI)
int scoreRight;

// Determines the sensitivity of the ball bouncing of the paddles
const double MAXBOUNCEANGLE = (1.2 * 3.1415) / 5;

// Determines the maximum speed at which the ball will 
// travel at as a combined speed for the x and y vectors.
double ballMaxSpeed;

// Initializes variables changed inside the game loop
void gameStateInit ( void ) {
  int scoreLeft = 0;
  int scoreRight = 0;
  
  gameBall1.posX = 64.0;
  gameBall1.posY = 16.0;
  gameBall1.speedX = -40.0 / 60.0;
  gameBall1.speedY = 15.0 / 60.0;

  paddleR.posX = 120;
  paddleR.posY = 16;
  paddleR.speedX = 0;
  paddleR.speedY = 0;
  paddleR.height = defaultPaddleHeight;

  
  paddleL.posX = 8;
  paddleL.posY = 16;
  paddleL.speedX = 0;
  paddleL.speedY = 0;
  paddleL.height = defaultPaddleHeight;

  const double MAXBOUNCEANGLE = (1.2 * 3.1415) / 5;
  double ballMaxSpeed = 85.0 / 60.0;
}

void gameLoop ( void ) {
  int timeoutcount = 0;

  gameStateInit();
  ballMaxSpeed = 85.0 / 60.0;
  
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

      // collison with left and right borders
      if (gameBall1.posX >= 127) {
        scoreLeft++;
        displayGameScore();
        gameStateInit();
      } 
      else if (gameBall1.posX < 0) {
        scoreRight++;
        displayGameScore();
        gameStateInit();
      }

      // collision with upper and lower borders
      if (gameBall1.posY >= 31 || gameBall1.posY < 0) {
        gameBall1.speedY *= -1;
      }

      // Right paddle and wall collison detection
      if (paddleR.posY < 0) {
        paddleR.posY = 0;
      } 
      else if ((paddleR.posY + paddleR.height) >= 32) {
        paddleR.posY = 32 - (paddleR.height + 1);
      }

      // Left paddle and wall collison detection
      if (paddleL.posY < 0) {
        paddleL.posY = 0;
      } 
      else if ((paddleL.posY + paddleL.height) >= 32) {
        paddleL.posY = 32 - (paddleL.height + 1);
      }

      // FIXME:
      // below needs to be cleaned up

      // Right paddle and ball collision detection
      int ballRPaddleXCollide = (paddleR.posX - 1.0 <= gameBall1.posX + 1.0 && paddleR.posX + 1.0 >= gameBall1.posX - 1.0);
      int ballRPaddleYCollide = ((paddleR.posY - 1.0 <= gameBall1.posY + 1.0) && (paddleR.posY + paddleR.height + 1.0) >= gameBall1.posY - 1.0);
      int ballRPaddleCollision = ballRPaddleXCollide && ballRPaddleYCollide;

      // Left paddle and ball collision detection
      int ballLPaddleXCollide = (paddleL.posX - 1.0 <= gameBall1.posX + 1.0 && paddleL.posX + 1.0 >= gameBall1.posX - 1.0);
      int ballLPaddleYCollide = ((paddleL.posY - 1.0 <= gameBall1.posY + 1.0) && (paddleL.posY + paddleR.height + 1.0) >= gameBall1.posY - 1.0);
      int ballLPaddleCollision = ballLPaddleXCollide && ballLPaddleYCollide;

      if (ballRPaddleCollision) {
        // Angle calculation
        double relativeY = (paddleR.posY + (paddleR.height/2)) - gameBall1.posY;
        double intersectCoefficient = relativeY / (paddleR.height + 20.0);
        double bounceAngle = intersectCoefficient * MAXBOUNCEANGLE;

        // New speeds
        gameBall1.speedX = -ballMaxSpeed * cos(bounceAngle);
        gameBall1.speedY = ballMaxSpeed * -sin(bounceAngle);
      }

      if (ballLPaddleCollision) {
        // Angle calculation
        double relativeY = (paddleL.posY + (paddleL.height/2)) - gameBall1.posY;
        double intersectCoefficient = relativeY / (paddleL.height / 2);
        double bounceAngle = intersectCoefficient * MAXBOUNCEANGLE;

        // New speeds
        gameBall1.speedX = ballMaxSpeed * cos(bounceAngle);
        gameBall1.speedY = ballMaxSpeed * -sin(bounceAngle);
      }

      // Change the AI difficulty by changing its movement speed
      double paddleSpeedAI;
      
      if (difficulty == 0) {
        paddleSpeedAI = 27.0;
      } else if (difficulty == 1) {
        paddleSpeedAI = 36.0;
      }
      
      // Right paddle movement for AI
      if (playerMode == 1) {
        int yPosCheck = gameBall1.posY > paddleR.posY + (paddleR.height / 2);
        int boundsCheckUpper = paddleR.posY > -1;
        int boundsCheckLower = (paddleR.posY + 4) < 32;

        // Will wait until the player hits the paddle
        int waitForHit = gameBall1.speedX > 0;

        if (yPosCheck && boundsCheckUpper && waitForHit) {
          paddleR.speedY = paddleSpeedAI / 60.0;
        } 
        else if (~yPosCheck && boundsCheckLower && waitForHit) {
          paddleR.speedY = -paddleSpeedAI / 60.0;
        } 
        else {
          paddleR.speedY = 0;
        }
      } 
      // Paddle movement for player
      else if (playerMode == 0) {
        if (btn2pressed() && paddleR.posY > -1) {
          paddleR.speedY = paddleSpeed / 60.0;
        } 
        else if (btn1pressed() && (paddleR.posY + 4) < 32) {
          paddleR.speedY = -paddleSpeed / 60.0;
        } 
        else {
          paddleR.speedY = 0;
        }
      }

      // Left paddle movement
      if (btn4pressed() && paddleL.posY > -1) {
        paddleL.speedY = paddleSpeed / 60.0;
      } 
      else if (btn3pressed() && (paddleL.posY + 4) < 32) {
        paddleL.speedY = -paddleSpeed / 60.0;
      } 
      else {
        paddleL.speedY = 0;
      }

      // Ball position update
      gameBall1.posX += gameBall1.speedX;
      gameBall1.posY += gameBall1.speedY;

      // Right paddle position update
      paddleR.posX += paddleR.speedX;
      paddleR.posY += paddleR.speedY;

      // Left paddle position update
      paddleL.posX += paddleL.speedX;
      paddleL.posY += paddleL.speedY;

      // Paddle pixels being written to displayBuffer
      displayPaddle(paddleR.posX, paddleR.posY);
      displayPaddle(paddleL.posX, paddleL.posY);

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

