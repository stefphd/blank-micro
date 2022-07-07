#!/bin/sh

#############################################################################
# Shell script for compiling and uploading files to arduino-like board
# Linux version
# Created by SL, 02-20-2022
#############################################################################

NAME="main.cpp"
SRC="./src"

BOARD="teensy:avr:teensy41"
BOARD_OPTIONS="speed=600,usb=mtpserial,opt=o3std,keys=en-us"

TEENSY_TOOLS="./tools/tools-linux"
ARDUINO_FOLDER="/usr/share/arduino"

HARDWARE="-hardware ./tools"
TOOLS="-tools $TEENSY_TOOLS -tools $ARDUINO_FOLDER/tools-builder"
BUILD_PATH=".build"
CACHE_PATH=".cache"
FQBN=-fqbn=$BOARD:$BOARD_OPTIONS
LIBRARIES="-libraries ./ -libraries ./include/ -libraries ./lib/"
FLAGS="-verbose"

BUILDER="arduino-builder" 
POSTCOMPILER="$TEENSY_TOOLS/teensy_post_compile"
REBOOT="$TEENSY_TOOLS/teensy_reboot"

TARGET_HEX=$BUILD_PATH/$NAME.hex

###################################################
# CREATING DIRECTORIES
###################################################

echo Creating necessary directories if not existing...
mkdir -p $BUILD_PATH
mkdir -p $CACHE_PATH
echo

###################################################
# COMPILING CODE
###################################################

echo Compiling $NAME for $BOARD with options $BOARD_OPTIONS
echo Build path is $BUILD_PATH
echo Cache path is $CACHE_PATH
echo Library paths are $LIBRARIES
$BUILDER -dump-prefs -build-path $BUILD_PATH -build-cache $CACHE_PATH $HARDWARE $TOOLS $LIBRARIES $FQBN $SRC/$NAME
$BUILDER -compile $FLAGS -build-path $BUILD_PATH -build-cache $CACHE_PATH $HARDWARE $TOOLS $LIBRARIES $FQBN $SRC/$NAME
echo Target hex file $TARGET_HEX generated
echo

###################################################
# UPLOADING CODE
###################################################

echo Uploading target hex $TARGET_HEX for board $BOARD
$POSTCOMPILER -file=$NAME -path=$BUILD_PATH -tools=$TEENSY_TOOLS -board $BOARD
$REBOOT 
echo