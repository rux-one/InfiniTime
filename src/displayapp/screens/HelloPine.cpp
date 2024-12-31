#include "displayapp/screens/HelloPine.h"
#include "displayapp/InfiniTimeTheme.h"
#include <lvgl/lvgl.h>

using namespace Pinetime::Applications::Screens;

static void btnEventHandler(lv_obj_t* obj, lv_event_t event) {
  auto* screen = static_cast<HelloPine*>(obj->user_data);
  if (event == LV_EVENT_SHORT_CLICKED) {
    screen->ToggleRunning();
  }
}

HelloPine::HelloPine(Controllers::Timer& timerController) : timer {timerController} {
  workInterval.Create();
  lv_obj_align(workInterval.GetObject(), nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);

  breakInterval.Create();
  lv_obj_align(breakInterval.GetObject(), nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  workInterval.SetValue(config[PomodoroState::Work]);
  breakInterval.SetValue(config[PomodoroState::Break]);
  
  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);

  // button
  btnObjectMask = lv_objmask_create(lv_scr_act(), nullptr);
  lv_obj_set_size(btnObjectMask, 240, 50);
  lv_obj_align(btnObjectMask, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  btnPlayPause = lv_btn_create(btnObjectMask, nullptr);
  btnPlayPause->user_data = this;
  lv_obj_set_style_local_radius(btnPlayPause, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_bg_color(btnPlayPause, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, Colors::bgAlt);
  lv_obj_set_event_cb(btnPlayPause, btnEventHandler);
  lv_obj_set_size(btnPlayPause, LV_HOR_RES, 50);

  txtPlayPause = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align(txtPlayPause, btnPlayPause, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text_static(txtPlayPause, "Start");
  // end of: button

  // progress circle
  progressCircle = lv_linemeter_create(lv_scr_act(), nullptr);
  lv_obj_set_size(progressCircle, 160, 160);
  lv_obj_align(progressCircle, nullptr, LV_ALIGN_CENTER, 0, -20);
  
  lv_linemeter_set_range(progressCircle, 0, 100);
  lv_linemeter_set_value(progressCircle, 0);
  lv_linemeter_set_angle_offset(progressCircle, 180);
  lv_linemeter_set_scale(progressCircle, 360, 60);
  lv_obj_set_style_local_line_color(progressCircle, LV_LINEMETER_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_set_style_local_scale_end_color(progressCircle, LV_LINEMETER_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x10, 0x10, 0x10));
  lv_obj_set_style_local_line_width(progressCircle, LV_LINEMETER_PART_MAIN, LV_STATE_DEFAULT, 3);
  // end of: progress circle

  txtElapsedTime = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align(txtElapsedTime, nullptr, LV_ALIGN_CENTER, -6, -20);
  lv_label_set_text(txtElapsedTime, "00:00");
}

HelloPine::~HelloPine() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void HelloPine::UpdateTime() {
  displaySeconds = std::chrono::duration_cast<std::chrono::seconds>(timer.GetTimeRemaining());

  uint16_t elapsedSeconds = displaySeconds.Get().count();
  uint16_t minutes = elapsedSeconds / 60;
  uint16_t seconds = elapsedSeconds % 60;
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", minutes, seconds);
  lv_label_set_text(txtElapsedTime, timeStr);

  int progress = config[state] - displaySeconds.Get().count();
  lv_linemeter_set_range(progressCircle, 0, config[state]);
  lv_linemeter_set_value(progressCircle, progress);
  if (progress > 95) {
    lv_obj_set_style_local_line_color(progressCircle, LV_LINEMETER_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
  } else {
    lv_obj_set_style_local_line_color(progressCircle, LV_LINEMETER_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  }
}

void HelloPine::SetTimerRunning() {
  workInterval.HideAll();
  breakInterval.HideControls();
  lv_label_set_text_static(txtPlayPause, "Pause");
}

void HelloPine::SetTimerStopped() {
  workInterval.ShowControls();
  breakInterval.ShowControls();
  lv_label_set_text_static(txtPlayPause, "Start");
}

void HelloPine::ToggleRunning() {
  if (timer.IsRunning()) {
    UpdateTime();
    timer.StopTimer();
    SetTimerStopped();
  } else if (breakInterval.GetValue() + workInterval.GetValue() > 0) {
    auto timerDuration = std::chrono::seconds(config[state]);
    timer.StartTimer(timerDuration);
    UpdateTime();
    SetTimerRunning();
  }
}

void HelloPine::Refresh() {
  if (timer.IsRunning()) {
    UpdateTime();
  }
}

bool HelloPine::OnTouchEvent([[maybe_unused]] TouchEvents event) {
  return false;
}

bool HelloPine::OnButtonPushed() {
  return false;
} 