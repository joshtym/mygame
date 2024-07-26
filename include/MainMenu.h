#ifndef MAINMENU_H
#define MAINMENU_H

#include <SDL2/SDL_image.h>
#include <vector>
#include "ScreenInterface.h"
#include "Display.h"

class MainMenu : public ScreenInterface
{
   public:
      // Constructors
      MainMenu();
      MainMenu(Display*);

      // Destructor
      ~MainMenu();

      // Virtual public funtcion instantiation
      bool screenDraw();
   private:
      // Private functions
      void initAssets();
      bool drawParralaxBackground();
      void updateButtonRect(int, int);

      // Private variables
      Display* display;
      SDL_Surface* backgroundImage;
      SDL_Texture* backgroundTexture;
      SDL_Rect backgroundRect;

      SDL_Surface* buttonImages[4] = {nullptr};
      SDL_Texture* buttonTextures[4] = {nullptr};
      SDL_Rect buttonRects[4];
      bool buttonActive[4] = {false};

      std::vector<SDL_Rect> backLayer;
      int fps;
      int counter;
      int titleHeight, titleWidth;
      int gameOption1Height, gameOption1Width;
      int gameOption2Height, gameOption2Width;
      bool shift = false;
};
#endif
