#ifndef SCREENINTERFACE_H
#define SCREENINTERFACE_H

class ScreenInterface
{
	public:
		ScreenInterface();
		~ScreenInterface();
		virtual void screenDraw() = 0;
	private:
};

#endif
