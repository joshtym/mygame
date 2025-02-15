#include <vector>
#include <iostream>
#include "GameScreen.h"
#include "MainMenu.h"

GameScreen::GameScreen()
{
   SDL_ShowCursor(SDL_DISABLE);
   display = nullptr;
   paddleImage = nullptr;
   paddleImageTexture = nullptr;
   initAssets();
}

GameScreen::GameScreen(Display* givenDisplay)
{
   SDL_ShowCursor(SDL_DISABLE);
   display = nullptr;
   display = givenDisplay;
   paddleImage = nullptr;
   paddleImageTexture = nullptr;
   ballImage = nullptr;
   ballImageTexture = nullptr;
   initAssets();
}

GameScreen::~GameScreen()
{
   // Free our image surface
   SDL_FreeSurface(paddleImage);
   Mix_FreeMusic(chosenMusic);

   levelBlockRenders.clear();
   
   delete testDraw;
   delete testEngine;
   
   // Set our appropriate values back to nullptrs
   display = nullptr;
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
               
      if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_ESCAPE))
      {
         display->updateScreen(new MainMenu(display));
         return true;
      }

      if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_EQUALS))
      {
         int musicLevel;
         musicLevel = Mix_VolumeMusic(-1);

         if (musicLevel != MIX_MAX_VOLUME)
         {
               musicLevel += 2;

               if (musicLevel > MIX_MAX_VOLUME)
                  musicLevel = MIX_MAX_VOLUME;

               Mix_VolumeMusic(musicLevel);
         }

      }

      if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_MINUS))
      {
         int musicLevel;
         musicLevel = Mix_VolumeMusic(-1);

         if (musicLevel != 0)
         {
               musicLevel -= 2;

               if (musicLevel < 0)
                  musicLevel = 0;

               Mix_VolumeMusic(musicLevel);
         }

      }
      
      if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_b))
         paddleImageRenderArea.w = paddleImageRenderArea.w * 3 / 2;
         
      if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_s))
         paddleImageRenderArea.w = paddleImageRenderArea.w / 3 * 2;
         
      if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
         handleResizeEvent();

      if (event.type == SDL_MOUSEMOTION)
         paddleImageRenderArea = testEngine->updatePaddle(event.motion.x);

      if (event.type == SDL_MOUSEBUTTONUP)
         testEngine->releaseBall();
   }

   ballImageRenderArea = testEngine->updateBall();
   levelBlockRenders = testEngine->updateLevelBlockRenders();

   if (testEngine->isGameOver())
   {
      display->updateScreen(new MainMenu(display));
      return true;
   }


   testDraw->setBlockRenders(levelBlockRenders);

   // Music jazz
   if (Mix_PlayingMusic() == 0)
      Mix_PlayMusic(chosenMusic, -1);
   
   SDL_RenderClear(display->getRenderer());

   SDL_RenderCopy(display->getRenderer(), paddleImageTexture, &paddleImageSourceTexture, &paddleImageRenderArea);
   SDL_RenderCopy(display->getRenderer(), ballImageTexture, &ballImageSourceTexture, &ballImageRenderArea);

   testDraw->drawLevel();

   SDL_RenderPresent(display->getRenderer());
   
   timerFps = SDL_GetTicks() - timerFps;
   display->fpsLock(timerFps);

   return true;
}

void GameScreen::initAssets()
{
   paddleImage = IMG_Load((display->getResourcePath("bats/bat_yellow.png")).c_str());
   ballImage = IMG_Load((display->getResourcePath("balls/ball_silverTEST.png")).c_str());
   
   // Check for bad loading
   if (paddleImage == NULL || ballImage == NULL)
      display->displayError(ErrorType::IMAGE_ERROR, "Error loading Image : ");

   // Load our music
   chosenMusic = Mix_LoadMUS((display->getResourcePath("music/ambient-wave-11-stretched.mp3")).c_str());

   if (chosenMusic == NULL)
      display->displayError(ErrorType::AUDIO_ERROR, "Error loading Music : ");
      
   // Load our textures
   paddleImageTexture = SDL_CreateTextureFromSurface(display->getRenderer(), paddleImage);
   ballImageTexture = SDL_CreateTextureFromSurface(display->getRenderer(), ballImage);
   
   paddleImageSourceTexture.x = 25;
   paddleImageSourceTexture.y = 180;
   paddleImageSourceTexture.w = 463;
   paddleImageSourceTexture.h = 100;

   ballImageSourceTexture.x = 0;
   ballImageSourceTexture.y = 0;
   ballImageSourceTexture.w = 367;
   ballImageSourceTexture.h = 367;
   
   paddleImageRenderArea.x = display->getWidth() / 2 - display->getWidth() / 5 / 2;
   paddleImageRenderArea.y = display->getHeight() - display->getHeight() / 20;
   paddleImageRenderArea.w = display->getWidth() / 5;
   paddleImageRenderArea.h = display->getHeight() / 20;

   ballImageRenderArea.w = paddleImageRenderArea.w / 10;
   ballImageRenderArea.h = ballImageRenderArea.w;
   ballImageRenderArea.y = paddleImageRenderArea.y - ballImageRenderArea.w;
   ballImageRenderArea.x = paddleImageRenderArea.x + paddleImageRenderArea.w / 2 - 10 - ballImageRenderArea.w / 2;

   testDraw = new LevelDraw(display, "levels/level.lvl");
   levelBlockRenders = testDraw->getBlockRenders();
   testEngine = new GameEngine(paddleImageRenderArea, ballImageRenderArea, levelBlockRenders, display->getWidth(), display->getHeight());
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
   paddleImageRenderArea.x = display->getWidth() / 2 - display->getWidth() / 10;
   paddleImageRenderArea.y = display->getHeight() - display->getHeight() / 20;
   paddleImageRenderArea.w = display->getWidth() / 5;
   paddleImageRenderArea.h = display->getHeight() / 20;
}