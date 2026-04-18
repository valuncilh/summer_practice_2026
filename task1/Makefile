TARGET = task1
SRC = task1.c
CFLAGS ?= -Wall -Wextra -O2
LDFLAGS = -lpthread

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	echo "5 3 8 -11 9 2" | ./$(TARGET) 2

.PHONY: all clean test
