/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from mipslabfunc.c */
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
void quicksleep(int cyc);

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);
int getsw(void);
void enable_interrupt(void);

/* gameLoop:

  Is the main game loop and houses the initilization of game entities,
  handling of gamestate updates, display and displayBuffer writing

  Probably want to split this into smaller chunks for better code

   Written by Casper Johansson
*/
void gameLoop ( void );

extern int defaultPaddleHeight;

/* Declarations from mipsdata.c */
extern uint8_t displayBuffer[128*4];
extern const uint8_t clearedBuffer[128*4];

/* More functions from mipsfunc.c */
void menuHandler ( void );

/*
   Tracks the current menu state
   0 = Splash menu
   1 = Highscores menu
   2 = Options menu
*/
extern int menuState;

/* displayPixel:

   Writes a pixel into displayBuffer, at position xPos and yPos on the OLED screen

   IMPORTANT: It does not actually update the display, so displayImage(0, displayBuffer)
   needs to be called in order to actually display the written pixels on the screen

   Made by Casper Johansson
*/
void displayPixel ( int xPos, int yPos );

/* displayClr:

   Clears displayBuffer / sets all bits in displayBuffer to 1. (black screen)

   Useful for after we have sent a displayBuffer to the screen, in order to remove any
   leftovers from the previous game updates displayBuffer writes.

   Made by Casper Johansson
*/
void displayClr ( void );

/*
   Displays a credits screen for a few seconds
*/
void displayCredits( void );

/*
   for AI Difficulty level

   0 = Easy 
   1 = Hard
*/
extern int difficulty;

/* for paddle speed
   40.0 or 80.0 
   can be toggled in the options menu
*/
extern double paddleSpeed;

extern double ballMaxSpeed;

/* 
   0 = 2-player mode 
   1 = 1-player VS AI mode

   can be toggled in the options menu
*/
extern int playerMode;

void displayGameScore ( void );

void displayWinnerScreen ( void );

void displayCountdown ( void );

extern int scoreLeft;

extern int scoreRight;

extern char highscorename1[];
extern char highscorename2[];
extern char highscorename3[];

extern int highScore1;
extern int highScore2;
extern int highScore3;

char *scoreToStr ( int, int );

void highScoreHandler ( int, int );

void quicksleep ( int );

int twoPower ( int );

double calculateBounceAngle ();

int ballPaddleCollide ();

void playerMode2( void );

void playerMode3( void );

void gameStateInit( void );

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

struct Ball gameBall;
struct Paddle paddleL;
struct Paddle paddleR;

extern double trajectoryModifier;
extern int ballTrajectoryAffectsGameplay;

