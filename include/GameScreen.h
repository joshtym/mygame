#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "ScreenInterface.h"
#include "Display.h"
#include <SDL2/SDL_image.h>

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
		SDL_Texture* background;
		SDL_Surface* image;
		SDL_Surface* paddleImage;
		SDL_Texture* paddleImageTexture;
		SDL_Rect paddleImageRenderArea;
};
#endif
