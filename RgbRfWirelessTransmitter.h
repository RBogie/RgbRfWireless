/*
 * RGB RF Wireless library v1.0.1 (20190603) made by Rob Bogie
 *
 * License: GNU LGPLv3. See license.txt
 */

#ifndef RgbRfTransmitter_h
#define RgbRfTransmitter_h

#include <Arduino.h>
#include "RgbRfButton.h"
/**
 * RgbRfTransmitter tries to simulate a remote commonly found bundled with chinese rgb controllers. These remotes operate on 433mhz.
 * 
 * Hardware required for this library: a 433MHz/434MHz SAW oscillator transmitter, e.g.
 * http://www.sparkfun.com/products/10534
 * http://www.conrad.nl/goto/?product=130428
 * 
 * Notes:
 * - I measured the period length with a logic analyzer. I can't guarantee that other remotes use the same periods; they may even
 *   have a different protocol. Use the ShowReceivedCode example to check whether the remote is supported, and the period is roughly 
 *   the same.
 * - You can copy the address of your "real" remotes, so you won't have to learn new codes into the receivers.
 *   In effect this duplicates a remote. But you can also pick a random number in the range 0..2^16-1.
 */
class RgbRfTransmitter {
  public:
  /**
    * Constructor.
    *
    * To obtain the correct period length, use the ShowReceivedCode example, or you can use an oscilloscope/logic analyzer.
    *
    * @param address  Address of this transmitter [0..2^16-1] Duplicate the address of your hardware, or choose a random number.
    * @param pin    Output pin on Arduino to which the transmitter is connected
    * @param periodusec Duration of one period, in microseconds. One bit takes 8 periods (but only 4 for 'dim' signal).
    * @param repeats  [0..8] The 2log-Number of times the signal is repeated. The actual number of repeats will be 2^repeats. 2 would be bare minimum, 4 seems robust, 8 is maximum (and overkill).
    */
    RgbRfTransmitter(unsigned long address, byte pin, unsigned int periodusec = 400, byte repeats = 3);

    /**
     * Send on/off command to the address group.
     *
     * @param RgbRfButton Which button's signal should be sent
     */
    void sendButton(RgbRfButton button);

  private:
    unsigned long _address;    // Address of this transmitter.
    byte _pin;          // Transmitter output pin
    unsigned int _periodusec; // Oscillator period in microseconds
    byte _repeats;        // Number over repetitions of one telegram

    /**
     * Transmits address part
     */
    void _sendAddress();

    /**
     * Transmits stop pulse.
     */
    void _sendStopPulse();

    /**
     * Transmits a single bit.
     *
     * @param isBitOne  True, to send '1', false to send '0'.
     */
    void _sendBit(boolean isBitOne);
};

#endif
