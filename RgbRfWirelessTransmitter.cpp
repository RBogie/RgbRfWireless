/*
 * RGB RF Wireless library v1.0.1 (20190603) made by Rob Bogie
 *
 * License: GNU LGPLv3. See license.txt
 */

 #include "RgbRfWirelessTransmitter.h"

 RgbRfTransmitter::RgbRfTransmitter(unsigned long address, byte pin, unsigned int periodusec, byte repeats) {
 _address = address;
  _pin = pin;
  _periodusec = periodusec;
  _repeats = (1 << repeats) - 1; // I.e. _repeats = 2^repeats - 1

  pinMode(_pin, OUTPUT);
}

void RgbRfTransmitter::sendButton(RgbRfButton button) {
  for (int8_t i = _repeats; i >= 0; i--) {
    _sendAddress();
    for (int8_t i=7; i>=0; i--) {
      _sendBit(((uint8_t)button >> i) & 1);
    }
    _sendStopPulse();
  }
}

void RgbRfTransmitter::_sendAddress() {
  for (int8_t i=15; i>=0; i--) {
     _sendBit((_address >> i) & 1);
  }
}

void RgbRfTransmitter::_sendStopPulse() {
  digitalWrite(_pin, HIGH);
  delayMicroseconds(_periodusec);
  digitalWrite(_pin, LOW);
  delayMicroseconds(_periodusec * 30);
}

void RgbRfTransmitter::_sendBit(boolean isBitOne) {
  if (isBitOne) {
    // Send '1' (3T, T)
    digitalWrite(_pin, HIGH);
    delayMicroseconds(_periodusec * 3);
    digitalWrite(_pin, LOW);
    delayMicroseconds(_periodusec);
  } else {
    // Send '0' (T, 3T)
    digitalWrite(_pin, HIGH);
    delayMicroseconds(_periodusec);
    digitalWrite(_pin, LOW);
    delayMicroseconds(_periodusec * 3);
  }
}
