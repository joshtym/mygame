#include "GameEngine.h"
#include <iostream>
#include <algorithm>
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
        ballRenderArea.x = paddleRenderArea.x + paddleRenderArea.w / 2 - 20 - ballRenderArea.w / 2;
    else
    {
        ++xCounter;
        ++yCounter;

        if (xSpeed != 0 && xCounter % xSpeed == 0)
        {
            if (ballGoingLeft)
                ballRenderArea.x -= 2;
            else
                ballRenderArea.x += 2;
        } 
            

        if (ySpeed != 0 && yCounter % ySpeed == 0)
        {
            if (ballGoingUp)
                ballRenderArea.y -= 2;
            else
                ballRenderArea.y += 2;
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
        calculateBallSpeed();
    }

}

bool GameEngine::isGameOver()
{
    if ((ballRenderArea.y - ballRenderArea.h) > screenHeight)
        return true;
    else
        return false;
}

std::vector<SDL_Rect> GameEngine::updateLevelBlockRenders()
{
    return blockRenders;
}

void GameEngine::calculateBallSpeed()
{
    int distanceFromMiddleOfPaddle = (paddleRenderArea.x + paddleRenderArea.w / 2) - (ballRenderArea.x + ballRenderArea.w / 2);
    double percentageFromMiddle = static_cast<double>(distanceFromMiddleOfPaddle) / static_cast<double>(paddleRenderArea.w / 2);

    double floatXSpeed = 10 - 10 * abs(percentageFromMiddle);
    ySpeed = 5;

    xSpeed = round(floatXSpeed);
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
    if (!(ballGoingUp))
        if ((ballRenderArea.y + ballRenderArea.h) > paddleRenderArea.y)
            if (((ballRenderArea.x + ballRenderArea.w / 2) > paddleRenderArea.x) && ((ballRenderArea.x + ballRenderArea.w / 2) < paddleRenderArea.x + paddleRenderArea.w))
            {
                if ((ballRenderArea.x + ballRenderArea.w / 2) <= (paddleRenderArea.x + paddleRenderArea.w / 2))
                {
                    ballGoingLeft = true;
                    ballGoingUp = true;
                    calculateBallSpeed();
                }
                else
                {
                    ballGoingLeft = false;
                    ballGoingUp = true;
                    calculateBallSpeed();
                }
            }
}

void GameEngine::collisionDetectionBricks()
{
    // For every block, check for collision
    for (unsigned int i = 0; i < blockRenders.size(); ++i)
    {
        // Variable declarations. Produces negative value if further than block value
        int intersectionFromRight = ballRenderArea.x - (blockRenders[i].x + blockRenders[i].w);
        int intersectionFromLeft = blockRenders[i].x - (ballRenderArea.x + ballRenderArea.w);
        int intersectionFromTop = blockRenders[i].y - (ballRenderArea.y + ballRenderArea.h);
        int intersectionFromBottom = ballRenderArea.y - (blockRenders[i].y + blockRenders[i].h);

        // If we've intersected with the block, call our break block function to update the ball and delete the block
        if (intersectionFromRight < 0 && intersectionFromLeft < 0 && intersectionFromBottom < 0 && intersectionFromTop < 0)
        {
            breakBlock(i, intersectionFromLeft, intersectionFromRight, intersectionFromTop, intersectionFromBottom);
            break;
        }
    }
}

void GameEngine::breakBlock(int index, int intersectionFromLeft, int intersectionFromRight, int intersectionFromTop, int intersectionFromBottom)
{
    if (std::max(std::max(intersectionFromRight, intersectionFromLeft),std::max(intersectionFromTop, intersectionFromBottom)) == intersectionFromRight)
        ballGoingLeft = false;
    else if (std::max(std::max(intersectionFromRight, intersectionFromLeft),std::max(intersectionFromTop, intersectionFromBottom)) == intersectionFromLeft)
        ballGoingLeft = true;
    else if (std::max(std::max(intersectionFromRight, intersectionFromLeft),std::max(intersectionFromTop, intersectionFromBottom)) == intersectionFromBottom)
        ballGoingUp = false;
    else if (std::max(std::max(intersectionFromRight, intersectionFromLeft),std::max(intersectionFromTop, intersectionFromBottom)) == intersectionFromTop)
        ballGoingUp = true;

    blockRenders.erase(blockRenders.begin() + index);
}