BINARY=main
OBJ=${BINARY}.o
CXXFLAGS=-Wall -Wextra -nostdlib -static

all: $(BINARY)

run: $(BINARY)
	./$(BINARY)

$(BINARY): $(OBJ)
	$(CXX) $(CXXFLAGS) $< -o $(BINARY)

clean:
	$(RM) $(OBJ) $(BINARY)
