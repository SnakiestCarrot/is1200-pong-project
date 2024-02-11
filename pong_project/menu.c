
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

/*displaySplashMenu:
Displays the menu that the game starts at
*/
void displaySplashMenu ( void ) {
  display_string(0, "1: Play");
  display_string(1, "2: HiScores");
  display_string(2, "3: Options");
  display_string(3, "4: Back");
  display_update();
}

/*displayHiScoreMenu:
  Displays the menu for highscores
*/
void displayHiScoreMenu ( void ) {
  display_string(0, "ABC 123"); // TODO: placeholders for now
  display_string(1, "DEF 456");
  display_string(2, "ACE 321");
  display_string(3, "4: Back");
  display_update();
}

void displayOptionsMenu ( void ) {
  display_string(0, "AI: Easy ");
  display_string(1, "Paddle Speed: 1");
  display_string(2, "3: ");
  display_string(3, "4: Back");
  display_update();
}

void displayCountdown ( void ) {
  displayClr();

  display_string(0, "    Game will");
  display_string(1, "    start in:");
  display_string(2, "        3");
  display_string(3, "");
  display_update();

  quicksleep(6000000);

  display_string(2, "        2");
  display_update();

  quicksleep(6000000);

  display_string(2, "        1");
  display_update();

  quicksleep(6000000);

  displayClr();
}

int menuState = 0;
int splashMenu = 0;
int hiScoreMenu = 2;
int optionsMenu = 3;

int difficulty = 0;

// FIXME: Just generally how the menu works, too many if-statements, probably better with switch case

/* menuHandler:
  Handles menu state and enters the game loop

  Written by Casper Johansson
*/
void menuHandler ( void ) {
  
  /*
    Splash menu cases
  */
  if (btn1pressed() && menuState == splashMenu) {
    displayCountdown();
    gameLoop();
  }

  if (btn2pressed() && menuState == splashMenu) {
    menuState = hiScoreMenu;
    displayHiScoreMenu();
  }

  if (btn3pressed() && menuState == splashMenu) {
    menuState = optionsMenu;
    displayOptionsMenu();
  }

  /*
    Highscore menu cases
  */
  if (btn4pressed() && menuState == hiScoreMenu) {
    menuState = splashMenu;
    displaySplashMenu();
  }

  /*
    Options menu cases
  */
  if (btn1pressed() && menuState == optionsMenu) {
    if (difficulty == 0) {
        difficulty = 1; // corresponds to Hard AI
        display_string(0, "AI: Hard");
    } else {
        difficulty = 0; // corresponds to Easy AI
        display_string(0, "AI: Easy");
    }
    display_update();
    quicksleep(1000000); //FIXME: difficulty toggles super fast when pressing btn2 otherwise, maybe fix in a cleaner way
  }

  if (btn4pressed() && menuState == optionsMenu) {
    menuState = splashMenu;
    displaySplashMenu();
  }
}
