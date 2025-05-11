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
        enum class States { Start, Game, GameOver };

        static constexpr int boardWidth = 16;
        static constexpr int boardHeight = 16;

        std::vector<Point> snake; // Snake body, head at front
        Point food;
        Direction direction;
        States currentState = States::Start;
        bool gameOver = false;
        int score = 0;
        lv_task_t* tickTask = nullptr; // LVGL periodic task for game loop

        // --- Game Logic Methods ---
        void StartGame();
        void MoveSnake();
        void PlaceFood();
        bool CheckCollision(const Point& nextHead) const;
        void ChangeDirection(Direction newDir);
        void OnTick(); // To be called on timer
        void DrawBoard(); // Render snake, food, and board

        // State handlers
        void OnStart();
        void OnGame();
        void OnGameOver();

        // Touch/gesture support
        static void TouchEventHandler(lv_obj_t* obj, lv_event_t event);
        lv_point_t touchStart;

      };


    }
  }
}
