#include "curses.h"
#include "character.h"
#include <sstream>
 
#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

string choices[4];
int n_choices;
void print_menu(WINDOW *menu_win, int highlight);
void setOptions(character*);
void fight(character*);
void buy(character*);
int getDecision(character*);

int getDecision(character* person) {
  	setOptions(person);
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;
	int winwidth = 0;
	int winheight = 0;
	
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	getmaxyx(stdscr, winheight, winwidth);
	startx = (winwidth - WIDTH);
	starty = (winheight - HEIGHT);
	
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
	refresh();
	print_menu(menu_win, highlight);
	//char input;
	//while (input == 'y') {
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else 
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}
	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	clrtoeol();
	refresh();
	getch();
	endwin();
}

void fight(character* enemy) {

}

void buy(character* vendor) {

}

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;
	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
		++y;
	}
	wrefresh(menu_win);
}

void setOptions(character* options) {
  int price = (options->rockGrade -1) * 100;
  ostringstream priceString;
  priceString << price;
  
  if(options->type == "Vendor") {
	n_choices = 4;
	choices[0] = options->rockName + priceString.str();
	choices[1] = options->paperName + priceString.str();
	choices[2] = options->scissorsName + priceString.str();
	choices[3] = "Exit";

  } else {
	n_choices = 3;
	choices[0] = options->rockName;
	choices[1] = options->paperName;
	choices[2] = options->scissorsName;
  }
}