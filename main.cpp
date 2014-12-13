#include "interact.h"
#include "character.h"
#include "world.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>

// main
int main(void) {
  // setup ncurses, get main window
  WINDOW * mainwin;
  if ((mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }

  // keypad mode allows for arrow keys
  keypad(mainwin, TRUE);

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
  player = new character(1,1,1,100,"Player","You","Pebble","Recycled Paper","Safety Scissors", 0);
  populateCharacters(people);
    Interact firstInteraction;
    firstInteraction.firstFight(people[0]);
    firstInteraction.firstFight(people[31]);
  // create world and run it
  World w("data/swamp");
  w.run(mainwin);

  // clean up and close
  delwin(mainwin);
  endwin();
  refresh();
  return EXIT_SUCCESS;
}
