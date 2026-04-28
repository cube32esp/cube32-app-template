---
name: copy-bsp-example
description: 'Copy a cube32_bsp example into this project. Replaces main/main.cpp and updates sdkconfig.defaults with the example version. Use when asked to copy, load, use, or apply any cube32_bsp example by name (e.g. "copy hello_display", "load the LVGL demo", "use hello_world").'
argument-hint: 'example name (e.g. hello_display)'
---

# Copy a cube32_bsp Example into This Project

## When to Use

Trigger phrases that should activate this skill:
- "copy example", "load example", "load demo"
- "use hello_X", "apply hello_X", "copy hello_X"
- "copy the display demo", "load the audio example", etc.

---

## Example Catalog

All examples live at:
```
https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/<example_name>/
```

| Example | Key Features | Status |
|---|---|---|
| `hello_world` | 3D wireframe cube, IMU gyro control, touch swipe, LVGL v9, hardware status screen | ✅ Available |
| `hello_display` | Raw ST7789 SPI drawing — color bars, gradient, bouncing rect. No LVGL required | ✅ Available |
| `hello_activity` | Accel-based software activity recognition (stationary / walk / jog / bike / drive) | 🔜 Migrating |
| `hello_aec_test` | Mic + speaker, Acoustic Echo Cancellation (AEC), WAV recording | 🔜 Migrating |
| `hello_audio_player` | WAV/MP3 playback from SD card, LVGL UI, ADC button volume control | 🔜 Migrating |
| `hello_audio_recorder` | Mic recording to WAV on SD card, LVGL playback UI | 🔜 Migrating |
| `hello_ble_ota` | BLE over-the-air firmware update service | 🔜 Migrating |
| `hello_camera` | OV5640 capture + live preview on ST7789, RGB565 | 🔜 Migrating |
| `hello_compass` | LSM6DSO + LIS3MDL, tilt-corrected compass rose, calibration, LVGL | 🔜 Migrating |
| `hello_imu_mag` | IMU + magnetometer live data, LVGL dark-theme UI | 🔜 Migrating |
| `hello_lvgl` | LVGL 9.x widgets — labels, buttons, styles, animations, touch swipe | 🔜 Migrating |
| `hello_mlc_activity` | LSM6DSOX Machine Learning Core hardware activity classification | 🔜 Migrating |
| `hello_modem_console` | A7670 LTE modem, AT commands, signal/operator/IP display, LVGL | 🔜 Migrating |
| `hello_mqtt` | MQTT pub/sub, AWS IoT, WiFi + LTE fallback, SSL/TLS, LVGL | 🔜 Migrating |
| `hello_pwm_servo` | PWM servo with smooth motion, LVGL angle control | 🔜 Migrating |
| `hello_rtc_ntp` | BM8563 RTC, NTP time sync over WiFi/LTE, timezone, LVGL | 🔜 Migrating |
| `hello_rtc_timer` | RTC alarm, AXP2101 auto power-on from timer, LVGL | 🔜 Migrating |
| `hello_sdcard` | SDMMC mount, card info, read/write speed benchmark, LVGL | 🔜 Migrating |
| `hello_spirit_level` | IMU accel roll/pitch, LVGL circular bubble level | 🔜 Migrating |
| `hello_touch` | CST816S / FT6336 touch trace drawing, coordinate log | 🔜 Migrating |
| `hello_usb_input` | USB HID keyboard + mouse, LVGL cursor control | 🔜 Migrating |
| `hello_web_cam` | OV5640, WiFi, MJPEG HTTP streaming server | 🔜 Migrating |
| `weather_station` | Open-Meteo weather API, WiFi/LTE, LVGL weather UI | 🔜 Migrating |

---

## Files Fetched Per Example

For each example the following files exist in the `cube32_bsp` repo:

| File in repo | Destination in this project |
|---|---|
| `examples/<name>/main/main.cpp` | `main/main.cpp` (overwrite) |
| `examples/<name>/sdkconfig.defaults` | `sdkconfig.defaults` (diff + replace) |
| `examples/<name>/sdkconfig.defaults.esp32s3` | `sdkconfig.defaults.esp32s3` (diff + replace) |
| `examples/<name>/partitions/partitions.csv` | `partitions/partitions.csv` (diff + replace if different) |
| `examples/<name>/README.md` | Read only — extract menuconfig checklist |

