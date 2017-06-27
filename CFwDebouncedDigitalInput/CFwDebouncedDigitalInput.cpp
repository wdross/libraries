//:---------------------------------------------------------------------------
//:Description
//
// The CFwDebouncedDigitalInput class adds a timer debounce before asserting the
// new state.
//
//:---------------------------------------------------------------------------

//:ClassInc
#include "CFwDebouncedDigitalInput.h"

//:Include
#include <Arduino.h>

//:Special
#define _ThisClass    CFwDebouncedDigitalInput
#define _NumBaseClass  0

//:>                  +-----------------------------------+
//:>------------------|    Member Function Definitions    |-------------------
//:>                  +-----------------------------------+

// +---------------+
// |  Constructor  |
// +---------------+

//:Description
//
// Construction: initialize input mode and stabilize initial reading
//
CFwDebouncedDigitalInput::CFwDebouncedDigitalInput(
  int input):            // what input to monitor
  //:>Private data initialization
  m_Input(input)
{
  pinMode(m_Input, INPUT_PULLUP);
  // don't know if we need to wait a bit for the reading to be valid...

  // this should be called from the main setup()
  m_Timer.SetTimer(0);
  // establish initial state
  m_bLast = (digitalRead(m_Input));
  m_bState = m_bLast;
}

// +-------------+
// |  Process()  |
// +-------------+

//:Description
//
// Once per loop(), a chance to update the debounced state
//
void CFwDebouncedDigitalInput::Process(void)
{
  bool thisReading = (digitalRead(m_Input));
  if (thisReading != m_bLast) {
    m_Timer.SetTimer(DEBOUNCE_TIMEOUT);
  }
  m_bLast = thisReading;

  // wait for state to remain unchanged for the full DEBOUNCE_TIMEOUT before
  // latching in the new reported state
  if (m_bState != thisReading &&
      m_Timer.IsTimeout()) {
    m_bState = thisReading;
  }
} // Process


//:Special
#undef _ThisClass
#undef _NumBaseClass

