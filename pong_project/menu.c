
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

// Used for difficulty selection menu
char difficultyStr[2][16] = {
    "AI: Easy", "AI: Hard"
};

// Used for paddle speed selection menu
char paddleStr[2][16] = {
    "Pddle Spd: 40.0", "Pddle Spd: 80.0"
};

// Used for player mode selection menu
char playerModeStr[2][16] = {
    "2 Players", "1 Player VS AI"
};

// makes menu a bit simpler
char *paddleSpdStr = "Pddle Spd: 40.0";

int difficulty = 0;
double paddleSpeed = 40.0;
int playerMode = 0;


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
  display_string(0, difficultyStr[difficulty]);
  display_string(1, paddleSpdStr);
  display_string(2, playerModeStr[playerMode]);
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

int splashMenu = 0;
int hiScoreMenu = 2;
int optionsMenu = 3;

int menuState = 0;


// FIXME: Just generally how the menu works, too many if-statements, probably better with something else

/* menuHandler:
  Handles menu state and enters the game loop

  Written by Casper Johansson
*/
void menuHandler ( void ) {

    // Backs out to splash menu
    if (btn4pressed()) {
        menuState = splashMenu;
        displaySplashMenu();
    }


    /*
        Splash menu cases
    */
    if (menuState == splashMenu) {
        
        displaySplashMenu();

        if (btn1pressed()) {
            displayCountdown();
            gameLoop();
        }

        if (btn2pressed()) {
            menuState = hiScoreMenu;
            displayHiScoreMenu();
        }

        if (btn3pressed()) {
            menuState = optionsMenu;
            displayOptionsMenu();
        }

        if (getsw() == 0x8) {
            displayCredits();
        }
    }
  

    /*
        Highscore menu cases
    */
    if (menuState == hiScoreMenu) {

    }




    /*
        Options menu cases
    */
    if (menuState == optionsMenu) {
        
        // changes AI difficulty
        if (btn1pressed()) {
            difficulty = (difficulty == 0 ? 1 : 0); // toggles difficulty
    
            display_string(0, difficultyStr[difficulty]);
            display_update();

            quicksleep(1000000); //FIXME: difficulty toggles super fast when pressing btn1 otherwise, maybe fix in a cleaner way
        }

        // changes paddle speed
        if (btn2pressed()) {
            paddleSpeed = (paddleSpeed == 40.0 ? 80.0 : 40.0); // toggles players paddle speed
            paddleSpdStr = (paddleSpeed == 40.0 ? paddleStr[0] : paddleStr[1]);

            display_string(1, paddleSpdStr);
            display_update();

            quicksleep(1000000); //FIXME: same as in difficulty
        }

        // changes player mode
        if (btn3pressed()) {
            playerMode = (playerMode == 0 ? 1 : 0); // toggles players paddle speed

            display_string(2, playerModeStr[playerMode]);
            display_update();

            quicksleep(1000000); //FIXME: same as in difficulty
        }
    }
}
