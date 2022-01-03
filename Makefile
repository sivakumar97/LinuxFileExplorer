CC = g++
CFLAG = -std=c++2a -Wall
DEPS = header.h
OBJS = commandMode.o fileOperations.o directoryOperations.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAG) -c -o $@ $<

main: $(OBJS)
	$(CC) $(CFLAG) -o $@ $^

clean:
	-rm -rf *.o main