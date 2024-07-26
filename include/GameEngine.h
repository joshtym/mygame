#ifndef GAMEENGINE_H
#define GAMESCREEN_H

#include <SDL2/SDL.h>
#include <vector>

class GameEngine
{
   public:
      GameEngine();
      GameEngine(SDL_Rect, SDL_Rect, std::vector<SDL_Rect>, int, int);
      ~GameEngine();

      SDL_Rect updatePaddle(int);
      SDL_Rect updateBall();
      void releaseBall();
      bool isGameOver();
      std::vector<SDL_Rect> updateLevelBlockRenders();
   private:
      void calculateBallSpeed();
      void collisionDetectionWalls();
      void collisionDetectionPaddle();
      void collisionDetectionBricks();
      void breakBlock(int, int, int, int, int);

      SDL_Rect ballRenderArea;
      SDL_Rect paddleRenderArea;
      std::vector<SDL_Rect> blockRenders;

      int screenWidth;
      int screenHeight;
      bool isOnPaddle;
      bool ballGoingLeft;
      bool ballGoingUp;
      int xSpeed;
      int ySpeed;
      int xCounter;
      int yCounter;
};
#endif