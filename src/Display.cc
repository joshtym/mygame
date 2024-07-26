#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Display.h"
#include "MainMenu.h"

/*
 * Default Constructor
 *
 * Initialize default width, height and fps values
*/
Display::Display()
{
    // Assign the width and height of the window
    width = 600;
    height = 400;
    fps = 1000;
}

/*
 * Constructor
 *
 * Take parameters for display width, height and fps and assign
 *
 * @param screenWidth
 * @param screenHeight
*/
Display::Display(int screenWidth, int screenHeight, int givenFps)
{
    // Assign the width and height of the window
    width = screenWidth;
    height = screenHeight;
    fps = givenFps;
}

/*
 * Destructor
 *
 * Destroy the render, window and assign to nullptr. Close all fonts and quit
 * all secondary services
*/
Display::~Display()
{
    // Destroy the renderer and set it to the nullptr
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    
    // Destroy the window an set it to the nullptr
    SDL_DestroyWindow(window);
    window = nullptr;
    
    // Close our font
    for (std::map<int, TTF_Font*>::const_iterator it = fonts.begin(); it != fonts.end(); ++it)
        TTF_CloseFont(it->second);
    
    // Quit TTF, Image, and SDL
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}


/*
 * PUBLIC FUNCTIONS
*/


/*
 * Public function to initialize the display
 *
 * Initialize the libraries, window and renderer. Hide cursor and initizlie starting screen
 *
 *@return Successfully Initialized
*/
bool Display::initDisplay()
{
    // Init libraries and ensure that it was properly initted
    if (!(initLibraries()))
        return false;
    
    // Initialize our window
    window = SDL_CreateWindow("Game", 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    // Check for errors on the window initialization
    if (window == nullptr)
    {
        printOutErrorMessage(ErrorType::SDL_ERROR, "SDL_CreateWindow Error : ");
        return false;
    }
    
    // Initialize our renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Check for nullptr and return appropriately
    if (renderer == nullptr)
    {
        printOutErrorMessage(ErrorType::SDL_ERROR, "SDL_CreateRenderer Error : ");
        return false;
    }
    
    // Disable the cursor
    SDL_ShowCursor(SDL_DISABLE);

    // Initialize to the main menu
    currentScreen = new MainMenu(this);
    
    return true;
}

/*
 * Public function that renders a texture with given parameters
 *
 * @param texture
 * @param x
 * @param y
 * @param source
*/
void Display::renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* source)
{
    // Variable declaration
    SDL_Rect destination;
    
    // Set the location of the rectangle on the plane
    destination.x = x;
    destination.y = y;
    
    // Check if our given rectangle is the nullptr and act accordingly to assign
    // the width and height of the renderer
    if (source != nullptr)
    {
        destination.x = source->x;
        destination.y = source->y;
        destination.w = source->w;
        destination.h = source->h;
    }
    else
        SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
    
    // Render the texture
    SDL_RenderCopy(renderer, texture, NULL, &destination);
}

/*
 * Public function to update the screen to the new screen
 *
 * @param newScreen
*/
void Display::updateScreen(ScreenInterface* newScreen)
{
    delete currentScreen;
    currentScreen = newScreen;
}

/*
 * Public function to drawn the current screen
 *
 * @return successful or not
*/
bool Display::drawScreen()
{
    if (currentScreen->screenDraw())
        return true;
    else
        return false;
}

/*
 * Public function to ensure fps lock to manage computer resources
 *
 * @param timerFps
*/
void Display::fpsLock(int timerFps)
{
    // Add a delay depending on the fps counter
    if (timerFps < (1000 / fps))
        SDL_Delay((1000/ fps) - timerFps);
}

/*
 * Public function to return the renderer
 *
 * @return renderer
*/
SDL_Renderer* Display::getRenderer()
{
    return renderer;
}

/*
 * Public function to return the window
 *
 * @return window
*/
SDL_Window* Display::getWindow()
{
    return window;
}

