#include <Wire.h>
#include <SoftwareSerial.h>

#define ADXL345_ADDRESS 0x53  // ADXL345 I2C address

// SIM800L GSM Setup
SoftwareSerial GSM(8, 7);  // SIM800L Tx & Rx is connected to Arduino #8 & #7
char phone_no[] = "+918428265833";  // Replace with your phone number

#define bt_M  A0  // Button for SMS
#define bt_C  A1  // Button for Call
#define LED   2   // LED Pin

const int threshold = 10;  // Set threshold for acceleration (adjust as needed)
int16_t ax, ay, az;  // Variables to store ADXL345 readings

char inchar;  // For incoming character from GSM

void setup() {
  // Serial communication for Arduino and GSM
  Serial.begin(9600);
  GSM.begin(9600);
  
  // Setup pins
  pinMode(bt_M, INPUT_PULLUP);
  pinMode(bt_C, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // Initialize ADXL345
  Wire.begin();
  initADXL345();

  // SIM800L Initialization
  Serial.println("Initializing SIM800L...");
  initModule("AT", "OK", 1000);
  initModule("ATE1", "OK", 1000);
  initModule("AT+CPIN?", "READY", 1000);
  initModule("AT+CMGF=1", "OK", 1000);   // Set SMS mode to text
  initModule("AT+CNMI=2,2,0,0,0", "OK", 1000);  // Incoming SMS config
  Serial.println("Initialization Complete");
}

void loop() {
  // Check for SMS commands and incoming messages
  if (GSM.available() > 0) {
    inchar = GSM.read();
    handleGSMCommands();
  }

  // Check button presses
  if (digitalRead(bt_M) == LOW) {
    sendSMS(phone_no, "Hello, this is a test message from the GSM module.");
  }
  
  if (digitalRead(bt_C) == LOW) {
    callUp(phone_no);
  }

  // Read ADXL345 accelerometer data
  readADXL345();

  // Check if the acceleration exceeds the threshold
  if (abs(ax) > threshold) {
    Serial.println("Threshold exceeded, making a call...");
    callUp(phone_no);
    delay(20000);  // Delay to prevent repeated calls
  }

  delay(100);
}

// Function to initialize ADXL345
void initADXL345() {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x2D);  // Power Control Register
  Wire.write(0x08);  // Enable measurement mode
  Wire.endTransmission();

  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x31);  // Data Format Register
  Wire.write(0x08);  // Set full resolution and +/-2g range
  Wire.endTransmission();
}

// Function to read ADXL345 data
void readADXL345() {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x32);  // Start reading from Data Register 0x32
  Wire.endTransmission();
  
  Wire.requestFrom(ADXL345_ADDRESS, 6);  // Request 6 bytes (2 bytes for each axis)
  
  if (Wire.available() == 6) {
    ax = (Wire.read() | (Wire.read() << 8));  // X-axis
    ay = (Wire.read() | (Wire.read() << 8));  // Y-axis
    az = (Wire.read() | (Wire.read() << 8));  // Z-axis
    Serial.print(ax);
  }
}

void handleGSMCommands() {
  if (inchar == 'R') {
    if (readGSMCommand("RING")) {
      initModule("ATH", "OK", 1000);  // Hang up
    }
  } else if (inchar == 'L') {
    if (readGSMCommand("LED1")) {
      digitalWrite(LED, HIGH);
      sendSMS(phone_no, "LED is On");
    } else if (readGSMCommand("LED0")) {
      digitalWrite(LED, LOW);
      sendSMS(phone_no, "LED is Off");
    }
  }
}

bool readGSMCommand(String command) {
  for (int i = 0; i < command.length(); i++) {
    inchar = GSM.read();
    if (inchar != command[i]) return false;
  }
  return true;
}

// Send SMS function
void sendSMS(char *number, char *msg) {
  GSM.print("AT+CMGS=\"");
  GSM.print(number);
  GSM.println("\"\r\n");
  delay(500);
  GSM.println(msg);
  delay(500);
  GSM.write(26);  // Ctrl+Z
  delay(3000);
}

// Make a phone call function
void callUp(char *number) {
  GSM.print("ATD ");
  GSM.print(number);
  GSM.println(";");
  delay(20000);  // Wait for 20 seconds for the call
  GSM.println("ATH");  // Hang up the call
  delay(100);
}

// Module initialization helper function
void initModule(String cmd, char *res, int t) {
  while (1) {
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while (GSM.available() > 0) {
      if (GSM.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}
