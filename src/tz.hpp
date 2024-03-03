#pragma once

namespace ardent
{
struct timezone
{
    int offset_sec;
    int daylight_offset_sec;
};
const timezone ROMANIA_WINTER = {2 * 3600, 0};

void setupNTP(const char *ntpServer = "pool.ntp.org", const timezone &tz = ROMANIA_WINTER);
timezone getTimezone();
} // namespace ardent
