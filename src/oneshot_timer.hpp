#pragma once
#include <Arduino.h>

namespace ardent
{
class oneshot_timer
{
  public:
    oneshot_timer(unsigned long interval) : interval(interval)
    {
        last_time = millis();
    }

    bool elapsed() const
    {
        const auto current_time = millis();
        if (current_time - last_time >= interval)
        {
            return true;
        }
        return false;
    }

  private:
    unsigned long interval;
    unsigned long last_time;
};
} // namespace ardent