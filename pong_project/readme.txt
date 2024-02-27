
IS1200 student Pong Project

For the IS1200 Hardware engineering course in 
spring 2024 at KTH Royal Institute of Technology

by: August Wikdahl and Casper Johansson

Requirements:
    
    If you are using windows:
    MSYS2, can found here:
    https://www.msys2.org/ 

    If not using windows look at the below links for mcb32tools
    
    MCB32 Tools, can be found here:
    https://github.com/is1200-example-projects/mcb32tools/releases/

    Microchip ChipKIT UNO32 with I/O shield, connected with USB:
    https://www.microchip.com/ 
    https://www.electrokit.com/chipkit-uno32 

To compile and install:

    Disclaimer: As the makers of this project, we have only tested the installation process
    on windows 10 and windows 11 using MSYS2 as the terminal and your experience may vary depending
    on the OS and terminal you are using. https://github.com/is1200-example-projects/mcb32tools/releases/
    contains useful troubleshooting information if it would be needed.

    1.
    Make sure the chipkit is connected with USB to your computer.

    2.
    You might have to change the USB device in "Makefile" at line 5, most likely
    it will be ".../ttySx" where x is the number of the USB device connected to the chipkit.
    Look at https://github.com/is1200-example-projects/mcb32tools/releases/ if you are having
    problems.

    3.
    For installation you need to enter the cross-compile environment, if you havent already,
    this can be done by entering (no quotes) ". /opt/mcb32tools/environment" into the terminal window.
    This will only work if mcb32tools is already installed on your system. If you did things
    correctly you should see [mcb32] on one of the lines in the terminal window.

    4.
    Once you have entered the cross-compile environment ([mcb32] in the terminal window), 
    navigate to the directory with the unzipped contents of the pong project zip file.
    This should be the same directory this readme.txt file is in.

    5.
    Write in the terminal without quotations ("):
    "make && make install"

    The installation process should now begin and will take a few seconds depending on your system.

    When the installation process is done, the start screen for the game should
    appear on the OLED screen and you are able to play the game.

Controls:
    Menu:
    The menu can be controlled by just using the Push buttons on the I/O shield.

    Flipping SW4 up (seen with the OLED screen above switches) when in the main menu
    will display a "secret" credits screen. Make sure to flip it down again.


    Gameplay:
    When playing the game, both paddles can be controlled also via the push buttons.

    Flipping SW1 on the I/O shield upwards while playing, will instantly end the game 
    and go back to main menu if no highscore was achieved.



















