#include "Arduino.h"
#include "SoftwareSerial_rs422.h"
#include "KeyboardQueue.h"
#include "NcrKeyCodes.h"

#define PIN_TX_MINUS A0
#define PIN_TX_PLUS A1
#define PIN_RX_PLUS A2
#define PIN_CMP_TRG PIND2

#define KEYBOARD_BAUD_SPEED 2400
#define SERIAL_BAUD_SPEED 115200
// #define DEBUG_MODE		1

SoftwareSerial_rs422 rs422(PIN_RX_PLUS, PIN_TX_PLUS, PIN_TX_MINUS, false); // RX, TX+, TX-
KeyboardQueue queue;
NcrKeyCodes ncr(&queue);
unsigned long lastPoll;

void sendBitToComputer(int _bit)
{
	digitalWrite(PIN_CMP_TRG, LOW);
	delayMicroseconds(_bit == 0 ? 20 : 95);
	digitalWrite(PIN_CMP_TRG, HIGH);
	delayMicroseconds(_bit == 0 ? 170 : 95);
}

void sendByteToComputer(uint8_t _byte)
{
	for (int i = 0; i < 8; i++)
	{
		sendBitToComputer((_byte >> i) & 0x01);
	}
}

void handleComputerPoll()
{
	unsigned long now = micros();
	unsigned long timeSinceLast;
	if (now < lastPoll)
	{
		timeSinceLast = now + (__LONG_MAX__ - lastPoll);
	}
	else
	{
		timeSinceLast = now - lastPoll;
	}
	if (timeSinceLast < 500)
	{
		delayMicroseconds(300);

		pinMode(PIN_CMP_TRG, OUTPUT);
		digitalWrite(PIN_CMP_TRG, HIGH);

		// Norwegian keyboard mapping
		sendBitToComputer(1);
		sendBitToComputer(0);
		sendBitToComputer(1);

		// Status Good
		for (int i = 0; i < 5; i++)
		{
			sendBitToComputer(1);
		}
		pinMode(PIN_CMP_TRG, INPUT_PULLUP);
	}
	else
	{
		if (queue.HasKeypress())
		{
			delayMicroseconds(300);
			pinMode(PIN_CMP_TRG, OUTPUT);
			digitalWrite(PIN_CMP_TRG, HIGH);
			sendByteToComputer(queue.Pop());
			pinMode(PIN_CMP_TRG, INPUT_PULLUP);
		}
	}
	lastPoll = now;
}

void setup()
{

#ifdef DEBUG_MODE
	Serial.begin(SERIAL_BAUD_SPEED);
#endif

	rs422.begin(KEYBOARD_BAUD_SPEED);
	rs422.listen();

	lastPoll = micros();
	pinMode(PIN_CMP_TRG, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(PIN_CMP_TRG), handleComputerPoll, FALLING);
}

void loop()
{
	while (rs422.available())
	{
		int key = rs422.read();
		uint8_t convkey = ncr.Push(key);
#ifdef DEBUG_MODE
		Serial.print("[ ");
		Serial.print(key);
		Serial.print(" --> ");
		Serial.print(convkey);
		Serial.println(" ]");
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
