#include <interface/video.h>

Video::Video(
    std::string windowTitle,
    int xPosition,
    int yPosition,
    int width,
    int height
)
{
    window = SDL_CreateWindow(windowTitle, xPosition, yPosition, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    pixel.w = width / NUM_COLUMNS;
    pixel.h = height / NUM_ROWS;
}

Video::~Video()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Video::drawScreen(unsigned char graphics[2048])
{
    std::pair<int, int> coordinatePair;

    for (int i = 0; i < 2048; ++i) {
        if (graphics[i] > 0) {
            coordinatePair = translateChip8GraphicsOffsetTo2DCoordinates(i);
            drawPixel(coordinatePair);
        }
    }

    SDL_RenderPresent(renderer);
}

void Video::clearScreen()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
}

void Video::drawPixel(const std::pair<int, int> &coordinatePair)
{
    //Set draw color to white
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    pixel.x = coordinatePair.first * pixel.w;
    pixel.y = coordinatePair.second * pixel.h;
    SDL_RenderFillRect(renderer, &pixel);
}
