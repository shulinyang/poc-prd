## Variables ##
CC=gcc
CFLAG=-Wall -lX11
DIR_SOURCE=projects/poc_kbd_x11/
SOURCE=poc_kbd
MKDIR_P = mkdir -p
BIN_RELEASE=bin/x64/Release/
BIN_DEBUG=bin/x64/Debug/
RFLAG=-O2 -o $(BIN_RELEASE)$(SOURCE).exe
DFLAG=-ggdb -Wall -o $(BIN_DEBUG)$(SOURCE).exe

## Set default target ##
default_target: all

.PHONY : default_target

## Target and dependancies
all: mkdir release debug

release:
	$(CC) $(CFLAG) $(RFLAG) $(SOURCE).c

debug:
	$(CC) $(CFLAG) $(DFLAG) $(SOURCE).c

mkdir:
	$(MKDIR_P) $(BIN_RELEASE)
	$(MKDIR_P) $(BIN_DEBUG)

clean:
	rm -r $(DIR_SOURCE)bin/x64/*.exe