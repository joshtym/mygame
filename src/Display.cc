#include <iostream>
#include <SDL2/SDL_image.h>
#include "Display.h"
#include "MainMenu.h"

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
	for (std::map<int, TTF_Font*>::const_iterator it = fonts.begin(); it != fonts.end(); ++it)
		TTF_CloseFont(it->second);
	
	// Quit TTF, Image, and SDL
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Display::initDisplay()
{
	if (!(initLibraries()))
		return false;
	
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
	
	
	currentScreen = new MainMenu(this);
	
	return true;
}

SDL_Renderer* Display::getRenderer()
{
	return renderer;
}

SDL_Window* Display::getWindow()
{
	return window;
}

TTF_Font* Display::getFont(int fontSize)
{
	// Iterator for the map
	std::map<int, TTF_Font*>::iterator it;
	it = fonts.find(fontSize);
	std::string resourcePath = getResourcePath("victor-pixel.ttf");
	
	// Check if the font is in the map. If it's not, create it with error checking and return the font
	if (it != fonts.end())
		return it->second;
	else
	{
		TTF_Font* newFont = TTF_OpenFont(resourcePath.c_str(), fontSize);
		
		if (newFont == NULL)
			printOutErrorMessage(ErrorType::TTF_ERROR, "Error opening Font : ");
		else
			fonts.insert(std::pair<int, TTF_Font*>(fontSize, newFont));
		
		return newFont;
	}
}	

void Display::renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* source)
{
	// Variable declaration
	SDL_Rect destination;
	
	// Set the location of the rectangle on the plane
	destination.x = x;
	destination.y = y;
	
	// Check if our given rectangle is the nullptr and act accordingly to assign
	// the width and height of the renderer
	if (source != nullptr)
	{
		destination.x = source->x;
		destination.y = source->y;
		destination.w = source->w;
		destination.h = source->h;
	}
	else
		SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
	
	// Render the texture
	SDL_RenderCopy(renderer, texture, NULL, &destination);
}

int Display::getHeight()
{
	SDL_GetWindowSize(window, &width, &height);
	return height;
}

int Display::getWidth()
{
	SDL_GetWindowSize(window, &width, &height);
	return width;
}

bool Display::drawScreen()
{
	if (currentScreen->screenDraw())
		return true;
	else
		return false;
}

void Display::updateScreen(ScreenInterface* newScreen)
{
	delete currentScreen;
	currentScreen = newScreen;
}

bool Display::initLibraries()
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
	
	// Initialize our image services
	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(imgFlags);
	
	if ((initted & imgFlags) != imgFlags)
	{
		printOutErrorMessage(ErrorType::IMAGE_ERROR, "Failed to init required jpg and png support. Error : ");
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
	else if (error == ErrorType::IMAGE_ERROR)
		std::cout << message << IMG_GetError() << std::endl;
}

std::string Display::getResourcePath(std::string resourceName)
{
	// Variable declarations
	char* basePath;
	std::string stringBasePath;
	std::string resourcePath = "";
	
	// Get the base path of the directory
	basePath = SDL_GetBasePath();
	
	// Check for null values and assign accordingly or error accordingly
	if (basePath != NULL)
	{
		stringBasePath = basePath;
		SDL_free(basePath);
	}
	else
		printOutErrorMessage(ErrorType::SDL_ERROR, "Error getting resource path : ");
	
	// Get the position of our bin directory and change to assets
	size_t pos = stringBasePath.rfind("/bin");
	stringBasePath = stringBasePath.substr(0, pos);
	stringBasePath.append("/assets/");
	
	// Now assign our first font path to what we want
	resourcePath = stringBasePath + resourceName;
	
	return resourcePath;
}
