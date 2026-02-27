all: corner_stitching

CXX = g++
LINKFLAGS = -std=c++11 -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3
SRC_DIR = src

corner_stitching: main.o parser.o corner_stitching.o
	$(CXX) $(LINKFLAGS) main.o parser.o corner_stitching.o -o corner_stitching

main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/parser.h $(SRC_DIR)/corner_stitching.h
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" $(SRC_DIR)/main.cpp -c

parser.o: $(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.h $(SRC_DIR)/corner_stitching.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/parser.cpp -c

corner_stitching.o: $(SRC_DIR)/corner_stitching.cpp $(SRC_DIR)/corner_stitching.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/corner_stitching.cpp -c

clean:
	rm -rf *.o corner_stitching $(SRC_DIR)/*.gch