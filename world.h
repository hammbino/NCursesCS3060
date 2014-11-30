// Copyright 2014 Nigel Nelson
//
// WORLD.H
//
// Describes the overworld the player moves around in. For now, it only
// represents a single room. The room data can be provided via a file, whose
// path should be passed in as an argument. 
//
// When run() is called, it will display the world in the given window, and
// will allow the player to move their character around the screen. 

#pragma once
#include <string>
#include <curses.h>

using namespace std;

class World {
  private:
    // name of the room
    string name;
    // array of tile data for the room
    int cols, rows;
    char * tiles;
    // location of the player on the map (top-left based)
    int playerX, playerY;

    // execute a single frame of the game. called by run().
    //  will take input for the user and update the player's location.
    //  will also spawn other interaction windows if the player talks to an
    //    NPC character.
    //  returns 1 if it should continue running, 0 if it is done
    int act(int key);
    // draw the world in its current state. called by run().
    //  will draw as many tiles as possible in the given window.
    void drawWorld(WINDOW* window);
    // draw the HUD at the top of the screen. called by run().
    //  will draw into the window specified.
    void drawHUD(WINDOW* window);
    // execute a single frame of the game. called by run() repeatedly.
    //  will take in input from the user, update the player's position,
    //  and redraw the screen to the given window.
    int doFrame(WINDOW* window, int key);
    // get the char representing the tile at that point on the map
    char tileAt(int x, int y);
    // draw a single tile somewhere in the given window.
    //  tile: the type of tile to draw (see World's member constants)
    //  window: the window to draw the tile in
    //  x: the leftmost column the tile will occupy
    //  y: the topmost row the tile will occupy
    //  frame: how many frames have been executed by doFrame. can be
    //    used to animate a tile.
    void drawTile(char tile, WINDOW* window, int x, int y, int frame);

  public:
    // constants representing the types of tiles
    static const char TILE_WALL = 'X';
    static const char TILE_FLOOR = ' ';
    static const char TILE_PLAYER = 'p';
    // default world constructor
    //  creates an 8x8 closed-off room with the player inside.
    World();
    // parameterized world constructor
    //  loads data from a file to create a world.
    World(string filename);
    // the deconstructor must delete the tile array
    ~World();
    // display the world in the given window, and allow the player
    // to move their character around the screen
    void run(WINDOW* window);

};
