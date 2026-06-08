CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -pedantic
TARGET  = contas
SRC     = contas.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) contas.dat

run: all
	./$(TARGET)

.PHONY: all clean run