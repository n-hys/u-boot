#include <common.h>
#include <cpu.h>
#include <spl.h>
#include <asm/io.h>
#include <wdt.h>

DECLARE_GLOBAL_DATA_PTR;

void *board_fdt_blob_setup(int *err)
{
	*err = 0;

	return (void *)(ulong)gd->arch.firmware_fdt_addr;
}

int board_init(void)
{
	/* https://lore.kernel.org/u-boot/31587574-4cd1-02da-9761-0134ac82b94b@sholland.org/ */
	return cpu_probe_all();
}

uint32_t spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}

#define SUNXI_SID_BASE 0x03006200
int sunxi_get_sid(unsigned int *sid)
{
#if defined SUNXI_SID_BASE
	int i;

	for (i = 0; i< 4; i++)
		sid[i] = readl((ulong)SUNXI_SID_BASE + 4 * i);

	return 0;
#else
	return -ENODEV;
#endif
}

static bool get_unique_sid(unsigned int *sid)
{
	if (sunxi_get_sid(sid) != 0)
		return false;

	if (!sid[0])
		return false;

	/* Ensure the NIC specific bytes of the mac are not all 0 */
	if ((sid[3] & 0xffffff) == 0)
		sid[3] |= 0x800000;

	return true;
}

/*
 * Note this function gets called multiple times.
 * It must not make any changes to env variables which already exist.
 */
static void setup_environment(const void *fdt)
{
	char serial_string[17] = { 0 };
	unsigned int sid[4];
	uint8_t mac_addr[6];
	char ethaddr[16];
	int i;

	if (!get_unique_sid(sid))
		return;

	for (i = 0; i < 4; i++) {
		sprintf(ethaddr, "ethernet%d", i);
		if (!fdt_get_alias(fdt, ethaddr))
			continue;

		if (i == 0)
			strcpy(ethaddr, "ethaddr");
		else
			sprintf(ethaddr, "eth%daddr", i);

		if (env_get(ethaddr))
			continue;

		/* Non OUI / registered MAC address */
		mac_addr[0] = (i << 4) | 0x02;
		mac_addr[1] = (sid[0] >>  0) & 0xff;
		mac_addr[2] = (sid[3] >> 24) & 0xff;
		mac_addr[3] = (sid[3] >> 16) & 0xff;
		mac_addr[4] = (sid[3] >>  8) & 0xff;
		mac_addr[5] = (sid[3] >>  0) & 0xff;

		eth_env_set_enetaddr(ethaddr, mac_addr);
	}

	if (!env_get("serial#")) {
		snprintf(serial_string, sizeof(serial_string),
			"%08x%08x", sid[0], sid[3]);

		env_set("serial#", serial_string);
	}
}

int misc_init_r(void)
{
	setup_environment(gd->fdt_blob);

	return 0;
}

void board_preboot_os(void)
{
	if (CONFIG_IS_ENABLED(WDT))
		wdt_stop_all();
}
