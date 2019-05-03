CXX		=g++
TESTLDFLAGS 	+= -lgtest -lgtest_main -lpthread
CXXFLAGS	= -std=c++14 -Wall -Werror

bin/proj3: obj/main.o obj/MancalaBoard.o
	$(CXX) obj/main.o obj/MancalaBoard.o -o bin/proj3

obj/main.o: src/main.cpp
	$(CXX) -c src/main.cpp -o obj/main.o

obj/MancalaBoard.o: src/MancalaBoard.cpp src/MancalaBoard.h
	$(CXX) -c src/MancalaBoard.cpp -o obj/MancalaBoard.o

clean:
	rm .o proj3
