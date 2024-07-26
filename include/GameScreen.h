#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "ScreenInterface.h"
#include "Display.h"
#include "LevelDraw.h"

class GameScreen : public ScreenInterface
{
    public:
        GameScreen();
        GameScreen(Display*);
        ~GameScreen();
        bool screenDraw();
    private:
        void initAssets();
        void handleResizeEvent();

        Display* display;
        
        SDL_Surface* backgroundImage;
        SDL_Texture* background;
        SDL_Rect backgroundClip;
        SDL_Rect backgroundDest;
        
        SDL_Surface* paddleImage;
        SDL_Texture* paddleImageTexture;
        SDL_Rect paddleImageSourceTexture;
        SDL_Rect paddleImageRenderArea;

        Mix_Music* chosenMusic;

        LevelDraw* testDraw;
};
#endif
