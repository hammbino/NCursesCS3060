//Copyright Jeff Hammond/ Richard Larsen 2014

#include "curses.h"
#include "interact.h"
#include <stdlib.h>
#include <sstream>
#include <iostream>

using namespace std;

int tugOfWarBar = 0;

bool bought = FALSE;

string choices[4];
int n_choices;

//void Interact::fight_or_buy(character* person) {
  //
//}

void Interact::getDecision(character* person) {
  WINDOW *character_win;
  WINDOW *menu_win;
  WINDOW *result_win;
  setOptions(person);
  int startx = 0;
  int starty = 0;
  int highlight = 1;
  int choice;
  int c;
  int winwidth = 0;
  int winheight = 0;

  initscr();
  clear();
  noecho();
  cbreak();	/* Line buffering disabled. pass on everything */
  getmaxyx(stdscr, winheight, winwidth);
  //startx = (winwidth - MENU_WIDTH);
  //starty = (winheight - MENU_HEIGHT);

  startx = (winwidth * .25);
  starty = (winheight * .1);
  character_win = newwin(winheight * .4, winwidth * .5, starty, startx);
  print_character(character_win);

  startx = (winwidth * .8);
  starty = (winheight * .75);

  menu_win = newwin(winheight - starty, winwidth - startx, starty, startx);
  startx = (winwidth - winwidth * .99);
  starty = (winheight - winheight * .25);
  result_win = newwin(winheight - starty, winwidth - winwidth *.25, starty, startx);

  keypad(menu_win, TRUE);
  //mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
  //refresh();

  print_results(result_win, person);

  if (person->encounterDone > 0) {
    //No introduction
    if (person->type == "Professor") {
	  // This is Prof. Wright. Fight him the 2nd time
	   } else if (person->type == "Student") {
      // If an enemy, display beaten message
      return;
    }
  }
  print_menu(menu_win, highlight);

  if (n_choices == 4) {
    mvwprintw(result_win,1,1, "$%d\n", player->money);
    mvwprintw(result_win,2,1, "Are you here to buy or waste my time?");
    box(result_win,0,0);
    wrefresh(result_win);

	while(choice != 4) {
    print_menu(menu_win, highlight);
    c = wgetch(menu_win);
    choice = 0;
    switch(c) {
      case KEY_UP:
      case 'w':
      case 'k':
        if(highlight == 1)
          highlight = n_choices;
        else
          --highlight;
          break;
      case KEY_DOWN:
      case 'j':
      case 's':
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
      if(choice != 0 && choice != 4) {
        buy(result_win, person, choice);
      }
      if (bought && choice != 4) {

      } else {
        // Display message to user indicating exiting store
        if (bought) {
          mvwprintw(result_win,1,1, "Thanks for the money. I'll see you again.    ");
		      mvwprintw(result_win,2,1, "                                             ");
		      mvwprintw(result_win,3,1, "                                             ");
          box(result_win,0,0);
		      wrefresh(result_win);
		      cin.get();
		    } else if (choice == 4 && !bought) {
          mvwprintw(result_win,1,1, "Good luck winning without my help.           ");
		      mvwprintw(result_win,2,1, "                                             ");
		      mvwprintw(result_win,3,1, "                                             ");
          box(result_win,0,0);
		      wrefresh(result_win);
		       cin.get();
		      } /*else - this means they bought something and they selected exit*/
	      }
      }
    } else {
      while(tugOfWarBar > -6 && tugOfWarBar < 6) {
        print_menu(menu_win, highlight);
        c = wgetch(menu_win);
        choice = 0;
        switch(c) {
          case KEY_UP:
          case 'w':
          case 'k':
            if(highlight == 1)
             highlight = n_choices;
            else
              --highlight;
            break;

           case KEY_DOWN:
           case 'j':
           case 's':
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
      if(choice != 0)
        fight(result_win, person, choice);
    }
    if (tugOfWarBar <= -6) {
      mvwprintw(result_win,1,1,"I'm the best! Go home!\n\n\n");
      box(result_win,0,0);
      wrefresh(result_win);
    } else if (tugOfWarBar >= 6) {
      person->encounterDone++;
      mvwprintw(result_win,1,1,"You're too lucky! WAAAAAA!\n\n\n");
      box(result_win,0,0);
      wrefresh(result_win);
    }

	cin.get();
	endwin(character_win);
	endwin(menu_win);
	endwin(result_win);
  }



//  clrtoeol();
//  refresh();
//  getch();
//  endwin();

}

int Interact::fightLogic(int userGrade, int winner, int enemyGrade) {
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

void Interact::fight(WINDOW *result_win, character* enemy, int choice) {  
  int rock = 0;
  int paper = 1;
  int scissors = 2;
  int p1, p2;
  int playerGrade;
  string playerWeap;
  int winner = 0;
  
  if (choice == 1) {// this means player chose rock
    p1 = rock;
	  playerGrade = player->rockGrade;
	  playerWeap = player->rockName;
  } else if (choice == 2) { // this means player chose paper
	   p1 = paper;
	   playerGrade = player->paperGrade;
	   playerWeap = player->paperName;
  } else {  // player chose scissors
	   p1 = scissors;
	   playerGrade = player->scissorsGrade;
	   playerWeap = player->scissorsName;
  }

  if (enemy->rockGrade == 0 && enemy->paperGrade == 0) { // it is an enemy with only scissors
  	 p2 = scissors;
	   winner = (p1 - p2 + 3) % 3;
	   tugOfWarBar += fightLogic(playerGrade, winner, enemy->scissorsGrade);
	   mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	   mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->scissorsName.c_str());
	   box(result_win,0,0);
     wrefresh(result_win);
  } else if (enemy->rockGrade == 0 && enemy->scissorsGrade == 0) { // it is an enemy with only paper
	p2 = paper;
	 winner = (p1 - p2 + 3) % 3;
	 tugOfWarBar += fightLogic(playerGrade, winner, enemy->paperGrade);
   mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	 mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->paperName.c_str());
   box(result_win,0,0);
   wrefresh(result_win);
  } else if (enemy->paperGrade == 0 && enemy->scissorsGrade == 0) { // it is an enemy with only rock
	 p2 = rock;
	 winner = (p1 - p2 + 3) % 3;
   tugOfWarBar += fightLogic(playerGrade, winner, enemy->rockGrade);
	 mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	 mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->rockName.c_str());
	 box(result_win,0,0);
   wrefresh(result_win);
  } else { // enemy has all weapons choose random
    p2 = rand() % 3; // computer chooses
	  if (p2 == 0) { // computer chose rock
	    winner = (p1 - p2 + 3) % 3;
	    tugOfWarBar += fightLogic(playerGrade, winner, enemy->rockGrade);
	    mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	    mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->rockName.c_str());
      box(result_win,0,0);
      wrefresh(result_win);
	  } else if (p2 == 1) { // computer chose paper
	    winner = (p1 - p2 + 3) % 3;
	    tugOfWarBar += fightLogic(playerGrade, winner, enemy->paperGrade);
	    mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	    mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->paperName.c_str());
  	  box(result_win,0,0);
      wrefresh(result_win);
  	} else { // computer chose scissors
	    winner = (p1 - p2 + 3) % 3;
	    tugOfWarBar += fightLogic(playerGrade, winner, enemy->scissorsGrade);
	    mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	    mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->scissorsName.c_str());
  	  box(result_win,0,0);
      wrefresh(result_win);
	  }
  }
  // ************** 
  //I don't understand the hard coded 6
  mvwprintw(result_win,4,1, "6 you win     -6 you lose = %d \n", tugOfWarBar);
  box(result_win,0,0);
  wrefresh(result_win);
}

