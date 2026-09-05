CC = g++

LIB_SRC = order_calc.cpp
LIB_H = order_calc.h
LIB_O = order_calc.o

TEST_SRC = test.cpp
TEST_O = test.o

TARGET = test

all: $(TARGET)

$(LIB_O): $(LIB_SRC) $(LIB_H)
	$(CC) -c $< -o $@

$(TEST_O): $(TEST_SRC) $(LIB_H)
	$(CC) -c $< -o $@

# Линковка
$(TARGET): $(TEST_O) $(LIB_O)
	$(CC) $^ -o $@

run:
	./$(TARGET)

clean:
	rm -f $(TEST_O) $(LIB_O) $(TARGET)

.PHONY: run clean