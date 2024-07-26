#ifndef LEVELDRAW_H
#define LEVELDRAW_H

#include <string>
#include <vector>
#include <SDL2/SDL_image.h>
#include "pugixml.h"
#include "Display.h"

class LevelDraw
{
    public:
        LevelDraw();
        LevelDraw(Display*, std::string);
        ~LevelDraw();
        void drawLevel();
    private:
        void init();
        void parseXML();

        Display *display;
        std::string fileName;
        pugi::xml_document file;
        std::vector<SDL_Rect> blockRenders;
        SDL_Rect blockSourceRect;
        SDL_Surface* blockImage;
        SDL_Texture* blockTexture;
};

#endif