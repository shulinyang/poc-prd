## Variables ##
CC=gcc
CFLAGS=-Wall
LDFLAGS=-lX11
DIR_SOURCE=projects/poc_kbd_x11/
SOURCE=poc_kbd
MKDIR_P = mkdir -p
BIN_RELEASE=bin/x64/Release/
BIN_DEBUG=bin/x64/Debug/
RFLAG= $(CFLAGS) -O2 -o $(BIN_RELEASE)poc_kbd_x11.exe
DFLAG= $(CFLAGS) -ggdb -o $(BIN_DEBUG)poc_kbd_x11.exe

## Set default target ##
default_target: all

.PHONY : default_target

## Target and dependancies
all: mkdir release debug

release:
	$(CC) $(RFLAG) $(SOURCE).c $(LDFLAGS)

debug:
	$(CC) $(DFLAG) $(SOURCE).c $(LDFLAGS)

mkdir:
	$(MKDIR_P) $(BIN_RELEASE)
	$(MKDIR_P) $(BIN_DEBUG)

clean:
	rm -r $(DIR_SOURCE)bin/x64/*.exe