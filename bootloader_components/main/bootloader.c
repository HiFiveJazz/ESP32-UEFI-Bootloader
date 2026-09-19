#include "bootloader_config.h"
#include "bootloader_utility.h"
#include "bootloader_init.h"
#include "esp_flash_partitions.h"
#include "bootloader_flash_priv.h"
#include "esp_rom_sys.h"
#include <string.h>

#define UEFI_PARTITION_TYPE            0x40
#define UEFI_PARTITION_SUBTYPE_PAYLOAD 0x00

static bool find_uefi_partition(esp_partition_pos_t *uefi);

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

	esp_partition_pos_t uefi = {0};

	if(!find_uefi_partition(&uefi)) {
		esp_rom_printf("UEFI Partition not found\n");
		bootloader_reset();
	}

	esp_rom_printf("Found UEFI Partition\n");
	esp_rom_printf("Offset: 0x%08lx\n", (unsigned long)uefi.offset);
	esp_rom_printf("Size  : 0x%08lx\n", (unsigned long)uefi.size);
	 

	while (1) {
	}
}


static bool find_uefi_partition(esp_partition_pos_t *uefi)
{
	const esp_partition_info_t *partitions;

	partitions = bootloader_mmap(
	ESP_PARTITION_TABLE_OFFSET,
	ESP_PARTITION_TABLE_MAX_LEN
	);

	if (partitions == NULL) {
	return false;
	}

	int num_partitions;

	if (esp_partition_table_verify(
		partitions,
		true,
		&num_partitions) != ESP_OK) {
	bootloader_munmap(partitions);
	return false;
	}

	for (int i = 0; i < num_partitions; i++) {
	const esp_partition_info_t *part = &partitions[i];

	if (part->type == UEFI_PARTITION_TYPE &&
	    part->subtype == UEFI_PARTITION_SUBTYPE_PAYLOAD &&
	    strncmp((const char *)part->label, 
		     "uefi", 
		     sizeof(part->label)) == 0) {
		
		*uefi = part->pos;

		bootloader_munmap(partitions);
		return true;
	}
	}

	bootloader_munmap(partitions);
	return false;
}
