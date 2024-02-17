
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>

/*
    Part of the advanced requirements to have highscores.

    The game tracks 3 highscores, where the user upon beating
    a previous top 3 score, can input their name (3 capital letters)
    and it will be saved in the "HiScores" submenu on the splashscreen.
    The saving of highscores is volatile, they will be reset if
    the system memory loses power.
*/

// Placeholder highscore strings, these will be modified in highScoreHandler
char highscorename1[] = "XXX 00";
char highscorename2[] = "YYY 00";
char highscorename3[] = "ZZZ 00";

// All highscores initialized to 0
int highScore1 = 0;
int highScore2 = 0;
int highScore3 = 0;

/*
    Copies a string on the format of the highscore strings

    Made by August Wikdahl
*/
void copyHighscoreString (char destination[], char source[]) {
    int i;
    for (i = 0; i <= 5; i++) {
        destination[i] = source[i];
    }
} 

/*
    Takes a string as input and modifies the fields (chars) corresponding
    to the highscore name and highscore itself.

    Input assumed to be a char[7] with a terminating null character

    Modifies the input string directly.

    Made by Casper Johansson
*/
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


/*
    Handles the highscore after a finished game.
*/
void highScoreHandler (int leftScore, int RightScore) {

    
    int scoreDelta = (scoreLeft - scoreRight);

    char firstLeft = (char)((scoreDelta / 10) + 48);
    char secondLeft = (char)((scoreDelta % 10) + 48);

    // FIXME: Below code is ugly, string literals are hard to manage in c
    // there is a better solution, I just havent found it
    if (highScore1 < scoreDelta) {
        copyHighscoreString(highscorename3, highscorename2);
        copyHighscoreString(highscorename2, highscorename1);
        
        highScoreInput(highscorename1);

        highscorename1[4] = firstLeft;
        highscorename1[5] = secondLeft;

        highScore3 = highScore2;
        highScore2 = highScore1;
        highScore1 = scoreDelta;
    }
    else if (highScore2 < scoreDelta) {
        copyHighscoreString(highscorename3, highscorename2);
        
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




