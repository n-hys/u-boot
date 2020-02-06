/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for Amlogic Meson 64bits SoCs
 * (C) Copyright 2016 Beniamino Galvani <b.galvani@gmail.com>
 */

#ifndef __MESON64_CONFIG_H
#define __MESON64_CONFIG_H

/* Generic Interrupt Controller Definitions */
#if (defined(CONFIG_MESON_AXG) || defined(CONFIG_MESON_G12A))
#define GICD_BASE			0xffc01000
#define GICC_BASE			0xffc02000
#else /* MESON GXL and GXBB */
#define GICD_BASE			0xc4301000
#define GICC_BASE			0xc4302000
#endif

/* For splashscreen */
#ifdef CONFIG_DM_VIDEO
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_BMP_16BPP
#define CONFIG_BMP_24BPP
#define CONFIG_BMP_32BPP
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define STDOUT_CFG "vidconsole,serial"
#else
#define STDOUT_CFG "serial"
#endif

#ifdef CONFIG_USB_KEYBOARD
#define STDIN_CFG "usbkbd,serial"
#else
#define STDIN_CFG "serial"
#endif

#define CONFIG_CPU_ARMV8
#define CONFIG_REMAKE_ELF
#define CONFIG_SYS_MAXARGS		32
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		1024

#define CONFIG_SYS_SDRAM_BASE		0
#define CONFIG_SYS_INIT_SP_ADDR		0x20000000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_BOOTM_LEN		(64 << 20) /* 64 MiB */

/* ROM USB boot support, auto-execute boot.scr at scriptaddr */
#define BOOTENV_DEV_ROMUSB(devtypeu, devtypel, instance) \
	"bootcmd_romusb=" \
		"if test \"${boot_source}\" = \"usb\" && " \
				"test -n \"${scriptaddr}\"; then " \
			"echo '(ROM USB boot)'; " \
			"source ${scriptaddr}; " \
		"fi\0"

#define BOOTENV_DEV_NAME_ROMUSB(devtypeu, devtypel, instance)	\
		"romusb "

#ifdef CONFIG_CMD_USB
#define BOOT_TARGET_DEVICES_USB(func) func(USB, usb, 0)
#else
#define BOOT_TARGET_DEVICES_USB(func)
#endif

#ifndef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
	func(ROMUSB, romusb, na)  \
	func(MMC, mmc, 0) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 2) \
	BOOT_TARGET_DEVICES_USB(func) \
	func(PXE, pxe, na) \
	func(DHCP, dhcp, na)
#endif

#ifndef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"stdin=" STDIN_CFG "\0" \
	"stdout=" STDOUT_CFG "\0" \
	"stderr=" STDOUT_CFG "\0" \
	"enet_boot=setenv bootargs -D /soc/ethernet@c9410000 && dhcp 0x11000000 && tftpboot 0x11100000 /root_aarch64/platform/SUNW,meson/meson-gxbb-odroidc2.dtb && bootm 0x11000000 - 0x11100000\0" \
	"mmc_boot=setenv bootargs -D /soc/sd@d0072000/blkdev@0 && fatload mmc 0 0x11000000 inetboot && fatload mmc 0 0x11100000 meson-gxbb-odroidc2.dtb && bootm 0x11000000 - 0x11100000\0" \
	"bootcmd=run enet_boot\0"
#endif

#include <config_distro_bootcmd.h>

#endif /* __MESON64_CONFIG_H */
