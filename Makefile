CC     = clang
CFLAGS = -std=c11 -Wall -Wextra -lm
TARGET = main

$(TARGET): cube.c
	$(CC) $(CFLAGS) -o $(TARGET) cube.c

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
