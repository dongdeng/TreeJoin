CXX=g++-4.8
CXXFLAGS=-std=c++11 -Wall -O3 -g -MMD -MP
TARGET=main

SRC=$(shell ls *.cpp)

all: $(TARGET)

$(TARGET): $(SRC:.cpp=.o)
	$(CXX) $(CXXFLAGS) $+ -o $@

-include $(SRC:.cpp=.d)

clean:
	$(RM) *.o *.d $(TARGET)

.PHONY: all clean