---

## Step-by-Step Procedure

### Step 1 — Identify the example

Extract the example name from the user's request (e.g. "copy hello_display" → `hello_display`).
If ambiguous, list the catalog table above and ask the user to choose.

### Step 2 — Check availability

Attempt to fetch:
```
https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/<name>/main/main.cpp
```

- **200 OK**: proceed.
- **404 / error**: the example has not been migrated yet. Tell the user:
  > "`<name>` is not yet available in the cube32_bsp repo. Check back later, or choose an available example: `hello_world`, `hello_display`."
  Stop here.

### Step 3 — Fetch and write `main/main.cpp`

Fetch:
```
https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/<name>/main/main.cpp
```
Write the full content to `main/main.cpp`, replacing the current file entirely.

> Some examples have additional source files in `main/`. If the README mentions them,
> fetch and create those files as well (same URL pattern: `examples/<name>/main/<file>`).

### Step 4 — Diff and replace `sdkconfig.defaults`

1. Fetch:
   ```
   https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/<name>/sdkconfig.defaults
   ```
2. Compare the fetched content against the current `sdkconfig.defaults`.
   Show a **brief diff summary** — list only `CONFIG_` keys that are:
   - **Added** in the example (present in example, absent in current)
   - **Removed** (present in current, absent in example)
   - **Changed** (same key, different value)
3. Write the fetched content to `sdkconfig.defaults`, replacing the current file entirely.
4. Remind the user: `idf.py fullclean && idf.py build` is required after this change.

### Step 5 — Diff and replace `sdkconfig.defaults.esp32s3`

Same procedure as Step 4 for `sdkconfig.defaults.esp32s3`.
This file usually does not change between examples — note "no changes" if identical.

### Step 6 — Check `partitions/partitions.csv`

Fetch:
```
https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/<name>/partitions/partitions.csv
```

If the content differs from the current `partitions/partitions.csv`, show the diff and replace it.
If identical, note "partition table unchanged" and skip.

### Step 7 — Extract post-copy checklist from README

Fetch:
```
https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/<name>/README.md
```

Extract and print the **"Required KConfig Settings"** or **"menuconfig"** section verbatim.
This tells the user which `idf.py menuconfig` settings must be enabled for the example to work.

If the README does not have a dedicated KConfig section, scan for any lines mentioning
`CONFIG_`, `menuconfig`, or `idf.py menuconfig` and summarise them.

### Step 8 — Print completion summary

Print a summary block like:

```
✅ Example '<name>' copied successfully.

Files updated:
  main/main.cpp          — replaced
  sdkconfig.defaults     — replaced (<N> CONFIG_ entries changed)
  sdkconfig.defaults.esp32s3 — unchanged / replaced
  partitions/partitions.csv  — unchanged / replaced

Next steps:
  1. Run: idf.py fullclean && idf.py build
  2. Review menuconfig requirements below and apply any missing settings.

--- Menuconfig Checklist ---
<paste extracted section from README here>
```

---

## China-Friendly Note

The skill's AI fetch (`fetch_webpage`) runs on Microsoft's Azure servers and is not
affected by the GFW. The `raw.githubusercontent.com` URLs work fine for the AI.

If **you** need to download files manually in a terminal inside China, use the
jsDelivr CDN mirror instead:
```
https://cdn.jsdelivr.net/gh/cube32esp/cube32_bsp@main/examples/<name>/<file>
```
Example:
```sh
curl -o main/main.cpp "https://cdn.jsdelivr.net/gh/cube32esp/cube32_bsp@main/examples/hello_display/main/main.cpp"
```

---

## Important Rules

- **Always replace** `sdkconfig.defaults` with the example's version — do not merge.
  Each example ships a self-contained `sdkconfig.defaults` tuned for its feature set.
- **Never add** `EXTRA_COMPONENT_DIRS` for the BSP — the component is fetched via `idf_component.yml`.
- After any `sdkconfig.defaults` change, `idf.py fullclean` is mandatory before building.
- If the example requires extra source files beyond `main.cpp`, create them too.
