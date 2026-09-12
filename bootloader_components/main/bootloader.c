#include "bootloader_utility.h"
#include "bootloader_init.h"
#include "esp_rom_sys.h"

void __attribute__((noreturn)) call_start_cpu0(void)
{
    esp_rom_printf("Initializing Bootloader \n");

    if (bootloader_init() != ESP_OK) {
        bootloader_reset();
    }

    esp_rom_printf("Bootloader Initialized\n");

    while (1) {
    }
}
