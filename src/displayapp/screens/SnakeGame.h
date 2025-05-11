#pragma once

#include "displayapp/screens/Screen.h"
#include <vector>

namespace Pinetime {
  namespace Applications {
    namespace Screens {

      class SnakeGame : public Screen {
      public:
        SnakeGame();
        ~SnakeGame() override;

        // --- Snake Game Data Structures ---
        struct Point {
          int x;
          int y;
          bool operator==(const Point& other) const { return x == other.x && y == other.y; }
        };


        enum class Direction { Up, Down, Left, Right };

        static constexpr int boardWidth = 16;
        static constexpr int boardHeight = 16;

        std::vector<Point> snake; // Snake body, head at front
        Point food;
        Direction direction;
        bool gameOver = false;
        int score = 0;

        // --- Game Logic Methods ---
        void StartGame();
        void MoveSnake();
        void PlaceFood();
        bool CheckCollision(const Point& nextHead) const;
        void ChangeDirection(Direction newDir);
        void OnTick(); // To be called on timer
        void DrawBoard(); // Render snake, food, and board
      };


    }
  }
}
