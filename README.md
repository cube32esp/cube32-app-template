# cube32-app-template

A ready-to-use ESP-IDF application template for the **CUBE32-S3 Core** development board.
Clone, rename, build — then use Copilot AI chat to load demos or build your own application.

---

## Hardware

| Item | Details |
|---|---|
| Board | CUBE32-S3 Core |
| MCU | ESP32-S3 (dual-core LX7, 240 MHz) |
| Flash | 16 MB QIO |
| PSRAM | 8 MB Octal SPI |
| Display | ST7789 TFT 240×240 or 240×320 |
| Touch | CST816S / FT6336 capacitive |
| IMU | LSM6DSOX (accel + gyro + MLC) |
| Sensors | LIS3MDL magnetometer, AXP2101 PMU, BM8563 RTC |
| Audio | ES7210 ADC mic + ES8311 DAC speaker |
| Camera | OV5640 (optional) |
| Connectivity | WiFi + BLE (built-in ESP32-S3) |

---

## Get Started

### Step 1 — Clone

```sh
git clone https://github.com/cube32esp/cube32-app-template.git my_app
cd my_app
```

### Step 2 — Rename the project

Open `CMakeLists.txt` and change the project name:

```cmake
project(my_app)   # ← change this to your app name
```

### Step 3 — Build and flash

```sh
source ~/esp/v5.5.1/esp-idf/export.sh
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

> Requires ESP-IDF ≥ 5.4.0. The BSP component (`cube32esp/cube32_bsp`) is fetched
> automatically from the Espressif Component Registry — no manual installation needed.

---

## Use Copilot AI Chat

Open the project in VS Code and use the Copilot chat panel.
The following skills are built into this template:

| Trigger phrase | What happens |
|---|---|
| `copy hello_display` | Copies the display demo into main.cpp + updates sdkconfig.defaults |
| `copy hello_world` | Copies the full 3D cube + IMU demo + updates sdkconfig.defaults |
| `copy hello_lvgl` | Copies the LVGL widgets demo (once migrated) |
| `copy <any example name>` | Same pattern for all 23 examples below |
| `create new project my_app` | Scaffolds a brand-new CUBE32 IDF project |

After copying an example the AI will print a **menuconfig checklist** — any
`idf.py menuconfig` settings that example needs. Then run:

```sh
idf.py fullclean && idf.py build
```

---

## Available BSP Examples

All examples come from the [`cube32esp/cube32_bsp`](https://github.com/cube32esp/cube32_bsp) repository.
Ask Copilot to copy any of them by name.

| Example | Key Features | Status |
|---|---|---|
| `hello_world` | 3D wireframe cube, IMU gyro, touch swipe, LVGL, hardware status screen | ✅ Available |
| `hello_display` | Raw ST7789 drawing — color bars, gradient, bouncing rect. No LVGL | ✅ Available |
| `hello_activity` | Accel-based activity recognition (walk / jog / bike / drive) | 🔜 Migrating |
| `hello_aec_test` | Mic + speaker, Acoustic Echo Cancellation, WAV recording | 🔜 Migrating |
| `hello_audio_player` | WAV/MP3 playback from SD card, LVGL UI, volume control | 🔜 Migrating |
| `hello_audio_recorder` | Mic recording to WAV, LVGL playback UI | 🔜 Migrating |
| `hello_ble_ota` | BLE over-the-air firmware update | 🔜 Migrating |
| `hello_camera` | OV5640 capture + live preview on TFT | 🔜 Migrating |
| `hello_compass` | Tilt-corrected compass rose, LIS3MDL + LSM6DSO, LVGL | 🔜 Migrating |
| `hello_imu_mag` | IMU + magnetometer live data, LVGL dark-theme UI | 🔜 Migrating |
| `hello_lvgl` | LVGL 9.x widgets — buttons, styles, animations, swipe gestures | 🔜 Migrating |
| `hello_mlc_activity` | LSM6DSOX MLC hardware activity classification | 🔜 Migrating |
| `hello_modem_console` | A7670 LTE modem, AT commands, signal/IP display, LVGL | 🔜 Migrating |
| `hello_mqtt` | MQTT pub/sub, AWS IoT, WiFi + LTE, SSL/TLS, LVGL | 🔜 Migrating |
| `hello_pwm_servo` | PWM servo with smooth motion, LVGL angle control | 🔜 Migrating |
| `hello_rtc_ntp` | BM8563 RTC, NTP sync over WiFi/LTE, timezone, LVGL | 🔜 Migrating |
| `hello_rtc_timer` | RTC alarm, AXP2101 auto power-on, LVGL | 🔜 Migrating |
| `hello_sdcard` | SDMMC mount, card info, read/write benchmark, LVGL | 🔜 Migrating |
| `hello_spirit_level` | Roll/pitch from IMU accel, LVGL bubble level | 🔜 Migrating |
| `hello_touch` | Touch trace drawing, coordinate readout | 🔜 Migrating |
| `hello_usb_input` | USB HID keyboard + mouse, LVGL cursor | 🔜 Migrating |
| `hello_web_cam` | OV5640, WiFi, MJPEG HTTP streaming server | 🔜 Migrating |
| `weather_station` | Open-Meteo weather API, WiFi/LTE, LVGL weather display | 🔜 Migrating |

---

## Project Structure

```
cube32-app-template/
├── CMakeLists.txt                  ← rename project() here when cloning
├── sdkconfig.defaults              ← common config (LVGL, BT, WiFi, FreeRTOS…)
├── sdkconfig.defaults.esp32s3      ← ESP32-S3 specifics (PSRAM, CPU freq, flash)
├── main/
│   ├── CMakeLists.txt
│   ├── idf_component.yml           ← pulls cube32esp/cube32_bsp from registry
│   └── main.cpp                    ← your application entry point
├── partitions/
│   └── partitions.csv              ← 16 MB: dual-OTA (4 MB each) + 8 MB assets
└── .github/
    ├── copilot-instructions.md     ← global Copilot context (auto-loaded)
    └── skills/
        ├── cube32-idf-project/     ← "create new project" skill
        └── copy-bsp-example/       ← "copy example" skill
```

---

## Requirements

- ESP-IDF ≥ 5.4.0
- VS Code with [ESP-IDF extension](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-vscode-extension)
- VS Code with [GitHub Copilot](https://marketplace.visualstudio.com/items?itemName=GitHub.copilot) (for AI-assisted development)
