#!/bin/sh

# Kil X11 session
killall Xorg

sleep 4

export DISPLAY=:0.0
/usr/bin/Xorg -depth 16&

sleep 2

#Run Tricube Application

tricube
