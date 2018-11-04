#!/bin/bash
TARGET := tp_mod_board
CROSS_COMPILE := x86_64-w64-mingw32-
WINCC := $(CROSS_COMPILE)g++
LINCC := g++
WIN_CFLAGS := -static-libstdc++ -static-libgcc
CFLAGS := -Wall

SRCS := tp_mod_board.c
#OBJS := $(SRCS:.c=.o)

.PHONE: clean

clean:
	rm -f $(TARGET).exe $(TARGET).o $(TARGET)

win: $(SRCS)
	$(WINCC) $(CFLAGS) $(WIN_CFLAGS) -o $(TARGET).exe $<

linux: $(SRCS)
	$(LINCC) $(CFLAGS) -o $(TARGET) $<