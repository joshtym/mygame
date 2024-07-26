#ifndef MAINMENU_H
#define MAINMENU_H

#include "ScreenInterface.h"
#include "Display.h"
#include <SDL2/SDL_image.h>
#include <vector>

class MainMenu : public ScreenInterface
{
    public:
        MainMenu();
        MainMenu(Display*);
        ~MainMenu();
        bool screenDraw();
    private:
        void initAssets();
        bool drawParralaxBackground();
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
};
#endif
