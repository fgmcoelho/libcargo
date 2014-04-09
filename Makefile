MAJOR = 1
MINOR = 0
PATCH = 0
TARGET = libcontainers.so
LIBS =
CC = gcc
CFLAGS = -Wall -Wextra
ifeq ($(DEBUG),1)
CFLAGS += -g -O0
else 
CFLAGS += -O2
endif

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
