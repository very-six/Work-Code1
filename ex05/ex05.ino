#define TOUCH_PIN 4
#define LED_PIN 2

int threshold = 600;
int touchValue;
int level = 1;        // 1-慢, 2-中, 3-快
int stepDelay;

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);
}

void loop() {
  touchValue = touchRead(TOUCH_PIN);

  if (touchValue < threshold) {
    level = level % 3 + 1;
    Serial.print("Level: ");
    Serial.println(level);
    delay(300);  // 简单去抖
  }

  switch (level) {
    case 1: stepDelay = 15; break;
    case 2: stepDelay = 8;  break;
    case 3: stepDelay = 2;  break;
  }

  for (int i = 0; i <= 255; i++) {
    ledcWrite(LED_PIN, i);
    delay(stepDelay);
  }
  for (int i = 255; i >= 0; i--) {
    ledcWrite(LED_PIN, i);
    delay(stepDelay);
  }
}