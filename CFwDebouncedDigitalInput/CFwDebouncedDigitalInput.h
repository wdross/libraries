//:---------------------------------------------------------------------------
//:Description
//
// The CFwDebouncedDigitalInput class adds a timer debounce before asserting the
// new state.
// One time per loop(), call Process() to update the state.
// During processing, multiple accesses of getState() are allowed.
//
//:---------------------------------------------------------------------------
#include <CFwTimer.h>

#if !defined(CFWDEBOUNCEDDIGITALINPUT_INCLUDED)
#define CFWDEBOUNCEDDIGITALINPUT_INCLUDED

//:Special
#define _ThisClass    CFwDebouncedDigitalInput
#define _NumBaseClass  0

//:Custom
#define DEBOUNCE_TIMEOUT 20

//:End Custom

//:Class
class CFwDebouncedDigitalInput
{
  public:

  CFwDebouncedDigitalInput(
    int input);      // what input to be used

  void Process(); // once per loop(), process the debounce logic and timer

  inline bool GetState();

  private:
  int  m_Input;
  bool m_bLast;
  bool m_bState;

  CFwTimer m_Timer;
};

inline bool CFwDebouncedDigitalInput::GetState() {
  return m_bState;
}

//:Special
#undef _ThisClass
#undef _NumBaseClass

#endif                  // CFWDEBOUNCEDDIGITALINPUT_INCLUDED
