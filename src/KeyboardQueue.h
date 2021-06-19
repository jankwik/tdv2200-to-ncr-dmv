#ifndef __KEYBOARD_QUEUE_H
#define __KEYBOARD_QUEUE_H

#include <Arduino.h>

#define DEFAULT_QUEUE_SIZE 64

class KeyboardQueue
{

private:
	uint8_t *queue;
	int queueSize;
	int writePos;
	int readPos;

public:
	KeyboardQueue(int queueSize = DEFAULT_QUEUE_SIZE);
	~KeyboardQueue();

	bool HasKeypress();
	bool Push(uint8_t keypress);
	uint8_t Pop();
};

#ifndef __KEYBOARD_QUEUE_MAIN
extern KeyboardQueue keyboardQueue;
#endif

#endif
