TARGET = task7
TARCHILD = child_copy
TARPARENT = parent_copy
SRC = task7.c
CFLAGS ?= -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(SRC)
	gcc $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) $(TARCHILD) $(TARPARENT)

.PHONY: all clean