#include <iostream>
#include "MainMenu.h"
#include "GameScreen.h"

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
    // Load our imageOne
    imageOne = IMG_Load((display->getResourcePath("Parallax100.png")).c_str());
    imageTwo = IMG_Load((display->getResourcePath("Parallax80.png")).c_str());
    imageThree = IMG_Load((display->getResourcePath("Parallax60.png")).c_str());
    
    // Check for bad loading
    if (imageOne == NULL || imageTwo == NULL || imageThree == NULL)
        std::cout << "Error loading images : " << IMG_GetError() << std::endl;
        
    // Load our backgroundTexture
    backgroundTexture = SDL_CreateTextureFromSurface(display->getRenderer(), imageOne);
    
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &backgroundRect.w, &backgroundRect.h);
    
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    
    backLayer.clear();
    middleLayer.clear();
    frontLayer.clear();
    
    for (int i = 0; i < 6; ++i)
    {
        backLayer.push_back(backgroundRect);
        middleLayer.push_back(backgroundRect);
        frontLayer.push_back(backgroundRect);
    }
    
    backLayer[1].y = 500;
    backLayer[2].x = 500;
    backLayer[3].x = 500;
    backLayer[3].y = 500;
    backLayer[4].x = 1000;
    backLayer[5].x = 1000;
    backLayer[5].y = 500;
    
    middleLayer[1].y = 500;
    middleLayer[2].x = 500;
    middleLayer[3].x = 500;
    middleLayer[3].y = 500;
    middleLayer[4].x = 1000;
    middleLayer[5].x = 1000;
    middleLayer[5].y = 500;
    
    frontLayer[1].y = 500;
    frontLayer[2].x = 500;
    frontLayer[3].x = 500;
    frontLayer[3].y = 500;
    frontLayer[4].x = 1000;
    frontLayer[5].x = 1000;
    frontLayer[5].y = 500;
    
    fps = 400;
    counter = 0;
}

bool MainMenu::screenDraw()
{
    SDL_Event event;
        
    int timerFps = SDL_GetTicks();

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
    
    if (!(drawParralaxBackground()))
        std::cout << "Failed to draw the parallax backgroundTexture" << std::endl;
    
    //display->renderTexture(backgroundTexture, -300, 0, &backgroundRect);
    display->renderTexture(titleTexture, (display->getWidth() / 2) - (titleWidth / 2), (display->getHeight() / 4), nullptr);
    display->renderTexture(gameOption1Texture, (display->getWidth() / 4) - (gameOption1Width / 2), (display->getHeight() / 4 * 3), nullptr);
    display->renderTexture(gameOption2Texture, (display->getWidth() / 4 * 3) - (gameOption2Width / 2), (display->getHeight() / 4 * 3), nullptr);
    SDL_RenderPresent(display->getRenderer());
    
    timerFps = SDL_GetTicks() - timerFps;
    display->fpsLock(timerFps);

    return true;
}

bool MainMenu::drawParralaxBackground()
{
    ++counter;
    
    if (counter % fps == 0)
        for (int i = 0; i < 6; ++i)
            --backLayer[i].x;
    
    if (counter % (fps / 2) == 0)
        for (int i = 0; i < 6; ++i)
            --middleLayer[i].x;
    
    if (counter % (fps / 4) == 0)
        for (int i = 0; i < 6; ++i)
            --frontLayer[i].x;
    
    for (int i = 0; i < 6; ++i)
    {
        if (backLayer[i].x < -500)
            backLayer[i].x += 1500;
            
        if (middleLayer[i].x < -500)
            middleLayer[i].x += 1500;
            
        if (frontLayer[i].x < -500)
            frontLayer[i].x += 1500;
    }
            
    display->renderTexture(backgroundTexture, -300, 0, &backLayer[0]);
    display->renderTexture(backgroundTexture, -300, 0, &backLayer[1]);
    display->renderTexture(backgroundTexture, -300, 0, &backLayer[2]);
    display->renderTexture(backgroundTexture, -300, 0, &backLayer[3]);
    display->renderTexture(backgroundTexture, -300, 0, &backLayer[4]);
    display->renderTexture(backgroundTexture, -300, 0, &backLayer[5]);
    
    display->renderTexture(middleTexture, -300, 0, &middleLayer[0]);
    display->renderTexture(middleTexture, -300, 0, &middleLayer[1]);
    display->renderTexture(middleTexture, -300, 0, &middleLayer[2]);
    display->renderTexture(middleTexture, -300, 0, &middleLayer[3]);
    display->renderTexture(middleTexture, -300, 0, &middleLayer[4]);
    display->renderTexture(middleTexture, -300, 0, &middleLayer[5]);
    
    display->renderTexture(frontTexture, -300, 0, &frontLayer[0]);
    display->renderTexture(frontTexture, -300, 0, &frontLayer[1]);
    display->renderTexture(frontTexture, -300, 0, &frontLayer[2]);
    display->renderTexture(frontTexture, -300, 0, &frontLayer[3]);
    display->renderTexture(frontTexture, -300, 0, &frontLayer[4]);
    display->renderTexture(frontTexture, -300, 0, &frontLayer[5]);
    
    return true;
}
