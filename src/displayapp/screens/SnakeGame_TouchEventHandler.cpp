#include "displayapp/screens/SnakeGame.h"
#include <lvgl/lvgl.h>
#include <cstdlib>

using namespace Pinetime::Applications::Screens;

void SnakeGame::TouchEventHandler(lv_obj_t* obj, lv_event_t event) {
  auto* self = static_cast<SnakeGame*>(lv_obj_get_user_data(obj));
  if (!self) return;
  if (event == LV_EVENT_RELEASED) {
    // Jeśli jesteśmy w stanie GameOver, restartuj grę
    if (self->currentState == SnakeGame::States::GameOver) {
      self->OnStart();
      return;
    }
    lv_point_t p;
    lv_indev_t* indev = lv_indev_get_act();
    lv_indev_get_point(indev, &p);
    int screenMid = LV_HOR_RES_MAX / 2;
    // Skręt w lewo/prawo względem obecnego kierunku
    if (p.x < screenMid) {
      // Skręt w lewo
      switch (self->direction) {
        case SnakeGame::Direction::Up: self->ChangeDirection(SnakeGame::Direction::Left); break;
        case SnakeGame::Direction::Down: self->ChangeDirection(SnakeGame::Direction::Right); break;
        case SnakeGame::Direction::Left: self->ChangeDirection(SnakeGame::Direction::Down); break;
        case SnakeGame::Direction::Right: self->ChangeDirection(SnakeGame::Direction::Up); break;
      }
    } else {
      // Skręt w prawo
      switch (self->direction) {
        case SnakeGame::Direction::Up: self->ChangeDirection(SnakeGame::Direction::Right); break;
        case SnakeGame::Direction::Down: self->ChangeDirection(SnakeGame::Direction::Left); break;
        case SnakeGame::Direction::Left: self->ChangeDirection(SnakeGame::Direction::Up); break;
        case SnakeGame::Direction::Right: self->ChangeDirection(SnakeGame::Direction::Down); break;
      }
    }
  }
}
