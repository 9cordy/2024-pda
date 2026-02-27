all: floorplanning

CXX = g++
LINKFLAGS = -std=c++11 -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3
SRC_DIR = src

floorplanning: main.o parser.o bstar_tree.o
	$(CXX) $(LINKFLAGS) main.o parser.o bstar_tree.o -o floorplanning

main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/parser.h $(SRC_DIR)/bstar_tree.h
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" $(SRC_DIR)/main.cpp -c

parser.o: $(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/parser.cpp -c

bstar_tree.o: $(SRC_DIR)/bstar_tree.cpp $(SRC_DIR)/bstar_tree.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/bstar_tree.cpp -c

clean:
	rm -rf *.o floorplanning $(SRC_DIR)/*.gch