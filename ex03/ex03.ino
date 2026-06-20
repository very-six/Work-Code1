// ESP32板载LED引脚
const int ledPin = 2;

// 时序常量（和题目示例完全一致）
const unsigned long DOT_ON = 200;    // 短亮时长
const unsigned long DASH_ON = 600;   // 长亮时长
const unsigned long DOT_GAP = 200;   // 码元内部熄灭间隔
const unsigned long CHAR_GAP = 500;  // 字母之间熄灭间隔
const unsigned long SOS_GAP = 2000;  // 整套SOS结束后长熄灭停顿

// 状态枚举：区分【点亮码元】和【纯熄灭等待】两类状态
enum State {
  // S：3个短点亮
  S_DOT1_ON, S_DOT1_GAP,
  S_DOT2_ON, S_DOT2_GAP,
  S_DOT3_ON, S_DOT3_GAP,
  S_TO_O_GAP,    // S与O之间字母间隔（纯灭）

  // O：3个长点亮
  O_DASH1_ON, O_DASH1_GAP,
  O_DASH2_ON, O_DASH2_GAP,
  O_DASH3_ON, O_DASH3_GAP,
  O_TO_S_GAP,    // O与末尾S之间字母间隔（纯灭）

  // 末尾S：3个短点亮
  S_DOT4_ON, S_DOT4_GAP,
  S_DOT5_ON, S_DOT5_GAP,
  S_DOT6_ON, S_DOT6_GAP,

  SOS_FULL_PAUSE // 整套SOS结束长停顿（纯灭）
};

// 全局计时变量
unsigned long lastTick = 0;
unsigned long stateDuration = DOT_ON;
State currentState = S_DOT1_ON;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("SOS Morse Signal Start (millis非阻塞版)");
}

void loop() {
  unsigned long now = millis();
  if (now - lastTick >= stateDuration) {
    lastTick = now;
    switchState();
  }
}

void switchState() {
  switch (currentState) {
    // ========== 第一段 S 3短闪 ==========
    case S_DOT1_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = S_DOT1_GAP;
      break;
    case S_DOT1_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = DOT_ON;
      currentState = S_DOT2_ON;
      break;

    case S_DOT2_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = S_DOT2_GAP;
      break;
    case S_DOT2_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = DOT_ON;
      currentState = S_DOT3_ON;
      break;

    case S_DOT3_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = S_DOT3_GAP;
      break;
    case S_DOT3_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = CHAR_GAP;
      currentState = S_TO_O_GAP;
      break;

    case S_TO_O_GAP: // S与O间隔，全程熄灭500ms
      digitalWrite(ledPin, LOW);
      stateDuration = DASH_ON;
      currentState = O_DASH1_ON;
      break;

    // ========== 第二段 O 3长闪 ==========
    case O_DASH1_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = O_DASH1_GAP;
      break;
    case O_DASH1_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = DASH_ON;
      currentState = O_DASH2_ON;
      break;

    case O_DASH2_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = O_DASH2_GAP;
      break;
    case O_DASH2_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = DASH_ON;
      currentState = O_DASH3_ON;
      break;

    case O_DASH3_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = O_DASH3_GAP;
      break;
    case O_DASH3_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = CHAR_GAP;
      currentState = O_TO_S_GAP;
      break;

    case O_TO_S_GAP: // O与末尾S间隔，全程熄灭500ms
      digitalWrite(ledPin, LOW);
      stateDuration = DOT_ON;
      currentState = S_DOT4_ON;
      break;

    // ========== 第三段 S 3短闪 ==========
    case S_DOT4_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = S_DOT4_GAP;
      break;
    case S_DOT4_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = DOT_ON;
      currentState = S_DOT5_ON;
      break;

    case S_DOT5_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = S_DOT5_GAP;
      break;
    case S_DOT5_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = DOT_ON;
      currentState = S_DOT6_ON;
      break;

    case S_DOT6_ON:
      digitalWrite(ledPin, HIGH);
      stateDuration = DOT_GAP;
      currentState = S_DOT6_GAP;
      break;
    case S_DOT6_GAP:
      digitalWrite(ledPin, LOW);
      stateDuration = SOS_GAP;
      currentState = SOS_FULL_PAUSE;
      break;

    // ========== 整套SOS结束长停顿 ==========
    case SOS_FULL_PAUSE:
      digitalWrite(ledPin, LOW);
      stateDuration = DOT_ON;
      currentState = S_DOT1_ON;
      Serial.println("完成一轮完整SOS信号");
      break;
  }
}