
#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

// returns switch 1-4 in bits 0-3
/*getsw:
Returns bits for switches 1-4 in bits 0-3
*/
int getsw ( void ) {
    return (PORTD & 0x0f00) >> 8;
}

/* getbtns
Returns bits for buttons 2-4 in bits 0-2
*/
int getbtns ( void ) {
    return (PORTD & 0x0e0) >> 5;
}

/* btn4pressed:
returns 1 as an int if btn4 is pressed, else 0
*/ 
int btn4pressed ( void ) {
    return (getbtns() & 0x4) >> 2;
}

/* btn3pressed:
returns 1 as an int if btn3 is pressed, else 0
*/
int btn3pressed ( void ) {
    return (getbtns() & 0x2) >> 1;
}

/* btn2pressed:
returns 1 as an int if btn2 is pressed, else 0
*/
int btn2pressed ( void ) {
    return (getbtns() & 0x1);
}

/* btn1pressed:
returns 1 as an int if btn1 is pressed, else 0

special case for btn1 since it is a different bit than 2-4
*/
int btn1pressed ( void ) {
    return (PORTF & 0x2) >> 1;
}