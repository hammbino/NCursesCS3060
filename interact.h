//Copyright Jeff Hammond/ Richard Larsen 2014

#pragma once

#include "curses.h"
#include "character.h"
#include <sstream>
#include <string>
using namespace std;
 
#define MENU_WIDTH 30
#define MENU_HEIGHT 10

#define RESULT_WIDTH 80
#define RESULT_HEIGHT 10

extern int tugOfWarBar;
extern int startx;
extern int starty;
extern bool bought;

extern string choices[4];
extern int n_choices;

class Interact {

public:
  int fightLogic(int userGrade, int winner, int enemyGrade);
  void setOptions(character*);
  void fight(WINDOW*, character*, int);
  void buy(WINDOW*, character*, int);
  void getDecision(character*);
  void print_character(WINDOW *character_win, character* person);
  void print_menu(WINDOW *menu_win, int highlight);
  void print_results(WINDOW *result_win, character*);
  void firstFight(character* person);
};
