
#---------------------------------------------------------------------------------
# MAKEFILE FOR COMPILATIONN IN LINUX/WINDOWS
#---------------------------------------------------------------------------------
# AUTHOR: STEFANO LOVATO
# CREATION: March 2022

#---------------------------------------------------------------------------------
# USER SETTINGS
#---------------------------------------------------------------------------------

#Operative system (Linux/Windows_NT)
ifneq ($(OS), Windows_NT)
OS := $(shell uname)
endif

ifneq ($(OS),$(filter $(OS),Linux Windows_NT))
$(error OS not supported)
endif

#Name of main file
NAME			:= main.cpp

#Board
BOARD			:= teensy41
BOARD_OPTIONS	:= speed=600,usb=mtpserial,opt=o3std,keys=en-us

#Directories
ifeq ($(OS), Linux)
ARDUINO_FOLDER	:= /usr/share/arduino
MATLAB_FOLDER   := /usr/local/MATLAB/R2022a
BUILD_TOOLS		:= ./tools/tools-linux
else ifeq ($(OS), Windows_NT) 
ARDUINO_FOLDER  := C:/Program Files (x86)/Arduino
MATLAB_FOLDER   := C:/Program Files/MATLAB/R2022a
BUILD_TOOLS		:= ./tools/tools-windows
endif
SRC				:= ./src
BUILD_PATH		:= ./.build
CACHE_PATH		:= ./.cache
DOCS_PATH		:= ./docs
MATLAB_TOOLS	:= ./matlab-tools

#---------------------------------------------------------------------------------
# USUALLY EDITING BELOW THIS LINE NOT NECESSARY
#---------------------------------------------------------------------------------

#Tools (be careful to change this)
BUILDER			:= $(ARDUINO_FOLDER)/arduino-builder
MATLAB          := $(MATLAB_FOLDER)/bin/matlab
POSTCOMPILER	:= $(BUILD_TOOLS)/teensy_post_compile
REBOOT			:= $(BUILD_TOOLS)/teensy_reboot

#Builder options (be careful to change this)
HARDWARE		:= -hardware ./tools
FQBN			:= -fqbn=teensy:avr:$(BOARD):$(BOARD_OPTIONS)
LIBRARIES		:= -libraries ./ -libraries ./include/ -libraries ./lib/
TOOLS			:= -tools "$(BUILD_TOOLS)" -tools "$(ARDUINO_FOLDER)/tools-builder"
FLAGS			:= #-verbose

#---------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

#Default Make
all: build upload

#Build the code
build: directories
	@$(BUILDER) -compile $(FLAGS) -build-path $(BUILD_PATH) -build-cache $(CACHE_PATH) $(HARDWARE) $(TOOLS) $(LIBRARIES) $(FQBN) $(SRC)/$(NAME)

#Dump preferances
dumpprefs:
	@$(BUILDER) -dump-prefs -build-path $(BUILD_PATH) -build-cache $(CACHE_PATH) $(HARDWARE) $(TOOLS) $(LIBRARIES) $(FQBN) $(SRC)/$(NAME)

#Upload the code
upload:
	$(POSTCOMPILER) -file=$(NAME) -path=$(BUILD_PATH) -tools=$(BUILD_TOOLS) -board teensy:avr:$(BOARD)
	$(REBOOT)

#Make documentation
doc: cleandoc
	@doxygen

#Clean documentation
cleandoc:
ifeq ($(OS), Linux)
	@$(RM) -rf $(DOCS_PATH)/html
else ifeq ($(OS), Windows_NT)
	@if exist "$(DOCS_PATH)/html" @rmdir /S /Q "$(DOCS_PATH)/html"
endif

#Remake
remake: clean all

#Rebuild
rebuild: clean build

#Clean build
clean:
ifeq ($(OS), Linux)
	@$(RM) -rf $(BUILD_PATH)
	@$(RM) -rf $(CACHE_PATH)
else ifeq ($(OS), Windows_NT)
	@if exist "$(BUILD_PATH)" @rmdir /S /Q "$(BUILD_PATH)"
	@if exist "$(CACHE_PATH)" @rmdir /S /Q "$(CACHE_PATH)"
endif

#Clean all
cleanall: clean cleandoc

#Make the Directories
directories:
ifeq ($(OS), Linux)
	@mkdir -p $(BUILD_PATH)
	@mkdir -p $(CACHE_PATH)
else ifeq ($(OS), Windows_NT)
	@if not exist "$(BUILD_PATH)" mkdir "$(BUILD_PATH)"
	@if not exist "$(CACHE_PATH)" mkdir "$(CACHE_PATH)"
endif

#Generate the code
#gencode:
#	@$(MATLAB) -batch "addpath('$(MATLAB_TOOLS)'); gencode();"

#Check for MATLAB tooboxes
#checktoolbox:
#	@$(MATLAB) -batch "addpath('$(MATLAB_TOOLS)'); check_toolbox();"

#Help
help:
	@echo "Description: Makefile for code utilities of the self-balancing bike."
	@echo "Usage: make [operation]"
	@echo "Options:"
	@echo "nothing or 'all'				Build and upload the code to the micro-controller."
	@echo "'build'						Build the code."
	@echo "'upload'					Upload the code to the micro-controller."
	@echo "'clean'						Clean the build and cache directories."
	@echo "'remake'					Clean, build and upload the code to the micro-controller"
	@echo "'rebuild'					Clean and rebuild the code."
#	@echo "'gencode'					Generate the code from the Simulink model."
#	@echo "						\note This may take some time."
#	@echo "						\attention This require MATLAB/Simulink >= 2022a."
#	@echo "'checktoolbox'					Check for the MATLAB toolboxes required by the code generation."
	@echo "'doc'						Build the documentation."
	@echo "'cleandoc'					Clean the documentation."

#Non-File Targets
.PHONY: all build upload remake clean doc cleandoc directories help # gencode checktoolbox
