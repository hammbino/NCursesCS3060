#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include "world.h"

int main(void) {
  WINDOW * mainwin;

  // initialize ncurses
  if ((mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }

  // colors
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_RED);
  init_pair(3, COLOR_BLACK, COLOR_YELLOW);
  init_pair(4, COLOR_BLACK, COLOR_GREEN);
  init_pair(5, COLOR_BLACK, COLOR_CYAN);
  init_pair(6, COLOR_BLACK, COLOR_BLUE);
  init_pair(7, COLOR_BLACK, COLOR_MAGENTA);

  // create world and run it
  World w("data/floor1");
  w.run(mainwin);

  // clean up and close
  delwin(mainwin);
  endwin();
  refresh();
  return EXIT_SUCCESS;
}
