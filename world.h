// Copyright 2014 Nigel Nelson
//
// WORLD.H
//
// 

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
    void drawTile(char tile, WINDOW* window, int x, int y, int frame);

  public:
    static const char TILE_WALL = 'X';
    static const char TILE_FLOOR = ' ';
    static const char TILE_PLAYER = 'p';

    World();
    World(string filename);
    ~World();
    void run(WINDOW* window);

};
