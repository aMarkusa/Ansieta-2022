/**************************************************************************
  Ansieta Anxiety monitor and digital helper.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ewma.h>

// Device pin configuration

#define ACC_VCC 26
#define HEART_VCC 14

#define BTN_1_PIN 13
#define BTN_2_PIN 12

#define HEARTRATE_PIN 27

#define VIBRATE_PIN 19
//############### Accelerometer ###################
#define X_PIN 25
#define Y_PIN 33
#define Z_PIN 32
//############### Accelerometer ###################

// Operative configuration

#define MIN_ATTACK_INTERVAL 30000

#define MIN_ATTACK_HEART_RATE 60

#define VIBRATE_PERIOD 500


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void noop() {}
void (*button1OnPress)(void) = &noop;
void (*button2OnPress)(void) = &noop;

void (*scheduledFunction)(void) = &noop;
unsigned long scheduledFunctionTime = 0;

bool anxietyDetected = false;

unsigned long lastAnxietyAttackTime = 0;

//################################### accelerometer #########################
double time_old = 0;
double x_raw, y_raw, z_raw;
int x_old, y_old, z_old;
Ewma exp_filter_x(0.0005);
Ewma exp_filter_y(0.0005);
Ewma exp_filter_z(0.0005);
//################################### accelerometer #########################

void setup() {
  Serial.begin(115200);

  pinMode(ACC_VCC, OUTPUT);
  pinMode(HEART_VCC, OUTPUT);

  digitalWrite(ACC_VCC, HIGH);
  digitalWrite(HEART_VCC, HIGH);

  pinMode(BTN_1_PIN, INPUT_PULLUP);
  pinMode(BTN_2_PIN, INPUT_PULLUP);
  pinMode(VIBRATE_PIN, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the message
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Show splash screen
  display.println("Ansieta");
  display.display(); 
  delay(2000);
    // Clear the buffer
  display.clearDisplay();
  display.display(); 

//############################################ accelerometer #######################################

  analogReadResolution(10);
  read_raw_values(&x_raw, &y_raw, &z_raw); 

// Read initial values
  x_old = exp_filter_x.filter(x_raw);
  y_old = exp_filter_y.filter(y_raw);
  z_old = exp_filter_z.filter(z_raw);
  time_old = micros(); 
}

void loop() {
  if(millis() - lastAnxietyAttackTime >= MIN_ATTACK_INTERVAL) {
    vibrateDevice();
    showAnxietyPrompt();
    lastAnxietyAttackTime = millis();
  }
  
  if(!anxietyDetected) {
    showScreenSaver();
  }

  // Handle scheduledFunction if requested:
  handleScheduledFunction();

  // Handle button operation:
  handleButtons();

  // Handle vibration stop:
  handleVibrate();

  // Constant monitoring operations:
  monitorHeartRate();

  // Let the processor breathe
  delay(20);
}


bool detectAnxiety() {
  // Main logic for detecting an anxiety attack.
    
  if(getHeartRate() > MIN_ATTACK_HEART_RATE && moving() == false) {
    return true;
  }
  return false;
}


void handleButtons() {
  // Trigger event handlers on button operation.
  // Set globals button1OnPress and button2OnPress to your event handler function.
  
  if (digitalRead(BTN_1_PIN) == LOW) {
    button1OnPress();
  }
  else if (digitalRead(BTN_2_PIN) == LOW) {
    button2OnPress();
  }
}

void handleScheduledFunction() {
  // By setting the global scheduledFunction and scheduledFunctionTime to a point in future, function is executed at that time.
  
  if(scheduledFunctionTime != 0 && millis()>=scheduledFunctionTime) {
    scheduledFunction();
    scheduledFunctionTime=0;
  }
}
