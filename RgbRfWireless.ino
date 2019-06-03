/*
 * Demo for RF remote switch receiver. 
 * For details, see RgbRfWirelessReceiver.h!
 *
 * This sketch shows how to send signals to a led controller which works with "RF Wireless" remotes
 * 
 * Connect the transmitter to digital pin 11. 
 * When run, open the serial console at 115200 baud. Enter a number corresponding to a button in the list.
 * 
 * NOTE: the actual receivers have the address and group numbers in this example are only for demonstration!
 * To use this actual example, you'd need to "learn" the used code in the receivers
 */

#include "RgbRfWirelessTransmitter.h"

RgbRfTransmitter transmitter(53265, 11);

void setup() {
  Serial.begin(115200);
  
  displayHelp();
}

void loop() {
  if (Serial.available()) {
    // Read the requested level from serial
    int button = Serial.parseInt();

    switch(button){
      case RgbRfButton::Power:
      case RgbRfButton::ModePlus:
      case RgbRfButton::SpeedMinus:
      case RgbRfButton::Demo:
      case RgbRfButton::SpeedPlus:
      case RgbRfButton::ColorPlus:
      case RgbRfButton::ModeMinus:
      case RgbRfButton::BrightPlus:
      case RgbRfButton::ColorMinus:
      case RgbRfButton::White:
      case RgbRfButton::BrightMinus:
      case RgbRfButton::Red:
      case RgbRfButton::Green:
      case RgbRfButton::DarkBlue:
      case RgbRfButton::Yellow:
      case RgbRfButton::LightBlue:
      case RgbRfButton::Pink:
        Serial.print("Sending signal for button ");
        switch(button){
          case RgbRfButton::Power:
            Serial.print("Power");
            break;
          case RgbRfButton::ModePlus:
            Serial.print("Mode+");
            break;
          case RgbRfButton::SpeedMinus:
            Serial.print("Speed-");
            break;
          case RgbRfButton::Demo:
            Serial.print("Demo");
            break;
          case RgbRfButton::SpeedPlus:
            Serial.print("Speed+");
            break;
          case RgbRfButton::ColorPlus:
            Serial.print("Color+");
            break;
          case RgbRfButton::ModeMinus:
            Serial.print("Mode-");
            break;
          case RgbRfButton::BrightPlus:
            Serial.print("Bright+");
            break;
          case RgbRfButton::ColorMinus:
            Serial.print("Color-");
            break;
          case RgbRfButton::White:
            Serial.print("White");
            break;
          case RgbRfButton::BrightMinus:
            Serial.print("Bright-");
            break;
          case RgbRfButton::Red:
            Serial.print("Red");
            break;
          case RgbRfButton::Green:
            Serial.print("Green");
            break;
          case RgbRfButton::DarkBlue:
            Serial.print("Dark Blue");
            break;
          case RgbRfButton::Yellow:
            Serial.print("Yellow");
            break;
          case RgbRfButton::LightBlue:
            Serial.print("Light Blue");
            break;
          case RgbRfButton::Pink:
            Serial.print("Pink");
            break;
        }
        Serial.print("...      ");
        transmitter.sendButton((RgbRfButton)button);
        Serial.println("Done");
        break;
      default:
        Serial.println("Invalid input\n");
        displayHelp();
    }
  }
}

void displayHelp() {
  Serial.println("Please enter a number representing a button to simulate a press.");
  Serial.println("1: Power; 5: Mode+; 7: Speed-; 8: Demo; 9: Speed+; 10: Color+; 11: Mode-; 12: Bright+; 13: Color-; 14: White; 15: Bright-; 16: Red; 17: Green; 18: Dark blue; 19: Yellow; 20: Light blue; 21: Pink");
}
