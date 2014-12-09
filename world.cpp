// Copyright 2014 nigel nelson

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
  this->loadFile(filename);
}

World::~World() {
  delete[] this->tiles;
}

void World::loadFile(string filename) {
  // open stream
  ifstream fs(filename.c_str());
  if(!fs.good()) throw "failed to open world file.";
  // read in the name and size from first 3 lines
  getline(fs, this->name);
  fs >> this->cols;
  fs.ignore(1);
  fs >> this->rows;
  fs.ignore(1);
  // the tile on the outside aaarea
  fs.get(this->outTile);
  fs.ignore(1);
  // default player position; should be set later
  this->playerX = 2;
  this->playerY = 2;
  // allocate array for tiles
  this->tiles = new char[this->rows * this->cols];
  // load in the rows, line by line
  int numTeleps = 0;
  int numSigns = 0;
  for(int r = 0; r < rows; ++r) {
    string line;
    getline(fs, line);
    strncpy(this->tiles + (r * this->cols), line.c_str(), this->cols);
    // see if the player's location was declared in this line
    // or if there's a teleport we need to keep track of
    for (int c = 0; c < cols; ++c) {
      switch(line.at(c)) {
        case World::TILE_PLAYER:
          this->movePlayerTo(c, r);
          this->tiles[(r * this->cols) + c] = World::TILE_FLOOR;
          break;
        case World::TILE_TELEP:
          ++numTeleps;
          break;
        case World::TILE_SIGN:
          ++numSigns;
          break;
      }
    }
  }
  //read in all teleports now
  for (int i = 0; i < numTeleps; ++i) {
    Telep t;
    fs >> t.x;
    fs.ignore(1);
    fs >> t.y;
    fs.ignore(1);
    fs.get(this->tiles[(t.y * this->cols) + t.x]);
    fs.ignore(1);
    fs >> t.toFile;
    fs.ignore(1);
    fs >> t.toX;
    fs.ignore(1);
    fs >> t.toY;
    fs.ignore(1);
    teleps.push_back(t);
  }
  for (int i = 0; i < numSigns; ++i) {
    Sign t;
    fs >> t.x;
    fs.ignore(1);
    fs >> t.y;
    fs.ignore(1);
    fs.get(this->tiles[(t.y * this->cols) + t.x]);
    fs.ignore(1);
    fs >> t.cols;
    fs.ignore(1);
    fs >> t.rows;
    fs.ignore(1);
    char* line = new char[t.cols + 1];
    for(int i = 0; i < t.rows; ++i) {
      fs.getline(line, t.cols + 1);
      t.lines.push_back(string(line));
    }
    delete[] line;
    signs.push_back(t);
  }
}

void World::changeFile(string filename) {
  delete[] this->tiles;
  teleps.clear();
  signs.clear();
  this->loadFile(filename);
}

void World::movePlayerTo(int x, int y) {
  this->playerX = x;
  this->playerY = y;
}

char World::tileAt(int x, int y) {
  return *(this->tiles + y * this->cols + x);
}

void World::run(WINDOW* win) {
  // create some HUD windows
  WINDOW* hud = newwin(5, 20, 1, 1);

  // structs needed for sleeping
  struct timespec sleeptime, notused;
  sleeptime.tv_sec = 0;
  sleeptime.tv_nsec = (1000000000/15);

  while (1) {
    // key
    timeout(0);
    int input = getch();
    // clear out the rest of the buffer
    while (getch() != ERR) continue;
    // frame action
    int donext = this->act(input);
    // draw the things
    wclear(win);
    this->drawWorld(win);
    this->drawHUD(hud);
    // do next frame?
    if (!donext) break;
    // wait
    nanosleep(&sleeptime, &notused);
  }
}

int World::act(int key) {
  // quitting the game?
  if (key == 113) {
    return 0;
  }

  // moving?
  int toX = this->playerX;
  int toY = this->playerY;
  if (key == KEY_RIGHT || key == 'd' || key == 'l')
    ++toX;
  else if (key == KEY_LEFT || key == 'a' || key == 'h')
    --toX;
  else if (key == KEY_DOWN || key == 's' || key == 'j')
    ++toY;
  else if (key == KEY_UP || key == 'w' || key == 'k')
    --toY;
  // if it's neither of those, just return.
  else {
    return 1;
  }
  // if it is... let's move!

  Telep* tp;
  Sign* sg;
  // teleport?
  if ((tp = this->getTeleport(toX, toY)) != NULL) {
    this->doTeleport(tp);
  }
  // sign?
  else if((sg = this->getSign(toX, toY)) != NULL) {
    this->doSign(sg);
  }
  // otherwise just move.
  else if (passable(tileAt(toX, toY))) {
    this->movePlayerTo(toX, toY);
  }

  // return 1 to continue running
  return 1;
}

