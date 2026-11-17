CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread

TARGET = pgrep

SRCDIR = src
OBJDIR = obj

MAIN_SOURCE = $(SRCDIR)/main.cpp
SEARCHER_SOURCE = $(SRCDIR)/searcher.cpp

MAIN_OBJ = $(OBJDIR)/main.o
SEARCHER_OBJ = $(OBJDIR)/searcher.o

OBJS = $(MAIN_OBJ) $(SEARCHER_OBJ)

FILE = books/ 
TEMPLATE = sEe
FLAGS = -i -c -w -r

all: $(TARGET)

$(TARGET): $(OBJS)
	@ $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/searcher.h
	@ mkdir -p $(OBJDIR)
	@ $(CXX) $(CXXFLAGS) -c $< -o $@


test1:
	./$(TARGET) $(FLAGS) $(TEMPLATE) $(FILE)

test2:
	./$(TARGET) $(FLAGS) -j 2 $(TEMPLATE) $(FILE)

test3:
	./$(TARGET) $(FLAGS) -j 4 $(TEMPLATE) $(FILE)

test4:
	./$(TARGET) $(FLAGS) -j 8 $(TEMPLATE) $(FILE)

test5:
	./$(TARGET) $(FLAGS) -j 16 $(TEMPLATE) $(FILE)

test6:
	./$(TARGET) $(FLAGS) -j 20 $(TEMPLATE) $(FILE)

test7:
	./$(TARGET) $(FLAGS) -j 24 $(TEMPLATE) $(FILE)

test8:
	./$(TARGET) $(FLAGS) -j 28 $(TEMPLATE) $(FILE)

test9:
	./$(TARGET) $(FLAGS) -j 32 $(TEMPLATE) $(FILE)

test10:
	./$(TARGET) $(FLAGS) -j 64 $(TEMPLATE) $(FILE)


clean:
	@ rm -rf $(OBJDIR) $(TARGET)

$(MAIN_OBJ): $(SRCDIR)/searcher.h
$(SEARCHER_OBJ): $(SRCDIR)/searcher.h

.PHONY: all clean test1 test2 test3 test4 test5 test6 test7 test8 test9 test10