#include "GameEngine.h"
#include <iostream>
#include <cmath>

GameEngine::GameEngine()
{
    blockRenders.clear();
    isOnPaddle = true;
    ballGoingLeft = true;
    ballGoingUp = true;
    xCounter = 0;
    yCounter = 0;
}

GameEngine::GameEngine(SDL_Rect givenPaddleRender, SDL_Rect givenBallRender, std::vector<SDL_Rect> givenBlockRenders, int width, int height)
{
    ballRenderArea = givenBallRender;
    paddleRenderArea = givenPaddleRender;

    blockRenders.clear();
    blockRenders = givenBlockRenders;

    screenWidth = width;
    screenHeight = height;

    ballGoingLeft = true;
    ballGoingUp = true;

    isOnPaddle = true;

    xCounter = 0;
    yCounter = 0;
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
    {
        ++xCounter;
        ++yCounter;

        if (xCounter % xSpeed == 0)
        {
            if (ballGoingLeft)
                ballRenderArea.x -= 1;
            else
                ballRenderArea.x += 1;
        } 
            

        if (yCounter % ySpeed == 0)
        {
            if (ballGoingUp)
                ballRenderArea.y -= 1;
            else
                ballRenderArea.y += 1;
        }
    
        collisionDetectionWalls();
        collisionDetectionPaddle();
        collisionDetectionBricks();
    }

    return ballRenderArea;
}

void GameEngine::releaseBall()
{
    if (isOnPaddle)
    {
            isOnPaddle = false;

        int distanceFromMiddleOfPaddle = (paddleRenderArea.x + paddleRenderArea.w / 2) - (ballRenderArea.x + ballRenderArea.w / 2);
        double percentageFromMiddle = static_cast<double>(distanceFromMiddleOfPaddle) / static_cast<double>(paddleRenderArea.w / 2);
        double floatXSpeed = 50 * percentageFromMiddle;
        double floatYSpeed = 25 - 25 * percentageFromMiddle;

        xSpeed = round(floatXSpeed);
        ySpeed = round(floatYSpeed);
    }

}

bool GameEngine::isGameOver()
{
    if ((ballRenderArea.y - ballRenderArea.h) > screenHeight)
        return true;
    else
        return false;
}

void GameEngine::collisionDetectionWalls()
{
    // Ensure no passage of the left hand side of the screen
    if (ballRenderArea.x < 0)
    {
        ballRenderArea.x = 0;
        ballGoingLeft = false;
    }
        

    // Ensure no passage of the top of the screen
    if (ballRenderArea.y < 0)
    {
        ballRenderArea.y = 0;
        ballGoingUp = false;
    }

    // Ensure no passage of the right hand side of the screen
    if ((ballRenderArea.x + ballRenderArea.w) > screenWidth)
    {
        ballRenderArea.x = screenWidth - ballRenderArea.w;
        ballGoingLeft = true;
    }
}

void GameEngine::collisionDetectionPaddle()
{
}

void GameEngine::collisionDetectionBricks()
{
}