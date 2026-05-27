CC ?= mipsel-linux-gnu-gcc
CFLAGS ?= -O2 -fno-sanitize=undefined -fPIC -Wall -Wextra -std=c99
LDFLAGS ?= -shared -Wl,-soname,libemu_buttondemo.so

TARGET := libemu_buttondemo.so
SRC := button_demo.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
