#pragma once
#include <IPAddress.h>
#include <espMqttClient.h>

class mqtt
{
  public:
    using mqtt_callback = void (*)(const espMqttClientTypes::MessageProperties &properties, const char *topic,
                                   const uint8_t *payload, size_t len, size_t index, size_t total);

    mqtt();
    void set_credentials(const char *username, const char *password);
    void set_client_id(const char *client_id);
    void set_server(const char *host, int port);
    void set_server(const IPAddress &host, int port);
    bool connect();
    void disconnect();
    void publish(const char *topic, const char *payload, bool retain = false, int qos = 0);
    void subscribe(const char *topic);
    void on_message(mqtt_callback callback);
    void loop();
    bool connected() const;

  private:
    espMqttClient mqttClient;
    bool mqttConnected = false;
};