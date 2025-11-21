CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -pthread

OBJS = main.o problem1.o problem2.o problem3.o problem4.o

# Default target
cse4001_sync: $(OBJS)
	$(CXX) $(CXXFLAGS) -o cse4001_sync $(OBJS)

# Generic rule to build each .o from its .cpp
%.o: %.cpp semaphore_class.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o cse4001_sync