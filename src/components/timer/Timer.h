#pragma once

#include <FreeRTOS.h>
#include <timers.h>

#include <chrono>
#include <optional>

namespace Pinetime {
  namespace Controllers {
    class Timer {
    public:
      Timer(void* timerData, TimerCallbackFunction_t timerCallbackFunction, const char* timerId);

      void StartTimer(std::chrono::milliseconds duration);

      void StopTimer();

      std::optional<TimerStatus> GetTimerState();

      bool IsRunning();

      void ResetExpiredTime();

    private:
      TimerHandle_t timer;
      TickType_t expiry;
      bool triggered = false;
    };
  }
}
