#pragma once

#include <lvgl/lvgl.h>
#include <unordered_map>
#include "displayapp/screens/Screen.h"
#include "components/ble/SimpleWeatherService.h"
#include "components/timer/Timer.h"
#include "displayapp/apps/Apps.h"
#include "displayapp/Controllers.h"
#include "displayapp/widgets/Counter.h"
#include "utility/DirtyValue.h"
#include "Symbols.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      enum PomodoroState {
        Work,
        Break,
        // Idle
      };

      class Pomodoro : public Screen {
        public:
          Pomodoro(Controllers::Timer& timerController);
          ~Pomodoro() override;
          void Refresh() override;
           
          bool OnTouchEvent(TouchEvents event) override;
          bool OnButtonPushed() override;
          void ToggleRunning();
          void UpdateTime();
          void OnIntervalDone();

        private:
          void SetTimerRunning();
          void SetTimerStopped();
          void UpdatePhaseTitle();
          void ApplyConfig();

          std::unordered_map<PomodoroState, uint16_t> config = {
              {PomodoroState::Work, 6},
              {PomodoroState::Break, 3},
          };
          PomodoroState state = PomodoroState::Work;
          uint_fast16_t elapsedSeconds = config[state] * 60;

          Controllers::Timer& timer;
          Widgets::Counter workInterval = Widgets::Counter(0, 59, jetbrains_mono_42);
          Widgets::Counter breakInterval = Widgets::Counter(0, 59, jetbrains_mono_42);
          Utility::DirtyValue<std::chrono::seconds> displaySeconds;

          lv_obj_t* txtPhase;
          lv_obj_t* progressCircle;
          lv_obj_t* txtElapsedTime;
          lv_obj_t* btnPlayPause;
          lv_obj_t* txtPlayPause;
          lv_obj_t* lineMeter;  // Add line meter object
          lv_obj_t* btnObjectMask;
          lv_obj_t* highlightObjectMask;
          lv_objmask_mask_t* btnMask;
          lv_objmask_mask_t* highlightMask;
          lv_task_t* taskRefresh;
      };
    }

    template <>
    struct AppTraits<Apps::Pomodoro> {
      static constexpr Apps app = Apps::Pomodoro;
      static constexpr const char* icon = Screens::Symbols::brain;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::Pomodoro(controllers.pomodoroController);
      };
    };
  }
} 