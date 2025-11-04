# 🚨 Arduino Fall Detector & GSM SOS Alert System

This project turns an Arduino into a personal safety device. It uses an ADXL345 accelerometer to detect a sudden fall or impact. When a fall is detected, it automatically places an SOS call to a pre-configured phone number using a SIM800L GSM module.

It also features manual buttons for making an emergency call or sending an SMS, and can be remotely controlled via SMS.

---

## 🚀 Features

* **Automatic Fall Detection:** Senses high G-force (impact) and automatically triggers an SOS call.
* **Manual SOS Call:** A dedicated button (`A1`) to manually initiate a call to the emergency contact.
* **Manual SMS Alert:** A dedicated button (`A0`) to manually send a pre-defined "Hello" message.
* **Remote LED Control:** Send an SMS to the device to remotely turn an LED on or off (`LED1` for ON, `LED0` for OFF).
* **SMS Confirmation:** The device sends an SMS back to confirm the remote command was successful.

---

## 🛠️ Hardware Required

* Arduino (e.g., Uno, Nano)
* **SIM800L** GSM Module
* An activated 2G SIM Card (with credit for calls/SMS)
* **ADXL345** Accelerometer Module (I2C)
* 2x Push Buttons (Momentary)
* 1x LED
* 1x Resistor (e.g., 220 Ohm for the LED)
* Breadboard and Jumper Wires
* **External Power Supply:** A 3.7V LiPo battery or a 5V 2A power adapter.

> **Warning:** The SIM800L module is power-hungry and can draw up to 2A. **Do not** power it from the Arduino's 5V pin. It will cause the system to crash and fail. Use a dedicated external power supply.

---

## 🔌 Wiring & Connections

| Component | Pin | Arduino Pin |
| :--- | :--- | :--- |
| **SIM800L** | TX | D7 |
| | RX | D8 |
| | VCC | External 3.7V - 4.2V |
| | GND | GND (common with Arduino) |
| **ADXL345** | SCL | A5 (SCL) |
| | SDA | A4 (SDA) |
| | VCC | 3.3V (from Arduino) |
| | GND | GND |
| **SMS Button**| Pin 1 | A0 |
| | Pin 2 | GND |
| **Call Button**| Pin 1 | A1 |
| | Pin 2 | GND |
| **LED** | Anode (+) | D2 (via 220Ω resistor) |
| | Cathode (-)| GND |

---

## 🔧 How to Use

1.  **Assemble the Circuit:** Follow the wiring diagram in the table above.
2.  **Update the Code:** Open the `.ino` sketch and change the following line to your emergency contact's phone number:
    ```cpp
    char phone_no[] = "+91xxxx";  // <-- REPLACE THIS with your number
    ```
3.  **Adjust Sensitivity (Optional):** You can change the fall detection sensitivity by modifying the `threshold` value. A lower number makes it more sensitive.
    ```cpp
    const int threshold = 10; // Adjust as needed
    ```
4.  **Upload:** Select your Arduino board and port, and upload the sketch.
5.  **Test:**
    * Open the Serial Monitor at 9600 baud to see the initialization steps.
    * Press the **SMS button (A0)** to send a test message.
    * Press the **Call button (A1)** to make a test call.
    * Send an SMS with the text `LED1` to the device's SIM number to test remote control.
    * Give the ADXL345 a sharp tap or jolt to simulate a fall and trigger the automatic call.

---

## 📚 Libraries Used

* `Wire.h` (Built-in, for I2C communication with ADXL345)
* `SoftwareSerial.h` (Built-in, for serial communication with SIM800L)
