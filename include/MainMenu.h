#ifndef MAINMENU_H
#define MAINMENU_H

#include <SDL2/SDL_image.h>
#include <vector>
#include "ScreenInterface.h"
#include "Display.h"

class MainMenu : public ScreenInterface
{
    public:
        // Constructors
        MainMenu();
        MainMenu(Display*);

        // Destructor
        ~MainMenu();

        // Virtual public funtcion instantiation
        bool screenDraw();
    private:
        // Private functions
        void initAssets();
        bool drawParralaxBackground();

        // Private variables
        Display* display;
        SDL_Texture* titleTexture;
        SDL_Texture* gameOption1Texture;
        SDL_Texture* gameOption2Texture;
        SDL_Surface* imageOne;
        SDL_Surface* imageTwo;
        SDL_Surface* imageThree;
        SDL_Texture* backgroundTexture;
        SDL_Texture* middleTexture;
        SDL_Texture* frontTexture;
        SDL_Rect backgroundRect;
        std::vector<SDL_Rect> backLayer;
        std::vector<SDL_Rect> middleLayer;
        std::vector<SDL_Rect> frontLayer;
        int fps;
        int counter;
        int titleHeight, titleWidth;
        int gameOption1Height, gameOption1Width;
        int gameOption2Height, gameOption2Width;
};
#endif
