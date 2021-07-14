#ifndef __NCR_KEYBOARD_INTERFACE_H
#define __NCR_KEYBOARD_INTERFACE_H

#include "KeyboardQueue.h"
#include "NcrKeyCodes.h"

class NcrKeyboardInterface {

    private:

        KeyboardQueue *queue;
        NcrKeyCodes *ncrKeyCodes;
        unsigned long lastPoll;

        void sendBitToComputer(int _bit);
        void sendByteToComputer(uint8_t _byte);

        static void handleComputerPoll();

    public:

        NcrKeyboardInterface(KeyboardQueue *queue);

        uint8_t Push(int key);

};

#endif


