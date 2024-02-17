/*
    pongUtil.c

    Has some nice utilities that are used and dont fit into another file
*/



#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/* 
  twoPower:
  takes an int y as input and returns 2^y

  Made by Casper Johansson
*/
int twoPower (int y) {
  int result = 1;
  int i = 0;
  for (i = 1; i <= y; i++) {
    result *= 2;
  }
  return result;
}