/*
 * Public function to return the font with the correct size
 *
 * @param fontSize
 * @return font with size fontSize
*/
TTF_Font* Display::getFont(int fontSize)
{
    // Get a resource path
    std::string resourcePath = getResourcePath("victor-pixel.ttf");

    // Iterator for the map
    std::map<int, TTF_Font*>::iterator it;

    // Try and find the current font
    it = fonts.find(fontSize);
    
    // Check if the font is in the map. If it's not, create it with error checking and return the font
    if (it != fonts.end())
        return it->second;
    else
    {
        TTF_Font* newFont = TTF_OpenFont(resourcePath.c_str(), fontSize);
        
        // Print out error message if new font does not initialize, else, add to map
        if (newFont == NULL)
            printOutErrorMessage(ErrorType::TTF_ERROR, "Error opening Font : ");
        else
            fonts.insert(std::pair<int, TTF_Font*>(fontSize, newFont));
        
        return newFont;
    }
}    

/*
 * Public function to get the resource path in the assets directory
 *
 * @param resourceName
 * @return resourcePath
*/
std::string Display::getResourcePath(std::string resourceName)
{
    // Variable declarations
    char* basePath;
    std::string stringBasePath;
    std::string resourcePath = "";
    
    // Get the base path of the directory
    basePath = SDL_GetBasePath();
    
    // Check for null values and assign accordingly or error accordingly
    if (basePath != NULL)
    {
        stringBasePath = basePath;
        SDL_free(basePath);
    }
    else
        printOutErrorMessage(ErrorType::SDL_ERROR, "Error getting resource path : ");
    
    // Get the position of our bin directory and change to assets
    size_t pos = stringBasePath.rfind("/bin");
    stringBasePath = stringBasePath.substr(0, pos);
    stringBasePath.append("/assets/");
    
    // Now assign our first font path to what we want
    resourcePath = stringBasePath + resourceName;
    
    return resourcePath;
}

/*
 * Public function to get the height of the display
 *
 * @return height
*/
int Display::getHeight()
{
    SDL_GetWindowSize(window, &width, &height);
    return height;
}

/*
 * Public function to get the width of the display
 *
 * @return width
*/
int Display::getWidth()
{
    SDL_GetWindowSize(window, &width, &height);
    return width;
}


/*
 * PRIVATE FUNCTIONS
*/


/*
 * Private function to initialize all libraries such as SDL, TTF, Mix and IMG
 *
 * @return success or not
*/
bool Display::initLibraries()
{
    // Initialize the SDL libraries with video capabilities. Check for error
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        printOutErrorMessage(ErrorType::SDL_ERROR, "SDL_Init Error : ");
        return false;
    }
    
    // Initialize the font services. Check for error
    if (TTF_Init() == -1)
    {
        printOutErrorMessage(ErrorType::TTF_ERROR, "TTF_Init Error : ");
        return false;
    }
    
    // Initialize our image services then check for error
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(imgFlags);
    
    if ((initted & imgFlags) != imgFlags)
    {
        printOutErrorMessage(ErrorType::IMAGE_ERROR, "Failed to init required jpg and png support. Error : ");
        return false;
    }

    // Initialize our audio services and check for error
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printOutErrorMessage(ErrorType::AUDIO_ERROR, "Failed to init support for Audio. Error: ");
        return false;
    }
    
    return true;
}

/*
 * Private function to print out the error message using a default ErrorType and given string
 *
 * @param ErrorType error
 * @param message
*/
void Display::printOutErrorMessage(ErrorType error, std::string message)
{
    // Some Variables
    std::string errorMessage = SDL_GetError();
    std::string fullMessage = message + errorMessage;

    // Print correct error message
    if (error == ErrorType::SDL_ERROR)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Display Error", fullMessage.c_str(), NULL);
    else if (error == ErrorType::TTF_ERROR)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error", fullMessage.c_str(), NULL);
    else if (error == ErrorType::IMAGE_ERROR)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Image Error", fullMessage.c_str(), NULL);
    else if (error == ErrorType::AUDIO_ERROR)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Audio Error", fullMessage.c_str(), NULL);
}