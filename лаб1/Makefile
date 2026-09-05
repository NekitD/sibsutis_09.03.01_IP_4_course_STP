CC = g++
CXXFLAGS = -std=c++11 -Wall -c
LDFLAGS = -std=c++11 -Wall

LIB_SRC = order_calc.cpp
LIB_H = order_calc.h
LIB_O = order_calc.o

TEST_SRC = test.cpp
TEST_O = test.o

TARGET = test

all: $(TARGET)

$(LIB_O): $(LIB_SRC) $(LIB_H)
	@ $(CC) $(CXXFLAGS) $< -o $@

$(TEST_O): $(TEST_SRC) $(LIB_H)
	@ $(CC) $(CXXFLAGS) $< -o $@

$(TARGET): $(TEST_O) $(LIB_O)
	@ $(CC) $(LDFLAGS) $^ -o $@

run:
	@ ./$(TARGET)

clean:
	@ rm -f $(TEST_O) $(LIB_O) $(TARGET) *.exe

.PHONY: run clean