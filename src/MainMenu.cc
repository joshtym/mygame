#include "MainMenu.h"

MainMenu::MainMenu()
{
}

MainMenu::MainMenu(Display* givenDisplay)
{
	display = givenDisplay;
}

MainMenu::~MainMenu()
{
}

void MainMenu::screenDraw()
{
	SDL_Color colour = {255, 255, 255};
	SDL_Surface* title = nullptr;
	SDL_Surface* gameOption1 = nullptr;
	SDL_Surface* gameOption2 = nullptr;
	SDL_Texture* titleTexture = nullptr;
	SDL_Texture* gameOption1Texture = nullptr;
	SDL_Texture* gameOption2Texture = nullptr;
	int titleHeight, titleWidth;
	int gameOption1Height, gameOption1Width;
	int gameOption2Height, gameOption2Width;
		
	title = TTF_RenderText_Solid(display->getFont(24), "Test Game", colour);
	gameOption1 = TTF_RenderText_Solid(display->getFont(16), "New Game", colour);
	gameOption2 = TTF_RenderText_Solid(display->getFont(16), "Quit Game", colour);
	
	TTF_SizeText(display->getFont(32), "Test Game", &titleWidth, &titleHeight);
	TTF_SizeText(display->getFont(16), "New Game", &gameOption1Width, &gameOption1Height);
	TTF_SizeText(display->getFont(16), "Quit Game", &gameOption2Width, &gameOption2Height);
	SDL_SetRenderDrawColor(display->getRenderer(), 0, 0, 0, 255);
	
	titleTexture = SDL_CreateTextureFromSurface(display->getRenderer(), title);
	gameOption1Texture = SDL_CreateTextureFromSurface(display->getRenderer(), gameOption1);
	gameOption2Texture = SDL_CreateTextureFromSurface(display->getRenderer(), gameOption2);
	
	SDL_FreeSurface(title);
	SDL_FreeSurface(gameOption1);
	SDL_FreeSurface(gameOption2);
	
	SDL_RenderClear(display->getRenderer());
	display->renderTexture(titleTexture, (display->getWidth() / 2) - (titleWidth / 2), (display->getHeight() / 4), nullptr);
	display->renderTexture(gameOption1Texture, (display->getWidth() / 4) - (gameOption1Width / 2), (display->getHeight() / 4 * 3), nullptr);
	display->renderTexture(gameOption2Texture, (display->getWidth() / 4 * 3) - (gameOption2Width / 2), (display->getHeight() / 4 * 3), nullptr);
	SDL_RenderPresent(display->getRenderer());
}
