//Copyright Jeff Hammond/ Richard Larsen 2014

#include "curses.h"
#include "character.h"
#include <sstream>
#include <string>
using namespace std;
 
#define MENU_WIDTH 30
#define MENU_HEIGHT 10

#define RESULT_WIDTH 80
#define RESULT_HEIGHT 10

int tugOfWarBar = 0;
int startx = 0;
int starty = 0;

string choices[4];
int n_choices;

character* player = new character(2,2,2,10,"Player","Dude","Rock","Paper","Scissors", 0);

class Interact {

public:
  int fightLogic(int userGrade, int winner, int enemyGrade);
  void setOptions(character*);
  void fight(WINDOW*, character*, int);
  void buy(WINDOW*, character*, int);
  void getDecision(character*);
  void print_menu(WINDOW *menu_win, int highlight);
  void print_results(WINDOW *result_win);
};

