# JD9365 ESP32 LCD Test
![ESP32-P4-NANO](https://img.shields.io/badge/Platform-ESP32-blue)
![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-green)
![License](https://img.shields.io/badge/License-Unlicense-lightgrey)

## Overview
This project demonstrates how to interface an **ESP32** with a **Waveshare JD9365 10.1" MIPI DSI LCD panel**.  

Features implemented:
- Initialize the JD9365 LCD via **ESP-IDF v5.5.2**  
- Power and configure the **MIPI DSI PHY**  
- Install JD9365 panel driver and control display  
- Draw **solid colors** and **SMPTE-style color bars** on the screen 


## Hardware
| Component | Details |
|-----------|--------|
| Microcontroller | ESP32-P4-NANO
| LCD Panel | Waveshare JD9365 10.1" MIPI DSI |
| Connections | Configured via `menuconfig` for SCL/SDA GPIOs |


## Software / Dependencies
- **ESP-IDF v5.5.2**  
- **FreeRTOS** (comes with ESP-IDF)  
- Optional: **PSRAM enabled** for full-screen frame buffers  
- CMake build system (standard with ESP-IDF projects)


## Setup

1. **Clone repository**:
    ```bash
    git clone https://github.com/yourusername/jd9365-esp32.git
    cd jd9365-esp32
    ```

2. **Set up ESP-IDF**:
    Follow the official [ESP-IDF setup guide](https://www.waveshare.com/wiki/ESP32-P4-Nano-StartPage)

3. **Configure project**:
    ```bash
    idf.py menuconfig
    ```
    - Set your I2C pins (CONFIG_I2C_MASTER_SCL and CONFIG_I2C_MASTER_SDA)
    - Enable PSRAM if using full-screen buffers

4. **Build and flash**:
    ```bash
    idf.py build
    idf.py flash monitor
    ```


## Project Structure
```bash
├── main/
│   ├── i2c_basic_example_main.c   # Main application
├── CMakeLists.txt
├── sdkconfig                       # Configurations (after menuconfig)
├── README.md                       # Project documentation

```

## Troubleshooting
- Black screen → ensure PSRAM enabled and buffer allocation succeeded
- Wrong colors → check RGB565 vs RGB888 mismatch, adjust pixel format
- I2C sensor errors → confirm dev_handle is valid and device is on the bus