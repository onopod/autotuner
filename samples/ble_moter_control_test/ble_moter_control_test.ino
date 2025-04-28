// ble_moter_control_test.ino

const int MotorPin1 = 18;  // DRV8833のAIN1に接続
const int MotorPin2 = 19;  // DRV8833のAIN2に接続
const int WakeUpPin = 0;   // タクトスイッチ（GPIO0）

void setup() {
  pinMode(MotorPin1, OUTPUT);
  pinMode(MotorPin2, OUTPUT);
  pinMode(WakeUpPin, INPUT_PULLUP); // DeepSleep解除用スイッチ

  Serial.begin(115200);
  delay(100); // 復帰後に少し待つ

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Wake up from Deep Sleep!");
    delay(100); // さらに待つ

    // モーター正回転
    digitalWrite(MotorPin1, HIGH);
    digitalWrite(MotorPin2, LOW);
    delay(3000);

    // モーター逆回転
    digitalWrite(MotorPin1, LOW);
    digitalWrite(MotorPin2, HIGH);
    delay(3000);

    // モーター停止
    digitalWrite(MotorPin1, LOW);
    digitalWrite(MotorPin2, LOW);
  } else {
    Serial.println("Power On or Reset!");
  }

  // 押しっぱなし防止：スイッチが離されるのを待つ
  while (digitalRead(WakeUpPin) == LOW) {
    delay(10); // 押している間は待機
  }

  // 次のDeepSleep設定
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0); // GPIO0がLOWになったら起きる設定
  Serial.println("Going to sleep now...");
  delay(100);
  esp_deep_sleep_start();
}

void loop() {
  // DeepSleep運用なのでここは使いません
}