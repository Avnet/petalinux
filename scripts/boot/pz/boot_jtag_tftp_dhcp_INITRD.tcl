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
#  Please direct any questions to the PicoZed community support forum:
#     http://avnet.me/picozed_forum
#
#  Product information is available at:
#     http://avnet.me/picozed
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

# You can run 'xsdb boot_jtag_tftp_dhcp_INITRD.tcl' to execute"

connect
puts stderr "INFO: Configuring the FPGA..."
puts stderr "INFO: Downloading bitstream: ./pre-built/linux/implementation/download.bit to the target."
fpga "./pre-built/linux/implementation/download.bit"
after 2000
targets -set -nocase -filter {name =~ "arm*#0"}

source ./project-spec/hw-description/ps7_init.tcl; ps7_post_config
catch {stop}
set mctrlval [string trim [lindex [split [mrd 0xF8007080] :] 1]]
puts "mctrlval=$mctrlval"
puts stderr "INFO: Downloading ELF file: ./pre-built/linux/images/zynq_fsbl.elf to the target."
dow  "./pre-built/linux/images/zynq_fsbl.elf"
after 2000
con
after 3000; stop
targets -set -nocase -filter {name =~ "arm*#0"}
puts stderr "INFO: Downloading ELF file: ./pre-built/linux/images/u-boot.elf to the target."
dow  "./pre-built/linux/images/u-boot.elf"
after 2000
con; after 1000; stop
targets -set -nocase -filter {name =~ "arm*#0"}
puts stderr "INFO: Loading image: ./pre-built/linux/images/system.dtb at 0x00100000"
dow -data  "./pre-built/linux/images/system.dtb" 0x00100000
after 2000
targets -set -nocase -filter {name =~ "arm*#0"}
puts stderr "INFO: Loading image: ./avnet_jtag_tftp_dhcp.scr at 0x3000000"
dow -data  "./avnet_jtag_tftp_dhcp.scr" 0x3000000
after 2000
con
exit
