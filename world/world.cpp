// copyright 2014 nigel nelson

#include <time.h>
#include "world.h"

#define FRAME_NANOS (1000000000/33)

World::World() {
  this->name = "Default Room";
  this->rows = 8;
  this->cols = 8;
  this->tiles = new char[this->rows * this->cols];
  for (int r = 0; r < this->rows; ++r) {
    for (int c = 0; c < this->cols; ++c) {
      char * t = tiles + r * this->cols + c;
      *t = (r < 2 || r >= 6 || c < 2 || c >= 6) ? 'X' : ' ';
    }
  }
  this->playerX = 4;
  this->playerY = 4;
}

char World::tileAt(int x, int y) {
  return *(this->tiles + y * this->cols + x);
}

void World::run(WINDOW* win) {
  // structs needed for sleeping
  struct timespec sleeptime, notused;
  sleeptime.tv_sec = 0;
  sleeptime.tv_nsec = FRAME_NANOS;

  while (1) {
    // key
    timeout(0);
    int input = getch();
    // frame action
    int donext = this->doFrame(win, input);
    // do next frame?
    if (!donext) break;
    // wait
    nanosleep(&sleeptime, &notused);
  }
}

int World::doFrame(WINDOW* win, int key) {
  // frame number
  static int frame = -1;
  ++frame;

  // input
  bool moved = true;
  if (key == 100) {
    // right
    if(tileAt(this->playerX+1, this->playerY) == TILE_FLOOR)
      ++this->playerX;
  } else if (key == 97) {
    // left
    if(tileAt(this->playerX-1, this->playerY) == TILE_FLOOR)
      --this->playerX;
  } else if (key == 115) {
    // down
    if(tileAt(this->playerX, this->playerY+1) == TILE_FLOOR)
      ++this->playerY;
  } else if (key == 119) {
    // up
    if(tileAt(this->playerX, this->playerY-1) == TILE_FLOOR)
      --this->playerY;
  } else {
    moved = false;
  }

  // hide cursor
  curs_set(0);

  // clear screen
  wclear(win);

  // draw the character's next frame
  int color = (frame / 2)%6 + 1;
  wattron(win, COLOR_PAIR(color));
  mvwaddstr(win, this->playerY, this->playerX, " o ");
  mvwaddstr(win, this->playerY+1, this->playerX, "+|+");
  mvwaddstr(win, this->playerY+2, this->playerX, "/ |");

  // output everything to the screen
  wrefresh(win);

  // return 1 to continue running
  return 1;
}
