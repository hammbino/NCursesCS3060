#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>

#define FRAME_NANOS (1000000000/33)

int doframe(int, struct timespec*, struct timespec*);

int main(void) {
  WINDOW * mainwin;

  /* Initialize ncurses */
  if ( (mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }
  start_color();

  /* Create colored pairs */
  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_YELLOW);
  init_pair(3, COLOR_BLACK, COLOR_GREEN);
  init_pair(4, COLOR_BLACK, COLOR_CYAN);
  init_pair(5, COLOR_BLACK, COLOR_BLUE);
  init_pair(6, COLOR_BLACK, COLOR_MAGENTA);

  /* Create our sleep timer (1/8 second) */
  struct timespec sleeptime, notused;
  sleeptime.tv_sec = 0;
  sleeptime.tv_nsec = FRAME_NANOS;

  int input;
  for (int i = 0; doframe(i, &sleeptime, &notused); ++i) continue;

  /* Clean up after ourselves */
  delwin(mainwin);
  endwin();
  refresh();

  return EXIT_SUCCESS;
}

int x=10,y=10,xv=2,yv=0;

int doframe(int frame, struct timespec *sleeptime, struct timespec *notused) {
  /* First clear off the screen */
  clear();
  
  /* Set the color and output the text */
  int color = (frame / 2)%6 + 1;
  attron(COLOR_PAIR(color));
  mvaddstr(y, x, " o ");
  mvaddstr(y+1, x, "+|+");
  mvaddstr(y+2, x, "/ |");

  /* Refresh the screen */
  refresh();

  /* Sleep */
  nanosleep(sleeptime, notused);

  /* Exit early if a key is pressed */
  timeout(0);
  int input = getch();
  if (input == 100) {
    ++x;
  } else if (input == 97) {
    --x;
  } else if (input == 115) {
    ++y;
  } else if (input == 119) {
    --y;
  }
  /*char key[4];
  sprintf(key, "%d", input);
  attron(COLOR_PAIR(color));
  mvaddstr(y,x+4, key);
  */
  return 1;

}
