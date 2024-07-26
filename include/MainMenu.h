#ifndef MAINMENU_H
#define MAINMENU_H

#include "ScreenInterface.h"
#include "Display.h"

class MainMenu : public ScreenInterface
{
	public:
		MainMenu();
		MainMenu(Display*);
		~MainMenu();
		void screenDraw();
	private:
		Display* display;
};
#endif
