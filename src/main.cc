#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Display.h"

int main(int argc, char **argv)
{
   // Init display
   Display testDisplay(800, 600, 1000);
   
   // check for the display to be initted
   if (!(testDisplay.initDisplay()))
      return 1;
   
   // Draw screen
   while (true)
      if (!(testDisplay.drawScreen()))
         break;
   
   return 0;
}
