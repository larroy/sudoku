#CXXFLAGS = -g
#CXXFLAGS = -O -mcpu=pentium4 -march=pentium4
CXXFLAGS = -O2 -Wall -Weffc++

all: Sudoku

# output of: g++ -MM *.cc

main.o: main.cc Sudoku.hh
Sudoku.o: Sudoku.cc Sudoku.hh

SUDOKO_OBJECTS:=Sudoku.o main.o

Sudoku: $(SUDOKO_OBJECTS)
	$(CXX) -o $@ $(SUDOKO_OBJECTS)

clean: 
	rm -f Sudoku *.o *~ 

