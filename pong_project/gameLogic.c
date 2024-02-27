

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

/*
 gameLogic.c

 Handles logic of the game, such as movement, positions, collisions etc. 

 Made by Casper Johansson and August Wikdahl
*/

// set the default paddle height here
int defaultPaddleHeight = 8;

// creating ball and paddles
struct Ball gameBall;
struct Paddle paddleR;
struct Paddle paddleL;

// Tracks the score for the left side paddle (Player)
int scoreLeft = 0;

// Tracks the score for the right side paddle (Player or AI)
int scoreRight = 0;

// Decides at what score to end the game
int scoreLimit = 10;

// Determines the sensitivity of the ball bouncing of the paddles
// it is essentially the maximum angle the ball can bounce at from horizontal
const double bounciness = (1.2 * 3.1415) / 5;

// Uses the Y speed of the ball to modify the final angle of reflection of the ball
// We can use only the Y speed since we have the constant speed of the balls total vector (ballMaxSpeed)
// from a gameplay perspective it is not very good, but it is a requirement for an advanced project
// The shallower the angle the less effect it will have
int ballTrajectoryAffectsGameplay = 1; // turn on or off if the ball trajectory affects gameplay
double trajectoryModifier;

// Determines the maximum speed at which the ball will 
// travel at as a combined speed for the x and y vectors.
double ballMaxSpeed;

//Used for gamemode with increasing speed
//Increases with each paddle bounce, for other game modes stays at 1.0.
double accelerator;

// Hitbox safety margin
double hitboxSize = 1.0;

//iterator
int i;

//Multiple balls for player mode 3
struct Ball ball1, ball2, ball3, ball4, ball5;
struct Ball *balls[5];

//Variables for balll count
int noOfBalls;
int maxBalls;

// Initializes variables changed inside the game loop
// By Casper Johansson
void gameStateInit ( void ) {
  gameBall.posX = 64.0;
  gameBall.posY = 16.0;
  gameBall.speedX = -40.0 / 60.0;
  gameBall.speedY = 0.0 / 60.0;

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

  accelerator = 1.0;
}

//Function for calculating bounce angle
double calculateBounceAngle (struct Paddle *p, struct Ball *b){
  double relativeY = ((p->posY) + ((p->height)/2)) - (b->posY);
  double intersectCoefficient = (relativeY / ((p->height) / 2)) * trajectoryModifier;
  double bounceAngle = intersectCoefficient * bounciness;
  return bounceAngle;
}

//Function for detecting ball and paddle collision
int ballPaddleCollide (struct Paddle *p, struct Ball *b){
  int ballPaddleXCollide = ((p->posX) - hitboxSize <= (b->posX) + hitboxSize &&
                              (p->posX) + hitboxSize >= (b->posX) - hitboxSize);
  int ballPaddleYCollide = (((p->posY) - hitboxSize <= (b->posY) + hitboxSize) &&
                              ((p->posY) + (p->height) + hitboxSize) >= (b->posY) - hitboxSize);

  int ballPaddleCollision = ballPaddleXCollide && ballPaddleYCollide;
  return ballPaddleCollision;
}

//Initialize balls for player mode 3
//By August Wikdahl
void ballsInit(){
    ball1.posX = 64.0;
    ball1.posY = 16.0;
    ball1.speedX = -0.5;
    ball1.speedY = 0.0;
    
    ball2.posX = 64.0;
    ball2.posY = 16.0;
    ball2.speedX = 0.4;
    ball2.speedY = 0.0;

    ball3.posX = 64.0;
    ball3.posY = 16.0;
    ball3.speedX = -0.3;
    ball3.speedY = 0.0;
    
    ball3.posX = 64.0;
    ball3.posY = 16.0; 
    ball4.speedX = 0.2;
    ball4.speedY = 0.0;
    
    ball5.posX = 64.0;
    ball5.posY = 16.0;
    ball5.speedX = -0.1;
    ball5.speedY = 0.0;

    noOfBalls = 1;
}

