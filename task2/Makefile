TARGET = task2
SRC = task2.c
CFLAGS ?= -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean test
