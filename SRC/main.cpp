#include <M5StickCPlus.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Define the IR LED pins
#define IR_BUILTIN_PIN 9  // Built-in IR LED on M5Stick-C Plus
#define IR_EXTERNAL_PIN 26 // External IR module on GPIO 26

// Define the built-in LED pin (M5Stick-C Plus uses GPIO 10 for the red LED)
#define LED_PIN 10

// Create an IRsend object for both IR outputs
IRsend irsendBuiltIn(IR_BUILTIN_PIN);
IRsend irsendExternal(IR_EXTERNAL_PIN);

// TV power-off codes for various brands (32-bit codes)
const uint32_t lgPowerOff[] = {0x20DF10EF, 0x20DF08F7}; // LG TV power-off codes
const uint32_t samsungPowerOff[] = {0xE0E040BF, 0xE0E019E6}; // Samsung TV power-off codes
const uint32_t panasonicPowerOff[] = {0x40040100, 0x40040101}; // Panasonic TV power-off codes
const uint32_t sonyPowerOff[] = {0xA90, 0xA91}; // Sony TV power-off codes
const uint32_t philipsPowerOff[] = {0x100C, 0x100D}; // Philips TV power-off codes
const uint32_t sharpPowerOff[] = {0x2FD48B7, 0x2FD08F7}; // Sharp TV power-off codes
const uint32_t toshibaPowerOff[] = {0x2FD48B7, 0x2FD08F7}; // Toshiba TV power-off codes
const uint32_t hisensePowerOff[] = {0x4FB48B7, 0x4FB08F7}; // Hisense TV power-off codes
const uint32_t tclPowerOff[] = {0x5EA18E7, 0x5EA08F7}; // TCL TV power-off codes
const uint32_t vizioPowerOff[] = {0x6F942B7, 0x6F908F7}; // Vizio TV power-off codes
const uint32_t grundigPowerOff[] = {0x7D42B7, 0x7D08F7}; // Grundig TV power-off codes
const uint32_t loewePowerOff[] = {0x8E942B7, 0x8E908F7}; // Loewe TV power-off codes
const uint32_t telefunkenPowerOff[] = {0x9F942B7, 0x9F908F7}; // Telefunken TV power-off codes
const uint32_t vestelPowerOff[] = {0xAF942B7, 0xAF908F7}; // Vestel TV power-off codes

// Projector power-off codes
const uint32_t epsonPowerOff[] = {0x3E80FF00, 0x3E807F80}; // Epson projector power-off codes
const uint32_t benqPowerOff[] = {0x4CB340BF, 0x4CB3C03F}; // BenQ projector power-off codes
const uint32_t optomaPowerOff[] = {0x5EA18E7, 0x5EA08F7}; // Optoma projector power-off codes

// LED remote power-off codes
const uint32_t ledRemotePowerOff[] = {0xFFA25D, 0xFF629D}; // Common LED remote power-off codes

// App states
enum AppState { MENU, RUNNING, COMPLETE };
AppState appState = MENU; // Current app state

// IR output sources
enum IRSource { BUILTIN, EXTERNAL_MODULE };
IRSource irSource = BUILTIN; // Current IR output source

unsigned long lastToggleTime = 0; // Timestamp of the last toggle
const unsigned long toggleCooldown = 3000; // 3-second cooldown after toggling

unsigned long lastButtonPressTime = 0; // Timestamp of the last button press
const unsigned long autoDimTimeout = 10000; // 10-second timeout for auto-dim
const unsigned long sleepTimeout = 30000; // 30-second timeout for energy saving mode
const uint8_t brightnessHigh = 50; // 50% brightness
const uint8_t brightnessLow = 1; // 1% brightness

// Function to send power-off codes for all TVs
void sendPowerOffCodes() {
  // Turn on the LED to indicate IR transmission
  digitalWrite(LED_PIN, LOW); // LOW turns on the LED (active low)

  // Select the IR output source
  IRsend& irsend = (irSource == BUILTIN) ? irsendBuiltIn : irsendExternal;

  // Send power-off codes for TVs
  for (uint32_t code : lgPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : samsungPowerOff) irsend.sendSAMSUNG(code, 32);
  for (uint32_t code : panasonicPowerOff) irsend.sendPanasonic(code, 32);
  for (uint32_t code : sonyPowerOff) irsend.sendSony(code, 20);
  for (uint32_t code : philipsPowerOff) irsend.sendRC5(code, 12);
  for (uint32_t code : sharpPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : toshibaPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : hisensePowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : tclPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : vizioPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : grundigPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : loewePowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : telefunkenPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : vestelPowerOff) irsend.sendNEC(code, 32);

  // Send power-off codes for projectors
  for (uint32_t code : epsonPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : benqPowerOff) irsend.sendNEC(code, 32);
  for (uint32_t code : optomaPowerOff) irsend.sendNEC(code, 32);

  // Send power-off codes for LED remotes
  for (uint32_t code : ledRemotePowerOff) irsend.sendNEC(code, 32);

  // Turn off the LED after sending IR codes
  digitalWrite(LED_PIN, HIGH); // HIGH turns off the LED (active low)
}

