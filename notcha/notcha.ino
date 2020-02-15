
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
int LIDAR_EMITTER = 13; //LIDAR Emitter pin
int TRIGGER_BUTTON = 11; //trigger pin
int CHANGE_MODE_BUTTON = 12;
int CHANGE_MODE_BUTTON_PRESSED = 0;

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

void pulse(int delayms, int cycles = 3) {
  for (int cycleCount = 1; cycleCount <= cycles; cycleCount++) {
    digitalWrite(LIDAR_EMITTER, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(LIDAR_EMITTER, LOW);
    delayMicroseconds(delayms);
  }
}

void sendPulses(int triggerState, int delayms, int cycles = 3, bool stealth = false) { //sends specified amount of pulses with specified delay if triggerState is 0
  if (triggerState == 0) {
        pulse(delayms, cycles);
  }
  if (stealth) { //Stealth Mode for Atlanta Laser
    delayMicroseconds(8402);  // need 4 delays units (4201*3) 1 is already in pulse()
    delayMicroseconds(12603); // need 6 delays units (4201*3)
  }
}

void loop() {

  if (digitalRead(CHANGE_MODE_BUTTON) == 1) {
    CHANGE_MODE_BUTTON_PRESSED = 0;
  }
  if (digitalRead(CHANGE_MODE_BUTTON) == 0 && CHANGE_MODE_BUTTON_PRESSED == 0) {
    if (choice < 12) {
      choice ++;
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
      sendPulses(fire, 9999);
      break;
    case 1:
      setLaserName("Ultralyte Rev.1"); // 100pps
      sendPulses(fire, 9999);
      break;
    case 2:
      setLaserName("Jenoptik LaserPL");// 100pps
      sendPulses(fire, 9999);
      break;
    case 3:
      setLaserName("Kustom Prolaser3");// 200 pps
      sendPulses(fire, 4999);
      break;
    case 4:
      setLaserName("Jenoptik Laveg"); // 600pps
      sendPulses(fire, 1666);
      break;
    case 5:
      setLaserName("Kustom Prolaser1"); // 380pps
      sendPulses(fire, 2631);
      break;
    case 6:
      setLaserName("Ultralyte Rev.2");  // 125 pps
      sendPulses(fire, 8000);
      break;
    case 7:
      setLaserName("Stalker LZ-1");  // 130pps
      sendPulses(fire, 7691);
      break;
    case 8:
      setLaserName("Kustom Prolaser2");  // 238pps
      sendPulses(fire, 4201);
      break;
    case 9:
      setLaserName("Laser Atlanta");  // 238pps
      sendPulses(fire, 4201);
      break;
    case 10:
      setLaserName("Laser Atlanta Stealth Mode");  // 238pps 2 pulses fire followed by 5 missing pulses
      sendPulses(fire, 4201, 2, true);
      break;
    case 11:
      setLaserName("Kustom ProLite");  // 200 pps
      sendPulses(fire, 4999);
      break;
  }

  displayData();
}


void setLaserName(char* name) {
  sprintf(message, "%s", name);
  displayLength = display.width();
}

void clearBUFFD() { //just clear the data buffer
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
