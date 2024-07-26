#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ScreenInterface.h"

enum class ErrorType {TTF_ERROR, SDL_ERROR, IMAGE_ERROR};

class Display
{
	public:
		Display();
		Display(int, int);
		~Display();
		bool initDisplay();
		SDL_Renderer* getRenderer();
		SDL_Window* getWindow();
		TTF_Font* getFont(int);
		void renderTexture(SDL_Texture*, int, int, SDL_Rect*);
		int getHeight();
		int getWidth();
		bool drawScreen();
		void updateScreen(ScreenInterface*);
		std::string getResourcePath(std::string);
	private:
		bool initLibraries();
		void printOutErrorMessage(ErrorType, std::string);
		int width;
		int height;
		SDL_Window* window;
		SDL_Renderer* renderer;
		std::map<int, TTF_Font*> fonts;
		ScreenInterface* currentScreen;
};

#endif
