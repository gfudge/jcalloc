TARGET 	= example
LIBS 	= -lm
CC		= gcc
CFLAGS	= -g -Wall

.PHONY: default all clean

SRC_DIR = src
INC 	= -Iinc
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,%.o,$(SOURCES))

default: $(TARGET)
all: default

$(OBJECTS): %.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(INC) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^ -Wall -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
