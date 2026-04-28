#include "cube32.h"

extern "C" void app_main(void)
{
    // Initialise all CUBE32 hardware drivers in the correct order
    esp_err_t ret = cube32_init();
    if (ret != ESP_OK) {
        ESP_LOGE("app", "Board init failed: %s", esp_err_to_name(ret));
        return;
    }

    // Display is up — draw something with LVGL
    cube32::LvglDisplay &lvgl = cube32::LvglDisplay::instance();
    lvgl.lock();
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello CUBE32!");
    lv_obj_center(label);
    lvgl.unlock();

}