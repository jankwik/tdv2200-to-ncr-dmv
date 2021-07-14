#include "NcrKeyboardInterface.h"

#define PIN_CMP_TRG PIND2

static NcrKeyboardInterface *currentNcrInt;

NcrKeyboardInterface::NcrKeyboardInterface(KeyboardQueue *queue) {
    currentNcrInt = this;
    this->queue = queue;
    ncrKeyCodes = new NcrKeyCodes(queue);
    lastPoll = micros();
	pinMode(PIN_CMP_TRG, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(PIN_CMP_TRG), handleComputerPoll, FALLING);
}

uint8_t NcrKeyboardInterface::Push(int key) {
    return ncrKeyCodes->Push(key);
}

void NcrKeyboardInterface::sendBitToComputer(int _bit)
{
	digitalWrite(PIN_CMP_TRG, LOW);
	delayMicroseconds(_bit == 0 ? 20 : 95);
	digitalWrite(PIN_CMP_TRG, HIGH);
	delayMicroseconds(_bit == 0 ? 170 : 95);
}

void NcrKeyboardInterface::sendByteToComputer(uint8_t _byte)
{
	for (int i = 0; i < 8; i++)
	{
		sendBitToComputer((_byte >> i) & 0x01);
	}
}

void NcrKeyboardInterface::handleComputerPoll()
{
	unsigned long now = micros();
	unsigned long timeSinceLast;
	if (now < currentNcrInt->lastPoll)
	{
		timeSinceLast = now + (__LONG_MAX__ - currentNcrInt->lastPoll);
	}
	else
	{
		timeSinceLast = now - currentNcrInt->lastPoll;
	}
	if (timeSinceLast < 500)
	{
		delayMicroseconds(300);

		pinMode(PIN_CMP_TRG, OUTPUT);
		digitalWrite(PIN_CMP_TRG, HIGH);

		// Norwegian keyboard mapping
		currentNcrInt->sendBitToComputer(1);
		currentNcrInt->sendBitToComputer(0);
		currentNcrInt->sendBitToComputer(1);

		// Status Good
		for (int i = 0; i < 5; i++)
		{
			currentNcrInt->sendBitToComputer(1);
		}
		pinMode(PIN_CMP_TRG, INPUT_PULLUP);
	}
	else
	{
		if (currentNcrInt->queue->HasKeypress())
		{
			delayMicroseconds(300);
			pinMode(PIN_CMP_TRG, OUTPUT);
			digitalWrite(PIN_CMP_TRG, HIGH);
			currentNcrInt->sendByteToComputer(currentNcrInt->queue->Pop());
			pinMode(PIN_CMP_TRG, INPUT_PULLUP);
		}
	}
	currentNcrInt->lastPoll = now;
}
