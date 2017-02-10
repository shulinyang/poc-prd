## Executable ##
CC=gcc
MKDIR_P = mkdir -p

## Folder ##
DIR_RELEASE=bin/x64/Release/
DIR_DEBUG=bin/x64/Debug/
DIR_SOURCE=projects/poc_kbd_x11/

## Source ##
SOURCE=poc_kbd.c

## flags ##
CFLAGS=-Wall
LDFLAGS=-lX11
RFLAG= $(CFLAGS) -O2 -o $(DIR_RELEASE)poc_kbd_x11.exe $(LDFLAGS)
DFLAG= $(CFLAGS) -ggdb -o $(DIR_DEBUG)poc_kbd_x11.exe $(LDFLAGS)

## Set default target ##
default_target: all

.PHONY : default_target

## Target and dependancies
all: mkdir release debug

release:
	$(CC) $(SOURCE) $(RFLAG)

debug:
	$(CC) $(SOURCE) $(DFLAG)

mkdir:
	$(MKDIR_P) $(DIR_RELEASE)
	$(MKDIR_P) $(DIR_DEBUG)

clean:
	rm -r $(DIR_SOURCE)bin/x64/*.exe