void World::drawWorld(WINDOW* win) {
  // frame number
  static int frame = -1;
  ++frame;

  // hide cursor
  curs_set(0);

  // draw what we can see of the map
  int winwidth, winheight;
  getmaxyx(win, winheight, winwidth);
  int radx = (winwidth / 5 - 1) / 2;
  int rady = (winheight / 3 - 1) / 2;
  int offx = (winwidth - (radx * 2 + 1) * 5) / 2;
  int offy = (winheight - (rady * 2 + 1) * 3) / 2;
  for (int r = this->playerY - rady; r <= this->playerY + rady; ++r) {
    for (int c = this->playerX - radx; c <= this->playerX + radx; ++c) {
      int cx = (c - this->playerX + radx) * 5 + offx;
      int cy = (r - this->playerY + rady) * 3 + offy;
      if(r < 0 || r >= this->rows || c < 0 || c >= this->cols)
        drawTile(this->outTile, win, cx, cy, frame);
      else if(r == this->playerY && c == this->playerX)
        drawTile(World::TILE_PLAYER, win, cx, cy, frame);
      else
        drawTile(this->tileAt(c, r), win, cx, cy, frame);
    }
  }

  // output everything to the screen
  wrefresh(win);
}

bool World::passable(char t) {
  return t == World::TILE_FLOOR ||
    t == World::TILE_GRASS;
}

World::Telep* World::getTeleport(int x, int y) {
  for(int i = 0; i < teleps.size(); ++i) {
    if(teleps[i].x == x && teleps[i].y == y)
      return &teleps[i];
  }
  return NULL;
}

World::Sign* World::getSign(int x, int y) {
  for(int i = 0; i < signs.size(); ++i) {
    if(signs[i].x == x && signs[i].y == y)
      return &signs[i];
  }
  return NULL;
}

void World::doTeleport(Telep* t) {
  int x = t->toX;
  int y = t->toY;
  string filename = t->toFile;
  this->changeFile(filename);
  this->movePlayerTo(x, y);
}

void World::doSign(Sign* t) {
  // figure out where to put the window
  int x, y;
  getmaxyx(stdscr, y, x);
  // create the window & display message
  WINDOW* win = newwin(t->rows + 2, t->cols + 4,
    (y - t->rows - 2) / 2, (x - t->cols - 4) / 2);
  wborder(win, '{', '}', '~', '~', '#', '#', '#', '#');
  for(int i = 0; i < t->rows; ++i) {
    mvwprintw(win, i + 1, 2, t->lines[i].c_str());
  }
  wrefresh(win);
  // wait for user to press a key
  timeout(-1);
  getch();
  // done. kill window
  delwin(win);
}

void World::drawTile(char tile, WINDOW* win, int x, int y, int frame) {
  // no floor
  if (tile == World::TILE_FLOOR) {
  }
  // nice brick wall
  else if (tile == World::TILE_WALL) {
    wattrset(win, COLOR_PAIR(2) | A_REVERSE);
    mvwaddstr(win, y + 0, x, "|__|_");
    mvwaddstr(win, y + 1, x, "_|__|");
    mvwaddstr(win, y + 2, x, "|_|__");
  }
  // trees!
  else if (tile == World::TILE_TREE) {
    wattrset(win, COLOR_PAIR(3) | A_REVERSE);
    mvwaddstr(win, y + 0, x + 1, "/\\");
    mvwaddstr(win, y + 1, x, "//\\\\");
    wattrset(win, COLOR_PAIR(5));
    mvwaddstr(win, y + 2, x + 1, "##");
  }
  // water
  else if(tile == World::TILE_WATER) {
    static string water = "~   ~   ~   ~   ~   ";
    wattrset(win, COLOR_PAIR(7) | A_REVERSE);
    for (int i = 0; i < 3; ++i) {
      int pos = 10 + (frame/4 + y + x + i)%4 * (((y + i) % 2)? -1 : 1);
      mvwaddstr(win, y + i, x, water.substr(pos, 5).c_str());
    }
  }
  // stairs
  else if(tile == World::TILE_STAIRS) {
    wattrset(win, A_REVERSE);
    mvwaddstr(win, y + 0, x + 4, "_");
    mvwaddstr(win, y + 1, x + 2, "___");
    mvwaddstr(win, y + 2, x, "     ");
  }
  // signs
  else if(tile == World::TILE_SIGN) {
    wattrset(win, COLOR_PAIR(3));
    mvwaddstr(win, y + 0, x, "+---+");
    mvwaddstr(win, y + 1, x, "|AAA|");
    mvwaddstr(win, y + 2, x, " _|_ ");
  }
  // the player
  else if (tile == World::TILE_PLAYER) {
    int color = frame % 6 + 2;
    wattrset(win, COLOR_PAIR(color));
    mvwaddstr(win, y + 0, x + 1,  " 0 ");
    mvwaddstr(win, y + 1, x + 1,  "+|+");
    mvwaddstr(win, y + 2, x + 1,  "/ \\");
  }
  // by default, just draw the character. basically, a shrug
  else {
    wattrset(win, A_REVERSE);
    for(int c = 0; c < 5; ++c)
      for(int r = 0; r < 3; ++r)
        mvwaddch(win, y + r, x + c, tile);
  }
}

void World::drawHUD(WINDOW* win) {
  // using wborder instead of box, because box seems to look weird while
  // using putty to connect to the machine remotely?? idk idk idk
  wclear(win);
  wattron(win, COLOR_PAIR(5));
  wborder(win, '|', '|', '-', '_', '+', '+', 'L', 'J');
  mvwprintw(win, 2, 2, "\"%s\"", this->name.c_str());
  wrefresh(win);
}
