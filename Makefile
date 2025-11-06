EXEC = Lab10

all: $(EXEC)

$(EXEC): Lab10.cpp
	g++ Lab10.cpp -o $(EXEC).exe

clean:
	rm -rf *.exe