
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

/*Menu handling and display written by Casper Johansson and August Wikdahl
    except where specified.
*/

// Used for difficulty selection menu
char difficultyStr[2][16] = {
    "AI: Easy", "AI: Hard"
};

// Used for ball speed selection menu
char ballMaxSpeedStr[2][16] = {
    "Ball Spd: 85.0", "Ball Spd: 130.0"
};

// Used for player mode selection menu
char playerModeStr[4][16] = {
    "2 Players", "1 Player VS AI", "Increasing speed", "Multiple Balls"
};

// makes menu a bit simpler
char *ballStr = "Ball spd: 85.0";

int difficulty = 0;
double paddleSpeed = 40.0;
int playerMode = 0;
double ballMaxSpeed = 85.0 / 60.0;

/*displaySplashMenu:
    Displays the menu that the game starts at
*/
void displaySplashMenu ( void ) {
  display4Strings("1: Play", "2: HiScores", "3: Options", "4: Back");
}

/*displayHiScoreMenu:
    Displays the menu for highscores
*/
void displayHiScoreMenu ( void ) {
  display4Strings(highscorename1, highscorename2, highscorename3, "4: Back");
}

void displayOptionsMenu ( void ) {
  display4Strings(difficultyStr[difficulty], ballStr, playerModeStr[playerMode], "4: Back");
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
            quicksleep(1500000);    // Accidentally detects input when entering the menu otherwise
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
        by Casper Johansson and August Wikdahl.
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
            ballMaxSpeed = (ballMaxSpeed == (85.0 / 60.0) ? (130.0 / 60.0) : (85.0 / 60.0)); // toggles players paddle speed
            ballStr = (ballMaxSpeed == (85.0 / 60.0) ? ballMaxSpeedStr[0] : ballMaxSpeedStr[1]);

            display_string(1, ballStr);
            display_update();

            quicksleep(1000000); //FIXME: same as in difficulty
        } 

        // changes player mode
        if (btn3pressed()) {
            playerMode = (playerMode == 3 ? 0 : playerMode + 1); // toggles player mode

            display_string(2, playerModeStr[playerMode]);
            display_update();

            quicksleep(1000000); //FIXME: same as in difficulty
        }
    }
}
