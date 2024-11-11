#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int pos = 0;
int y = 1;
const int buttonPin = 2;
const int buzzerPin = 4;
bool gameOver = false;
int score = 0; // Variabel untuk menyimpan angka skor

byte kiarjuno[8] = {
  B00100,
  B01010,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B10001
};

byte pocong[8] = {
  B01110,
  B01110,
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
  B00100
};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, kiarjuno);
  lcd.createChar(1, pocong);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (gameOver) {
    return; // Menghentikan loop jika game over
  }

  // Bersihkan layar dan tampilkan kaktus
  lcd.clear();
  for(int i = 0; i < 4; i++) {
    lcd.setCursor(3 + (i * 3), 1);
    lcd.write((byte)1);
  }

  // Cek apakah tombol ditekan untuk membuat ki arjuno melompat
  if (digitalRead(buttonPin) == LOW) {
    y = 0;  // Dino melompat
    tone(buzzerPin, 2000); // Aktifkan buzzer dengan frekuensi 1500 Hz
  } else {
    y = 1;  // ki arjuno kembali ke bawah
    noTone(buzzerPin);     // Matikan suara buzzer
  }

  // Cek apakah ki arjuno menabrak pocong
  if (y == 1 && (pos == 3 || pos == 6 || pos == 9 || pos == 12)) {
    // Jika ki arjuno berada di posisi pocong, Game Over
    tone(buzzerPin, 500, 300); // Nada rendah untuk Game Over
    delay(300);
    tone(buzzerPin, 400, 300);
    delay(300);
    tone(buzzerPin, 300, 500);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("ilmunya kurang");
    lcd.setCursor(3, 1);
    lcd.print("sakti mas!");
    gameOver = true; // Set game over flag
    score = 0; // Reset skor saat Game Over
    return;
  }

  // Tampilkan ki arjuno di posisi tertentu
  lcd.setCursor(pos, y);
  lcd.write((byte)0);

  // Gerakkan ki arjuno ke kanan
  pos++;
  if (pos > 15) {
    pos = 0;
    // ki arjuno mencapai ujung kanan, beri suara "point" dan tambahkan skor
    tone(buzzerPin, 2000, 100); // Nada tinggi untuk "point"
    delay(100);
    noTone(buzzerPin);
    score++; // Tambahkan skor
  }

  // Tampilkan angka skor di pojok kanan atas layar
  lcd.setCursor(14, 0);
  lcd.print(score);

  // Delay untuk mengontrol kecepatan animasi
  delay(250);
}
