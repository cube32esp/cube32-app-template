---
name: cube32-idf-project
description: 'Create a new ESP-IDF project using the cube32esp/cube32_bsp component for the CUBE32-S3 board. Use when asked to create, scaffold, or initialize an ESP32 IDF project with the cube32_bsp, CUBE32 BSP, or cube32esp component. Covers project structure, CMakeLists, idf_component.yml, sdkconfig defaults, partition table, and target configuration for esp32s3.'
argument-hint: 'project name (e.g. my_app)'
---

# Create an ESP-IDF Project with cube32esp/cube32_bsp

## When to Use

- User asks to create/scaffold/initialize a new ESP32-S3 IDF project
- User references the CUBE32 board, cube32_bsp, or cube32esp component

> **To load a BSP example** (hello_world, hello_display, etc.) into an existing project,
> use the **`copy-bsp-example`** skill instead: say "copy hello_X" or "load hello_X".

---

## Build Requirements

| Requirement | Value |
|-------------|-------|
| ESP-IDF     | ≥ 5.4.0 |
| Target      | esp32s3 |
| Component   | `cube32esp/cube32_bsp` via Espressif Component Registry |

---

## Project Structure

Create all files under `<workspace>/<project_name>/`:

```
<project_name>/
├── CMakeLists.txt
├── sdkconfig.defaults
├── sdkconfig.defaults.esp32s3
├── .vscode/
│   └── settings.json
├── main/
│   ├── CMakeLists.txt
│   ├── idf_component.yml
│   └── main.cpp
└── partitions/
    └── partitions.csv
```

---

## Step-by-Step Procedure

### 1. Create Directories

```
<project_name>/
<project_name>/main/
<project_name>/partitions/
<project_name>/.vscode/
```

### 2. Root `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)

# Pull in ESP-IDF cmake infrastructure
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(<project_name>)
```

> Note: Do NOT add `EXTRA_COMPONENT_DIRS` for the BSP — the component is fetched
> from the Espressif Component Registry via `idf_component.yml`.

### 3. `main/CMakeLists.txt`

```cmake
idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
)
```

### 4. `main/idf_component.yml`

```yaml
dependencies:
  cube32esp/cube32_bsp: "*"
  idf:
    version: ">=5.4.0"
```

This pulls the latest `cube32esp/cube32_bsp` from the Espressif Component Registry.
To pin a version, replace `"*"` with a specific version (e.g., `">=0.0.3"`).

### 5. `main/main.cpp` — Minimal Application Template

```cpp
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "cube32.h"

