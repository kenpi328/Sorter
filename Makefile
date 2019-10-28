# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGET = sorter

SOURCES = main.c sorter.c segDisplay.c potDriver.c -lm


PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread
# -pg for supporting gprof profiling.
CFLAGS += -pg



# Convert the targets to full-path executables to delete.
OUTPUTS= $(addprefix $(OUTDIR)/, $(TARGETS))

all: $(TARGET)

$(TARGET):
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET) 


clean:
	rm -f $(OUTPUTS)

