#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
#include <time.h>
#include <HTTPClient.h>

const char *ssid = "Betussi";
const char *password = "P@ssword1";

WebServer server(80);
DHT dht(4, DHT11);

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600; // GMT-3 for Brasilia
const int daylightOffset_sec = 0; // No daylight saving time in Brasilia

const char* storageAccountName = "iotesp32";
const char* containerName = "iotesp32weather";
const char* sasToken = "sp=rcwl&st=2024-08-09T19:18:49Z&se=2026-07-31T03:18:49Z&sv=2022-11-02&sr=c&sig=%2BfXgdU8A2ZikqxFZeMU2gRob3lCGhif79ssXmF8FBgs%3D"; // Ensure this token has write permissions

void handleRoot() {
  char msg[2000];

  snprintf(msg, 2000,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='10'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>Home Office Weather Report</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
    <script>\
      function updateTime() {\
        var now = new Date();\
        document.getElementById('datetime').innerHTML = now.toLocaleString('en-US', { timeZone: 'America/Sao_Paulo' });\
      }\
      setInterval(updateTime, 1000);\
    </script>\
  </head>\
  <body onload='updateTime()'>\
      <h2>Home Office Weather Report</h2>\
      <p>Current Date and Time: <span id='datetime'></span></p>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity()
          );
  server.send(200, "text/html", msg);
}

void setup(void) {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  // Initialize NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");

  sendToAzureStorage(readDHTTemperature(), readDHTHumidity());
}

void loop(void) {
  server.handleClient();
  delay(2); // Allow the CPU to switch to other tasks
}

float readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
}

void sendToAzureStorage(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    //WiFiClient client;
    HTTPClient http;

    //String fileName = "HO_WeatherData_" + String(time(nullptr)) + ".json";
    //String url = String("https://") + storageAccountName + ".blob.core.windows.net/" + containerName + "/" + fileName + "?" + sasToken;
    String url = String("https://") + storageAccountName + ".blob.core.windows.net/" + containerName + "/weatherData.json" + "?" + sasToken;
    Serial.print("URL: ");
    //http.begin(url);
    http.begin(client, url);

    http.addHeader("Content-Type", "application/json");
    http.addHeader("x-ms-blob-type", "BlockBlob");  // Specify the blob type

    String payload = "{ \"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidity, 2) + " }";
    //String payload = "{ \"timestamp\": \"" + String(time(nullptr)) + "\", \"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidity, 2) + " }";
    Serial.print("Payload: ");
    Serial.println(payload);



    int httpResponseCode = http.PUT(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else {
      Serial.print("Error on sending PUT request: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}


