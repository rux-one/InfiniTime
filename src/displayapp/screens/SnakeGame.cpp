#include "displayapp/screens/SnakeGame.h"
#include <lvgl/lvgl.h>
#include <cstdlib> // for rand
#include <cstdio>  // for snprintf

using namespace Pinetime::Applications::Screens;

SnakeGame::SnakeGame() : Screen() {
  StartGame();
}


SnakeGame::~SnakeGame() {
  // Cleanup if needed
}

void SnakeGame::StartGame() {
  // Start in the center
  snake.clear();
  Point start = {boardWidth / 2, boardHeight / 2};
  snake.push_back(start);
  direction = Direction::Right;
  score = 0;
  gameOver = false;
  PlaceFood();
  DrawBoard();
}

void SnakeGame::PlaceFood() {
  // Simple random placement, not on snake
  while (true) {
    int fx = rand() % boardWidth;
    int fy = rand() % boardHeight;
    Point candidate = {fx, fy};
    bool onSnake = false;
    for (const auto& s : snake) {
      if (s == candidate) {
        onSnake = true;
        break;
      }
    }
    if (!onSnake) {
      food = candidate;
      break;
    }
  }
}

void SnakeGame::MoveSnake() {
  if (gameOver) return;
  // Calculate new head position
  Point head = snake.front();
  Point next = head;
  switch (direction) {
    case Direction::Up: next.y--; break;
    case Direction::Down: next.y++; break;
    case Direction::Left: next.x--; break;
    case Direction::Right: next.x++; break;
  }
  // Check for collision
  if (CheckCollision(next)) {
    gameOver = true;
    return;
  }
  // Insert new head
  snake.insert(snake.begin(), next);
  // Check if food eaten
  if (next == food) {
    score++;
    PlaceFood();
  } else {
    // Remove tail
    snake.pop_back();
  }
}

bool SnakeGame::CheckCollision(const Point& nextHead) const {
  // Check wall collision
  if (nextHead.x < 0 || nextHead.x >= boardWidth || nextHead.y < 0 || nextHead.y >= boardHeight)
    return true;
  // Check self collision
  for (const auto& s : snake) {
    if (s == nextHead) return true;
  }
  return false;
}

void SnakeGame::OnTick() {
  if (!gameOver) {
    MoveSnake();
    DrawBoard();
  }
}

void SnakeGame::DrawBoard() {
  // Clear screen
  lv_obj_clean(lv_scr_act());

  // Board pixel size
  constexpr int margin = 8;
  constexpr int cellSize = 12;
  const int boardPxW = boardWidth * cellSize;
  const int boardPxH = boardHeight * cellSize;
  const int x0 = (LV_HOR_RES_MAX - boardPxW) / 2;
  const int y0 = (LV_VER_RES_MAX - boardPxH) / 2;

  // Draw food
  lv_obj_t* food_rect = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_size(food_rect, cellSize, cellSize);
  lv_obj_set_style_local_bg_color(food_rect, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000)); // red
  lv_obj_set_style_local_border_width(food_rect, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_radius(food_rect, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 2);
  lv_obj_set_pos(food_rect, x0 + food.x * cellSize, y0 + food.y * cellSize);

  // Draw snake
  bool isHead = true;
  for (const auto& seg : snake) {
    lv_obj_t* seg_rect = lv_obj_create(lv_scr_act(), nullptr);
    lv_obj_set_size(seg_rect, cellSize, cellSize);
    if (isHead) {
      lv_obj_set_style_local_bg_color(seg_rect, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00CC00)); // green head
      isHead = false;
    } else {
      lv_obj_set_style_local_bg_color(seg_rect, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x007700)); // darker body
    }
    lv_obj_set_style_local_border_width(seg_rect, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(seg_rect, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 2);
    lv_obj_set_pos(seg_rect, x0 + seg.x * cellSize, y0 + seg.y * cellSize);
  }

  // Draw score (top left)
  char scoreStr[32];
  snprintf(scoreStr, sizeof(scoreStr), "Score: %d", score);
  lv_obj_t* label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(label, scoreStr);
  lv_obj_align(label, nullptr, LV_ALIGN_IN_TOP_LEFT, 4, 2);

  // Draw game over
  if (gameOver) {
    lv_obj_t* overLabel = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text(overLabel, "Game Over!");
    lv_obj_align(overLabel, nullptr, LV_ALIGN_CENTER, 0, 0);
  }
}
