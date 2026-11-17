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

all: $(TARGET)

$(TARGET): $(OBJS)
	@ $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/searcher.h
	@ mkdir -p $(OBJDIR)
	@ $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@ rm -rf $(OBJDIR) $(TARGET)

$(MAIN_OBJ): $(SRCDIR)/searcher.h
$(SEARCHER_OBJ): $(SRCDIR)/searcher.h

.PHONY: all clean