default: clean build 
build: game
game: main.o world.o interact.o character.o
	g++ -o game main.o world.o interact.o character.o -lncurses
main.o: main.cpp
	g++ -c main.cpp
world.o: world.cpp
	g++ -c world.cpp
interact.o: interact.cpp
	g++ -c interact.cpp
character.o: character.cpp
	g++ -c character.cpp
clean:
	rm -f game *.o
