#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>

class Controls
{
    private:
        enum keyMap {
            one, two, three, four,
            q, w, e, r,
            a, s, d, f,
            z, x, c, v
        };
        unsigned char keypadState[16];
        const uint8_t *SDLKeyStates;
    public:
        Controls(const uint8_t *SDLKeyStates) : SDLKeyStates(SDLKeyStates) {};
        unsigned char (&getKeypadState())[16];
        bool userRequestedExit();
};

#endif
