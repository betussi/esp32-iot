#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include "DHT.h"

const char* ssid = "Betussi 2.4Ghz";       
const char* password = "Rfb@Sup23cpd";        
const char* mqtt_server = "18.222.156.182";      
const int mqtt_port = 1883;                    // MQTT broker port
const char* mqtt_topic = "iot/sensor/data";    // MQTT topic to publish data
const char* mqtt_topic_led = "led/sensor/control"; 

// DHT sensor setup
#define DHTPIN 4         
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

// LED setup
#define LED_PIN 2        

// LCD setup (I2C Address: 0x27, 16x2 display)
LiquidCrystal_PCF8574 lcd(0x27);

WiFiClient espClient;
PubSubClient client(espClient);

// Callback function for MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Control LED based on message
  if (message == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  } else if (message == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  }
}

// Connect to WiFi
void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

// Reconnect to MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT");
      client.subscribe(mqtt_topic_led); 
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  lcd.begin(16, 2);  // Set up for 16x2 LCD
  lcd.setBacklight(255);  // Turn on the backlight

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error    ");
    return;
  }

  // Display values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(humidity);
  lcd.print(" %");

  // Publish data to MQTT
  String jsonPayload = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
  client.publish(mqtt_topic, jsonPayload.c_str());
  Serial.println("Data published: " + jsonPayload);

  // Wait 2 minutes before next reading
  delay(120000);
}
