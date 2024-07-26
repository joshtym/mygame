#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Display.h"

Display::Display()
{
	// Assign the width and height of the window
	width = 600;
	height = 400;
}

Display::Display(int screenWidth, int screenHeight)
{
	// Assign the width and height of the window
	width = screenWidth;
	height = screenHeight;
}

Display::~Display()
{
	// Destroy the renderer and set it to the nullptr
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	
	// Destroy the window an set it to the nullptr
	SDL_DestroyWindow(window);
	window = nullptr;
	
	// Close our font
	TTF_CloseFont(font);
	
	// Quit TTF and SDL
	TTF_Quit();
	SDL_Quit();
}

bool Display::initDisplay()
{
	// Initialize the SDL libraries with video capabilities. Check for error
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printOutErrorMessage(ErrorType::SDL_ERROR, "SDL_Init Error : ");
		return false;
	}
	
	// Initialize the font services. Check for error
	if (TTF_Init() == -1)
	{
		printOutErrorMessage(ErrorType::TTF_ERROR, "TTF_Init Error : ");
		return false;
	}
	
	// Initialize our window
	window = SDL_CreateWindow("Game", 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	
	// Check for errors on the window initialization
	if (window == nullptr)
	{
		printOutErrorMessage(ErrorType::SDL_ERROR, "SDL_CreateWindow Error : ");
		return false;
	}
	
	// Initialize our renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	// Check for nullptr and return appropriately
	if (renderer == nullptr)
	{
		printOutErrorMessage(ErrorType::SDL_ERROR, "SDL_CreateRenderer Error : ");
		return false;
	}
	
	// Initialize fonts and check for error
	if (!(initializeFonts()))
		return false;
	return true;
}

SDL_Renderer* Display::getRenderer()
{
	return renderer;
}

TTF_Font* Display::getFont()
{
	return font;
}

bool Display::initializeFonts()
{
	// Variable declarations
	char* basePath;
	std::string stringBasePath;
	std::string font1Path;
	
	// Get the base path of the directory
	basePath = SDL_GetBasePath();
	
	// Check for null values and assign accordingly or error accordingly
	if (basePath != NULL)
	{
		stringBasePath = basePath;
		SDL_free(basePath);
	}
	else
	{
		printOutErrorMessage(ErrorType::SDL_ERROR, "Error getting resource path : ");
		return false;
	}
	
	// Get the position of our bin directory and change to assets
	size_t pos = stringBasePath.rfind("/bin");
	stringBasePath = stringBasePath.substr(0, pos);
	stringBasePath.append("/assets/");
	
	// Now assign our first font path to what we want
	font1Path = stringBasePath + "victor-pixel.ttf";
	
	// Open up the font
	font=TTF_OpenFont(font1Path.c_str(), 16);
	
	// Check for a nullptr on our font
	if (font == nullptr)
	{
		printOutErrorMessage(ErrorType::TTF_ERROR, "TTF_OpenFont Error : ");
		return false;
	}
	
	return true;
}

void Display::printOutErrorMessage(ErrorType error, std::string message)
{
	if (error == ErrorType::SDL_ERROR)
		std::cout << message << SDL_GetError() << std::endl;
	else if (error == ErrorType::TTF_ERROR)
		std::cout << message << TTF_GetError() << std::endl;
}
