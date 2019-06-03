# RgbRfWireless
Transmitter and Receiver for 433mhz signals of the RF Remote commonly found on chinese RGB Controllers. I tried to make the library as compatible as possible with the NewRemoteSwitch library commonly used for 433mhz light switches/dimmers.

You can find a picture of one of the remotes I used to figure out the protocol [here](extras/example_remote.jpg) (I tried multiple, to figure out which part is the address).

You'll need a 433mhz receiver and/or transmitter. For the receiver you should get a more decent one (RXB12 works well) instead of the shitty one often bundled with the FS1000A transmitter.

# License
This library is licensed under the LGPLv3 license.

# Getting started
Download a release from the release page, and add it through the library manager in arduino.

# Examples
## Transmitting values
Create a transmitter with the address it should have, and the pin on which your 433mhz transmitter is connected. Below the transmitter will transmit on address 123 and the transmitter is connected on pin 11.
```c++
RgbRfTransmitter transmitter(123, 11);
```

Now you can send the button you want with:
```c++
transmitter.sendButton(RgbRfButton::Power);
```

See the `SerialTransmitter` example for a more detailed example.

## Receiving values
Initialize the receiver with the interrupt pin on which it should listen, the number of times it must see a code before calling your callback, and a callback.
```c++
RgbRfReceiver::init(0, 2, showCode);
```

Your callback function will be called each time a code is received.
```c++
void showCode(RgbRfCode receivedCode) {
  Serial.print("Addr ");
  Serial.print(receivedCode.address);
  Serial.print(", button: ");
  Serial.print(receivedCode.button);
  Serial.print(", period: ");
  Serial.print(receivedCode.period);
  Serial.println("us.");
}
```


See the `ShowReceivedCode` example for a more detailed example.

# Protocol
An explanation of the protocol can be found [here](extras/protocol_details.txt)