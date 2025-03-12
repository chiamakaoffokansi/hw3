CXX := g++
CXXFLAGS := -g -Wall --std=c++11
VALGRIND := valgrind --tool=memcheck --leak-check=yes

all: llrec-test

llrec-test: llrec-test.o
	$(CXX) $(CXXFLAGS) -o llrec-test llrec-test.o

llrec-test.o: llrec-test.cpp llrec.h
	$(CXX) $(CXXFLAGS) -c llrec-test.cpp

stack_test: stack_test.o
	$(CXX) $(CXXFLAGS) -o stack_test stack_test.o

stack_test.o: stack_test.cpp stack.h
	$(CXX) $(CXXFLAGS) -c stack_test.cpp

clean:
	rm -f *.o llrec-test *~

.PHONY: clean
