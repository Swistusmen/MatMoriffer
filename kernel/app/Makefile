CC = gcc
CFLAGS = -Wall
INCLUDES = -I../lib

SRCS = vateoctl.c

TARGET = vateoctl

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

clean:
	rm -f $(TARGET)

all: $(TARGET)

.PHONY: all clean