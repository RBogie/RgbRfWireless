/*
 * RGB RF Wireless library v1.0.0 (20190603) made by Rob Bogie
 *
 * License: GNU LGPLv3. See license.txt
 */

#ifndef RgbRfButton_h
#define RgbRfButton_h

#include <stdint.h>

enum RgbRfButton : uint8_t {
  Power       = 0x1,
  ModePlus    = 0x5,
  SpeedMinus  = 0x7,
  Demo        = 0x8,
  SpeedPlus   = 0x9,
  ColorPlus   = 0xA,
  ModeMinus   = 0xB,
  BrightPlus  = 0xC,
  ColorMinus  = 0xD,
  White       = 0xE,
  BrightMinus = 0xF,
  Red         = 0x10,
  Green       = 0x11,
  DarkBlue    = 0x12,
  Yellow      = 0x13,
  LightBlue   = 0x14,
  Pink        = 0x15,
};

#endif
