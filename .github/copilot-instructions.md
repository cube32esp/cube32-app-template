# CUBE32 App Template — Copilot Context

This project is an ESP-IDF application template for the **CUBE32-S3 Core** development board.
Every Copilot response in this workspace should assume this hardware and build system context.

---

## Hardware

| Item | Details |
|---|---|
| Board | CUBE32-S3 Core |
| MCU | ESP32-S3 (Xtensa dual-core LX7, 240 MHz) |
| Flash | 16 MB (SPI QIO) |
| PSRAM | 8 MB Octal SPI |
| Display | ST7789 TFT — 240×240 (CUBE_TFT_TOUCH_154) or 240×320 (CUBE_TFT_TOUCH_200) |
| Touch | CST816S / FT6336 capacitive (I²C) |
| IMU | LSM6DSOX (accel + gyro + MLC, I²C 0x6A) |
| Magnetometer | LIS3MDL (I²C 0x1C) |
| PMU | AXP2101 (battery management, I²C 0x34) |
| RTC | BM8563 (I²C 0x51) |
| Audio | ES7210 ADC (mic) + ES8311 DAC (speaker) |
| Camera | OV5640 (optional, 2 MP) |
| SD Card | SDMMC 1-bit / 4-bit |
| BLE / WiFi | Built-in ESP32-S3 radio |

---

## BSP Component

The board support package is `cube32esp/cube32_bsp`, pulled from the Espressif Component Registry via `main/idf_component.yml`.

**Key public headers:**
```cpp
#include "cube32.h"              // main entry point — includes all sub-headers
#include "utils/hw_manifest.h"  // hardware init result manifest
```

**Core API:**
```cpp
// Initialize all on-board hardware in the correct order
esp_err_t ret = cube32_init();

// Per-driver singletons — all follow this pattern
cube32::LvglDisplay  &lvgl = cube32::LvglDisplay::instance();
cube32::ST7789Display &lcd  = cube32::ST7789Display::instance();
cube32::IMU          &imu  = cube32::IMU::instance();
cube32::PMU          &pmu  = cube32::PMU::instance();
cube32::RTC          &rtc  = cube32::RTC::instance();
cube32::Touch        &tch  = cube32::Touch::instance();
cube32::Audio        &aud  = cube32::Audio::instance();
cube32::Camera       &cam  = cube32::Camera::instance();

// LVGL thread-safe rendering (always lock/unlock around LVGL calls)
lvgl.lock();
lv_obj_t *scr = lv_screen_active();
lvgl.unlock();

// Kconfig guards — wrap driver code so it compiles when a driver is disabled
#if CONFIG_CUBE32_IMU_ENABLED
    cube32::IMUData d;
    imu.getData(d);  // d.accel.x/y/z, d.gyro.x/y/z
#endif
```

---

## Build System

| Item | Value |
|---|---|
| ESP-IDF | ≥ 5.4.0 (v5.5.1 installed at `~/esp/v5.5.1/esp-idf/`) |
| Target | `esp32s3` |
| Language | C++17 |
| Build tool | `idf.py` / VS Code ESP-IDF extension |

Common build commands:
```sh
source ~/esp/v5.5.1/esp-idf/export.sh
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

---

## Project Structure

```
CMakeLists.txt                  ← project("<name>") — rename this when cloning
sdkconfig.defaults              ← common config (LVGL, BT, WiFi, FreeRTOS…)
sdkconfig.defaults.esp32s3      ← ESP32-S3 specifics (PSRAM, CPU freq, flash)
main/
  CMakeLists.txt
  idf_component.yml             ← pulls cube32esp/cube32_bsp from component registry
  main.cpp                      ← app entry point
partitions/
  partitions.csv                ← 16 MB dual-OTA + 8 MB assets (SPIFFS)
.github/
  copilot-instructions.md       ← this file (global Copilot context)
  skills/
    cube32-idf-project/SKILL.md    ← scaffold a new CUBE32 IDF project
    copy-bsp-example/SKILL.md      ← copy a BSP example into this project
```

---

## Available Copilot Skills

Use the following trigger phrases to activate skills:

| Trigger phrase | Skill | What it does |
|---|---|---|
| "create project", "new project", "scaffold" | `cube32-idf-project` | Creates a new CUBE32 IDF project from scratch |
| "copy example", "load example", "use hello_X", "apply hello_X" | `copy-bsp-example` | Copies a BSP example (main.cpp + sdkconfig.defaults) into this project |

---

## Conventions

- Never add `EXTRA_COMPONENT_DIRS` for the BSP — it is fetched via `idf_component.yml`
- `sdkconfig` is git-ignored; `sdkconfig.defaults` and `sdkconfig.defaults.esp32s3` are the source of truth
- After changing `sdkconfig.defaults`, run `idf.py fullclean && idf.py build`
- Each example in `cube32_bsp` has its own `sdkconfig.defaults` tuned to what that example needs — when copying an example, replace the template's `sdkconfig.defaults` with the example's version
