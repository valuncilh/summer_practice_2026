TARGET = task8
SRC = task8.c
CFLAGS ?= -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean