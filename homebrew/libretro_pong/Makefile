CC ?= mipsel-linux-gnu-gcc
CFLAGS ?= -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99
LDFLAGS ?= -shared -nostdlib -Wl,-soname,libemu_pong.so

TARGET := libemu_pong.so
SRC := pong.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
