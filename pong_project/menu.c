
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

// Used for difficulty selection menu
char difficultyStr[2][16] = {
    "AI: Easy", "AI: Hard"
};

// Used for ball speed selection menu
char ballMaxSpeedStr[2][16] = {
    "Ball Spd: 85.0", "Ball Spd: 130.0"
};

// Used for player mode selection menu
char playerModeStr[2][16] = {
    "2 Players", "1 Player VS AI"
};

// makes menu a bit simpler
char *ballStr = "Ball spd: 85.0";

int difficulty = 0;
double paddleSpeed = 40.0;
int playerMode = 0;
double ballMaxSpeed = 85.0 / 60.0;

char highscorename1[] = "XXX 00";
char highscorename2[] = "YYY 00";
char highscorename3[] = "ZZZ 00";

int highScore1 = 0;
int highScore2 = 0;
int highScore3 = 0;


void highScoreInput ( char inputName[] ) {
    char name[] = "      AAA"; // spaces to center text
    int exit = 1;

    while ( exit ) {
        display_string(0, "Input your name:");
        display_string(1, name);
        display_string(2, "");
        display_string(3, "1: save");
        display_update();

        if (btn4pressed()) {
            name[6] = (name[6] == 'Z' ? ('A') : (name[6] + 1));
            quicksleep(1000000);
        }

        if (btn3pressed()) {
            name[7] = (name[7] == 'Z' ? ('A') : (name[7] + 1));
            quicksleep(1000000);
        }

        if (btn2pressed()) {
            name[8] = (name[8] == 'Z' ? ('A') : (name[8] + 1));
            quicksleep(1000000);
        }

        if (btn1pressed()) {
            exit = 0;
        }
    }

    inputName[0] = name[6];
    inputName[1] = name[7];
    inputName[2] = name[8];
    display_string(0, "");
    display_string(1, "     saved!");
    display_string(2, "");
    display_string(3, "");
    display_update();
    quicksleep(10000000);
}

void highScoreHandler (int leftScore, int RightScore) {
    int scoreDelta = (scoreLeft - scoreRight);

    char firstLeft = (char)((scoreDelta / 10) + 48);
    char secondLeft = (char)((scoreDelta % 10) + 48);

    if (highScore1 < scoreDelta) {

        highscorename3[0] = highscorename2[0];
        highscorename3[1] = highscorename2[1];
        highscorename3[2] = highscorename2[2];
        highscorename3[4] = highscorename2[4];
        highscorename3[5] = highscorename2[5];

        highscorename2[0] = highscorename1[0];
        highscorename2[1] = highscorename1[1];
        highscorename2[2] = highscorename1[2];
        highscorename2[4] = highscorename1[4];
        highscorename2[5] = highscorename1[5];
        
        highScoreInput(highscorename1);

        highscorename1[4] = firstLeft;
        highscorename1[5] = secondLeft;

        highScore3 = highScore2;
        highScore2 = highScore1;
        highScore1 = scoreDelta;
    }
    else if (highScore2 < scoreDelta) {
        highscorename3[0] = highscorename2[0];
        highscorename3[1] = highscorename2[1];
        highscorename3[2] = highscorename2[2];
        highscorename3[4] = highscorename2[4];
        highscorename3[5] = highscorename2[5];
        
        highScoreInput(highscorename2);

        highscorename2[4] = firstLeft;
        highscorename2[5] = secondLeft;

        highScore3 = highScore2;
        highScore2 = scoreDelta;
    }
    else if (highScore3 < scoreDelta) {
        highScoreInput(highscorename3);

        highscorename3[4] = firstLeft;
        highscorename3[5] = secondLeft;

        highScore3 = scoreDelta;
    }
}

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
  display_string(0, highscorename1); // TODO: placeholders for now
  display_string(1, highscorename2);
  display_string(2, highscorename3);
  display_string(3, "4: Back");
  display_update();
}

void displayOptionsMenu ( void ) {
  display_string(0, difficultyStr[difficulty]);
  display_string(1, ballStr);
  display_string(2, playerModeStr[playerMode]);
  display_string(3, "4: Back");
  display_update();
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
            playerMode = (playerMode == 0 ? 1 : 0); // toggles players paddle speed

            display_string(2, playerModeStr[playerMode]);
            display_update();

            quicksleep(1000000); //FIXME: same as in difficulty
        }
    }
}