void Interact::buy(WINDOW *result_win, character* merchant, int choice) {
  int cost = (merchant->rockGrade - 1) * 100;
  int rock = 1;
  int paper = 2;
  int scissors = 3;
  string ownedWeapons[3];

  // player weapons
  ownedWeapons[0] = player->rockName;
  ownedWeapons[1] = player->paperName;
  ownedWeapons[2] = player->scissorsName;
  // output player money
  mvwprintw(result_win,1,1, "$%d\n", player->money);
  box(result_win,0,0);
  wrefresh(result_win);
  //check grade if weapon is weaker or equal dont allow them to buy

  // Compare items cost to the money the user has
  if (cost > player->money) {
    mvwprintw(result_win,2,1, "Do I look like the charitable type to you?\n");
	mvwprintw(result_win,3,1, "Don't come back without a bigger wallet.\n");
	wrefresh(result_win);
  } else {
    mvwprintw(result_win,2,1, "Excellent choice, that should help you beat\n");
	mvwprintw(result_win,3,1, "the others.\n");
	player->money = player->money - cost;
	mvwprintw(result_win,1,1, "$%d\n", player->money);
  box(result_win,0,0);
	wrefresh(result_win);
    
    bought = TRUE;
    if (choice == rock) {
      player->rockGrade = merchant->rockGrade;
      player->rockName = merchant->rockName;
    } else if (choice == paper) {
      player->paperGrade = merchant->paperGrade;
      player->paperName = merchant->paperName;
    } else {
      player->scissorsGrade = merchant->scissorsGrade;
      player->scissorsName = merchant->scissorsName;
    }
    // Update player money
	mvwprintw(result_win,4,1, "%d %s\n", player->rockGrade, player->rockName.c_str());
	mvwprintw(result_win,5,1, "%d %s\n", player->paperGrade, player->paperName.c_str());
	mvwprintw(result_win,6,1, "%d %s\n", player->scissorsGrade, player->scissorsName.c_str());
	wrefresh(result_win);
  }
}

void Interact::print_menu(WINDOW *menu_win, int highlight) {
  int x, y, i;
  x = 2;
  y = 2;
  box(menu_win, 0, 0);
  for(i = 0; i < n_choices; ++i) {
    if(highlight == i + 1) {
      wattron(menu_win, A_REVERSE); 
      mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
      wattroff(menu_win, A_REVERSE);
    } else 
      mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
    ++y;
  }
  wrefresh(menu_win);
}

void Interact::print_results(WINDOW *result_win, character* person) {
  
  //*******
  //instead of mvwprintw in fight I think strings should be passed to this function
  //I don't think was want any text hard coded. It should pull from a file.
  //So this can be used over and over again.
  mvwprintw(result_win,1,2,"You approach %s\n", person->name.c_str());
  box(result_win,0,0);
  wrefresh(result_win);
}

void Interact::print_character(WINDOW *character_win) {
  box(character_win,0,0);
  // NEED HELP TO PRINT TEXT FILE
  //mvwprintw(character_win, 1, 1, ); 
  wrefresh(character_win);
}


void Interact::setOptions(character* options) {
  int price = (options->rockGrade -1) * 100;
  ostringstream priceString;
  priceString << price;
  
  if(options->type == "Vendor") {
	n_choices = 4;
	choices[0] = options->rockName +  " $" + priceString.str();
	choices[1] = options->paperName + " $" + priceString.str();
	choices[2] = options->scissorsName + " $" + priceString.str();
	choices[3] = "Exit";

  } else {
	n_choices = 3;
	choices[0] = player->rockName;
	choices[1] = player->paperName;
	choices[2] = player->scissorsName;
  }
}
