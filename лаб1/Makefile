CC = g++

LIB_SRC = order_calc.cpp
LIB_H = order_calc.h
LIB_O = order_calc.o

TEST_SRC = test.cpp
TEST_O = test.o

all: $(TEST_O)

$(TEST_O) : $(TEST_SRC) $(LIB_SRC)
	$(CC) $< -o $@

run:
	./$(TEST_O)

.PHONY: run
