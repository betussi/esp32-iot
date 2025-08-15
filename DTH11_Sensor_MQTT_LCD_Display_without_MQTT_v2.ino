#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include "DHT.h"

// Configurações do Wi-Fi
const char* ssid = "";
const char* password = "";

// DHT sensor
#define DHTPIN 4         
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

// LCD (I2C Address: 0x27, 16x2 display)
LiquidCrystal_PCF8574 lcd(0x27);

// Conexão Wi-Fi
void setup_wifi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  lcd.begin(16, 2);      // LCD 16x2
  lcd.setBacklight(255); // Liga backlight

  setup_wifi();
}

void loop() {
  // Lê temperatura e umidade
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verifica leitura
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler o sensor DHT!");
    lcd.setCursor(0, 0);
    lcd.print("Erro no sensor ");
    delay(2000);
    return;
  }

  // Mostra no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(humidity);
  lcd.print(" %");

  // Mostra no Serial
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C | Umid: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000); // Atualiza a cada 2 segundos
}

