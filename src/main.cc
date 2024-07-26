#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "Display.h"

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr){
	SDL_RenderCopy(ren, tex, clip, &dst);
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

int main(int argc, char **argv)
{
	Display testDisplay(600, 400);
	
	if (!(testDisplay.initDisplay()))
		return 1;
		
	SDL_Color colour = {255, 255, 255};
	SDL_Surface *textSurface = nullptr;
	SDL_Texture *texture = nullptr;
	
	textSurface = TTF_RenderText_Solid(testDisplay.getFont(), "Test Text", colour);
	
	SDL_SetRenderDrawColor(testDisplay.getRenderer(), 0, 0, 0, 255);
	
	texture = SDL_CreateTextureFromSurface(testDisplay.getRenderer(), textSurface);
	
	while (true)
	{
		SDL_Event event;
		
		if (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				break;
		
		SDL_RenderClear(testDisplay.getRenderer());
		renderTexture(texture,testDisplay.getRenderer(), 300, 300);
		SDL_RenderPresent(testDisplay.getRenderer());
	}
	
	return 0;
}
