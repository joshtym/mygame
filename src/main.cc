#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Display.h"

int main(int argc, char **argv)
{
   // Init display
   Display myDisplay(800, 600, 1000);
   
   // check for the display to be initted
   if (!(myDisplay.initDisplay()))
      return 1;
   
   // Draw screen
   while (true)
      if (!(myDisplay.drawScreen()))
         break;
   
   return 0;
}
