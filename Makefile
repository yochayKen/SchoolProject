CC=gcc
CFLAGS=-Wall -ansi -pedantic
DEPS=file_utils.h list.h handle_string.h preprocessor.h error.h first_stage.h
OBJ=main.o file_utils.o list.o handle_string.o preprocessor.o error.o first_stage.o

TARGET_EXEC=assembler

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -rf *.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(TARGET_EXEC) *.o
