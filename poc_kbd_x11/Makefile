## Variables ##
CC=gcc
CFLAG=-Wall -lX11
DIR_SOURCE=projects/poc_kbd_x11/
SOURCE=poc_kbd
RFLAG=-O2 -o $(DIR_SOURCE)bin/x64/Release/$(SOURCE).exe
DFLAG=-ddbg -Wall -o $(DIR_SOURCE)bin/x64/Release/$(SOURCE).exe

## Set default target ##
default_target: all

.PHONY : default_target

## Target and dependancies
all: release debug

release:
	$(CC) $(CFLAG) $(RFLAG) $(DIR_SOURCE)$(SOURCE).c

debug:
	$(CC) $(CFLAG) $(DFLAG) $(SOURCE).c

clean:
	rm -r $(DIR_SOURCE)bin/x64/*.exe