#include "GameEngine.h"

GameEngine::GameEngine()
{
    blockRenders.clear();
    isOnPaddle = true;
    motionX = 0;
    motionY = 0;
}

GameEngine::GameEngine(SDL_Rect givenPaddleRender, SDL_Rect givenBallRender, std::vector<SDL_Rect> givenBlockRenders, int width, int height)
{
    ballRenderArea = givenBallRender;
    paddleRenderArea = givenPaddleRender;

    blockRenders.clear();
    blockRenders = givenBlockRenders;

    screenWidth = width;
    screenHeight = height;

    isOnPaddle = true;

    motionX = 1;
    motionY = 0;
}

GameEngine::~GameEngine()
{
}

SDL_Rect GameEngine::updatePaddle(int mouseXLocation)
{
    if (mouseXLocation - paddleRenderArea.w / 2 < 0)
        paddleRenderArea.x = 0;
    else if (mouseXLocation + paddleRenderArea.w / 2 > screenWidth)
        paddleRenderArea.x = screenWidth - paddleRenderArea.w;
    else
        paddleRenderArea.x = mouseXLocation - paddleRenderArea.w / 2;

    if (isOnPaddle)
        updateBall();

    return paddleRenderArea;
}

SDL_Rect GameEngine::updateBall()
{
    if (isOnPaddle)
        ballRenderArea.x = paddleRenderArea.x + paddleRenderArea.w / 2 - 10 - ballRenderArea.w / 2;
    else
        ballRenderArea.x += motionX;

    collisionDetectionWalls();
    collisionDetectionPaddle();
    collisionDetectionBricks();

    return ballRenderArea;
}

void GameEngine::releaseBall()
{
    isOnPaddle = false;
}

void GameEngine::collisionDetectionWalls()
{
    // Ensure no passage of the left hand side of the screen
    if (ballRenderArea.x < 0)
    {
        ballRenderArea.x = 0;
        motionX = motionX * -1;
    }
        

    // Ensure no passage of the top of the screen
    if (ballRenderArea.y < 0)
    {
        ballRenderArea.y = 0;
        motionY = motionY * -1;
    }

    // Ensure no passage of the right hand side of the screen
    if ((ballRenderArea.x + ballRenderArea.w) > screenWidth)
    {
        ballRenderArea.x = screenWidth - ballRenderArea.w;
        motionX = motionX * -1;
    }
}

void GameEngine::collisionDetectionPaddle()
{
}

void GameEngine::collisionDetectionBricks()
{
}