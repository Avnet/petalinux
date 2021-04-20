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
#!/bin/bash

# This script will boot u-boot using JTAG and then boot the OS image
# using TFTP over the user's LAN

# Stop the script whenever we had an error (non-zero returning function)
set -e

PROJECT_FOLDER=$(basename $PWD)

IMAGE=MINIMAL

# Set the SERVER_IP to the IP address of this PC
SERVER_IP=$(hostname -I)
FIT_IMAGE=image_INITRD_$IMAGE.ub

if [ ! -f $FIT_IMAGE ]
  then
    echo "ERROR: Can't find $FIT_IMAGE file inside folder '$PROJECT_FOLDER'"
    exit 1
  else
    echo "Copy OS image $FIT_IMAGE to /tftpboot as image.ub"
    cp $FIT_IMAGE /tftpboot/image.ub
fi

sh create_dhcp_boot_scr.sh $SERVER_IP

xsdb boot_jtag_tftp_dhcp_INITRD.tcl

