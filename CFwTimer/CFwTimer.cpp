//:---------------------------------------------------------------------------
//:Description
//
// The Class Framework Timer is a support base class for classes that measure elapsed time
// or simple timing support. Its static method GetSystemTime() needs the system's time in milliseconds.
//
//:---------------------------------------------------------------------------

//:ClassInc
#include "CFwTimer.h"

//:Include
#include <Arduino.h>

//:Special
#define _ThisClass    CFwTimer
#define _NumBaseClass  0

//:>                  +-----------------------------------+
//:>------------------|    Member Function Definitions    |-------------------
//:>                  +-----------------------------------+

// +---------------+
// |  Constructor  |
// +---------------+

//:Description
//
// Set timer to timeout value.
//
// Default timeout period is 0 (not timing).
//
CFwTimer::CFwTimer(
  long lTimeout):            // timeout period in ms

  //:>Private data initialization
  m_lStart(
    0),
  m_lTimerPeriod(
    0)
{
  SetTimer(lTimeout);
}

// +----------------+
// |  GetElapsed()  |
// +----------------+

//:Description
//
// Get the elasped time since timer set.
//
// Returns elasped time in ms
//
long CFwTimer::GetElapsed(void)
{
  return(GetSystemTime() - m_lStart);
}

// +------------------+
// |  GetExpiredBy()  |
// +------------------+

//:Description
//
// Get the amount of time the timer has expired.
//
// returns long ms. Value is positive if expired, negative if unexpired and
// INFINITE if timer is disabled.
//
long CFwTimer::GetExpiredBy(void)
{
  if(m_lTimerPeriod == INFINITE)
      return(INFINITE);  // special handling
  long lDelta = GetSystemTime() - (m_lStart + m_lTimerPeriod);
  return (lDelta);
}

// +------------------+
// |  GetRemaining()  |
// +------------------+

//:Description
//
// Get the remaining time in timeout period.
//
// If the period is set to INFINITE then returns INFINITE.
// If the timer is beyound the timeout period returns 0.
//
// NOTE: This routine uses the fact that twos complement subtraction will always
// yield a signed value that is the difference of the two values. This type of
// operation does not have to check for boundary conditions around sign changes of
// values. This is important when the Timer was started near the max positive
// value and the Period effectively wraps around the the maximum negative value.
//
// Returns remaining time in ms or zero if expired
//
long CFwTimer::GetRemaining(void)
{
  if(m_lTimerPeriod == INFINITE)
      return(INFINITE);
  long lDelta = (m_lStart + m_lTimerPeriod) - GetSystemTime();
  return((lDelta < 0) ? 0 : lDelta);
}

// +-------------------+
// |  GetSystemTime()  |
// +-------------------+

//:Description
//
// Get system time in milliseconds since system boot.
// We'll use the Arduino's included millis() function, which is actually
// an UNSIGNED LONG (32 bits).  We'll use it here as SIGNED LONG, which allows
// for handling clock wrap correctly.  The 32-bit signed usage allows for 2^31
// milliseconds of time to be measured (2147483647 milliseconds, or 24.85 days).
// Be aware that if your application is constantly checking an expired timer,
// after it is expired by half the measurable time (12.42 days), it is possible
// the timer could be considered 'active' again (it would wrap and become positive).
//
// The Arduino library also has a micros() function available, but is also
// an Unsigned Long (32 bits), which would then limit the elapsable time to
// 2147483647 *microseconds*, or 35.79 minutes.  Depending upon the projects
// requirements, either could be used.
//
long CFwTimer::GetSystemTime() {
  return(millis());
}

// +--------------------+
// |  IncrementTimer()  |
// +--------------------+

//:Description
//
// Increments previous timeout period by the new period.
//
// If the timer was disabled (SetTimer(INFINITE)) then calls SetTime() to start
// timing.
//
// This method is used where timing must maintain fixed intervals.
//
// returns void
//
void CFwTimer::IncrementTimer(
  long lPeriod)            // timer period to add to last set time
{
  switch(m_lTimerPeriod)
  {
  case INFINITE:
  case NOWAIT:  // timer disabled
      SetTimer(lPeriod);  // set starting now
      break;

  default:  // timer was set
      if(lPeriod == INFINITE)
      {  // turning off timer
        m_lTimerPeriod = lPeriod;
      }
      else
      {  // increment period
        m_lTimerPeriod += lPeriod;
      }
      break;
  }
}

// +-----------------------------------+
// |  IncrementTimerUnlessWayBehind()  |
// +-----------------------------------+

//:Description
//
// Increments previous timeout period by the new period, unless we
// see the timer is significantly behind (if more than 2/3 of the incrementing
// Period has already elapsed)
//
// This supports the concept of maintaining a fixed interval, but also
// can handle a huge step function (where perhaps the timer hasn't been
// serviced in a timely fashion lately)
//
// returns void
//
void CFwTimer::IncrementTimerUnlessWayBehind(long lPeriod)
{
  if (lPeriod < INFINITE/2 && GetExpiredBy() > (lPeriod*2)/3)
    SetTimer(lPeriod); // jump to it
  else
    IncrementTimer(lPeriod);
}

// +---------------+
// |  IsTimeout()  |
// +---------------+

//:Description
//
// Check if timeout has occured
//
// returns true if timeout, else false if still timing
//
bool CFwTimer::IsTimeout(void)
{
  return(!IsTiming());
}

// +--------------+
// |  IsTiming()  |
// +--------------+

//:Description
//
// Checks if timer is still timing.
//
// Returns true is still timing, false if timed out.
//
bool CFwTimer::IsTiming(void)
{
  if(GetRemaining() == 0)
  {  // Timed out
      return(false);
  }

  return(true);
}

// +----------------+
// |  ResetTimer()  |
// +----------------+

//:Description
//
// Reset timer to using current timeout period.
// The registered time-out period (m_lTimerPeriod) remains unchanged,
// causing the timer to 'restart' at the current time.
//
// returns void
//
void CFwTimer::ResetTimer(void)
{
  m_lStart = GetSystemTime();
}

// +--------------+
// |  SetTimer()  |
// +--------------+

//:Description
//
// Set timer to timeout period, which must be positive.
// This effectively limits the maximum timer to 2^31-2 milliseconds (24.85 days!)
// 2^31-1 milliseconds is the same as INFINITE
//
// returns void
//
void CFwTimer::SetTimer(
  long lTimeout)
{
  m_lStart = GetSystemTime();
  m_lTimerPeriod = lTimeout;
}

//:Special
#undef _ThisClass
#undef _NumBaseClass

