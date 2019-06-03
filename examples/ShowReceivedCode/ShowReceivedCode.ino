/*
* Demo for RF remote switch receiver. 

* For details, see RgbRfWirelessReceiver.h!
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to digital pin 2.
*/

#include "RgbRfWirelessReceiver.h"

void setup() {
  Serial.begin(115200);
  
  // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  // after 2 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment)
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  RgbRfReceiver::init(0, 2, showCode);
}

void loop() {
}

// Callback function is called only when a valid code is received.
void showCode(RgbRfCode receivedCode) {
  // Note: interrupts are disabled. You can re-enable them if needed.
  
  // Print the received code.
  Serial.print("Addr ");
  Serial.print(receivedCode.address);
  
  Serial.print(", button: ");
  switch(receivedCode.button){
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
    default:
      Serial.print(receivedCode.button);
      break;
  }
  
  Serial.print(", period: ");
  Serial.print(receivedCode.period);
  Serial.println("us.");
}
