#include <iostream>
#include "LevelDraw.h"

LevelDraw::LevelDraw()
{
    display = nullptr;
}

LevelDraw::LevelDraw(Display *givenDisplay, std::string levelFile)
{
    display = nullptr;
    display = givenDisplay;
    blockRenders.clear();

    fileName = display->getResourcePath(levelFile);

    init();
}

LevelDraw::~LevelDraw()
{
}

void LevelDraw::init()
{
    blockImage = IMG_Load((display->getResourcePath("Pack/Bricks/brick_blue_small.png")).c_str());

    // Check for bad loading
    if (blockImage == NULL)
        std::cout << "Error loading Image : " << IMG_GetError() << std::endl;

    blockTexture = SDL_CreateTextureFromSurface(display->getRenderer(), blockImage);

    blockSourceRect.x = 141;
    blockSourceRect.y = 178;
    blockSourceRect.w = 232;
    blockSourceRect.h = 102;

    parseXML();
}

void LevelDraw::drawLevel()
{
    for (unsigned int i = 0; i != blockRenders.size(); ++i)
        SDL_RenderCopy(display->getRenderer(), blockTexture, &blockSourceRect, &blockRenders[i]);
}

void LevelDraw::parseXML()
{
    if (!file.load_file(fileName.c_str()))
        return;

    pugi::xml_node tools = file.child("level");

    for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
    {
        SDL_Rect tempRect;

        pugi::xml_attribute_iterator ait = it->attributes_begin();

        tempRect.x = atoi(ait->value());
        ++ait;

        tempRect.y = atoi(ait->value());
        ++ait;

        tempRect.w = atoi(ait->value());
        ++ait;

        tempRect.h = atoi(ait->value());

        blockRenders.push_back(tempRect);
    }
}