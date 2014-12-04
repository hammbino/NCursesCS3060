#include "curses.h"
#include "character.h"
 
#define WIDTH 30
#define HEIGHT 10 

int startx = 0;
int starty = 0;

const char *choices[4];
int n_choices;
void print_menu(WINDOW *menu_win, int highlight);
void setOptions(character*);
void fight(character*);
void buy(character*);

void fight(character* enemy) {
	setOptions(enemy);
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;
	
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

void buy(character* vendor) {
	setOptions(vendor);
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;
	
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

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;
	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

void setOptions(character* options) {
  int index = 0;
  const char * rock = options->rockName.c_str();
  const char * paper = options->paperName.c_str();
  const char * scissors = options->scissorsName.c_str();
  if(options->type == "Vendor") {
	n_choices = 4;
	choices[index] = rock;
	index++;
	choices[index] = paper;
	index++;
	choices[index] = scissors;
	index++;
	choices[index] = "Exit";
  }

  if(options->type == "Player") {
	n_choices = 3;
    choices[index] = rock;
	index++;
	choices[index] = paper;
	index++;
	choices[index] = scissors;

  }
}