static const char *TAG = "<project_name>";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting...");

    esp_err_t ret = cube32_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Board init failed: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "CUBE32 initialized successfully");

    while (true) {
        ESP_LOGI(TAG, "Heap: %" PRIu32, esp_get_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
```

> To load a full BSP example (hello_world, hello_display, etc.) instead of the minimal
> template, use the **`copy-bsp-example`** skill after the project is created:
> say "copy hello_world" or "copy hello_display".

### 6. `sdkconfig.defaults`

Use the template's `sdkconfig.defaults` as-is. It covers:
- `CONFIG_COMPILER_CXX_EXCEPTIONS=y` and `CONFIG_COMPILER_CXX_RTTI=y`
- `CONFIG_PARTITION_TABLE_CUSTOM=y` pointing to `partitions/partitions.csv`
- LVGL 9.x with Montserrat fonts enabled
- NimBLE stack for BLE OTA
- `CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192`

If you copy a BSP example, the `copy-bsp-example` skill will replace this file
with the example's own `sdkconfig.defaults`.

### 7. `sdkconfig.defaults.esp32s3`

Use the template's `sdkconfig.defaults.esp32s3` as-is. It configures:
- `CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y`
- `CONFIG_SPIRAM=y` / `CONFIG_SPIRAM_MODE_OCT=y` (8 MB Octal PSRAM)
- `CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_240=y`

### 8. `partitions/partitions.csv`

Use the template's standard CUBE32 16 MB layout:

| Name     | Type | SubType       | Size    | Purpose                        |
|----------|------|---------------|---------|-------------------------------|
| nvs      | data | nvs           | 16 KB   | WiFi credentials, config       |
| otadata  | data | ota           | 8 KB    | OTA slot tracking              |
| phy_init | data | phy           | 4 KB    | RF calibration                 |
| ota_0    | app  | ota_0         | ~4 MB   | Application image (slot A)     |
| ota_1    | app  | ota_1         | ~4 MB   | Application image (slot B)     |
| assets   | data | spiffs        | 8 MB    | Fonts, audio, ML models        |

### 9. `.vscode/settings.json`

```json
{
    "idf.adapterTargetName": "esp32s3",
    "idf.customExtraVars": {
        "IDF_TARGET": "esp32s3"
    }
}
```

### 10. Set Target and Build

Using the VS Code ESP-IDF extension:
1. Open the project folder in VS Code
2. Use **ESP-IDF: Set Espressif Device Target** → select `esp32s3`
3. Use **ESP-IDF: Build your project**

Using the terminal (ESP-IDF v5.5.1 is installed at `~/esp/v5.5.1/esp-idf/`):

```sh
source ~/esp/v5.5.1/esp-idf/export.sh
cd <project_name>
idf.py set-target esp32s3
idf.py build
idf.py flash monitor
```

---

## BSP API Quick Reference

```cpp
#include "cube32.h"
#include "utils/hw_manifest.h"

// Initialize ALL on-board hardware in correct order
esp_err_t ret = cube32_init();

// Per-driver singletons (all follow same pattern)
cube32::LvglDisplay &lvgl = cube32::LvglDisplay::instance();
cube32::IMU         &imu  = cube32::IMU::instance();
cube32::PMU         &pmu  = cube32::PMU::instance();
cube32::RTC         &rtc  = cube32::RTC::instance();

// LVGL thread-safe rendering
lvgl.lock();
lv_obj_t *label = lv_label_create(lv_screen_active());
lv_label_set_text(label, "Hello CUBE32!");
lv_obj_center(label);
lvgl.unlock();

// IMU data
cube32::IMUData d;
if (imu.getData(d) == CUBE32_OK) { /* d.accel, d.gyro */ }

// PMU / battery
pmu.getBatteryPercent();   // 0–100
pmu.getBatteryVoltage();   // mV
pmu.isCharging();          // bool

// Hardware manifest — read all driver init results
const cube32_hw_manifest_t *m = cube32_hw_manifest();
// m->init_records[i].code == CUBE32_INIT_OK / FAIL / SKIPPED ...
```

### Kconfig Guard Macros

Wrap driver code in guards so it compiles safely when a driver is disabled:

```cpp
#if CONFIG_CUBE32_IMU_ENABLED
    // IMU code
#endif
#if CONFIG_CUBE32_LVGL_ENABLED
    // LVGL code
#endif
#if CONFIG_CUBE32_PMU_ENABLED
    // PMU / battery code
#endif
```

Full list: `CUBE32_DISPLAY_ENABLED`, `CUBE32_TOUCH_ENABLED`, `CUBE32_LVGL_ENABLED`,
`CUBE32_RTC_ENABLED`, `CUBE32_IMU_ENABLED`, `CUBE32_MAG_ENABLED`,
`CUBE32_AUDIO_ENABLED`, `CUBE32_CAMERA_ENABLED`, `CUBE32_SDCARD_ENABLED`,
`CUBE32_MODEM_ENABLED`, `CUBE32_USB_INPUT_ENABLED`, `CUBE32_ADC_BUTTON_ENABLED`,
`CUBE32_IO_EXPANDER_ENABLED`, `CUBE32_BLE_OTA_ENABLED`, `CUBE32_SERVO_ENABLED`.

---

## Basing a Project on the hello_world Example

When the user asks to base the project on the hello_world example, download
all source files directly from the upstream repository using `curl -sL`:

| File | URL |
|------|-----|
| `main/main.cpp` | `https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/hello_world/main/main.cpp` |
| `main/CMakeLists.txt` | `https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/hello_world/main/CMakeLists.txt` |
| `sdkconfig.defaults` | `https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/hello_world/sdkconfig.defaults` |
| `sdkconfig.defaults.esp32s3` | `https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/hello_world/sdkconfig.defaults.esp32s3` |
| `partitions/partitions.csv` | `https://raw.githubusercontent.com/cube32esp/cube32_bsp/main/examples/hello_world/partitions/partitions.csv` |

The project-level `CMakeLists.txt`, `main/idf_component.yml`, and
`.vscode/settings.json` must still be created as described above — they are
NOT part of the upstream example (the example uses a local BSP path override
which is not appropriate for standalone projects).

---

## Important Notes

- The upstream example's `CMakeLists.txt` contains `set(EXTRA_COMPONENT_DIRS ...)` pointing to a local BSP path. **Do not copy this line** for standalone projects; use `idf_component.yml` instead.
- ESP-IDF ≥ 5.4.0 is required. On this machine, v5.5.1 is at `~/esp/v5.5.1/esp-idf/`.
- The hello_world app requires LVGL Montserrat fonts 14, 20, and 24 — these are enabled by `sdkconfig.defaults`.
- For speech recognition / AEC, add `espressif/esp-sr: "~2.1.5"` to `idf_component.yml` (adds ~8 MB to binary).
