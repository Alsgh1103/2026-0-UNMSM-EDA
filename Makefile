CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -fdiagnostics-color=always
LDFLAGS =

TARGET = main
SRCS = main.cpp util.cpp pointers.cpp DemoArray.cpp DemoLists.cpp
OBJS = main.o util.o pointers.o DemoArray.o DemoLists.o sorting.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

sorting.o: algorithms/sorting.cpp
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean