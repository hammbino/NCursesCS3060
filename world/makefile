default: clean build 
build: game
game: game.o world.o
	g++ -o game game.o world.o -lncurses
game.o: game.cpp
	g++ -c game.cpp
world.o: world.h world.cpp
	c++ -c world.cpp
clean:
	rm -f game *.o
