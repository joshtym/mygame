#include "MainMenu.h"
#include "GameScreen.h"
#include <iostream>

MainMenu::MainMenu()
{
	titleTexture = nullptr;
	gameOption1Texture = nullptr;
	gameOption2Texture = nullptr;
	display = nullptr;
	initAssets();
}

MainMenu::MainMenu(Display* givenDisplay)
{
	titleTexture = nullptr;
	gameOption1Texture = nullptr;
	gameOption2Texture = nullptr;
	display = givenDisplay;
	initAssets();
}

MainMenu::~MainMenu()
{
	display = nullptr;
	SDL_DestroyTexture(titleTexture);
	SDL_DestroyTexture(gameOption1Texture);
	SDL_DestroyTexture(gameOption2Texture);
	titleTexture = nullptr;
	gameOption1Texture = nullptr;
	gameOption2Texture = nullptr;
}

void MainMenu::initAssets()
{
	// Load our image
	image = IMG_Load((display->getResourcePath("opl-parallax-universe.jpg")).c_str());
	
	// Check for bad loading
	if (image == NULL)
		std::cout << "Error loading Image : " << IMG_GetError() << std::endl;
		
	// Load our background
	background = SDL_CreateTextureFromSurface(display->getRenderer(), image);
	
	SDL_QueryTexture(background, NULL, NULL, &backgroundRect.w, &backgroundRect.h);
	
	backgroundRect.x = 0;
	backgroundRect.y = 0;
}

bool MainMenu::screenDraw()
{
	SDL_Event event;
		
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return false;
				
		if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
			return false;
			
		if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_n))
		{
			display->updateScreen(new GameScreen(display));
			return true;
		}
		
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			if (display->getWidth() > 1200)
				backgroundRect.w = display->getWidth();
			else
				backgroundRect.w = 1200;
		
			if (display->getHeight() > 500)
				backgroundRect.h = display->getHeight();
			else
				backgroundRect.h = 500;
		}
	}
	
	int titleHeight, titleWidth;
	int gameOption1Height, gameOption1Width;
	int gameOption2Height, gameOption2Width;
	
	TTF_SizeText(display->getFont(32), "Test Game", &titleWidth, &titleHeight);
	TTF_SizeText(display->getFont(16), "New Game", &gameOption1Width, &gameOption1Height);
	TTF_SizeText(display->getFont(16), "Quit Game", &gameOption2Width, &gameOption2Height);

	SDL_Color colour = {255, 255, 255, 255};
	SDL_Surface* title = nullptr;
	SDL_Surface* gameOption1 = nullptr;
	SDL_Surface* gameOption2 = nullptr;
	
	if (title == nullptr)
		title = TTF_RenderText_Solid(display->getFont(24), "Test Game", colour);
		
	if (gameOption1 == nullptr)
		gameOption1 = TTF_RenderText_Solid(display->getFont(16), "New Game", colour);
		
	if (gameOption2 == nullptr)
		gameOption2 = TTF_RenderText_Solid(display->getFont(16), "Quit Game", colour);
		
	TTF_SizeText(display->getFont(32), "Test Game", &titleWidth, &titleHeight);
	TTF_SizeText(display->getFont(16), "New Game", &gameOption1Width, &gameOption1Height);
	TTF_SizeText(display->getFont(16), "Quit Game", &gameOption2Width, &gameOption2Height);
	
	if (titleTexture == nullptr)	
		titleTexture = SDL_CreateTextureFromSurface(display->getRenderer(), title);
	
	if (gameOption1Texture == nullptr)
		gameOption1Texture = SDL_CreateTextureFromSurface(display->getRenderer(), gameOption1);
		
	if (gameOption2Texture == nullptr)
		gameOption2Texture = SDL_CreateTextureFromSurface(display->getRenderer(), gameOption2);
		
	SDL_FreeSurface(title);
	SDL_FreeSurface(gameOption1);
	SDL_FreeSurface(gameOption2);

	SDL_SetRenderDrawColor(display->getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(display->getRenderer());
	display->renderTexture(background, -300, 0, &backgroundRect);
	display->renderTexture(titleTexture, (display->getWidth() / 2) - (titleWidth / 2), (display->getHeight() / 4), nullptr);
	display->renderTexture(gameOption1Texture, (display->getWidth() / 4) - (gameOption1Width / 2), (display->getHeight() / 4 * 3), nullptr);
	display->renderTexture(gameOption2Texture, (display->getWidth() / 4 * 3) - (gameOption2Width / 2), (display->getHeight() / 4 * 3), nullptr);
	SDL_RenderPresent(display->getRenderer());

	
	return true;
}
