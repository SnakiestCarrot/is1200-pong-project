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
#include <math.h>



/* Interrupt Service Routine */
void user_isr( void )                   // called from vectors.S which handles interrupts
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // port E init
  TRISE = ~(0xff);                   // set first 8 bits as output
  PORTE = PORTE & ~(0xff);

  // port D init
  TRISDSET = 0xfe0;

  // timer initialize
  T2CON = 0x0;         // clear all in T2CON
  PR2 = 5208;         // set value when to throw interrupt, 5208 gives 60 hz with 1:256 scaling
  T2CONSET = 0x70;     // set scaling to 1:256
  T2CONSET = 0x08000;  // start clock

  displaySplashMenu();
}

// registers v0 and v1 are used for returning function values so btn and sw will be put there

// the three device registers TRISE is the actual values that control the behaviour,
// TRISESET is a place in memory to affect TRISE, andd it will set TRISE corresponding
// bits to 1 if TRISESET bit is 1. TRISECLR is the opposite, it will clear bits set to 1 in
// TRISECLR in TRISE

// when we press btn2 and btn3 at the same time the expected behaviour 
// according to the assignment happens
// this is because we were told to make it that way and its because each condition for the buttons 
// are non dependent on each other so each time digit can be changed separetely

