default: clean build 
build: game
game: main.o world.o
	g++ -o game main.o world.o -lncurses
main.o: main.cpp
	g++ -c main.cpp
world.o: world.h world.cpp
	g++ -c world.cpp
clean:
	rm -f game *.o
