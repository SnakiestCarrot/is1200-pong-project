/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

volatile int* trisE;
volatile int* portE;

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // port E init
  trisE = (volatile int*) 0xbf886100;
  portE = (volatile int*) 0xbf886110;
  *trisE = ~(0xff);                   // set first 8 bits as output
  *portE = *portE & ~(0xff);

  // port D init
  TRISDSET = 0xfe0; 
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  delay( 1000 );

  if (btn4pressed()) {
    mytime = mytime & 0x0fff;
    mytime = mytime | (getsw() << 12);
  }
  if (btn3pressed()) {
    mytime = mytime & 0xf0ff;
    mytime = mytime | (getsw() << 8);

  }
  if (btn2pressed()) {
    mytime = mytime & 0xff0f;
    mytime = mytime | (getsw() << 4);
  }
  
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  (*portE)++;
  display_image(96, icon);
}


// registers v0 and v1 are used for returning function values so btn and sw will be put there

// the three device registers TRISE is the actual values that control the behaviour,
// TRISESET is a place in memory to affect TRISE, nad it will set TRISE corresponding
// bits to 1 if TRISESET bit is 1. TRISECLR is the opposite, it will clear bits set to 1 in
// TRISECLR in TRISE

// when we press btn2 and btn3 at the same time the expected beahviour 
// according to the assignment happens
// this is because we were told to make it that way and its because each condition for the buttons 
// are non dependent on each other so each time digit can be changed separetely

