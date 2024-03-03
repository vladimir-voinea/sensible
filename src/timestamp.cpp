#include <time.h>
#include <timestamp.hpp>

namespace ardent
{
String get_timestamp()
{
    time_t now;
    struct tm timeinfo_local, timeinfo_utc;
    char buf[32], tzOffset[16];

    // Obtain current time
    ::time(&now);

    // Get local time
    getLocalTime(&timeinfo_local);

    // Get UTC time
    gmtime_r(&now, &timeinfo_utc);

    // Format the date and time portion in ISO 8601 format
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &timeinfo_local);

    // Calculate timezone offset in seconds
    long offset = mktime(&timeinfo_local) - mktime(&timeinfo_utc);

    // Calculate offset in hours and minutes
    int offsetHours = offset / 3600;
    int offsetMinutes = abs((offset % 3600) / 60);

    // Format the timezone offset string
    if (offset == 0)
    {
        strcpy(tzOffset, "Z");
    }
    else
    {
        snprintf(tzOffset, sizeof(tzOffset), "%+03d:%02d", offsetHours, offsetMinutes);
    }

    // Construct the final timestamp string
    String timestamp = String(buf) + String(tzOffset);

    return timestamp;
}
} // namespace ardent