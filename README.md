# 🚗 Car Black Box – Event Data Recorder

This project implements a **car black box** (event data recorder) on a microcontroller to log critical vehicle information before and after an incident.  
It is designed as a low‑cost embedded solution to improve vehicle safety and post‑event analysis.

---

## 🔧 Hardware Overview

- MCU: <MCU name, e.g., PIC18F458 / STM32F1xx etc.>
- Sensors / Inputs:
  - Speed / RPM: <e.g., pulse input from wheel sensor / CAN / UART>
  - Acceleration / impact: <e.g., accelerometer model>
  - Ignition / key status: <digital input>
  - Others: <add any additional signals>
- Memory:
  - <e.g., onboard flash / external EEPROM / SD card> for event logging
- Interfaces:
  - UART for configuration and log download
  - GPIOs for status LEDs / buzzer

Edit this section with your exact components.

---

## 🧠 Firmware Features

- Continuous monitoring of vehicle parameters (speed, acceleration, status signals).
- Event detection based on:
  - Sudden deceleration / impact thresholds
  - Power loss / ignition off
- Circular buffer for **pre‑event and post‑event** data storage.
- Time‑stamped logging of:
  - Speed / RPM
  - Acceleration
  - Event flags (e.g., crash, hard brake, power loss)
- UART‑based interface to:
  - Configure parameters (thresholds, sampling rates)
  - Download recorded logs for analysis
- Implemented in C with interrupt‑driven I/O and state‑machine based logic.

---

## 📁 Project Structure

```text
CAR_BLACK_BOX_PROJECT.X/
  ├── src/               # Application source files
  ├── inc/               # Header files
  ├── config/            # MCU configuration / project settings
  └── ...                # Toolchain-generated files
```

Adjust folder names if they are different.

---

## ⚙️ Development Environment

- IDE / Toolchain: <e.g., MPLAB X + XC8 / STM32CubeIDE / Keil / etc.>
- Language: C
- Target: <MCU or board name>

### Build and Flash

1. Clone the repository:
   ```bash
   git clone https://github.com/chandu-kc/car-black-box.git
   ```
2. Open the project in `<your IDE name>`.
3. Select the correct target MCU / board.
4. Build the project.
5. Flash the firmware to the board using `<programmer/debugger name>`.

---

## 📝 Example Log Format

Example of recorded log data over UART (adjust to your actual format):

```text
Timestamp, Speed_kmph, Accel_mps2, Event
0001, 45, 0.2, NORMAL
0002, 46, 0.1, NORMAL
0003, 80, -5.6, HARD_BRAKE
0004, 0, -9.8, CRASH
```

You can import logs into Excel, Python, or other tools for analysis.

---

## 🚀 Possible Extensions

- Add CAN bus support for modern vehicles.
- Add SD card for longer log storage.
- Add GPS module to record location data.
- Add Bluetooth / Wi‑Fi for wireless log retrieval.

---

## 📚 Learning Outcomes

This project demonstrates:

- Embedded C programming for real‑time systems.
- Interrupt‑driven I/O and state‑machine design.
- Sensor interfacing and signal processing.
- Data logging and serial communication (UART).
- Safety‑oriented embedded system design.
