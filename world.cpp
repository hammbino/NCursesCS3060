// Copyright 2014 nigel nelson

#include "./world.h"
#include "./interact.h"
#include "./character.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <string>

using std::string;
using std::vector;
using std::ifstream;

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
        World::TILE_REDBRICK: World::TILE_FLOOR;
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
  if (!fs.good()) throw "failed to open world file.";
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
  int numPeople = 0;
  for (int r = 0; r < rows; ++r) {
    string line;
    getline(fs, line);
    strncpy(this->tiles + (r * this->cols), line.c_str(), this->cols);
    // see if the player's location was declared in this line
    // or if there's a teleport we need to keep track of
    for (int c = 0; c < cols; ++c) {
      switch (line.at(c)) {
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
        case World::TILE_PERSON:
          ++numPeople;
          break;
      }
    }
  }
  // read in all teleports now
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
    for (int i = 0; i < t.rows; ++i) {
      fs.getline(line, t.cols + 1);
      t.lines.push_back(string(line));
    }
    delete[] line;
    signs.push_back(t);
  }
  for (int i = 0; i < numPeople; ++i) {
    Person t;
    fs >> t.x;
    fs.ignore(1);
    fs >> t.y;
    fs.ignore(1);
    fs.get(this->tiles[(t.y * this->cols) + t.x]);
    fs.ignore(1);
    fs >> t.index;
    fs.ignore(1);
    mapPeople.push_back(t);
  }
}

void World::changeFile(string filename) {
  delete[] this->tiles;
  teleps.clear();
  signs.clear();
  mapPeople.clear();
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
    int actresult = this->act(input);
    if (actresult == -1) break;
    // draw the things
    this->drawWorld(win, actresult > 0);
    /* no hud for now. I'll make it work later.
     * 
    if (actresult == 2)
      this->drawHUD(hud);
    wrefresh(hud);
    */
    // wait
    nanosleep(&sleeptime, &notused);
  }
}

