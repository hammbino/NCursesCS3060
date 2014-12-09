// Copyright 2014 Nigel Nelson
//
// WORLD.H
//
// Describes the world a player can move around in. Data can be loaded in
// from a file, containing the data for a specific map. As the player
// navigates the world, it might enter a new map, and so load in some new
// data. As the player explores they may also find students and vendors to
// interact with; this will call out to their specific functions, waiting for
// the interaction to finish.
//
// To begin running the world logic, simply call run(). This will display
// the world in the window given.

#pragma once
#include <string>
#include <vector>
#include <curses.h>

using namespace std;

class World {
  private:
    // teleport class
    //  store where on the map a teleport is, and where it goes to
    class Telep {
      public:
        int x, y;
        int toX, toY;
        string toFile;
    };
    // sign class
    //  stores where on the map a sign is, and what its message is
    class Sign {
      public:
        int x, y;
        int cols, rows;
        vector<string> lines;
    };
    // name of the room
    string name;
    // array of tile data for the room
    int cols, rows;
    char * tiles;
    // what tile is the outside?
    char outTile;
    // location of the player on the map (top-left based)
    int playerX, playerY;
    // vectors containing various objects
    vector<Telep> teleps;
    vector<Sign> signs;

    // load in a world
    void loadFile(string filename);
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
    // is the player able to pass over this kind of tile?
    bool passable(char tile);
    // functions to check if there is some object there
    Telep* getTeleport(int x, int y);
    Sign* getSign(int x, int y);
    // actually do teleport!
    //  unloads the current map and loads the new one, then sets the
    //  player's location.
    void doTeleport(Telep*);
    // display sign message
    void doSign(Sign*);

  public:
    // constants representing the types of tiles
    static const char TILE_FLOOR = ' ';
    static const char TILE_WALL = 'X';
    static const char TILE_TREE = 'T';
    static const char TILE_WATER = '~';
    static const char TILE_GRASS = ',';
    static const char TILE_STAIRS = '/';
    static const char TILE_PLAYER = 'p';
    static const char TILE_TELEP = '!';
    static const char TILE_SIGN = ':';
    // default world constructor
    //  creates an 8x8 closed-off room with the player inside.
    World();
    // parameterized world constructor
    //  loads data from a file to create a world.
    World(string filename);
    // the deconstructor must delete the tile array
    ~World();
    // move player to a position
    void movePlayerTo(int x, int y);
    // change to a different world file
    //  note that the player position will be set to the specified location
    //  in the file, or to a default location.
    void changeFile(string filename);
    // display the world in the given window, and allow the player
    // to move their character around the screen
    void run(WINDOW* window);

};
