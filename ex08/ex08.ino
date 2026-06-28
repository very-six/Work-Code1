#include <WiFi.h>
#include <WebServer.h>

// WiFi 配置
const char* ssid = "Very";
const char* password = "Mmsx627426946";

// 引脚定义
#define TOUCH_PIN 4      // 触摸引脚 (T0 -> GPIO4)
#define LED_PIN   2      // 板载LED (GPIO2)

// 全局状态定义
#define STATE_DISARMED 0   // 撤防
#define STATE_ARMED    1   // 布防
#define STATE_ALARM    2   // 报警中

int systemState = STATE_DISARMED;  // 初始为撤防状态

WebServer server(80);

// ---------- 网页生成 ----------
String makePage() {
  String stateLabel;
  if (systemState == STATE_DISARMED) stateLabel = "撤防";
  else if (systemState == STATE_ARMED)   stateLabel = "布防";
  else                                   stateLabel = "报警中";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 安防主机</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    button { padding: 15px 30px; font-size: 18px; margin: 10px; }
    .status { font-size: 24px; margin: 20px 0; }
    .arm   { background-color: #4CAF50; color: white; border: none; }
    .disarm{ background-color: #f44336; color: white; border: none; }
  </style>
</head>
<body>
  <h1>ESP32 安防主机</h1>
  <div class="status">当前状态：<b>)rawliteral" + stateLabel + R"rawliteral(</b></div>
  <a href="/arm"><button class="arm">布防 (Arm)</button></a>
  <a href="/disarm"><button class="disarm">撤防 (Disarm)</button></a>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleArm() {
  systemState = STATE_ARMED;
  digitalWrite(LED_PIN, LOW);    // 确保LED熄灭
  Serial.println(">> 系统已布防");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDisarm() {
  systemState = STATE_DISARMED;
  digitalWrite(LED_PIN, LOW);    // 熄灭LED
  Serial.println(">> 系统已撤防");
  server.sendHeader("Location", "/");
  server.send(303);
}

// ---------- LED闪烁控制 ----------
unsigned long lastBlink = 0;
bool ledState = LOW;

void updateLED() {
  if (systemState == STATE_DISARMED) {
    // 撤防状态：LED 熄灭
    digitalWrite(LED_PIN, LOW);
  } 
  else if (systemState == STATE_ARMED) {
    // 布防状态：LED 慢闪（表示警戒中）
    if (millis() - lastBlink > 500) {
      lastBlink = millis();
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  } 
  else if (systemState == STATE_ALARM) {
    // 报警状态：LED 高频闪烁
    if (millis() - lastBlink > 100) {
      lastBlink = millis();
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}

// ---------- 触摸检测 ----------
void checkTouch() {
  static unsigned long lastTouchCheck = 0;
  if (millis() - lastTouchCheck < 100) return;  // 每100ms检测一次
  lastTouchCheck = millis();

  int touchValue = touchRead(TOUCH_PIN);

  // 仅在布防状态下，触摸触发报警
  if (systemState == STATE_ARMED && touchValue < 20) {
    systemState = STATE_ALARM;
    Serial.println("!!! 触发报警 !!!");
  }
}

// ---------- setup / loop ----------
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("HTTP 服务器已启动");
}

void loop() {
  server.handleClient();   // 处理网页请求
  checkTouch();            // 检测触摸
  updateLED();             // 更新LED状态
}
