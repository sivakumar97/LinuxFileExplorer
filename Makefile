CC = g++
CFLAG = -std=c++2a -Wall
DEPS = header.h
OBJS = commandmode.o fileoperations.o directoryops.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAG) -c -o $@ $<

main: $(OBJS)
	$(CC) $(CFLAG) -o $@ $^

clean:
	-rm -rf *.o main