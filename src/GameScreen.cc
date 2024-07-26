#include "GameScreen.h"
#include "MainMenu.h"
#include <iostream>

GameScreen::GameScreen()
{
	display = nullptr;
	background = nullptr;
	image = nullptr;
	paddleImage = nullptr;
	paddleImageTexture = nullptr;
	initAssets();
}

GameScreen::GameScreen(Display* givenDisplay)
{
	display = nullptr;
	display = givenDisplay;
	background = nullptr;
	image = nullptr;
	paddleImage = nullptr;
	paddleImageTexture = nullptr;
	initAssets();
}

GameScreen::~GameScreen()
{
	// Free our image surface
	SDL_FreeSurface(image);
	SDL_FreeSurface(paddleImage);
	
	// Set our appropriate values back to nullptrs
	display = nullptr;
	background = nullptr;
	image = nullptr;
}

bool GameScreen::screenDraw()
{
	// Event handling
	SDL_Event event;
		
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return false;
				
		if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
		{
			display->updateScreen(new MainMenu(display));
			return true;
		}
		
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			handleResizeEvent();
	}
	
	SDL_RenderClear(display->getRenderer());
	
	SDL_RenderCopy(display->getRenderer(), background, NULL, NULL);
	//display->renderTexture(paddleImageTexture, 0, 0, nullptr);
	SDL_RenderCopy(display->getRenderer(), paddleImageTexture, NULL, &paddleImageRenderArea);
	SDL_RenderPresent(display->getRenderer());
	
	return true;
}

void GameScreen::initAssets()
{
	// Load our image
	image = IMG_Load((display->getResourcePath("breakout_bg.png")).c_str());
	paddleImage = IMG_Load((display->getResourcePath("bat_yellow.png")).c_str());
	
	// Check for bad loading
	if (image == NULL || paddleImage == NULL)
		std::cout << "Error loading Image : " << IMG_GetError() << std::endl;
		
	// Load our background
	background = SDL_CreateTextureFromSurface(display->getRenderer(), image);
	paddleImageTexture = SDL_CreateTextureFromSurface(display->getRenderer(), paddleImage);
	
	SDL_Rect tempRect;
	SDL_QueryTexture(paddleImageTexture, NULL, NULL, &tempRect.w, &tempRect.h);
	
	paddleImageRenderArea.x = 0;
	paddleImageRenderArea.y = 0;
	paddleImageRenderArea.w = display->getWidth() / 5;
	paddleImageRenderArea.h = display->getHeight();
}

void GameScreen::handleResizeEvent()
{
	// Variable declarations
	SDL_Rect renderArea, newWindowSize;
	
	// Let's take a look at the viewport
	SDL_RenderGetViewport(display->getRenderer(), &renderArea);
	
	// And grab our current display size
	newWindowSize.w = display->getWidth();
	newWindowSize.h = display->getHeight();
	
	// Check for consistencies, and update if necessary
	if (renderArea.w != display->getWidth() || renderArea.h != display->getHeight())
		SDL_RenderSetViewport(display->getRenderer(), &newWindowSize);
}
