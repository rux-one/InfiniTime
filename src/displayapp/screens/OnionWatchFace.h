#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "components/settings/Settings.h"
#include "utility/DirtyValue.h"
#include "displayapp/apps/Apps.h"
#include "displayapp/Controllers.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {

      class OnionWatchFace : public Screen {
      public:
        OnionWatchFace(Controllers::DateTime& dateTimeController, Controllers::Settings& settingsController);
        ~OnionWatchFace() override;

      private:
        void Refresh() override;

        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>> currentDateTime {};

        lv_obj_t* label_time {nullptr};
        lv_task_t* taskRefresh {nullptr};

        Controllers::DateTime& dateTimeController;
        Controllers::Settings& settingsController;
      };
    }

    template <>
    struct WatchFaceTraits<WatchFace::Onion> {
      static constexpr WatchFace watchFace = WatchFace::Onion;
      static constexpr const char* name = "Onion";

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::OnionWatchFace(controllers.dateTimeController, controllers.settingsController);
      };

      static bool IsAvailable(Pinetime::Controllers::FS& /*filesystem*/) {
        return true;
      }
    };
  }
}
