

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>


/*
Logic for game mode 3 and 4.
By August Wikdahl, reusing code written by 
August and Casper Johansson for game logic.
*/


//Ball bounces faster and faster
void playerMode2( void ){
    double extraspeed = 1.0;
    ballMaxSpeed = 1.0;
    int stop = 0;
    int i;
    
    int timeoutcount = 0;

    gameStateInit();

    while (getsw() != 0x1 && stop == 0) {

    // usage of the timer from lab 3
    if (IFS(0) & 0x100) {
      timeoutcount++;
      IFSCLR(0) = 0x100;
    }

    // When timer gives an interrupt, do a game update
    if (timeoutcount == 1) {
      
      displayClr();

      // Ball and wall collision detection
      if (gameBall.posX >= 127) {
        scoreLeft++;
        stop = 1;
      } 
      else if (gameBall.posX < 0) {
        scoreRight++;
        stop = 1;
  
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

      // Paddles and wall collison detection
      if (paddleR.posY < 0) {
        paddleR.posY = 0;
      } 
      else if ((paddleR.posY + paddleR.height) >= 32) {
        paddleR.posY = 32 - (paddleR.height + 1);
      }

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

      /* Update : Now adds speed at every paddle bounce.*/
      //Right Paddle Collision
      if (ballPaddleCollide(&paddleR, &gameBall)) {
        extraspeed += 0.1;

        double bounceAngle = calculateBounceAngle(&paddleR, &gameBall);

        gameBall.speedX = -ballMaxSpeed * (cos(bounceAngle));
        gameBall.speedY = ballMaxSpeed * (-sin(bounceAngle));
      }

      //Left Paddle Collision
      if (ballPaddleCollide(&paddleL, &gameBall)) {
        extraspeed += 0.1;

        double bounceAngle = calculateBounceAngle(&paddleL, &gameBall);

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
      
      // Paddle movement for player
      if (playerMode == 2) {
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
   
      /*Update : Now moves with increasing speed*/
      gameBall.posX += (gameBall.speedX * extraspeed);
      gameBall.posY += (gameBall.speedY);

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

  displayWinnerScreen();
  // Reset scores
  scoreLeft = 0;
  scoreRight = 0;
}


//More balls at every bounce
void playerMode3( void ){
    ballMaxSpeed = 1.0;
    int stop = 0;
    int i;

    /*Multiple balls*/
    struct Ball *balls[5];
    struct Ball ball1 = {64.0, 16.0, -0.5, 0.0};
    struct Ball ball2 = {64.0, 16.0, 0.4, 0.0};
    struct Ball ball3 = {64.0, 16.0, -0.3, 0.0};
    struct Ball ball4 = {64.0, 16.0, 0.2, 0.0};
    struct Ball ball5 = {64.0, 16.0, -0.1, 0.0};
    balls[0] = &ball1;
    balls[1] = &ball2;
    balls[2] = &ball3;
    balls[3] = &ball4;
    balls[4] = &ball5;

    //Starting ball amount
    int noOfBalls = 1; 
    int maxBalls = 3;   
    
    int timeoutcount = 0;

    gameStateInit();

    while (getsw() != 0x1 && stop == 0) {

    // usage of the timer from lab 3
    if (IFS(0) & 0x100) {
      timeoutcount++;
      IFSCLR(0) = 0x100;
    }

    // When timer gives an interrupt, do a game update
    if (timeoutcount == 1) {
      
      displayClr();

      /*Update : checks end collision with all balls*/
      for (i = 0; i < noOfBalls; i++){
        if (balls[i]->posX >= 127) {
          scoreLeft++;
          stop = 1;
        }
        else if (balls[i]->posX < 0) {
          scoreRight++;
          stop = 1;
        }
      }

      /*Update : checks border collision with all balls*/
      for (i = 0; i < noOfBalls; i++){
        if (balls[i]->posY < 0) {
          balls[i]->posY = 0.0;
          balls[i]->speedY *= -1;
        }
        if (balls[i]->posY >= 31) {
          balls[i]->posY = 31.0;
          balls[i]->speedY *= -1;
        }
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

      /*Update : Left and Right paddle collision with all balls.*/
      for (i = noOfBalls; i >= 0; i--){
        if (ballPaddleCollide(&paddleR, balls[i])) {
          double bounceAngle = calculateBounceAngle(&paddleR, balls[i]);

          balls[i]->speedX = -ballMaxSpeed * (cos(bounceAngle));
          balls[i]->speedY = ballMaxSpeed * (-sin(bounceAngle));  
          if (noOfBalls < maxBalls){
            noOfBalls ++;
          }
        }
      }
      for (i = noOfBalls; i >= 0; i--){
        if (ballPaddleCollide(&paddleL, balls[i])) {
          double bounceAngle = calculateBounceAngle(&paddleL, balls[i]);

          balls[i]->speedX = ballMaxSpeed * cos(bounceAngle);
          balls[i]->speedY = ballMaxSpeed * -sin(bounceAngle);
          if (noOfBalls < maxBalls){
            noOfBalls ++;
          }
        }
      }

      // Change the AI difficulty by changing its movement speed
      double paddleSpeedAI;
      
      if (difficulty == 0) {
        paddleSpeedAI = 30.0;
      } else if (difficulty == 1) {
        paddleSpeedAI = 45.0;
      }
      
      // Right paddle movement for AI
      if (playerMode == 0) {
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
      else if (playerMode == 3) {
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
   
      /*Update : updates positions of all balls*/
      for (i = 0; i < noOfBalls; i++){
        balls[i]->posX += balls[i]->speedX;
        balls[i]->posY += balls[i]->speedY;
      }

      // Right paddle position update
      paddleR.posX += paddleR.speedX;
      paddleR.posY += paddleR.speedY;

      // Left paddle position update
      paddleL.posX += paddleL.speedX;
      paddleL.posY += paddleL.speedY;

      // Paddle pixels being written to displayBuffer
      displayPaddle(paddleR.posX, paddleR.posY);  // Right paddle
      displayPaddle(paddleL.posX, paddleL.posY);  // Left paddle

      /*Update : writes all position of all balls to display-buffer.*/
      for (i = 0; i < noOfBalls; i++) {
        displayBall(balls[i]->posX, balls[i]->posY);
      }

      // Sending the display buffer to OLED screen
      display_image(0, displayBuffer);
      timeoutcount = 0;
    }
  }

  displayWinnerScreen();
  // Reset scores
  scoreLeft = 0;
  scoreRight = 0;
}