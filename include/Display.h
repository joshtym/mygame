#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ScreenInterface.h"

enum class ErrorType {TTF_ERROR, SDL_ERROR, IMAGE_ERROR, AUDIO_ERROR};

class Display
{
    public:
        // Constructors
        Display();
        Display(int, int, int);

        // Destructor
        ~Display();
        
        // Public functions
        bool initDisplay();
        void renderTexture(SDL_Texture*, int, int, SDL_Rect*);
        void updateScreen(ScreenInterface*);
        bool drawScreen();
        void fpsLock(int);

        // Function getters
        SDL_Renderer* getRenderer();
        SDL_Window* getWindow();
        TTF_Font* getFont(int);
        std::string getResourcePath(std::string);
        int getHeight();
        int getWidth();
    private:
        // Private functions
        bool initLibraries();
        void printOutErrorMessage(ErrorType, std::string);

        // Private variables
        int width;
        int height;
        int fps;
        SDL_Window* window;
        SDL_Renderer* renderer;
        std::map<int, TTF_Font*> fonts;
        ScreenInterface* currentScreen;
};

#endif
