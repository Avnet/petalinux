# ----------------------------------------------------------------------------
#
#        ** **        **          **  ****      **  **********  ********** ®
#       **   **        **        **   ** **     **  **              **
#      **     **        **      **    **  **    **  **              **
#     **       **        **    **     **   **   **  *********       **
#    **         **        **  **      **    **  **  **              **
#   **           **        ****       **     ** **  **              **
#  **  .........  **        **        **      ****  **********      **
#     ...........
#                                     Reach Further™
#
# ----------------------------------------------------------------------------
#
#  This design is the property of Avnet.  Publication of this
#  design is not authorized without written consent from Avnet.
#
#  Please direct any questions to the UltraZed community support forum:
#     http://avnet.me/uzegforum and http://avnet.me/uzevforum
#
#  Product information is available at:
#     http://avnet.me/ultrazed-eg and http://avnet.me/ultrazed-ev
#
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2021 Avnet, Inc.
#                              All rights reserved.
#
# ----------------------------------------------------------------------------

# You can run 'xsdb boot_jtag.tcl' to execute"

connect
puts stderr "INFO: Configuring the FPGA..."
puts stderr "INFO: Downloading bitstream: ./pre-built/linux/implementation/download.bit to the target."
fpga "./pre-built/linux/implementation/download.bit"
after 2000
targets -set -nocase -filter {name =~ "*PSU*"}
mask_write 0xFFCA0038 0x1C0 0x1C0
targets -set -nocase -filter {name =~ "*MicroBlaze PMU*"}

catch {stop}; after 1000
puts stderr "INFO: Downloading ELF file: ./pre-built/linux/images/pmufw.elf to the target."
dow  "./pre-built/linux/images/pmufw.elf"
after 2000
con
targets -set -nocase -filter {name =~ "*APU*"}
mwr 0xffff0000 0x14000000
mask_write 0xFD1A0104 0x501 0x0
targets -set -nocase -filter {name =~ "*A53*#0"}

source ./project-spec/hw-description/psu_init.tcl
puts stderr "INFO: Downloading ELF file: ./pre-built/linux/images/zynqmp_fsbl.elf to the target."
dow  "./pre-built/linux/images/zynqmp_fsbl.elf"
after 2000
con
after 4000; stop; catch {stop}; psu_ps_pl_isolation_removal; psu_ps_pl_reset_config
targets -set -nocase -filter {name =~ "*A53*#0"}
puts stderr "INFO: Downloading ELF file: ./pre-built/linux/images/u-boot.elf to the target."
dow  "./pre-built/linux/images/u-boot.elf"
after 2000
targets -set -nocase -filter {name =~ "*A53*#0"}
puts stderr "INFO: Downloading ELF file: ./pre-built/linux/images/bl31.elf to the target."
dow  "./pre-built/linux/images/bl31.elf"
after 2000
