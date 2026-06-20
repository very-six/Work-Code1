// ESP32板载LED引脚
const int ledPin = 2;
// 存储上一次状态切换的时间
unsigned long lastTime = 0;
// 闪烁间隔：500ms切换一次亮灭，完整周期1000ms = 1Hz
const unsigned long interval = 500;
// LED状态标记
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

void loop() {
  // 获取当前系统毫秒时间
  unsigned long currentTime = millis();

  // 判断是否到达切换时间
  if (currentTime - lastTime >= interval) {
    lastTime = currentTime;
    // 翻转LED状态
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    // 串口打印状态
    if (ledState) {
      Serial.println("LED ON");
    } else {
      Serial.println("LED OFF");
    }
  }
  // 此处可添加其他业务代码，不会被计时阻塞
}