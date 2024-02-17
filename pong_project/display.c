/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson
   Updated for Pong Project by Casper Johansson and August Wikdahl 2024

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

const int pageWidth = 128;
const int pageHeight = 8;

// Change this to make ball larger, measured in pixels
int ballSize = 2;


uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

/* display4Strings:

  Takes 4 strings as input and writes them to the corresponding line
  on the display, then updates the display so the display actually shows the strings
*/
void display4Strings ( char *line0, char *line1, char *line2, char *line3 ) {
  display_string(0, line0);
  display_string(1, line1);
  display_string(2, line2);
  display_string(3, line3);
  display_update();
}

void display_image(int x, const uint8_t *data) {
	int i, j;
	
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 128; j++)
			spi_send_recv(~data[i*128 + j]);
	}
}

void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void displayPixel (int xPos, int yPos) {
  
  
  // Check if index is outside screen
  if (yPos < 0 || yPos >= 32 || xPos < 0 || xPos >= 128) {
    return;
  }

  int yByte; // for storing yPos as a byte
  
  if (yPos < 8) {
    yByte = twoPower(yPos);
    displayBuffer[xPos] &= (~yByte);
  }

  if (yPos >= 8 && yPos < 16) {
    yByte = twoPower(yPos - pageHeight);
    displayBuffer[xPos + pageWidth] &= (~yByte);
  }

  if (yPos >= 16 && yPos < 24) {
    yByte = twoPower(yPos - (pageHeight * 2));
    displayBuffer[xPos + (pageWidth * 2)] &= (~yByte);
  }

  if (yPos >= 24 && yPos < 32) {
    yByte = twoPower(yPos - (pageHeight * 3));
    displayBuffer[xPos + (pageWidth * 3)] &= (~yByte);
  }
  return;
}

/*
  Writes a ball ((ballSize)x(ballSize) pixels) with its upper left corner
  on the specified x and y coordinates into the display buffer

  Made by Casper Johansson
*/
void displayBall (double x, double y) {
  int i, j;
  int ballX = (int) x; // casting solves visual bug
  int ballY = (int) y; // casting solves visual bug
  for (i = 0; i < ballSize; i++) {
    for (j = 0; j < ballSize; j++) {
      displayPixel(ballX + j, ballY + i);
    }
  }
}

/*
  Writes a paddle into the display buffer with its
  top-most pixel on the specified x and y coordinates
  into the display buffer

  Made by Casper Johansson
*/
void displayPaddle (double x, double y) {
  // typecasting here solves a rare bug
  int paddleX = (int) x;
  int paddleY = (int) y;
  
  int i = 0;
  for (i = 0; i < defaultPaddleHeight; i++) {
    displayPixel(paddleX, paddleY + i);
  }
}

void displayClr ( void ) {
  int byte = 0;
  for (byte = 0; byte < 512; byte++) {
    displayBuffer[byte] = 255;
  }
}

void displayCredits ( void ) {
  display4Strings("Made by;", "August Wikdahl", "Casper", "Johansson");
  quicksleep(30000000);
}

char scoreStr[] = "       -  ";

char *scoreToStr (int scoreLeft, int scoreRight) {
  
  char firstLeft = (char)((scoreLeft / 10) + 48);
  char secondLeft = (char)((scoreLeft % 10) + 48);

  char firstRight = (char)((scoreRight / 10) + 48);
  char secondRight = (char)((scoreRight % 10) + 48);

  scoreStr[4] = firstLeft;
  scoreStr[5] = secondLeft;

  scoreStr[9] = firstRight;
  scoreStr[10] = secondRight;

  return scoreStr;
}

void displayGameScore ( void ) {
  display4Strings("    L    R", scoreToStr(scoreLeft, scoreRight),"","");
  quicksleep(10000000);
}

void displayWinnerScreen ( void ) {
    display_string(1, scoreToStr(scoreLeft, scoreRight));
    display_string(2, "");
    display_string(3, "");
    
    if (scoreLeft > scoreRight) {
        display_string(0, "   Left wins!");
    }

    else if (scoreLeft < scoreRight) {
        display_string(0, "  Right wins!");
    }

    else {
        display_string(0, " It's a draw!");
    }

    display_update();
    quicksleep(10000000);
}

void displayCountdown ( void ) {
  displayClr();

  display4Strings("    Game will", "    start in:", "        3", "");

  quicksleep(6000000);

  display_string(2, "        2");
  display_update();

  quicksleep(6000000);

  display_string(2, "        1");
  display_update();

  quicksleep(6000000);

  displayClr();
}