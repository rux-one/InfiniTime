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

      class CccWatchFace : public Screen {
      public:
        CccWatchFace(Controllers::DateTime& dateTimeController, Controllers::Settings& settingsController);
        ~CccWatchFace() override;

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
    struct WatchFaceTraits<WatchFace::Ccc> {
      static constexpr WatchFace watchFace = WatchFace::Ccc;
      static constexpr const char* name = "39c3";

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::CccWatchFace(controllers.dateTimeController, controllers.settingsController);
      };

      static bool IsAvailable(Pinetime::Controllers::FS& /*filesystem*/) {
        return true;
      }
    };
  }
}
