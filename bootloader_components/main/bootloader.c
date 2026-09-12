#include "esp_rom_sys.h"

void __attribute__((noreturn)) call_start_cpu0(void)
{
    esp_rom_printf("Hello from Jazz BootLoader\n");

    while (1) {
    }
}
