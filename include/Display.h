#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

enum class ErrorType {TTF_ERROR, SDL_ERROR};

class Display
{
	public:
		Display();
		Display(int, int);
		~Display();
		bool initDisplay();
		SDL_Renderer* getRenderer();
		TTF_Font* getFont();
	private:
		bool initializeFonts();
		void printOutErrorMessage(ErrorType, std::string);
		int width;
		int height;
		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font* font;
};

#endif
