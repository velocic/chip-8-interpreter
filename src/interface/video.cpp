#include <interface/video.h>

Video::Video(
    std::string windowTitle,
    int xPosition,
    int yPosition,
    int screenWidth,
    int screenHeight
)
{
    window = SDL_CreateWindow(windowTitle.c_str(), xPosition, yPosition, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    pixel.w = screenWidth / NUM_COLUMNS;
    pixel.h = screenHeight / NUM_ROWS;
    // Subtracting 1 makes the pixels fit the screen, but is probably the wrong way to solve 
    // pixel.w = (screenWidth / NUM_COLUMNS) -1;
    // pixel.h = (screenHeight / NUM_ROWS) - 1;
}

Video::~Video()
{
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
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    pixel.x = coordinatePair.first;
    pixel.y = coordinatePair.second;
    SDL_RenderFillRect(renderer, &pixel);
}

std::pair<int, int> Video::translateChip8GraphicsOffsetTo2DCoordinates(int graphicsOffset)
{
    std::pair<int, int> coordinatePoint;
    coordinatePoint.first = (graphicsOffset % 64) + ((pixel.w -1) * (graphicsOffset % 64));
    coordinatePoint.second = (graphicsOffset / 64) + ((pixel.h -1) * (graphicsOffset / 64));
    return coordinatePoint;
}
