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

# You can run 'xsdb boot_jtag_INITRD_MINIMAL.tcl' to execute"

source boot_jtag.tcl

targets -set -nocase -filter {name =~ "arm*#0"}
puts stderr "INFO: Loading image: ./pre-built/linux/images/image_INITRD_MINIMAL.ub at 0x04000000"
dow -data  "./pre-built/linux/images/image_INITRD_MINIMAL.ub" 0x04000000
after 2000
targets -set -nocase -filter {name =~ "arm*#0"}
puts stderr "INFO: Loading image: ./pre-built/linux/images/avnet-boot/avnet_jtag.scr at 0x3000000"
dow -data  "./pre-built/linux/images/avnet-boot/avnet_jtag.scr" 0x3000000
after 2000
con
