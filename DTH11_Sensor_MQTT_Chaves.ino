#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 4         // GPIO pin where DHT11 is connected
#define DHTTYPE DHT11    // DHT 11

const char* ssid = "Your WiFi SSID";       
const char* password = "Your WiFi password";        
const char* mqtt_server = "52.14.56.16";      
const int mqtt_port = 1883;                    // MQTT broker port
const char* mqtt_topic = "iot/sensor/data";    // MQTT topic to publish data

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void reconnect() {
  // Loop until we're reconnected to MQTT
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {  // Client ID
      Serial.println("connected");
      // Once connected, you can subscribe to topics, if needed
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Create JSON payload
  String jsonPayload = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";

  // Publish the message to the MQTT topic
  if (client.publish(mqtt_topic, jsonPayload.c_str())) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Error in sending message");
  }

  // Wait for 2 minutes before sending the next reading
  delay(150000);  // 2 minutes
}
