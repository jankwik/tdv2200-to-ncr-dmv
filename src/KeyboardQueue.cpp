#define __KEYBOARD_QUEUE_MAIN
#include "KeyboardQueue.h"

KeyboardQueue keyboardQueue;

KeyboardQueue::KeyboardQueue(int queueSize)
{
	this->queueSize = queueSize;
	queue = new uint8_t[queueSize];
	writePos = 0;
	readPos = 0;
}

KeyboardQueue::~KeyboardQueue()
{
	if (queue != NULL)
		delete[] queue;
}

bool KeyboardQueue::HasKeypress()
{
	return readPos != writePos;
}

bool KeyboardQueue::Push(uint8_t keypress)
{
	queue[writePos] = keypress;
	writePos++;
	if (writePos > queueSize)
		writePos = 0;
	return true;
}

uint8_t KeyboardQueue::Pop()
{
	uint8_t keypress = 0x00;
	if (readPos != writePos)
	{
		keypress = queue[readPos];
		readPos++;
		if (readPos > queueSize)
			readPos = 0;
	}
	return keypress;
}
