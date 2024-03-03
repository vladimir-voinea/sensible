#include <Adafruit_AHTX0.h>
#include <ArduinoJson.h>
#include <IPAddress.h>
#include <WiFiManager.h>
#include <espMqttClient.h>
#include <interval_timer.hpp>
#include <mqtt.hpp>
#include <nslookup.hpp>
#include <timestamp.hpp>
#include <tz.hpp>

Adafruit_AHTX0 temp_humidity_sensor;
WiFiManager wifi_manager;
mqtt mqtt_client;

void ensureWifiIsConnected()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected, connecting to wi-fi first.");

        wifi_manager.disconnect();
        WiFi.mode(WIFI_STA);
        if (!wifi_manager.autoConnect())
        {
            Serial.println("Failed to connect to WiFi network");
        }
    }
}

void ensureMqttIsConnected()
{
    int attempts = 0;
    while (!mqtt_client.connected())
    {
        attempts++;
        const auto success = mqtt_client.connect();

        if (!success && attempts > 100)
        {
            Serial.println("Failed to connect to MQTT. Restarting...");
            ESP.restart();
        }

        if (success)
        {
            Serial.println(String("Connected to MQTT after ") + attempts + " attempts.");
        }
        else
        {
            delay(10);
        }
    }
}

void setupMqtt()
{
    mqtt_client.set_server(ardent::nslookup("raspberrypi.local"), 1883);
}

void publish_sensor_data()
{
    sensors_event_t temperature, humidity;
    const auto event = temp_humidity_sensor.getEvent(&humidity, &temperature);
    const auto timestamp = ardent::get_timestamp();

    {
        JsonDocument temp_doc;
        temp_doc["source"] = "sensor/AHT21";
        temp_doc["timestamp"] = timestamp;
        temp_doc["value"] = temperature.temperature;
        temp_doc["unit"] = "C";

        String buffer;
        serializeJson(temp_doc, buffer);
        mqtt_client.publish("/sensible/1/temperature", buffer.c_str(), true);

        Serial.println(buffer);
    }

    {
        JsonDocument hum_doc;
        hum_doc["source"] = "sensor/AHT21";
        hum_doc["timestamp"] = timestamp;
        hum_doc["value"] = humidity.relative_humidity;
        hum_doc["unit"] = "%";

        String buffer;
        serializeJson(hum_doc, buffer);
        mqtt_client.publish("/sensible/1/humidity", buffer.c_str(), true);

        Serial.println(buffer);
    }
}

void publish_and_sleep()
{
    publish_sensor_data();

    mqtt_client.disconnect();
    wifi_manager.disconnect();

    Serial.println("Going to sleep");
    Serial.println("-----------------------------");
    Serial.flush();

    ESP.deepSleep(10e6);
}

void check_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
}

void setup()
{
    Serial.begin(9600);
    check_wakeup_reason();

    temp_humidity_sensor.begin();

    wifi_manager.disconnect();
    ensureWifiIsConnected();
    ardent::setupNTP();

    setupMqtt();
    ensureMqttIsConnected();

    publish_and_sleep();
}

void loop()
{
}