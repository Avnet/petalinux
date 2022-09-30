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
#  Please direct any questions to the ZUBoard community support forum:
#     http://avnet.me/zuboard-1cg-forum
#
#  Product information is available at:
#     http://avnet.me/zuboard-1cg
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

source boot_jtag.tcl

targets -set -nocase -filter {name =~ "*A53*#0"}
puts stderr "INFO: Loading image: ./avnet_jtag_tftp_dhcp.scr at 0x20000000"
dow -data  "./avnet_jtag_tftp_dhcp.scr" 0x20000000
after 2000
con
