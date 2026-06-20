// 定义LED引脚，ESP32板载LED默认GPIO2
const int ledPin = 2;

void setup() {
  Serial.begin(115200);    // 初始化串口，波特率115200
  pinMode(ledPin, OUTPUT); // LED引脚设为输出模式
  Serial.println("SOS求救信号灯已启动");
}

void loop() {
  // ========== 发送字母S：短闪3次 ==========
  Serial.print("发送S:");
  for(int i = 0; i < 3; i++){
    digitalWrite(ledPin, HIGH);
    delay(200);   // 短亮200ms
    digitalWrite(ledPin, LOW);
    delay(200);    // 熄灭间隔200ms
    Serial.print("·");
  }
  Serial.println();
  delay(500); // 字母S与O之间间隔500ms

  // ========== 发送字母O：长闪3次 ==========
  Serial.print("发送O:");
  for(int i = 0; i < 3; i++){
    digitalWrite(ledPin, HIGH);
    delay(600);   // 长亮600ms
    digitalWrite(ledPin, LOW);
    delay(200);    // 熄灭间隔200ms
    Serial.print("-");
  }
  Serial.println();
  delay(500); // 字母O与S之间间隔500ms

  // ========== 再次发送字母S：短闪3次 ==========
  Serial.print("发送S:");
  for(int i = 0; i < 3; i++){
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
    Serial.print("·");
  }
  Serial.println("\n------------------------");
  delay(2000); // 一整组SOS完成后，停顿2秒再循环
}