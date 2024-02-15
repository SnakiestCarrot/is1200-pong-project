
#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

// returns switch 1-4 in bits 0-3
int getsw ( void ) {
    return (PORTD & 0x0f00) >> 8;
}

// returns buttons 2-4 in bit 0-2
int getbtns ( void ) {
    return (PORTD & 0x0e0) >> 5;
}

// returns 1 if pressed, else 0
int btn4pressed ( void ) {
    return (getbtns() & 0x4) >> 2;
}

int btn3pressed ( void ) {
    return (getbtns() & 0x2) >> 1;
}

int btn2pressed ( void ) {
    return (getbtns() & 0x1);
}