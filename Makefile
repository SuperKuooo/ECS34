CCX=g++

INCLUDE=-I src/
CXXFLAGS= -std=c++14 $(INCLUDE)

all:bin/mancala

bin/mancala: obj/main.o obj/MancalaBoard.o
	$(CXX) -std=c++14 obj/main.o obj/MancalaBoard.o -o bin/Mancala

obj/main.o: main.cpp MancalaBoard.h
	$(CXX) $(CXXFLAGS) src/main.cpp -o ojb/main.o

obj/MancalaBoard.o: src/MancalaBoard.cpp src/MancalaBoard.h
	$(CXX) $(CXXFLAGS) src/MancalaBoard.cpp -0 bin/Mancala
