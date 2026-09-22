BINARY=main
OBJ=start.o ${BINARY}.o
CXXFLAGS=-Wall -Wextra -nostdlib -static -g -fno-stack-protector -O2

all: $(BINARY)

run: $(BINARY)
	./$(BINARY)

$(BINARY): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(BINARY)

clean:
	$(RM) $(OBJ) $(BINARY)
