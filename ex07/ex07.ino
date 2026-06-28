#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Very";
const char* password = "Mmsx627426946";

const int ledPin = 2;                // LED引脚（大部分ESP32开发板为GPIO2）
const int freq = 5000;               // PWM频率 5000Hz
const int resolution = 8;            // 分辨率 8位 (0-255)

WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 LED 亮度控制</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>ESP32 LED 亮度控制</h1>
  <p>拖动滑块调节 LED 亮度</p>
  <input type="range" id="brightness" min="0" max="255" value="0"
         style="width:300px; height:30px;">
  <p>当前亮度值: <span id="valueDisplay">0</span></p>
  <script>
    const slider = document.getElementById('brightness');
    const display = document.getElementById('valueDisplay');
    slider.oninput = function() {
      const val = this.value;
      display.textContent = val;
      fetch('/set?value=' + val);
    };
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html; charset=UTF-8", html);
}

void handleSet() {
  if (server.hasArg("value")) {
    int val = server.arg("value").toInt();
    val = constrain(val, 0, 255);
    ledcWrite(ledPin, val);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing value");
  }
}

void setup() {
  Serial.begin(115200);

  // 配置 PWM（新版ESP32 Arduino核心语法）
  ledcAttach(ledPin, freq, resolution);

  // 连接 WiFi
  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("HTTP服务器已启动");
}

void loop() {
  server.handleClient();
}
