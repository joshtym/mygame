#ifndef SCREENINTERFACE_H
#define SCREENINTERFACE_H

class ScreenInterface
{
    public:
        virtual ~ScreenInterface() {}
        virtual bool screenDraw() = 0;
};

#endif
