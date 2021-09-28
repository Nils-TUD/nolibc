BINARY=main
OBJ=${BINARY}.o start.o
CXXFLAGS=-std=c++14 -Wall -Wextra -nostdlib -m32 -static -fno-PIC -g -fno-stack-protector

all: $(BINARY)

run: $(BINARY)
	./$(BINARY)

$(BINARY): $(OBJ)
	$(CXX) $(CXXFLAGS) $+ -o $(BINARY)

start.o: start.S
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(BINARY)
