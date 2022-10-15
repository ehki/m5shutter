#include <M5StickC.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("M5 Shutter");

unsigned int textPosX = 25;
unsigned int textPosY = (M5.Lcd.height() - 48) / 2;

void setup() {
  M5.begin();
  M5.Axp.begin(false, false, false, false, true);
  M5.Axp.ScreenBreath(9);
  setCpuFrequencyMhz(80);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextFont(7);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setRotation(0);
  pinMode(GPIO_NUM_10, OUTPUT);
  digitalWrite(GPIO_NUM_10, 1);
  bleKeyboard.begin();
  M5.update();

  while (not bleKeyboard.isConnected()) {
    M5.Lcd.fillScreen(WHITE);
    delay(100);
  }

  // If Bluetooth is connected, check BtnA.
  M5.Lcd.fillScreen(BLUE);
  int counter = 9;
  while (counter >= 0) {
    M5.Lcd.setCursor(textPosX, textPosY);
    M5.Lcd.fillScreen(BLUE);
    M5.Lcd.startWrite();
    M5.Lcd.printf("%1d", counter);
    M5.Lcd.endWrite();
    counter--;
    delay(1000);
  }
  // Start record
  digitalWrite(GPIO_NUM_10, 0);
  bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  counter = 4;
  while (counter >= 0) {
    M5.Lcd.setCursor(textPosX, textPosY);
    M5.Lcd.fillScreen(RED);
    M5.Lcd.startWrite();
    M5.Lcd.printf("%1d", counter);
    M5.Lcd.endWrite();
    counter--;
    delay(1000);
  }
  M5.Lcd.fillScreen(BLUE);
  bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  digitalWrite(GPIO_NUM_10, 1);
  M5.Lcd.startWrite();
  M5.Lcd.setCursor(textPosX, textPosY);
  M5.Lcd.printf("%1d", 0);
  M5.Lcd.endWrite();
  delay(1000);
  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, LOW);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, LOW);
  M5.Axp.DeepSleep();
}

void loop() {
  delay(100);
}