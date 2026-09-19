#include "bootloader_config.h"
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

	bootloader_state_t bs = {0};

	esp_rom_printf("Loading partition table ...\n");

	if (!bootloader_utility_load_partition_table(&bs)) {
		esp_rom_printf("Failed to load partition table\n");
		bootloader_reset();
	}

	esp_rom_printf("Partition table loaded successfully\n");

	while (1) {
	}
}
