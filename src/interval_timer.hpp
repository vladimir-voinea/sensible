#pragma once
#include <Arduino.h>

namespace ardent
{
class interval_timer
{
  public:
    interval_timer(unsigned long interval) : interval(interval)
    {
        last_time = millis();
    }

    bool elapsed() const
    {
        const auto current_time = millis();
        if (current_time - last_time >= interval)
        {
            last_time = current_time;
            return true;
        }
        return false;
    }

    bool operator()() const
    {
        return elapsed();
    }

  private:
    unsigned long interval;
    mutable unsigned long last_time;
};
} // namespace ardent