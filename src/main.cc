#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "Display.h"

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *        default of nullptr draws the entire texture
 */


int main(int argc, char **argv)
{
    Display testDisplay(800, 600);
    
    if (!(testDisplay.initDisplay()))
        return 1;
    
    while (true)
        if (!(testDisplay.drawScreen()))
            break;
    
    return 0;
}
