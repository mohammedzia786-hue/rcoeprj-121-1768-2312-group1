*#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ---------------- FLEX SENSOR PINS ----------------
#define INDEX  A3
#define MIDDLE A2
#define RING   A1
#define PINKY  A0

// ---------------- THRESHOLDS ----------------
// If bending lowers values:
// bent = value below threshold

int indexTh  = 400;
int middleTh = 400;
int ringTh   = 430;
int pinkyTh  = 245;

// ---------------- VARIABLES ----------------
String lastMsg = "";
unsigned long msgTime = 0;
bool visible = false;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);
  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

// ---------------- LOOP ----------------
void loop() {

  int i = analogRead(INDEX);
  int m = analogRead(MIDDLE);
  int r = analogRead(RING);
  int p = analogRead(PINKY);

  // Serial Monitor readings
  Serial.print("I:");
  Serial.print(i);
  Serial.print(" M:");
  Serial.print(m);
  Serial.print(" R:");
  Serial.print(r);
  Serial.print(" P:");
  Serial.println(p);

  // bent = LOW value
  bool bi = i < indexTh;
  bool bm = m < middleTh;
  bool br = r < ringTh;
  bool bp = p < pinkyTh;

  String msg = "";

  // ------------------------------------------------
  // GESTURES
  // false = open finger
  // true  = bent finger
  // Order = Index Middle Ring Pinky
  // ------------------------------------------------

  // Open Open Open Open
  if (!bi && !bm && !br && !bp) msg = "HELLO";

  // Bent Bent Bent Bent
  else if (bi && bm && br && bp) msg = "STOP";

  // Bent Open Open Open
  // Only index bent
  else if (bi && !bm && !br && !bp) msg = "COME HERE";

  // Open Bent Open Open
  // Only middle bent
  else if (!bi && bm && !br && !bp) msg = "THANK YOU";

  // Open Open Bent Open
  // Only ring bent
  else if (!bi && !bm && br && !bp) msg = "YES";

  // Open Open Open Bent
  // Only pinky bent
  else if (!bi && !bm && !br && bp) msg = "NO";

  // Bent Bent Open Open
  // Index + Middle bent
  else if (bi && bm && !br && !bp) msg = "WATER";

  // Open Open Bent Bent
  // Ring + Pinky bent
  else if (!bi && !bm && br && bp) msg = "GOOD NIGHT";

  // Bent Open Bent Open
  // Index + Ring bent
  else if (bi && !bm && br && !bp) msg = "HELP";

  // Open Bent Bent Open
  // Middle + Ring bent
  else if (!bi && bm && br && !bp) msg = "CALL HOME";

  // ---------------- DISPLAY ----------------
  if (msg != "" && msg != lastMsg) {
    showText(msg);
    lastMsg = msg;
    msgTime = millis();
    visible = true;
  }

  // Clear screen after 5 sec
  if (visible && millis() - msgTime > 5000) {
    display.clearDisplay();
    display.display();
    visible = false;
    lastMsg = "";
  }

  delay(1000);
}

// ---------------- OLED FUNCTION ----------------
void showText(String txt) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.println(txt);
  display.display();
}*
