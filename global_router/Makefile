all: D2DGRter

CXX = g++
LINKFLAGS = -std=c++11 -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -static
SRC_DIR = src

D2DGRter: main.o parser.o route.o
	$(CXX) $(LINKFLAGS) main.o parser.o route.o -o D2DGRter

main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/parser.h $(SRC_DIR)/route.h
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" $(SRC_DIR)/main.cpp -c

parser.o: $(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/parser.cpp -c

route.o: $(SRC_DIR)/route.cpp $(SRC_DIR)/route.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/route.cpp -c

clean:
	rm -rf *.o D2DGRter $(SRC_DIR)/*.gch