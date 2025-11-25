ifeq ($(OS), Windows_NT)
	TARGET := calc.exe
	LINK 	:= 
else
	TARGET := calc
	LINK := -lcurses
endif
CC		= gcc
CCFLAGS	= -Wall -std=c23

n ?= 2

SRC		= main.c \
			src/Calc.c \
			src/analyzer.c \
			src/evaluate.c \
			src/reader.c \
			class/src/List.c \
			class/src/Number.c \
			class/src/Stack.c \
			class/src/Token.c 

OBJ		= $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(CCFLAGS) -o $@ $^

%.o: %.c
	@$(CC) $(CCFLAGS) -c -o $@ $<

test: $(TARGET) 
	./$(TARGET) $(n)

clean:
	@rm -f $(OBJ) $(DEP) $(TARGET)

.PHONY: all test clean
