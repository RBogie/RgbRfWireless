/*
 * RGB RF Wireless library v1.0.1 (20190603) made by Rob Bogie
 *
 * License: GNU LGPLv3. See license.txt
 */

#include "RgbRfWirelessReceiver.h"


/************
* RgbRfReceiver

Protocol.
      __ 
'0': |  |______| (T,3T)
      ______   
'1': |      |__| (3T,T)

T = short period of ~400µs. However, this code tries
to figure out the correct period

A full frame looks like this:

- 16 bit:  Address
- 8  bit:  button
- stop pulse: 1T high, 30T low

************/

unsigned short RgbRfReceiver::_interrupt;
volatile short RgbRfReceiver::_state;
unsigned short RgbRfReceiver::_minRepeats;
RgbRfReceiverCallBack RgbRfReceiver::_callback;
boolean RgbRfReceiver::_inCallback = false;
boolean RgbRfReceiver::_enabled = false;

void RgbRfReceiver::init(short interrupt, unsigned short minRepeats, RgbRfReceiverCallBack callback) {
  _interrupt = interrupt;
  _minRepeats = minRepeats;
  _callback = callback;

  enable();
  if (_interrupt >= 0) {
    attachInterrupt(_interrupt, interruptHandler, CHANGE);
  }
}

void RgbRfReceiver::enable() {
  _state = -1;
  _enabled = true;
}

void RgbRfReceiver::disable() {
  _enabled = false;
}

void RgbRfReceiver::deinit() {
  _enabled = false;
  if (_interrupt >= 0) {
    detachInterrupt(_interrupt);
  }
}

void RgbRfReceiver::interruptHandler() {
  if (!_enabled) {
    return;
  }

  static unsigned short receivedBit;    // Contains "bit" currently receiving
  static RgbRfCode receivedCode;    // Contains received code
  static RgbRfCode previousCode;    // Contains previous received code
  static unsigned short repeats = 0;    // The number of times the an identical code is received in a row.
  static unsigned long edgeTimeStamp[3] = {0, };  // Timestamp of edges
  static unsigned int min1Period, max1Period, min3Period, max3Period;
  static bool skip;

  // Filter out too short pulses. This method works as a low pass filter.
  edgeTimeStamp[1] = edgeTimeStamp[2];
  edgeTimeStamp[2] = micros();

  if (skip) {
    skip = false;
    return;
  }

  if (_state >= 0 && edgeTimeStamp[2]-edgeTimeStamp[1] < min1Period) {
    // Last edge was too short.
    // Skip this edge, and the next too.
    skip = true;
    return;
  }

  unsigned int duration = edgeTimeStamp[1] - edgeTimeStamp[0];
  edgeTimeStamp[0] = edgeTimeStamp[1];

  // Note that if state>=0, duration is always >= 1 period.

  if (_state == -1) {
    // wait for the long low part of a stop bit.
    // Stopbit: 1T high, 30T low
    // By default 1T is 400us, but for maximum compatiblity go as low as 200us
    if (duration > 6000) { // =30*200us, minimal time between two edges before decoding starts.
      // Sync signal received.. Preparing for decoding
      repeats = 0;

      receivedCode.period = duration / 30; // Measured signal is 40T, so 1T (period) is measured signal / 40.

      // Allow for large error-margin. ElCheapo-hardware :(
      min1Period = receivedCode.period / 2; // Lower limit for 1 period is 0.5 times measured period
      max1Period = receivedCode.period * 3 / 2; // Upper limit for 1 period is 1.5 times measured period
      min3Period = max1Period; // Lower limit for 3 periods is 1.5 times measured period
      max3Period = receivedCode.period * 8; // Upper limit for 3 periods is 6 times measured period
    }
    else {
      return;
    }
  } else if (_state < 48) { // state 146 is first edge of stop-sequence. All bits before that adhere to default protocol, with exception of dim-bit
    receivedBit <<= 1;

    // One bit consists out of 4 bit parts.
    // bit part durations can ONLY be 1 or 3 periods.
    if (duration <= max1Period) {
      receivedBit &= B10; // Clear LSB of receivedBit
    }
    else if (duration >= min3Period && duration <= max3Period) {
      receivedBit |= B1; // Set LSB of receivedBit
    }
    else { // Otherwise the entire sequence is invalid
      _state = -1;
      return;
    }

    if (_state % 2 == 1) { // Check if we received a whole bit
      // There are 2 valid options for receivedBit:
      // 0, indicated by short long == B01.
      // 1, long short == B10.
      // Everything else: inconsistent data, trash the whole sequence.


      if (_state < 32) {
        // States 0 - 32 are address bit states

        receivedCode.address <<= 1;

        // Decode bit. Only 2 LSB's of receivedBit are used; trim the rest.
        switch (receivedBit & B11) {
          case B01: // Bit "0" received.
            break;
          case B10: // Bit "1" received.
            receivedCode.address |= 1;
            break;
          default: // Bit was invalid. Abort.
            _state = -1;
            return;
        }
      } else {
        receivedCode.button <<= 1;
        switch (receivedBit & B11) {
          case B01: // Bit "0" received.
            break;
          case B10: // Bit "1" received.
            receivedCode.button |= 1;
            break;
          default: // Bit was invalid. Abort.
            _state = -1;
            return;
        }
      }
    }
  } else if (_state == 48) { // Verify stop bit part 1 of 2
    // Duration must be ~1T
    if (duration < min1Period || duration > max1Period) {
      _state = -1;
      return;
    }
  } else if (_state == 49) { // Verify stop bit part 2 of 2
    // Duration must be ~30T
    if (duration < 15 * receivedCode.period || duration > 60 * receivedCode.period) {
      _state = -1;
      return;
    }

    // receivedCode is a valid code!

    if (
        receivedCode.address != previousCode.address ||
        receivedCode.button != previousCode.button
      ) { // memcmp isn't deemed safe
      repeats=0;
      previousCode = receivedCode;
    }

    repeats++;

    if (repeats>=_minRepeats) {
      if (!_inCallback) {
        _inCallback = true;
        (_callback)(receivedCode);
        _inCallback = false;
      }
      // Reset after callback.
      _state=-1;
      return;
    }

    // Reset for next round
    _state=0; // no need to wait for another sync-bit!
    return;
  }

  _state++;
  return;
}

boolean RgbRfReceiver::isReceiving(int waitMillis) {
  unsigned long startTime=millis();

  int waited; // Signed int!
  do {
    if (_state >= 12) { // Abort if at least half of a code (12 bits) has been received
      return true;
    }
    waited = (millis()-startTime);
  } while(waited>=0 && waited <= waitMillis); // Yes, clock wraps every 50 days. And then you'd have to wait for a looooong time.

  return false;
}
