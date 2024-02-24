

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

/*
Logic for game mode 3 and 4.
By August Wikdahl
*/


//Ball bounces faster and faster
//
void playerMode2( void ){
    double extraspeed = 1.0;
    ballMaxSpeed = 1.0;
    
    int timeoutcount = 0;

    gameStateInit();

    while (getsw() != 0x1) {

    // usage of the timer from lab 3
    if (IFS(0) & 0x100) {
      timeoutcount++;
      IFSCLR(0) = 0x100;
    }

    // When timer gives an interrupt, do a game update
    if (timeoutcount == 1) {
      
      // Clear displayBuffer from previous screen
      displayClr();

      // Ball and wall collision detection
      if (gameBall.posX >= 127) {
        scoreLeft++;
        gameStateInit();
      } 
      else if (gameBall.posX < 0) {
        scoreRight++;
        gameStateInit();
      }

      // collision with upper and lower borders
      if (gameBall.posY < 0) {
        gameBall.posY = 0.0;
        gameBall.speedY *= -1;
      }
      if (gameBall.posY >= 31) {
        gameBall.posY = 31.0;
        gameBall.speedY *= -1;
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

      // Needed for advanced project requirements
      if (ballTrajectoryAffectsGameplay == 1) { 
        trajectoryModifier = 1.0 - (gameBall.speedY / ballMaxSpeed);  
      } else {
        trajectoryModifier = 1.0;
      }

    // Now adds speed at every paddle bounce.
      //Right Paddle Collision
      if (ballPaddleCollide(&paddleR, &gameBall)) {
        extraspeed += 0.1;
        // Angle calculation 
        double bounceAngle = calculateBounceAngle(&paddleR, &gameBall);
        // New speeds
        gameBall.speedX = -ballMaxSpeed * (cos(bounceAngle));
        gameBall.speedY = ballMaxSpeed * (-sin(bounceAngle));
      }

      //Left Paddle Collision
      if (ballPaddleCollide(&paddleL, &gameBall)) {
        extraspeed += 0.1;
        // Angle calculation
        double bounceAngle = calculateBounceAngle(&paddleL, &gameBall);
        // New speeds
        gameBall.speedX = ballMaxSpeed * cos(bounceAngle);
        gameBall.speedY = ballMaxSpeed * -sin(bounceAngle);
      }

      // Change the AI difficulty by changing its movement speed
      double paddleSpeedAI;
      
      if (difficulty == 0) {
        paddleSpeedAI = 30.0;
      } else if (difficulty == 1) {
        paddleSpeedAI = 45.0;
      }
      
      // Right paddle movement for AI
      if (playerMode == 2) {
        int yPosCheck = gameBall.posY > paddleR.posY + (paddleR.height / 2);
        int boundsCheckUpper = paddleR.posY > -1;
        int boundsCheckLower = (paddleR.posY + 4) < 32;

        // Will wait until the player hits the paddle
        int waitForHit = gameBall.speedX > 0;

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
      gameBall.posX += (gameBall.speedX * extraspeed);
      gameBall.posY += gameBall.speedY;

      // Right paddle position update
      paddleR.posX += paddleR.speedX;
      paddleR.posY += paddleR.speedY;

      // Left paddle position update
      paddleL.posX += paddleL.speedX;
      paddleL.posY += paddleL.speedY;

      // Paddle pixels being written to displayBuffer
      displayPaddle(paddleR.posX, paddleR.posY);  // Right paddle
      displayPaddle(paddleL.posX, paddleL.posY);  // Left paddle

      // Ball pixels being written to displayBuffer
      displayBall(gameBall.posX, gameBall.posY);

      // Sending the display buffer to OLED screen
      display_image(0, displayBuffer);
      timeoutcount = 0;
    }
  }
}


//More balls at every bounce
void playerMode3( void ){

}