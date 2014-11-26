// Copyright 2014 Nigel Nelson
//
// WORLD.H
//
// Describes the overworld the player will walk around in.

#pragma once
#include <string>
#include <curses.h>

using namespace std;

const char TILE_WALL = 'X';
const char TILE_FLOOR = ' ';

class World {
  private:
    string name;
    char * tiles;
    int cols, rows;
    int playerX, playerY;
    int doFrame(WINDOW* window, int key);
    char tileAt(int x, int y);

  public:
    World();
    World(string filename);
    void run(WINDOW* window);

};