int World::act(int key) {
  // quitting the game?
  if (key == 'q') {
    return -1;
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
  // if didn't move, return 0 to indicate no movement
  else
    return 0;
  // if it is... let's move!

  Telep* tp;
  Sign* sg;
  Person* pp;
  if ((tp = this->getTeleport(toX, toY)) != NULL) {
    // teleport?
    this->doTeleport(tp);
    return 2;
  } else if ((sg = this->getSign(toX, toY)) != NULL) {
    // sign?
    this->doSign(sg);
    return 1;
  } else if((pp = this->getPerson(toX, toY)) != NULL) {
    // person?
    this->doInteract(pp);
    return 2;
  } else if (passable(tileAt(toX, toY))) {
    // can move here?
    this->movePlayerTo(toX, toY);
    return 1;
  } else {
    // tried to move but can't
    return 0;
  }
}

void World::drawWorld(WINDOW* win, bool moved) {
  // frame number
  static int frame = -1;
  ++frame;
  // compare window width and height
  static int lastwidth = 0, lastheight = 0;
  int winwidth, winheight;
  getmaxyx(win, winheight, winwidth);

  // clear and redraw if moved or resized
  bool redraw = moved || lastwidth != winwidth || lastheight != winheight;
  if (redraw) {
    wclear(win);
  }

  // store new window size
  lastwidth = winwidth;
  lastheight = winheight;

  // hide cursor
  curs_set(0);

  // draw what we can see of the map
  int radx = (winwidth / 5 - 1) / 2;
  int rady = (winheight / 3 - 1) / 2;
  int offx = (winwidth - (radx * 2 + 1) * 5) / 2;
  int offy = (winheight - (rady * 2 + 1) * 3) / 2;
  bool change = false;
  for (int r = this->playerY - rady; r <= this->playerY + rady; ++r) {
    for (int c = this->playerX - radx; c <= this->playerX + radx; ++c) {
      int cx = (c - this->playerX + radx) * 5 + offx;
      int cy = (r - this->playerY + rady) * 3 + offy;
      if (r < 0 || r >= this->rows || c < 0 || c >= this->cols)
        change = drawTile(this->outTile, win, cx, cy, redraw, frame)
          || change;
      else if (r == this->playerY && c == this->playerX)
        change = drawTile(World::TILE_PLAYER, win, cx, cy, redraw, frame)
          || change;
      else
        change = drawTile(this->tileAt(c, r), win, cx, cy, redraw, frame)
          || change;
    }
  }

  // output everything to the screen
  if (change)
    wrefresh(win);
}

void World::drawHUD(WINDOW* win) {
  // using wborder instead of box, because box seems to look weird while
  // using putty to connect to the machine remotely?? idk idk idk
  wclear(win);
  wattron(win, COLOR_PAIR(5));
  wborder(win, '|', '|', '-', '_', '+', '+', 'L', 'J');
  mvwprintw(win, 2, 2, "\"%s\"", this->name.c_str());
}

bool World::passable(char t) {
  return t == World::TILE_FLOOR ||
    t == World::TILE_GRASS ||
    t == World::TILE_SAND ||
    t == World::TILE_PEBBLES ||
    t == World::TILE_FAKEWALL ||
    t == World::TILE_CLOUDGROUND;
}

World::Telep* World::getTeleport(int x, int y) {
  for (int i = 0; i < teleps.size(); ++i) {
    if (teleps[i].x == x && teleps[i].y == y)
      return &teleps[i];
  }
  return NULL;
}

World::Sign* World::getSign(int x, int y) {
  for (int i = 0; i < signs.size(); ++i) {
    if (signs[i].x == x && signs[i].y == y)
      return &signs[i];
  }
  return NULL;
}

World::Person* World::getPerson(int x, int y) {
  for (int i = 0; i < mapPeople.size(); ++i) {
    if (mapPeople[i].x == x && mapPeople[i].y == y)
      return &mapPeople[i];
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
  for (int i = 0; i < t->rows; ++i) {
    mvwprintw(win, i + 1, 2, t->lines[i].c_str());
  }
  wrefresh(win);
  // wait for user to press a key
  timeout(-1);
  getch();
  // done. kill window
  delwin(win);
}

void World::doInteract(Person* p) {
  Interact interact;
  interact.getDecision(people[p->index]);
}

bool World::drawTile(char tile, WINDOW* win, int x, int y,
    bool moved, int frame) {
  if (tile == World::TILE_FLOOR) {
    return false;
  } else if (tile == World::TILE_GRASS) {
    if (!moved) return false;
    wattrset(win, COLOR_PAIR(4));
    mvwaddstr(win, y + 0, x, ", ,,,");
    mvwaddstr(win, y + 1, x, " , ,,");
    mvwaddstr(win, y + 2, x, ",,,, ");
  } else if (tile == World::TILE_SAND) {
    if (!moved) return false;
    wattrset(win, COLOR_PAIR(3));
    mvwaddstr(win, y + 0, x, ".  . ");
    mvwaddstr(win, y + 1, x, " . ..");
    mvwaddstr(win, y + 2, x, ". . .");
  } else if (tile == World::TILE_PEBBLES) {
    if (!moved) return false;
    wattrset(win, 0);
    mvwaddstr(win, y + 0, x, "o .o ");
    mvwaddstr(win, y + 1, x, ". o .");
    mvwaddstr(win, y + 2, x, " .  o");
  } else if (tile == World::TILE_REDBRICK) {
    if (!moved) return false;
    wattrset(win, COLOR_PAIR(2) | A_REVERSE);
    mvwaddstr(win, y + 0, x, "|__|_");
    mvwaddstr(win, y + 1, x, "_|__|");
    mvwaddstr(win, y + 2, x, "|_|__");
  } else if (tile == World::TILE_ROCK) {
    if (!moved) return false;
    wattrset(win, A_REVERSE);
    mvwaddstr(win, y + 0, x + 1, "/.|");
    mvwaddstr(win, y + 1, x, "/..\\");
    mvwaddstr(win, y + 2, x, "\\...|");
  } else if (tile == World::TILE_TREE) {
    if (!moved) return false;
    wattrset(win, COLOR_PAIR(3) | A_REVERSE);
    mvwaddstr(win, y + 0, x + 1, "/\\");
    mvwaddstr(win, y + 1, x, "//\\\\");
    wattrset(win, COLOR_PAIR(5));
    mvwaddstr(win, y + 2, x + 1, "##");
  } else if (tile == World::TILE_CLOUD) {
    if (!moved) return false;
    wattrset(win, COLOR_PAIR(1) | A_REVERSE);
    mvwaddstr(win, y + 0, x, "( )( ");
    mvwaddstr(win, y + 1, x, "_) ()");
    mvwaddstr(win, y + 2, x, " ((_)");
  } else if (tile == World::TILE_CLOUDGROUND) {
    if (!moved) return false;
    wattrset(win, 0);
    mvwaddstr(win, y + 0, x, "( )( ");
    mvwaddstr(win, y + 1, x, "_) ()");
    mvwaddstr(win, y + 2, x, " ((_)");
  } else if (tile == World::TILE_WATER) {
    if (!moved && frame % 4 != 0) return false;
    static string water = "~   ~   ~   ~   ~   ";
    wattrset(win, COLOR_PAIR(7) | A_REVERSE);
    for (int i = 0; i < 3; ++i) {
      int pos = 10 + (frame/4 + y + x + i)%4 * (((y + i) % 2)? -1 : 1);
      mvwaddstr(win, y + i, x, water.substr(pos, 5).c_str());
    }
  } else if (tile == World::TILE_STAIRS) {
    if (!moved) return false;
    wattrset(win, A_REVERSE);
    mvwaddstr(win, y + 0, x + 4, "_");
    mvwaddstr(win, y + 1, x + 2, "___");
    mvwaddstr(win, y + 2, x, "     ");
  } else if (tile == World::TILE_SIGN) {
    if (!moved) return false;
    wattrset(win, COLOR_PAIR(3));
    mvwaddstr(win, y + 0, x, "+---+");
    mvwaddstr(win, y + 1, x, "|AAA|");
    mvwaddstr(win, y + 2, x, " _|_ ");
  } else if (tile == World::TILE_FAULTROCK) {
    if (!moved) return false;
    
  } else if (tile == World::TILE_PLAYER) {
    int color = frame % 6 + 2;
    wattrset(win, COLOR_PAIR(color));
    mvwaddstr(win, y + 0, x + 1,  " 0 ");
    mvwaddstr(win, y + 1, x + 1,  "+|+");
    mvwaddstr(win, y + 2, x + 1,  "/ \\");
  } else if (tile == World::TILE_PERSON) {
    wattrset(win, COLOR_PAIR(2));
    mvwaddstr(win, y + 0, x + 1,  "*>*");
    mvwaddstr(win, y + 1, x + 1,  "+|+");
    mvwaddstr(win, y + 2, x + 1,  "/ \\");
  } else if (tile == World::TILE_VENDOR) {
    wattrset(win, COLOR_PAIR(4));
    mvwaddstr(win, y + 0, x + 1,  " $ ");
    mvwaddstr(win, y + 1, x + 1,  "+|+");
    mvwaddstr(win, y + 2, x + 1,  "/ \\");
  } else if (tile == World::TILE_FRIENDLY) {
    wattrset(win, COLOR_PAIR(5));
    mvwaddstr(win, y + 0, x + 1,  " U ");
    mvwaddstr(win, y + 1, x + 1,  "+|+");
    mvwaddstr(win, y + 2, x + 1,  "/ \\");
  } else {
    if (!moved) return false;
    wattrset(win, A_REVERSE);
    for (int c = 0; c < 5; ++c)
      for (int r = 0; r < 3; ++r)
        mvwaddch(win, y + r, x + c, tile);
  }
  return true;
}
