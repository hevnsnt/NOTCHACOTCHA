//************************************************************
// NOTCHA-COTCHA CODE
//************************************************************

#include <Wire.h>
#include <LOLIN_I2C_BUTTON.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int displayLength, minX;
#define BUFFDSIZE 200
char message[BUFFDSIZE];

int pulse = 1; // pulse size length in microseconds
int choice = 0;  // select laser gun model type
int lastchoice = 0; // recall last selection choice
int fire; // trigger fire button
int LIDAR_EMITTER = 13;
int TRIGGER_BUTTON = 11;
int CHANGE_MODE_BUTTON = 12;
int CHANGE_MODE_BUTTON_PRESSED = 0;
int counter = 0;
int switchPin = 13;
int LEDARRAY = 0;
int LEDB = 2;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  pinMode(LIDAR_EMITTER, OUTPUT);  // 940nM LED output (mimic 904nM laser diode)
  pinMode(CHANGE_MODE_BUTTON, INPUT);
  pinMode(TRIGGER_BUTTON, INPUT);
  Serial.println("start");

  setLaserName("Power On");
  displayLength = display.width();
  minX = -12 * strlen(message);
  displayData();
}

void hipulselo() {
  digitalWrite(LIDAR_EMITTER, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(LIDAR_EMITTER, LOW);
}

void emit(int fire, int delay, int n=3, bool quirk=false) {
  if (fire == 0) {
    for (int a = 1; a <= n; a++) {
      hipulselo()
      delayMicroseconds(delay); // 10ms

      if quirk {
        hipulselo()
        delayMicroseconds(12603);  // need 6 delays units (4201*3)
        delayMicroseconds(12603);  // (4201*3)
      }
    }
  }
}

void loop() {
  if (digitalRead(CHANGE_MODE_BUTTON) == 1) {
    CHANGE_MODE_BUTTON_PRESSED = 0;
  }

  if (digitalRead(CHANGE_MODE_BUTTON) == 0 && CHANGE_MODE_BUTTON_PRESSED == 0) {
    if (choice < 12) {
      choice ++ ;
    } else {
      choice = 0;
    }

    CHANGE_MODE_BUTTON_PRESSED = 1;
    Serial.print("choice ");
    Serial.println(choice);
  }


  if (choice != lastchoice) {
    // clear display only if it's different
    displayLength = display.width();
  }
  lastchoice = choice;

  fire = digitalRead(TRIGGER_BUTTON);

  if (fire == 0) {
    Serial.print("fired ");
    Serial.println(choice);
  }

  switch (choice) {
    case 0:
      setLaserName("Ultralyte Non-LR"); // 100 pulses per second
      emit(fire, 9999);
      break;
    case 1:
      setLaserName("Ultralyte Rev.1"); // 100pps
      emit(fire, 9999);
      break;
    case 2:
      setLaserName("Jenoptik LaserPL");// 100pps
      emit(fire, 9999);
      break;
    case 3:
      setLaserName("Kustom Prolaser3");// 200 pps
      emit(fire, 4999);
      break;
    case 4:
      setLaserName("Jenoptik Laveg"); // 600pps
      emit(fire, 1666);
      break;
    case 5:
      setLaserName("Kustom Prolaser1"); // 380pps
      emit(fire, 2631);
      break;
    case 6:
      setLaserName("Ultralyte Rev.2");  // 125 pps
      emit(fire, 8000);
      break;
    case 7:
      setLaserName("Stalker LZ-1");  // 130pps
      emit(fire, 7691);
      break;
    case 8:
      setLaserName("Kustom Prolaser2");  // 238pps
      emit(fire, 4201);
      break;
    case 9:
      setLaserName("Laser Atlanta");  // 238pps
      emit(fire, 4201);
      break;
    case 10:
      setLaserName("Laser Atlanta Stealth Mode");  // 238pps  // 2 pulses fire followed by 5 missing pulses
      emit(fire, 4201, 2, true);
      break;
    case 11:
      setLaserName("Kustom ProLite");  // 200 pps
      emit(fire, 4999);
      break;
  }

  displayData();
}

void setLaserName(char* name) {
  sprintf(message, "%s", name);
  // displayLength = display.width();
}

void clearBUFFD() { // just clear the data buffer
  memset(message, 0x00, BUFFDSIZE);
}

void displayData() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  display.setCursor(displayLength, 00);
  display.println(message);

  if (fire == 0) {
    display.setTextSize(1.5);
    display.println("      ...FIRE...");
  }

  display.display();

  if (--displayLength < minX) displayLength = display.width();
}