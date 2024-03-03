#include <tz.hpp>

#include <Arduino.h>

namespace ardent
{
timezone CURRENT_TIMEZONE = {0, 0};

void setupNTP(const char *ntpServer, const timezone &tz)
{
    ::configTime(tz.offset_sec, tz.daylight_offset_sec, ntpServer);
    CURRENT_TIMEZONE = tz;
}

timezone getTimezone()
{
    return CURRENT_TIMEZONE;
}
} // namespace ardent