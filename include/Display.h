#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ScreenInterface.h"

enum class ErrorType {TTF_ERROR, SDL_ERROR};

class Display
{
	public:
		Display();
		Display(int, int);
		~Display();
		bool initDisplay();
		SDL_Renderer* getRenderer();
		TTF_Font* getFont(int);
		void renderTexture(SDL_Texture*, int, int, SDL_Rect*);
		int getHeight();
		int getWidth();
		void drawScreen();
	private:
		void printOutErrorMessage(ErrorType, std::string);
		std::string getResourcePath(std::string );
		int width;
		int height;
		SDL_Window* window;
		SDL_Renderer* renderer;
		std::map<int, TTF_Font*> fonts;
		ScreenInterface* currentScreen;
};

#endif
