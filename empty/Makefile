BINARY=main
OBJ=${BINARY}.o start.o
CXXFLAGS=-std=c++14 -Wall -Wextra -nostdlib -m32 -static -fno-PIC -g -fno-stack-protector
ASFLAGS=-m32

all: $(BINARY)

run: $(BINARY)
	./$(BINARY)

$(BINARY): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(BINARY)

clean:
	$(RM) $(OBJ) $(BINARY)
