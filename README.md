# MOVED

https://codeberg.org/jankwik/tdv2200-to-ncr-dmv


### tdv2200-to-ncr-dmv
Admittedly a pretty niche project, but one I needed to integrate the Tandberg TDV2200-keyboard I had found at a thrift store, with the NCR Decision Mate V I had picked up locally following an online auction. The keyboard came without the Norsk Data terminal for which it had been produced, and the NCR machine without its proprietary keyboard. The original keyboard for the NCR is similar in apperance to the Tandberg - even if the colour scheme is slightly different. The keyboard protocols, though - are completely different. The TDV2200 uses RS-422 to communicate with its terminal, while the NCR uses a simple routine to poll the keyboard for new keypresses. 

The excellent SoftwareSerial (https://www.arduino.cc/en/Reference/SoftwareSerial) is used to implement the communication with the TDV2200, while the keypress output to the NCR is provided through events on the Arduino's D2 pin. These events are also used to provide the initialization byte expected by the NCR. 

![NCR Decision Mate V running with the Tandberg keyboard](https://github.com/jankwik/tdv2200-to-ncr-dmv/blob/main/NCR%20DMV.jpg?raw=true)
