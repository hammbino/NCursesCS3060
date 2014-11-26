// copyright 2014 nigel nelson

#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "world.h"
using namespace std;

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

World::World(string filename) {
  ifstream fs(filename.c_str());

  if(!fs.good()) {
    throw "failed to open world file.";
  }

  getline(fs, this->name);
  fs >> this->cols;
  fs.ignore(1);
  fs >> this->rows;
  fs.ignore(1);
  this->playerX = 2;
  this->playerY = 2;
  this->tiles = new char[this->rows * this->cols];
  for(int r = 0; r < rows; ++r) {
    string line;
    getline(fs, line);
    strncpy(this->tiles + (r * this->cols), line.c_str(), this->cols);
    int c;
    if((c = line.find('p')) != string::npos) {
      this->playerX = c;
      this->playerY = r;
      this->tiles[(r * this->cols) + c] = ' ';
    }
  }
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

  // draw what we can see of the map
  int radx, rady;
  getmaxyx(win, rady, radx);
  radx /= 6;
  rady /= 6;
  int left = max(this->playerX-radx, 0);
  int right = min(this->playerX+radx, this->cols-1);
  int top = max(this->playerY-rady, 0);
  int bottom = min(this->playerY+rady, this->rows-1);
  for (int r = top; r <= bottom; ++r) {
    for (int c = left; c <= right; ++c) {
      int cx = (c - this->playerX + radx) * 3;
      int cy = (r - this->playerY + rady) * 3;
      // the player
      if(r == this->playerY && c == this->playerX) {
        int color = (frame / 2)%6 + 2;
        wattron(win, COLOR_PAIR(color));
        mvwaddstr(win, cy, cx, " o ");
        mvwaddstr(win, cy + 1, cx,  "+|+");
        mvwaddstr(win, cy + 2, cx,  "/ \\");
      }
      // the tile
      else {
        wattron(win, COLOR_PAIR(1));
        for(int x = 0; x < 3; ++x)
          for(int y = 0; y < 3; ++y)
            mvwaddch(win, cy + y, cx + x, tileAt(c, r));
      }
    }
  }

  // draw the character's next frame

  // output everything to the screen
  wrefresh(win);

  // return 1 to continue running
  return 1;
}
