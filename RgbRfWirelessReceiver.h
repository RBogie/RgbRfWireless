/*
 * RGB RF Wireless library v1.0.1 (20190603) made by Rob Bogie
 *
 * License: GNU LGPLv3. See license.txt
 */

#ifndef RgbRfReceiver_h
#define RgbRfReceiver_h

#include <Arduino.h>

#include "RgbRfButton.h"

struct RgbRfCode {
  uint16_t period;  //Duration of a single period in the signal. Observed to be around 400us
  uint16_t address; // Remote's address [0..2^16-1]
  RgbRfButton button;     // Which button on the remote was pressed
};

typedef void (*RgbRfReceiverCallBack)(RgbRfCode);

/**
* RgbRfReceiver decodes the signal received from a 433MHz-receiver, which is sent by a "RF Wireless" rgb remote used in a lot of chinese rgb controllers.
*
* Interrupts are disabled during the callback function. Enable them if you need. The callback will not be called as long as the previous call has not returned.
* 
* This library is designed to be as similar as possible to the NewRemoteSwitch library used for a lot of 433mhz controlled relays (such as KAKU)
*/

class RgbRfReceiver {
  public:
    /**
    * Initializes the decoder.
    *
    * If interrupt >= 0, init will register pin <interrupt> to this library.
    * If interrupt < 0, no interrupt is registered. In that case, you have to call interruptHandler()
    * yourself whenever the output of the receiver changes, or you can use InterruptChain.
    *
    * @param interrupt  The interrupt as is used by Arduino's attachInterrupt function. See attachInterrupt for details.
              If < 0, you must call interruptHandler() yourself.
    * @param minRepeats The number of times the same code must be received in a row before the callback is calles
    * @param callback Pointer to a callback function, with signature void (*func)(NewRemoteCode)
    */
    static void init(short interrupt, unsigned short minRepeats, RgbRfReceiverCallBack callback);

    /**
    * Enable decoding. No need to call enable() after init().
    */
    static void enable();

    /**
    * Disable decoding. You can re-enable decoding by calling enable();
    */
    static void disable();

    /**
    * Deinitializes the decoder. Disables decoding and detaches the interrupt handler. If you want to
    * re-enable decoding, call init() again.
    */
    static void deinit();

    /**
    * Tells wether a signal is being received. If a compatible signal is detected within the time out, isReceiving returns true.
    * Since it makes no sense to transmit while another transmitter is active, it's best to wait for isReceiving() to false.
    * By default it waits for 160ms, in which an average RgbRf signal can be broadcasted three times.
    *
    * Note: isReceiving() depends on interrupts enabled. Thus, when disabled()'ed, or when interrupts are disabled (as is
    * the case in the callback), isReceiving() will not work properly.
    *
    * @param waitMillis number of milliseconds to monitor for signal.
    * @return boolean If after waitMillis no signal was being processed, returns false. If before expiration a signal was being processed, returns true.
    */
    static boolean isReceiving(int waitMillis = 160);

    /**
     * Called every time the signal level changes (high to low or vice versa). Usually called by interrupt.
     */
    static void interruptHandler();

  private:

    static unsigned short _interrupt;     // Radio input interrupt
    volatile static short _state;    // State of decoding process. There are 49 states, 1 for "waiting for signal" and 48 for decoding the 48 edges in a valid code.
    static unsigned short _minRepeats;
    static RgbRfReceiverCallBack _callback;
    static boolean _inCallback;         // When true, the callback function is being executed; prevents re-entrance.
    static boolean _enabled;          // If true, monitoring and decoding is enabled. If false, interruptHandler will return immediately.

};

#endif
