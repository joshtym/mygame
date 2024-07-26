#ifndef MAINMENU_H
#define MAINMENU_H

#include "ScreenInterface.h"
#include "Display.h"
#include <SDL2/SDL_image.h>

class MainMenu : public ScreenInterface
{
	public:
		MainMenu();
		MainMenu(Display*);
		~MainMenu();
		bool screenDraw();
	private:
		void initAssets();
		Display* display;
		SDL_Texture* titleTexture;
		SDL_Texture* gameOption1Texture;
		SDL_Texture* gameOption2Texture;
		SDL_Surface* image;
		SDL_Texture* background;
		SDL_Rect backgroundRect;
};
#endif
