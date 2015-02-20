#include <interface/controls.h>

// unsigned char Controls::(&getKeypadState())[16]
unsigned char (&Controls::getKeypadState())[16]
{
    keypadState[one] = SDLKeyStates[SDL_SCANCODE_1];
    keypadState[two] = SDLKeyStates[SDL_SCANCODE_2];
    keypadState[three] = SDLKeyStates[SDL_SCANCODE_3];
    keypadState[four] = SDLKeyStates[SDL_SCANCODE_4];
    keypadState[q] = SDLKeyStates[SDL_SCANCODE_Q];
    keypadState[w] = SDLKeyStates[SDL_SCANCODE_W];
    keypadState[e] = SDLKeyStates[SDL_SCANCODE_E];
    keypadState[r] = SDLKeyStates[SDL_SCANCODE_R];
    keypadState[a] = SDLKeyStates[SDL_SCANCODE_A];
    keypadState[s] = SDLKeyStates[SDL_SCANCODE_S];
    keypadState[d] = SDLKeyStates[SDL_SCANCODE_D];
    keypadState[f] = SDLKeyStates[SDL_SCANCODE_F];
    keypadState[z] = SDLKeyStates[SDL_SCANCODE_Z];
    keypadState[x] = SDLKeyStates[SDL_SCANCODE_X];
    keypadState[c] = SDLKeyStates[SDL_SCANCODE_C];
    keypadState[v] = SDLKeyStates[SDL_SCANCODE_V];

    return keypadState;
}

bool Controls::userRequestedExit()
{
    if (SDLKeyStates[SDL_SCANCODE_ESCAPE]) {
        return true;
    }

    return false;
}
