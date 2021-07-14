#include "Arduino.h"
#include "SoftwareSerial_rs422.h"
#include "KeyboardQueue.h"
#include "NcrKeyboardInterface.h"

#define PIN_TX_MINUS A0
#define PIN_TX_PLUS A1
#define PIN_RX_PLUS A2

#define KEYBOARD_BAUD_SPEED 2400
#define SERIAL_BAUD_SPEED 115200
// #define DEBUG_MODE		1

SoftwareSerial_rs422 rs422(PIN_RX_PLUS, PIN_TX_PLUS, PIN_TX_MINUS, false); // RX, TX+, TX-
KeyboardQueue queue;
NcrKeyboardInterface ncrInt(&queue);

void setup()
{

#ifdef DEBUG_MODE
	Serial.begin(SERIAL_BAUD_SPEED);
#endif

	rs422.begin(KEYBOARD_BAUD_SPEED);
	rs422.listen();

}

void loop()
{
	while (rs422.available())
	{
		int key = rs422.read();
#ifdef DEBUG_MODE
		uint8_t convkey = ncrInt.Push(key);
		Serial.print("[ ");
		Serial.print(key);
		Serial.print(" --> ");
		Serial.print(convkey);
		Serial.println(" ]");
#else
		ncrInt.Push(key);
#endif
	}
#ifdef DEBUG_MODE
	if (queue.HasKeypress()) {
		uint8_t key = queue.Pop();
		Serial.print("<<  ");
		Serial.println(key);
	}
#endif
}
