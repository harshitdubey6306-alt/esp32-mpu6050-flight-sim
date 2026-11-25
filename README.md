# ESP32 + MPU6050 3D Flight Simulator

A real-time 3D airplane ring-flyer browser game controlled by tilting an **MPU6050** accelerometer/gyroscope wired to an **ESP32S**, communicating directly with Chrome/Edge over USB via the **Web Serial API**—no Bluetooth pairing, backend server, or extra drivers required.

## Live Demo
Play directly in **Google Chrome** or **Microsoft Edge** (desktop):  
[https://harshitdubey6306-alt.github.io/esp32-mpu6050-flight-sim/](https://harshitdubey6306-alt.github.io/esp32-mpu6050-flight-sim/)

*(No hardware on hand? Click **Test with Keyboard** to fly with the Arrow Keys.)*

## Features
- **Zero-dependency ESP32 firmware:** Reads raw I2C registers directly via `Wire.h` (works with both genuine and clone GY-521 MPU6050 modules, auto-detecting `0x68` and `0x69`).
- **Smooth flight control:** Uses a complementary filter (96% gyroscope, 4% accelerometer) to combine responsiveness with drift correction.
- **Browser-native Web Serial connection:** Streams `roll,pitch` over USB at `115200` baud with automatic DTR/RTS boot-signal handling so the ESP32S boots cleanly into run mode on Windows.
- **In-game HUD controls:** Live flight speed slider, one-click **Zero / Calibrate** button, ring score counter, and a built-in raw serial monitor bar for quick debugging.

## Hardware Required
- **ESP32S** (ESP-WROOM-32 / DevKit)
- **MPU6050 (GY-521)** 6-axis IMU module
- 4x Jumper wires + Micro-USB data cable

## Wiring Diagram

| MPU6050 Pin | ESP32S Pin | Purpose |
| :--- | :--- | :--- |
| **VCC** | **3V3** *(or **VIN / 5V** if LED stays off)* | Power |
| **GND** | **GND** | Ground |
| **SDA** | **GPIO 21** | I2C Data |
| **SCL** | **GPIO 22** | I2C Clock |

## Getting Started

### 1. Clone the Repository
```bash
git clone [https://github.com/harshitdubey6306-alt/esp32-mpu6050-flight-sim.git](https://github.com/harshitdubey6306-alt/esp32-mpu6050-flight-sim.git)
```

### 2. Flash the ESP32S
1. Open `firmware/esp32_mpu6050_flight.ino` in **Arduino IDE**.
2. Select **Tools → Board → esp32 → ESP32 Dev Module** and choose your active COM port.
3. Click **Upload**. *(No third-party MPU6050 libraries are needed—only the built-in `Wire.h` library.)*
4. **Close the Arduino Serial Monitor** after uploading (only one application can access the COM port at a time).

### 3. Launch the Game
1. Open the [Live Demo](https://harshitdubey6306-alt.github.io/esp32-mpu6050-flight-sim/) or run `index.html` locally in **Chrome** or **Edge**.
2. Click **Connect ESP32 & Fly** and select your ESP32's COM port.
3. Hold the MPU6050 level in your hand and click **Zero / Calibrate**.
4. Adjust the **Flight Speed** slider in the top-left HUD and tilt to fly through the golden rings.

## Project Structure
```text
├── index.html                        # Three.js 3D game + Web Serial API controller
├── firmware/
│   └── esp32_mpu6050_flight.ino      # ESP32S raw I2C Wire.h + complementary filter sketch
└── README.md
```

## Troubleshooting
- **`NO_SENSOR_CHECK_WIRES` in the HUD:** Verify `SDA` (GPIO 21) and `SCL` (GPIO 22), confirm header pins are properly soldered, and try powering `VCC` from `VIN`/`5V` if the onboard MPU6050 LED is not glowing.
- **Port won't open:** Make sure Arduino IDE's Serial Monitor is closed and no other browser tab is connected to the same COM port.
- **Web Serial not supported:** Use desktop Google Chrome, Microsoft Edge, or Brave (Firefox and mobile browsers do not support Web Serial).