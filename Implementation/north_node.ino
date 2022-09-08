#include "PubSubClient.h" // connect and publish to the mqtt broker
#include "ESP8266WiFi.h"  // enables esp to connect to the local wifi
#define PIR_1 D1
//#define PIR_2 D2
int pir_value;
// WiFi
const char *ssid = "RAMMZZZ 2743";
const char *wifi_password = "123456789";
// MQTT
const char *mqtt_server = "192.168.137.243";
const char *alert_topic = "animal/alert_north";
const char *mqtt_username = "testUser";
const char *mqtt_password = "test";
const char *clientID = "client_farm";
// Initialize the Wifi and MQTT client objects
WiFiClient wifiClient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient);
// custom function to connect to the mqtt broker via wifi
void connect_MQTT()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // connect to the WiFi
    WiFi.begin(ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Debugging - Outputting the ip for testing
    Serial.println("WiFI connection established");
    Serial.println("IP address : ");
    Serial.println(WiFi.localIP());
    // connect to MQTT Broker
}
void setup()
{
    // put your setup code here, to run once:
    pinMode(PIR_1, INPUT);
    // pinMode(PIR_2, INPUT);
    Serial.begin(115200);
    connect_MQTT();
}
void loop()
{
    Serial.setTimeout(2000);
    pir_value = digitalRead(PIR_1);
    Serial.print("Pir value:");
    Serial.println(pir_value);
    // Serial.println("Testing");
    // MQTT can only transmit strings
    String alert_data = String((int)pir_value);
    // publish to mqtt broker
    if (pir_value == HIGH)
    {
        if (client.connect(clientID, mqtt_username, mqtt_password))
        {
            Serial.println("Connected to MQTT Broker!");
        }
        else
        {
            Serial.println("Connection to MQTT Broker failed..");
        }
        if (client.publish(alert_topic, String(pir_value).c_str()))
        {
            Serial.print("Alert sent to the RPi : ");
            Serial.println(String(pir_value).c_str());
        }
        else
        {
            // Failed to tranmit data. Reconnecting to MQTT broker and trying again.
            Serial.println("Reconnecting to MQTT broker");
            client.connect(clientID, mqtt_username, mqtt_password);
            delay(10); // this delay ensures that client.publish doesn't clash with client.connect call

            client.publish(alert_topic, String(pir_value).c_str());
        }
    }
    client.disconnect();
    delay(1000 * 3); // execute this loop every 2 seconds.
}