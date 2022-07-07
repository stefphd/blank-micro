:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::Shell script for compiling and uploading files to arduino-like board
::Windows version
::Created by SL, 02-20-2022
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo off

set NAME=main.cpp
set SRC=.\src
set BOARD=teensy:avr:teensy41
set BOARD_OPTIONS=speed=600,usb=mtpserial,opt=o3std,keys=en-us

set ARDUINO_FOLDER=C:\Program Files (x86)\Arduino
set TEENSY_TOOLS=.\tools\tools-windows

set PATH="%ARDUINO_FOLDER%";%PATH%

set HARDWARE=-hardware .\tools
set TOOLS=-tools %TEENSY_TOOLS% -tools "%ARDUINO_FOLDER%\tools-builder" 
set BUILD_PATH=.build
set CACHE_PATH=.cache
set FQBN=-fqbn=%BOARD%:%BOARD_OPTIONS%
set LIBRARIES=-libraries .\ -libraries .\include\ -libraries .\lib\
set FLAGS=-verbose

set BUILDER=arduino-builder
set POSTCOMPILER=%TEENSY_TOOLS%\teensy_post_compile
set REBOOT=%TEENSY_TOOLS%\teensy_reboot

set TARGET_HEX=%BUILD_PATH%\%NAME%.hex

:::::::::::::::::::::::::::::::::::::::::::::::::::
::CREATING DIRECTORIES
:::::::::::::::::::::::::::::::::::::::::::::::::::

echo Creating necessary directories if not existing...
if not exist %BUILD_PATH% mkdir %BUILD_PATH%
if not exist %CACHE_PATH% mkdir %CACHE_PATH%
echo 

:::::::::::::::::::::::::::::::::::::::::::::::::::
::COMPILING CODE
:::::::::::::::::::::::::::::::::::::::::::::::::::

echo Compiling %NAME% for %BOARD% with options %BOARD_OPTIONS%
echo Build path is %BUILD_PATH%
echo Cache path is %CACHE_PATH%
echo Library paths are %LIBRARIES%
%BUILDER% -dump-prefs -build-path %BUILD_PATH% -build-cache %CACHE_PATH% %HARDWARE% %TOOLS% %LIBRARIES% %FQBN% %SRC%\%NAME%
%BUILDER% -compile %FLAGS% -build-path %BUILD_PATH% -build-cache %CACHE_PATH% %HARDWARE% %TOOLS% %LIBRARIES% %FQBN% %SRC%\%NAME%
echo Target hex file %TARGET_HEX% generated
echo

:::::::::::::::::::::::::::::::::::::::::::::::::::
::UPLOADING CODE
:::::::::::::::::::::::::::::::::::::::::::::::::::

echo Uploading target hex %TARGET_HEX% for board %BOARD%
%POSTCOMPILER% -file=%NAME% -path=%BUILD_PATH% -tools=%TEENSY_TOOLS% -board %BOARD%
%REBOOT%
echo