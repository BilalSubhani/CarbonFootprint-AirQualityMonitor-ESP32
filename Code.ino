#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

// Network credentials
const char* ssid = "WILCOX";
const char* password = "245250309";

// esp32 -> C++
// webserver -> [JS, html, CSS] (JSX)

// Pins for the gas sensors and DHT11
#define DHTPIN 14
#define DHTTYPE DHT11
#define MQ4_PIN 34  // Methane (CH4)
#define MQ7_PIN 35  // Carbon Monoxide (CO)
#define MQ9_PIN 32  // Liquefied Petroleum Gas (LPG)
#define MQ135_PIN 33 // Carbon Dioxide (CO2)

// Updated harmful gas levels (in ppm) based on Lahore air quality
#define MQ4_THRESHOLD 1.5    // Methane (CH4)
#define MQ7_THRESHOLD 50      // Carbon Monoxide (CO)
#define MQ9_THRESHOLD 2000    // LPG
#define MQ135_THRESHOLD 1500   // Carbon Dioxide (CO2)

DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);
String gasAlerts[4]; // Array to hold individual gas alerts

// Function to read sensor values with averaging
int readAnalogSensor(int pin, int samples = 10) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delay(50); // Small delay between samples
  }
  return sum / samples; // Return average
}
// delay(50) 1s = 1000 ms
void setup() {
  Serial.begin(115200);

  // Initialize DHT11
  dht.begin();

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print IP Address once connected to Wi-Fi
  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Start web server JSX (Javascript + Html)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Carbon Footprint and Air Quality Monitor</title>";
    html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 0; padding: 0; background-color: #f0f8ff; color: #333; }";
    html += ".container { width: 90%; max-width: 800px; margin: 0 auto; padding: 20px; }";
    html += "header { text-align: center; background: #007BFF; color: white; padding: 20px; border-radius: 10px; }";
    html += "h1 { margin: 0; font-size: 2em; }";
    html += ".alert { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; padding: 15px; border-radius: 10px; margin-top: 15px; font-size: 1.2em; }";
    html += "p { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); margin: 15px 0; font-size: 1.2em; }";
    html += "footer { margin-top: 20px; text-align: center; font-size: 14px; color: #888; }";
    html += ".sensor-info { display: flex; flex-wrap: wrap; justify-content: space-between; align-items: center; margin-bottom: 20px; }";
    html += ".sensor-info div { flex: 1 1 45%; padding: 20px; margin: 5px; border-radius: 10px; background: #ffffff; box-shadow: 0 4px 8px rgba(0,0,0,0.1); transition: background 0.3s; }";
    html += ".sensor-info div:hover { background-color: #e0f7ff; }";  // Hover effect
    html += ".sensor-info div.warning { background-color: #ffeb3b; }";  // Warning color
    html += ".sensor-info div.danger { background-color: #f44336; color: white; }";  // Danger color
    html += ".sensor-info div h2 { font-size: 1.5em; margin: 0; }";
    html += ".sensor-info div p { font-size: 1.2em; margin: 5px 0 0; }";
    html += "@media (max-width: 600px) { .sensor-info { flex-direction: column; } .sensor-info div { flex: 1 1 100%; margin: 10px 0; } }"; // Responsive styles for small screens
    html += "@media (min-width: 600px) and (max-width: 900px) { .sensor-info div { flex: 1 1 48%; } }"; // Adjust for medium screens
    html += "@media (min-width: 900px) { .sensor-info div { flex: 1 1 30%; } }"; // Adjust for large screens
    html += "</style></head><body>";

    html += "<header><h1>Carbon Footprint and Air Quality Monitor</h1></header>";
    html += "<div class='container'>";
    html += "<div id='alert' class='alert' style='display: none;'>No harmful gas detected.</div>";

    // Display sensor data sections
    html += "<div class='sensor-info'>";
    html += "<div><h2>Temperature</h2><p id='temperature'>Loading...</p></div>";
    html += "<div><h2>Humidity</h2><p id='humidity'>Loading...</p></div>";
    html += "</div>";

    html += "<div class='sensor-info'>";
    html += "<div><h2>Methane (MQ4)</h2><p id='mq4'>Loading...</p></div>";
    html += "<div><h2>Carbon Monoxide (MQ7)</h2><p id='mq7'>Loading...</p></div>";
    html += "</div>";

    html += "<div class='sensor-info'>";
    html += "<div><h2>LPG (MQ9)</h2><p id='mq9'>Loading...</p></div>";
    html += "<div><h2>Carbon Dioxide (MQ135)</h2><p id='mq135'>Loading...</p></div>";
    html += "</div>";

    // Add footer
    html += "<footer>Designed by Haider Ali</footer>";
    html += "</div>";

    // JavaScript to fetch data periodically
    html += "<script>function fetchData() { fetch('/data').then(response => response.json()).then(data => {";
    html += "document.getElementById('temperature').innerText = data.temperature !== null ? data.temperature + ' °C' : 'Sensor Error';";
    html += "document.getElementById('humidity').innerText = data.humidity !== null ? data.humidity + ' %' : 'Sensor Error';";
    html += "document.getElementById('mq4').innerText = data.mq4 !== null ? data.mq4 + ' ppm' : 'Sensor Error';";
    html += "document.getElementById('mq7').innerText = data.mq7 !== null ? data.mq7 + ' ppm' : 'Sensor Error';";
    html += "document.getElementById('mq9').innerText = data.mq9 !== null ? data.mq9 + ' ppm' : 'Sensor Error';";
    html += "document.getElementById('mq135').innerText = data.mq135 !== null ? data.mq135 + ' ppm' : 'Sensor Error';";
    html += "let alerts = ''; data.alerts.forEach(alert => { if(alert) alerts += alert + '<br>'; });";
    html += "const alertBox = document.getElementById('alert');";
    html += "if (alerts) { alertBox.innerHTML = alerts; alertBox.style.display = 'block'; } else { alertBox.innerHTML = 'No harmful gas detected.'; alertBox.style.display = 'none'; }";
    html += "});} setInterval(fetchData, 2000);</script></body></html>";

    request->send(200, "text/html", html);
  });

  // Endpoint to serve sensor data
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"temperature\":" + String(dht.readTemperature()) + ",";
    json += "\"humidity\":" + String(dht.readHumidity()) + ",";
    json += "\"mq4\":" + String(readAnalogSensor(MQ4_PIN) / 1000) + ",";
    json += "\"mq7\":" + String(readAnalogSensor(MQ7_PIN)/ 50) + ",";
    json += "\"mq9\":" + String(readAnalogSensor(MQ9_PIN)) + ",";
    json += "\"mq135\":" + String(readAnalogSensor(MQ135_PIN)) + ",";

    // Check gas levels and set alerts
    if (readAnalogSensor(MQ4_PIN)  / 1000 > MQ4_THRESHOLD) {
      gasAlerts[0] = "Methane (MQ4) is at harmful levels!";
    } else {
      gasAlerts[0] = "";
    }
    if ((readAnalogSensor(MQ7_PIN) / 50 )> MQ7_THRESHOLD) {
      gasAlerts[1] = "Carbon Monoxide (MQ7) is at harmful levels!";
    } else {
      gasAlerts[1] = "";
    }
    if (readAnalogSensor(MQ9_PIN) > MQ9_THRESHOLD) {
      gasAlerts[2] = "LPG (MQ9) is at harmful levels!";
    } else {
      gasAlerts[2] = "";
    }
    if (readAnalogSensor(MQ135_PIN) > MQ135_THRESHOLD) {
      gasAlerts[3] = "Carbon Dioxide (MQ135) is at harmful levels!";
    } else {
      gasAlerts[3] = "";
    }

    json += "\"alerts\":[";
    for (int i = 0; i < 4; i++) {
      json += "\"" + gasAlerts[i] + "\"";
      if (i < 3) json += ",";
    }
    json += "],";
    
    json += "\"status\":\"ok\""; // Add status key to JSON
    json += "}";

    request->send(200, "application/json", json);
  });

  // Start server
  server.begin();
}

void loop() {
}

// ESP32 -> 5V
// VCC -> 3.3V
// GND -> Ground
// Ao -> Analog values
// Do -> 0, 1 Digital values