#include "nslookup.hpp"
#include <WiFi.h>

namespace ardent
{
IPAddress nslookup(const char *hostname)
{
    IPAddress ip;
    WiFi.hostByName(hostname, ip);
    return ip;
}
} // namespace ardent