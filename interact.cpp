// Copyright Jeff Hammond
// 11/24/14
// interact.cpp

#include "Interact.h"
#include <stdio.h>
#include <ncurses.h>

char *choices[] = { 
      "Choice 1",
      "Choice 2",
      "Choice 3",
      "Choice 4",
      "Exit",
      };

int fightLogic(int userGrade, int winner, int enemyGrade) {
  	int gradeCompare = userGrade - enemyGrade;
	int difference = 2;
    int result = 0;
    if (winner == 0) { // draw
        result = gradeCompare + 0;
    } else if (winner == 1) { // player wins
        result = gradeCompare + difference;
    } else { // computer wins
        result = gradeCompare - difference;
    }
	return result;
}

void Interact::fight(character* player, character* enemy) {
  // Do we need to store hit points?

  // Put weapons for player in an array.

  // Put weapns for enemy in an array.

  // Display players weapons

  // Display enemy weapons

  // Begin while loop while player or enemy hit points do not equal zero
  while(tugOfWarBar < -6 && tugOfWarBar > 6) {
    int rock = 0;
	int paper = 1;
	int scissors = 2;
	int p1, p2;

	// players choose this uses the window

	int winner = (p1 - p2 + 3) % 3;
	tugOfWarBar += fightLogic(/*player->rockpaperscissorsgrade*/, winner, /*enemy->rockpaperscissorsgrade*/);
    
  }
  // Get users selection for weapon to use

  // Randomly generat which weapon the enemy will use

  // Compare the weapons used between the player and the enemy

  // Remove appropriate hit points from the player an enemy

  // If either player or enemy's hit points are zero set the opponent as the victor.

  // End while loop

  // Display the victor to the user

  // Update the players money if they are the victor
 
  // Return
}

void Interact::buy(character* player, character* merchant) {

WINDOW *menu_win;
  int highlight = 1;
  int choice = 0;
  int c;

  initscr();
  clear();
  noecho();
  cbreak(); // Line buffering disabled. pass on everything 
  startx = (80 - WIDTH) / 2;
  starty = (24 - HEIGHT) / 2;
    
  menu_win = newwin(HEIGHT, WIDTH, starty, startx);
  keypad(menu_win, TRUE);
  mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
  refresh();
  print_menu(menu_win, highlight);
  while(1)
  { c = wgetch(menu_win);
    switch(c)
    { case KEY_UP:
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
      default:
        mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
        refresh();
        break;
    }
    print_menu(menu_win, highlight);
    if(choice != 0) /* User did a choice come out of the infinite loop */
      break;
  } 
  mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
  clrtoeol();
  refresh();
  endwin();
  return 0;
}

  // Put weapons for player in an array

  // Put weapons for merchant in an array

  // Display the players weapons

  // Dislpay the merchant's weapons

  // Display question to user asking if they would like to buy anything

  // Get user input

  // Begin while loop while user input is y

  // Ask user what they would like to buy

  // Get user input

  // Compare items cost to the money the user has

  // If player has enough money confirm purchase
  
  // If player does not have enough money display error message

  // If player  has enough money  confirm purchase and subtract cost of item from players purse and equip weapon

  // Ask user if they would like to make another purchase

  // Get user's input

  // End while loop

  // Display message to user indicating exiting store

  // return void
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0 , 0);   // 0, 0 y, x coordinates
  wrefresh(local_win);

  return local_win;

}

void print_menu(WINDOW *menu_win, int highlight)
{
  int x, y, i;  

  x = 2;
  y = 2;
  box(menu_win, 0, 0);
  for(i = 0; i < n_choices; ++i)
  { if(highlight == i + 1) /* High light the present choice */
    { wattron(menu_win, A_REVERSE); 
      mvwprintw(menu_win, y, x, "%s", choices[i]);
      wattroff(menu_win, A_REVERSE);
    }
    else
      mvwprintw(menu_win, y, x, "%s", choices[i]);
    ++y;
  }
  wrefresh(menu_win);
}
