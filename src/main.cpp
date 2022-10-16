#include <M5StickC.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("M5 Shutter");

unsigned int textPosX = 25;
unsigned int textPosY = (M5.Lcd.height() - 48) / 2;
unsigned int LCDBatMargin = 10;
unsigned int LCDBatPadding = 5;

void show_bat_status(){
  const float max_bat_v = 4.2f;
  const float min_bat_v = 3.0f;
  float bat_v, bat_p;
  unsigned int color;
  bat_v = M5.Axp.GetBatVoltage();
  bat_p = roundf(
    (M5.Axp.GetBatVoltage() - min_bat_v) /
    (max_bat_v - min_bat_v) * 100.0f
  );
  M5.Lcd.drawRect(
    LCDBatMargin,
    LCDBatMargin,
    M5.Lcd.width() - 2 * LCDBatMargin,
    M5.Lcd.height() - 2 * LCDBatMargin,
    WHITE
  );
  if (bat_p > 50) {
    color = GREEN;
  } else if (bat_p > 20) {
    color = YELLOW;
  } else {
    color = RED;
  }
  M5.Lcd.fillRect(
    LCDBatMargin + LCDBatPadding,
    (M5.Lcd.height() - (LCDBatMargin + LCDBatPadding)) * (100 - bat_p) / 100.0f,
    M5.Lcd.width() - 2 * (LCDBatMargin + LCDBatPadding),
    (M5.Lcd.height() - (LCDBatMargin + LCDBatPadding)) * bat_p / 100.0f,
    color
  );
}

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

  M5.Lcd.fillScreen(BLACK);
  while (not bleKeyboard.isConnected()) {
    show_bat_status();
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