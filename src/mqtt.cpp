#include <mqtt.hpp>

mqtt::mqtt()
{
    mqttClient.setKeepAlive(60);
    mqttClient.setCleanSession(true);

    mqttClient.onDisconnect([this](auto reason) {
        mqttConnected = false;
        Serial.println(String("Disconnected from MQTT broker. Reason: " + String(static_cast<uint8_t>(reason))));
    });

    mqttClient.onConnect([this](auto session_present) {
        mqttConnected = true;
        Serial.println("Connected to MQTT broker. Session present: " + String(session_present));
    });
}

void mqtt::set_credentials(const char *username, const char *password)
{
    mqttClient.setCredentials(username, password);
}

void mqtt::set_client_id(const char *client_id)
{
    mqttClient.setClientId(client_id);
}

void mqtt::set_server(const char *host, int port)
{
    mqttClient.setServer(host, port);
}

void mqtt::set_server(const IPAddress &host, int port)
{
    mqttClient.setServer(host, port);
}

bool mqtt::connect()
{
    if (!mqttConnected)
    {
        if (!mqttClient.connect())
        {
            mqttConnected = false;
        }
        else
        {
            mqttConnected = true;
        }
    }

    return mqttConnected;
}

void mqtt::disconnect()
{
    mqttClient.disconnect();
    mqttConnected = false;
}

void mqtt::publish(const char *topic, const char *payload, bool retain, int qos)
{
    mqttClient.publish(topic, qos, retain, payload);
}

void mqtt::subscribe(const char *topic)
{
    mqttClient.subscribe(topic, 0);
}

void mqtt::on_message(mqtt_callback callback)
{
    mqttClient.onMessage(callback);
}

void mqtt::loop()
{
    mqttClient.loop();
}

bool mqtt::connected() const
{
    return mqttConnected;
}
