#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include "world.h"
#include "character.h"

// definitions
void populateCharacters(vector<character*> &people);

// globals
character* player; 
vector<character*> people;

// methods
void populateCharacters(vector<character*> &people) {

  people.push_back(new character(5,5,5,"Professor","Wright","Obsidian","Plywood","Hedge Trimmers", 0));
  people.push_back(new character(0,1,0,"Student","Slim Steven","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(2,2,2,"Student","Fred","Rock","Paper","Scissors", 0));
  people.push_back(new character(0,0,3,"Student","Sharp Shawn","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(4,4,4,"Student","Clint","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(6,6,6,"Vendor","Trafficing Trevor","Diamond","Steel Sheet","Steel Snips", 0));

}

// main
int main(void) {
  // setup ncurses, get main window
  WINDOW * mainwin;
  if ((mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }

  // colors
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

  // create player & other characters
  player = new character(1,1,1,"Player","Dude","Pebble","Recycled Paper","Safety Scissors", 0);
  populateCharacters(people);

  // create world and run it
  World w("data/swamp");
  w.run(mainwin);

  // clean up and close
  delwin(mainwin);
  endwin();
  refresh();
  return EXIT_SUCCESS;
}
