// copyright 2014 nigel nelson

#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "world.h"
using namespace std;

World::World() {
  this->name = "Default Room";
  this->rows = 8;
  this->cols = 8;
  // allocate array for the tiles
  this->tiles = new char[this->rows * this->cols];
  // initialize the array to be empty, surrounded by walls
  for (int r = 0; r < this->rows; ++r) {
    for (int c = 0; c < this->cols; ++c) {
      // pointer to that tile
      char * t = tiles + r * this->cols + c;
      // set it to a wall if it's the edge,
      // otherwise it's blank.
      *t = (r < 2 || r >= 6 || c < 2 || c >= 6) ?
        World::TILE_WALL: World::TILE_FLOOR;
    }
  }
  // put the player in there somewhere
  this->playerX = 4;
  this->playerY = 4;
}

World::World(string filename) {
  // open stream
  ifstream fs(filename.c_str());
  if(!fs.good()) throw "failed to open world file.";
  // read in the name and size from first 3 lines
  getline(fs, this->name);
  fs >> this->cols;
  fs.ignore(1);
  fs >> this->rows;
  fs.ignore(1);
  // default player position; should be set later
  this->playerX = 2;
  this->playerY = 2;
  // allocate array for tiles
  this->tiles = new char[this->rows * this->cols];
  // load in the rows, line by line
  for(int r = 0; r < rows; ++r) {
    string line;
    getline(fs, line);
    strncpy(this->tiles + (r * this->cols), line.c_str(), this->cols);
    // see if the player's location was declared in this line
    int c;
    if((c = line.find(World::TILE_PLAYER)) != string::npos) {
      this->playerX = c;
      this->playerY = r;
      // there will just be a blank space where he started
      this->tiles[(r * this->cols) + c] = World::TILE_FLOOR;
    }
  }
}

World::~World() {
  delete[] this->tiles;
}

char World::tileAt(int x, int y) {
  return *(this->tiles + y * this->cols + x);
}

void World::run(WINDOW* win) {
  // create some HUD windows
  WINDOW* hud = newwin(5, 20, 2, 2);
  box(hud, 0, 0);
  mvwaddstr(hud, 4, 4, "hi");

  // structs needed for sleeping
  struct timespec sleeptime, notused;
  sleeptime.tv_sec = 0;
  sleeptime.tv_nsec = (1000000000/33);

  while (1) {
    // key
    timeout(0);
    int input = getch();
    // frame action
    int donext = this->doFrame(win, input);
    wrefresh(hud);
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
    if(tileAt(this->playerX+1, this->playerY) == World::TILE_FLOOR)
      ++this->playerX;
  } else if (key == 97) {
    // left
    if(tileAt(this->playerX-1, this->playerY) == World::TILE_FLOOR)
      --this->playerX;
  } else if (key == 115) {
    // down
    if(tileAt(this->playerX, this->playerY+1) == World::TILE_FLOOR)
      ++this->playerY;
  } else if (key == 119) {
    // up
    if(tileAt(this->playerX, this->playerY-1) == World::TILE_FLOOR)
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
  radx /= 10;
  rady /= 6;
  int left = max(this->playerX-radx, 0);
  int right = min(this->playerX+radx, this->cols-1);
  int top = max(this->playerY-rady, 0);
  int bottom = min(this->playerY+rady, this->rows-1);
  for (int r = top; r <= bottom; ++r) {
    for (int c = left; c <= right; ++c) {
      int cx = (c - this->playerX + radx) * 5;
      int cy = (r - this->playerY + rady) * 3;
      if(r == this->playerY && c == this->playerX)
        drawTile(World::TILE_PLAYER, win, cx, cy, frame);
      else
        drawTile(this->tileAt(c, r), win, cx, cy, frame);
    }
  }

  // output everything to the screen
  wrefresh(win);

  // return 1 to continue running
  return 1;
}

void World::drawTile(char tile, WINDOW* win, int x, int y, int frame) {
  // the player
  if (tile == World::TILE_PLAYER) {
    int color = (frame / 2)%6 + 2;
    wattron(win, COLOR_PAIR(color));
    mvwaddstr(win, y + 0, x + 1,  " 0 ");
    mvwaddstr(win, y + 1, x + 1,  "+|+");
    mvwaddstr(win, y + 2, x + 1,  "/ \\");
  }
  // no floor
  else if (tile == World::TILE_FLOOR) {
  }
  // nice brick wall
  else if (tile == World::TILE_WALL) {
    wattron(win, COLOR_PAIR(2));
    mvwaddstr(win, y + 0, x, "|__|_");
    mvwaddstr(win, y + 1, x, "_|__|");
    mvwaddstr(win, y + 2, x, "|_|__");
  }
  // by default, just draw the character. basically, a shrug
  else {
    wattron(win, COLOR_PAIR(1));
    for(int c = 0; c < 5; ++c)
      for(int r = 0; r < 3; ++r)
        mvwaddch(win, y + r, x + r, tile);
  }
}
