#ifndef _POLLEDTOUCH_H_INCLUDED
#define _POLLEDTOUCH_H_INCLUDED

#define POLLEDTOUCH_LANDSCAPE // else Portrait (default)

#define T_DCLK     6
#define T_CS       5
#define T_DIN      4
#define T_DOUT     3

#include "CFwTimer.h"

// call one time during setup()
void ToucherSetup();

// call occasionally.  It will process 1 time every 40ms looking for a touch
// it will return TRUE one time per touch, and the variables will be updated
// with the location of that touch.
// The touch has to lift (be broken) before another touch will be registered
bool ToucherLoop(int &x, int &y, int TouchTime=0);

#endif // _POLLEDTOUCH_H_INCLUDED