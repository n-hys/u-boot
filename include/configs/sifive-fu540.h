/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

#define CONFIG_SYS_SDRAM_BASE		0x80000000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + SZ_2M)

#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + SZ_2M)

#define CONFIG_SYS_MALLOC_LEN		SZ_8M

#define CONFIG_SYS_BOOTM_LEN		SZ_64M

#define CONFIG_STANDALONE_LOAD_ADDR	0x80200000

/* Environment options */

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(DHCP, dhcp, na)

#include <config_distro_bootcmd.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	"fdt_high=0xffffffffffffffff\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"enet_boot=setenv bootargs -D /soc/ethernet@10090000 && dhcp 0x80400000 && tftpboot 0x80500000 /root_riscv64/platform/SUNW,sifive/hifive-unleashed-a00.dtb && bootm 0x80400000 - 0x80500000\0" \
	"mmc_boot=setenv bootargs -D /soc/spi@10050000 && fatload mmc 0:3 0x80400000 inetboot && fatload mmc 0:3 0x80500000 hifive-unleashed-a00.dtb && bootm 0x80400000 - 0x80500000\0" \
	"bootcmd=run enet_boot\0"

#define CONFIG_PREBOOT \
	"setenv fdt_addr ${fdtcontroladdr};" \
	"fdt addr ${fdtcontroladdr};"

#endif /* __CONFIG_H */
