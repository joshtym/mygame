#include "MainMenu.h"
#include "GameScreen.h"
#include <iostream>

/*
* Default Constructor
*
* Default constructor to force some textures to the nullptr
*/
MainMenu::MainMenu()
{
   SDL_ShowCursor(SDL_ENABLE);
   backLayer.clear();

   display = nullptr;
}

/*
* Constructor
*
* Constructor  to initialize the display using the pointer provided as well as
* force textures to start as null. Clears vectors
*
* @param givenDisplay
*/
MainMenu::MainMenu(Display* givenDisplay)
{
   SDL_ShowCursor(SDL_ENABLE);
   display = givenDisplay;

   backLayer.clear();

   initAssets();
}

/*
* Destructor
*
* Destroys all textures, frees all surfaces and sets everything to the nullptr
*/
MainMenu::~MainMenu()
{
   // Destroy textures
   SDL_DestroyTexture(backgroundTexture);

   for (int i = 0; i < 4; ++i)
   {
      SDL_DestroyTexture(buttonTextures[i]);
      SDL_FreeSurface(buttonImages[i]);
      buttonTextures[i] = nullptr;
      buttonImages[i] = nullptr;
   }

   // Free Surfaces
   SDL_FreeSurface(backgroundImage);

   // Ensure everything is set to the nullptr
   display = nullptr;
   backgroundTexture = nullptr;
   backgroundImage = nullptr;
}

/*
* Public function to draw content to the screen and track incoming events
*
* @return success or not
*/
bool MainMenu::screenDraw()
{
   // Create an SDL_Event
   SDL_Event event;
      
   // Create a starting ticker
   int tickStart = SDL_GetTicks();

   /// TODO : ADD MOUSE INTERACTION

   // Poll for events and handle them accordingly
   if (SDL_PollEvent(&event))
   {
      // Exit the program if user hits the x
      if (event.type == SDL_QUIT)
         return false;
               
      // Exit the program if user hits escape
      if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_ESCAPE))
         return false;
         
      // Start a new game if user hits the n key
      if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_n))
      {
         display->updateScreen(new GameScreen(display));
         return true;
      }
      
      // Handle resize event
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

      if (event.type == SDL_MOUSEMOTION)
         updateButtonRect(event.motion.x, event.motion.y);

      if (event.type == SDL_MOUSEBUTTONUP)
      {
         if (buttonActive[0])
         {
            display->updateScreen(new GameScreen(display));
            return true;
         }
         else if (buttonActive[3])
            return false;
      }
   }

   SDL_RenderClear(display->getRenderer());
   
   if (!(drawParralaxBackground()))
      display->displayError(ErrorType::PARALLAX_ERROR, "Failed to draw Parallax Background");

   for (int i = 0; i < 4; ++i)
      display->renderTexture(buttonTextures[i], 0, 0, &(buttonRects[i]));

   SDL_RenderPresent(display->getRenderer());
   
   tickStart = SDL_GetTicks() - tickStart;
   display->fpsLock(tickStart);

   return true;
}

void MainMenu::initAssets()
{
   // Load our backgroundImage
   backgroundImage = IMG_Load((display->getResourcePath("background/Parallax100.png")).c_str());
   buttonImages[0] = IMG_Load(display->getResourcePath("buttons/PlayButton.png").c_str());
   buttonImages[1] = IMG_Load(display->getResourcePath("buttons/LevelSelectButton.png").c_str());
   buttonImages[2] = IMG_Load(display->getResourcePath("buttons/OptionsButton.png").c_str());
   buttonImages[3] = IMG_Load(display->getResourcePath("buttons/QuitButton.png").c_str());
   
   // Check for bad loading
   if (backgroundImage == NULL)
      display->displayError(ErrorType::IMAGE_ERROR, "Error loading images : ");

   for (int i = 0; i < 4; ++i)
      if (buttonImages[i] == NULL)
         display->displayError(ErrorType::IMAGE_ERROR, "Error loading images : ");
      
   // Load our backgroundTexture
   backgroundTexture = SDL_CreateTextureFromSurface(display->getRenderer(), backgroundImage);

   for (int i = 0; i < 4; ++i)
      buttonTextures[i] = SDL_CreateTextureFromSurface(display->getRenderer(), buttonImages[i]);
   
   SDL_QueryTexture(backgroundTexture, NULL, NULL, &backgroundRect.w, &backgroundRect.h);

   buttonRects[0].x = buttonRects[2].x = 200;
   buttonRects[1].x = buttonRects[3].x = 450;
   buttonRects[0].y = buttonRects[1].y = 210;
   buttonRects[2].y = buttonRects[3].y = 350;
   
   for (int i = 0; i < 4; ++i)
   {
      buttonRects[i].w = 150;
      buttonRects[i].h = 40;
   }
   
   backgroundRect.x = 0;
   backgroundRect.y = 0;
   
   for (int i = 0; i < 6; ++i)
      backLayer.push_back(backgroundRect);
   
   backLayer[1].y = 500;
   backLayer[2].x = 500;
   backLayer[3].x = 500;
   backLayer[3].y = 500;
   backLayer[4].x = 1000;
   backLayer[5].x = 1000;
   backLayer[5].y = 500;
   
   fps = 20;
   counter = 0;
}

bool MainMenu::drawParralaxBackground()
{
   ++counter;
   
   if (counter % fps == 0)
      for (int i = 0; i < 6; ++i)
         --backLayer[i].x;
   
   for (int i = 0; i < 6; ++i)
      if (backLayer[i].x < -500)
         backLayer[i].x += 1500;

         
   display->renderTexture(backgroundTexture, -300, 0, &backLayer[0]);
   display->renderTexture(backgroundTexture, -300, 0, &backLayer[1]);
   display->renderTexture(backgroundTexture, -300, 0, &backLayer[2]);
   display->renderTexture(backgroundTexture, -300, 0, &backLayer[3]);
   display->renderTexture(backgroundTexture, -300, 0, &backLayer[4]);
   display->renderTexture(backgroundTexture, -300, 0, &backLayer[5]);
   
   return true;
}

void MainMenu::updateButtonRect(int x, int y)
{
   for (int i = 0; i < 4; ++i)
   {
      if (buttonActive[i])
      {
         if (x < buttonRects[i].x || x > buttonRects[i].x + buttonRects[i].w || y < buttonRects[i].y || y > buttonRects[i].y + buttonRects[i].h)
         {
            buttonActive[i] = false;
            buttonRects[i].x -= 5;
            buttonRects[i].y -= 5;
         }
      }
      else
      {
          if (x >= buttonRects[i].x && x <= buttonRects[i].x + buttonRects[i].w)
            if (y >= buttonRects[i].y && y <= buttonRects[i].y + buttonRects[i].h)
            {
               buttonActive[i] = true;
               buttonRects[i].x += 5;
               buttonRects[i].y += 5;
            }
      }
   }
}