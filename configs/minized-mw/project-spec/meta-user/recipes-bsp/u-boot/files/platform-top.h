
#include <configs/platform-auto.h>

#undef CONFIG_ENV_IS_IN_SPI_FLASH
#undef CONFIG_ENV_OFFSET
#undef CONFIG_ENV_SIZE
#undef CONFIG_ENV_SECT_SIZE

/* Place uboot evnironment in eMMC */
#define CONFIG_ENV_SIZE 0x80000
#define CONFIG_ENV_IS_IN_FAT
#define FAT_ENV_DEVICE_AND_PART "1:1"
#define FAT_ENV_FILE    "uboot.env"
#define FAT_ENV_INTERFACE       "mmc"
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_ENV_OVERWRITE

#undef CONFIG_EXTRA_ENV_SETTINGS

/* Extra U-Boot Env settings */
#define CONFIG_EXTRA_ENV_SETTINGS \
	SERIAL_MULTI \ 
	CONSOLE_ARG \ 
	PSSERIAL0 \ 
	"importbootenv=echo \"Importing environment from SD ...\"; " \ 
		"env import -t ${loadbootenv_addr} $filesize\0" \ 
	"loadbootenv=load mmc $sdbootdev:$partid ${loadbootenv_addr} ${bootenv}\0" \ 
	"sd_uEnvtxt_existence_test=test -e mmc $sdbootdev:$partid /uEnv.txt\0" \ 
	"uenvboot=" \ 
	"if run sd_uEnvtxt_existence_test; then" \ 
		"run loadbootenv" \ 
		"echo Loaded environment from ${bootenv};" \ 
		"run importbootenv; \0" \ 
	"sdboot=echo boot Petalinux with MATLAB and Simulink support; fatload mmc 1 ${fpga_load_address} ${bitstream_image} && fpga loadb 0 ${fpga_load_address} $filesize && fatload mmc 1 ${kernel_load_address} ${kernel_image} && fatload mmc 1 ${ramdisk_load_address} ${ramdisk_image} && fatload mmc 1 ${devicetree_load_address} ${devicetree_image} && bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \ 
	"autoload=no\0" \ 
	"clobstart=0x10000000\0" \ 
	"kernel_load_address=0x10000000\0" \
	"ramdisk_load_address=0x15000000\0" \ 
	"devicetree_load_address=0xF000000\0" \
	"fpga_load_address=0xF100000\0" \ 
	"fpga_size=0x0xF111B\0" \
	"bitstream_image=design_1_wrapper.bit\0" \
	"loadaddr=0x10000000\0" \ 
	"bootsize=0x200000\0" \ 
	"bootstart=0x0\0" \ 
	"boot_image=BOOT.BIN\0" \
	"devicetree_image=system.dtb\0" \
	"ramdisk_image=uramdisk.image.gz\0" \
	"ramdisk_image_fb=fb.uramdisk.image.gz\0" \
	"kernel_image=uImage\0" \
	"kernel_image_fb=image.ub\0" \
	"sd_update_boot=echo Updating boot from SD; mmcinfo && fatload mmc 0:1 ${clobstart} ${boot_image} && run install_boot\0" \ 
	"install_boot=sf probe 0 && sf erase ${bootstart} ${bootsize} && " \ 
	"sf write ${clobstart} ${bootstart} ${filesize}\0" \ 
	"bootenvsize=0x20000\0" \ 
	"bootenvstart=0x200000\0" \ 
	"eraseenv=sf probe 0 && sf erase ${bootenvstart} ${bootenvsize}\0" \ 
	"jffs2_img=rootfs.jffs2\0" \ 
	"sd_update_jffs2=echo Updating jffs2 from SD; mmcinfo && fatload mmc 0:1 ${clobstart} ${jffs2_img} && run install_jffs2\0" \ 
	"install_jffs2=sf probe 0 && sf erase ${jffs2start} ${jffs2size} && " \ 
	"sf write ${clobstart} ${jffs2start} ${filesize}\0" \ 
	"install_kernel=mmcinfo && fatwrite mmc 1 ${clobstart} ${kernel_image} ${filesize}\0" \ 
	"cp_kernel2ram=mmcinfo && fatload mmc 1 ${kernel_load_address} ${kernel_image}\0" \ 
	"sd_update_dtb=echo Updating dtb from SD; mmcinfo && fatload mmc 0:1 ${clobstart} ${devicetree_image} && run install_dtb\0" \ 
	"fault=echo ${img} image size is greater than allocated place - partition ${img} is NOT UPDATED\0" \ 
	"test_crc=if imi ${clobstart}; then run test_img; else echo ${img} Bad CRC - ${img} is NOT UPDATED; fi\0" \ 
	"test_img=setenv var \"if test ${filesize} -gt ${psize}\\; then run fault\\; else run ${installcmd}\\; fi\"; run var; setenv var\0" \ 
	"kernelsize=0xcf0000\0" \ 
	"kernelstart=0x220000\0" \ 
	"default_bootcmd=run sdboot\0" \ 
	"boot_qspi=echo Booting backup kernel from QSPI..; sf probe 0 50000000; sf read ${kernel_load_address} ${kernelstart} ${kernelsize}; bootm ${kernel_load_address}\0" \ 
        "sdboot_mmc0=echo boot Petalinux with MathWorks support; fatload mmc 0 ${fpga_load_address} ${bitstream_image} && fpga loadb 0 ${fpga_load_address} $filesize && fatload mmc 0 ${kernel_load_address} ${kernel_image} && fatload mmc 0 ${ramdisk_load_address} ${ramdisk_image} && fatload mmc 0 ${devicetree_load_address} ${devicetree_image} && bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"sdboot_fb=echo boot Petalinux backup from eMMC (image.ub); fatload mmc 1 ${fpga_load_address} ${bitstream_image} && fpga loadb 0 ${fpga_load_address} $filesize && fatload mmc 1 ${kernel_load_address} ${kernel_image_fb} && bootm ${kernel_load_address}\0" \
""

