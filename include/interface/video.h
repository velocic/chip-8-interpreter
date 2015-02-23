#ifndef VIDEO_H
#define VIDEO_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <SDL2/SDL.h>

class Video
{
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Rect pixel;
        const int NUM_ROWS = 32;
        const int NUM_COLUMNS = 64;
        void drawPixel(const std::pair<int, int> &coordinatePair);
        std::pair<int, int> translateChip8GraphicsOffsetTo2DCoordinates(int graphicsOffset);
    public:
        Video(
            std::string windowTitle,
            int xPosition,
            int yPosition,
            int screenWidth,
            int screenHeight
        );
        ~Video();
        void clearScreen();
        void drawScreen(unsigned char graphics[2048]);
};

#endif