// Function to draw the main menu
void drawMenu() {
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(20, 20); // Centered text
  M5.Lcd.println("I.R.S.");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.println("Press M5 to start");
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("By: Orpheas / Zorikos3");
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.print("IR Out: ");
  M5.Lcd.println(irSource == BUILTIN ? "Built-in" : "G26");

  // Display battery level
  float batteryVoltage = M5.Axp.GetBatVoltage();
  int batteryPercent = map(batteryVoltage * 1000, 3200, 4200, 0, 100); // Convert voltage to percentage
  batteryPercent = constrain(batteryPercent, 0, 100); // Ensure percentage is within 0-100
  M5.Lcd.setCursor(10, 120);
  M5.Lcd.print("Battery: ");
  M5.Lcd.print(batteryPercent);
  M5.Lcd.println("%");

  // Draw a battery icon
  M5.Lcd.fillRect(10, 140, 50, 20, TFT_WHITE); // Battery outline
  M5.Lcd.fillRect(60, 145, 5, 10, TFT_WHITE); // Battery tip
  M5.Lcd.fillRect(12, 142, 46 * batteryPercent / 100, 16, TFT_GREEN); // Battery level
}

// Function to draw the "Running" screen
void drawRunning() {
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Running...");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.println("Sending IR codes");
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.print("IR Out: ");
  M5.Lcd.println(irSource == BUILTIN ? "Built-in" : "G26");
}

// Function to draw the "Complete" screen
void drawComplete() {
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Complete!");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.println("Press M5 to retry");
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.print("IR Out: ");
  M5.Lcd.println(irSource == BUILTIN ? "Built-in" : "G26");
}

// Function to set display brightness
void setBrightness(uint8_t brightness) {
  M5.Axp.ScreenBreath(brightness);
}

// Function to enter energy saving mode
void enterSleepMode() {
  M5.Lcd.fillScreen(TFT_BLACK); // Clear the screen
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Entering sleep mode...");
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.println("Press M5 to exit sleep mode");

  // Turn off the display
  setBrightness(0);

  // Put the device into deep sleep
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, LOW); // Wake up on M5 button press
  esp_deep_sleep_start();
}

void setup() {
  // Initialize M5Stick-C
  M5.begin();
  M5.Lcd.setRotation(1); // Set screen orientation to default (portrait mode)

  // Set initial brightness to 50%
  setBrightness(brightnessHigh);

  // Draw the main menu
  drawMenu();

  // Initialize IR senders
  irsendBuiltIn.begin();
  irsendExternal.begin();

  // Initialize the LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn off the LED initially

  // Record the initial button press time
  lastButtonPressTime = millis();
}

void loop() {
  M5.update(); // Update button states

  // Handle button press based on app state
  if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed()) {
    // Restore brightness to 50% on button press
    setBrightness(brightnessHigh);
    lastButtonPressTime = millis(); // Update the last button press time

    if (M5.BtnA.wasPressed()) {
      switch (appState) {
        case MENU:
          // Start sending IR codes
          appState = RUNNING;
          drawRunning();
          break;
        case COMPLETE:
          // Return to the main menu
          appState = MENU;
          drawMenu();
          break;
        default:
          break;
      }
    }

    // Handle "Next" button press to toggle IR output source
    if (M5.BtnB.wasPressed()) {
      irSource = (irSource == BUILTIN) ? EXTERNAL_MODULE : BUILTIN; // Toggle IR source
      drawMenu(); // Update the menu to show the new IR source
    }
  }

  // Handle auto-dim mode
  if (millis() - lastButtonPressTime > autoDimTimeout) {
    setBrightness(brightnessLow); // Dim the screen to 1%
  }

  // Handle energy saving mode
  if (millis() - lastButtonPressTime > sleepTimeout) {
    enterSleepMode(); // Enter deep sleep mode
  }

  // Handle app state logic
  switch (appState) {
    case RUNNING:
      // Send IR codes
      sendPowerOffCodes();
      appState = COMPLETE; // Move to the "Complete" state
      drawComplete(); // Draw the "Complete" screen
      break;
    default:
      break;
  }
}