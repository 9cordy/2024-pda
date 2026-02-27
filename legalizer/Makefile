all: Legalizer

CXX = g++
LINKFLAGS = -std=c++11 -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -static
SRC_DIR = src

Legalizer: main.o parser.o legalizer.o
	$(CXX) $(LINKFLAGS) main.o parser.o legalizer.o -o Legalizer

main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/parser.h $(SRC_DIR)/legalizer.h
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" $(SRC_DIR)/main.cpp -c

parser.o: $(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.h $(SRC_DIR)/legalizer.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/parser.cpp -c

legalizer.o: $(SRC_DIR)/legalizer.cpp $(SRC_DIR)/legalizer.h $(SRC_DIR)/parser.h
	$(CXX) $(LINKFLAGS) $(SRC_DIR)/legalizer.cpp -c

clean:
	rm -rf *.o Legalizer $(SRC_DIR)/*.gch