#include <iostream>
#include "GameScreen.h"
#include "MainMenu.h"

GameScreen::GameScreen()
{
    display = nullptr;
    background = nullptr;
    backgroundImage = nullptr;
    paddleImage = nullptr;
    paddleImageTexture = nullptr;
    initAssets();
}

GameScreen::GameScreen(Display* givenDisplay)
{
    display = nullptr;
    display = givenDisplay;
    background = nullptr;
    backgroundImage = nullptr;
    paddleImage = nullptr;
    paddleImageTexture = nullptr;
    initAssets();
}

GameScreen::~GameScreen()
{
    // Free our image surface
    SDL_FreeSurface(backgroundImage);
    SDL_FreeSurface(paddleImage);
    Mix_FreeMusic(chosenMusic);
    
    // Set our appropriate values back to nullptrs
    display = nullptr;
    background = nullptr;
    backgroundImage = nullptr;
}

bool GameScreen::screenDraw()
{
    // Event handling
    SDL_Event event;

    int timerFps = SDL_GetTicks();
        
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;
                
        if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
        {
            display->updateScreen(new MainMenu(display));
            return true;
        }
        
        if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_b))
            paddleImageRenderArea.w = paddleImageRenderArea.w * 3 / 2;
            
        if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_s))
            paddleImageRenderArea.w = paddleImageRenderArea.w / 3 * 2;
            
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            handleResizeEvent();

        if (event.type == SDL_MOUSEMOTION)
        {
            paddleImageRenderArea.x = event.motion.x - paddleImageRenderArea.w / 2;

            if (paddleImageRenderArea.x < 0)
                paddleImageRenderArea.x = 0;
            else if ((paddleImageRenderArea.x + paddleImageRenderArea.w) > display->getWidth())
                paddleImageRenderArea.x = display->getWidth() - paddleImageRenderArea.w;
        }
    }
    
    // Music jazz
    if (Mix_PlayingMusic() == 0)
        Mix_PlayMusic(chosenMusic, -1);
    
    SDL_RenderClear(display->getRenderer());
    
    //SDL_RenderCopy(display->getRenderer(), background, &backgroundClip, &backgroundDest);

    SDL_RenderCopy(display->getRenderer(), paddleImageTexture, &paddleImageSourceTexture, &paddleImageRenderArea);
    testDraw->drawLevel();
    SDL_RenderPresent(display->getRenderer());
    
    timerFps = SDL_GetTicks() - timerFps;
    display->fpsLock(timerFps);

    return true;
}

void GameScreen::initAssets()
{
    // Load our images
    backgroundImage = IMG_Load((display->getResourcePath("breakout_bg.png")).c_str());
    paddleImage = IMG_Load((display->getResourcePath("Pack/Bats/bat_yellow.png")).c_str());
    
    // Check for bad loading
    if (backgroundImage == NULL || paddleImage == NULL)
        std::cout << "Error loading Image : " << IMG_GetError() << std::endl;

    // Load our music
    chosenMusic = Mix_LoadMUS((display->getResourcePath("ambient-wave-11-stretched.mp3")).c_str());

    if (chosenMusic == NULL)
        std::cout << "Error loading Music : " << Mix_GetError() << std::endl;
        
    // Load our textures
    background = SDL_CreateTextureFromSurface(display->getRenderer(), backgroundImage);
    paddleImageTexture = SDL_CreateTextureFromSurface(display->getRenderer(), paddleImage);
    
    SDL_QueryTexture(background, NULL, NULL, &backgroundClip.w, &backgroundClip.h);
    backgroundClip.x = 0;
    backgroundClip.y = 0;
    
    backgroundDest.x = 0;
    backgroundDest.y = 0;
    backgroundDest.w = display->getWidth();
    backgroundDest.h = display->getHeight();
    
    paddleImageSourceTexture.x = 25;
    paddleImageSourceTexture.y = 180;
    paddleImageSourceTexture.w = 463;
    paddleImageSourceTexture.h = 100;
    
    paddleImageRenderArea.x = display->getWidth() / 2 - display->getWidth() / 5 / 2;
    paddleImageRenderArea.y = display->getHeight() - display->getHeight() / 20;
    paddleImageRenderArea.w = display->getWidth() / 5;
    paddleImageRenderArea.h = display->getHeight() / 20;

    testDraw = new LevelDraw(display, "levels/example.lvl");
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
        
    // Update all of our drawings
    backgroundDest.w = display->getWidth();
    backgroundDest.h = display->getHeight();
    
    paddleImageRenderArea.x = display->getWidth() / 2 - display->getWidth() / 10;
    paddleImageRenderArea.y = display->getHeight() - display->getHeight() / 20;
    paddleImageRenderArea.w = display->getWidth() / 5;
    paddleImageRenderArea.h = display->getHeight() / 20;
}
