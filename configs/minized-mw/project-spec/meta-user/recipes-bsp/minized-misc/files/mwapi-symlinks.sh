#!/bin/sh

# Create symbolic links for MathWorks APIs and U-Boot utils
# SSH commands (non-login) only have PATH=/usr/bin:/bin

cd /usr/bin

# MathWorks API scripts
ln -s /usr/sbin/_mw_setbootfile _mw_setbootfile
ln -s /usr/sbin/mw_setboot mw_setboot
ln -s /usr/sbin/fw_getbitstream		fw_getbitstream
ln -s /usr/sbin/fw_getdevicetree	fw_getdevicetree
ln -s /usr/sbin/fw_getrdname		fw_getrdname
ln -s /usr/sbin/fw_setbitstream		fw_setbitstream
ln -s /usr/sbin/fw_setdevicetree	fw_setdevicetree
ln -s /usr/sbin/fw_setrdname		fw_setrdname

# Linux U-Boot Utilities
ln -s /sbin/fw_printenv	fw_printenv
ln -s /sbin/fw_setenv	fw_setenv

# Linux reboot command
ln -s /sbin/reboot	reboot
