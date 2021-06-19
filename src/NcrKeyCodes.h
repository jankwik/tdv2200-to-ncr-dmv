#ifndef __NCR_KEY_CODES_H
#define __NCR_KEY_CODES_H

#include <Arduino.h>
#include "KeyboardQueue.h"

class NcrKeyCodes
{

private:
	KeyboardQueue *queue;

public:
	NcrKeyCodes(KeyboardQueue *queue);

	uint8_t Push(int key);
};

#endif
