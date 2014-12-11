//Copyright Jeff Hammond/ Richard Larsen 2014

#include "curses.h"
#include "Interact.h"
#include <sstream>

void Interact::getDecision(character* person) {
  setOptions(person);
  WINDOW *menu_win;
  WINDOW *result_win;
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
  startx = (winwidth - MENU_WIDTH);
  starty = (winheight - MENU_HEIGHT);

  menu_win = newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);
  startx = (winwidth - RESULT_WIDTH);
  starty = (winheight - RESULT_HEIGHT);
  result_win = newwin(RESULT_HEIGHT,RESULT_WIDTH,starty,startx);

  keypad(menu_win, TRUE);
  //mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
  //refresh();

  print_results(result_win);
  if (person->encounterDone > 0) {
    //No introduction
    
    if (person->type == "Student") { 
      // If an enemy, display beaten message

	  return;
	}
  }
  print_menu(menu_win, highlight);

  while(tugOfWarBar > -6 && tugOfWarBar < 6/*&& choice != 4*/) {
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
    if(choice != 0)	{
	  if (n_choices == 4) {
        buy(result_win, person, choice);
        //this will continue to be called until choice == 4
      } else {
        fight(result_win, person, choice);
      }
    
    }
  }
  if (tugOfWarBar <= -6) {
    mvwprintw(result_win,1,1,"I'm the best! Go home!\n");
	wrefresh(result_win);
  }
  else if (tugOfWarBar >= 6) {
    person->encounterDone++;
    mvwprintw(result_win,1,1,"You're too lucky! WAAAAAA!\n");
	wrefresh(result_win); 
  }
  while (getch() != '10') {
	mvwprintw(result_win,4,1,"Press enter to continue");
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
	mvwprintw(result_win,3,1, "Tug of war bar = %d ", tugOfWarBar);
	wrefresh(result_win);

  } else if (enemy->rockGrade == 0 && enemy->scissorsGrade == 0) { // it is an enemy with only paper
	p2 = paper;
	winner = (p1 - p2 + 3) % 3;
	tugOfWarBar += fightLogic(playerGrade, winner, enemy->paperGrade);
    mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->paperName.c_str());
	mvwprintw(result_win,3,1, "Tug of war bar = %d ", tugOfWarBar);
	wrefresh(result_win);

  } else if (enemy->paperGrade == 0 && enemy->scissorsGrade == 0) { // it is an enemy with only rock
	p2 = rock;
	winner = (p1 - p2 + 3) % 3;
    tugOfWarBar += fightLogic(playerGrade, winner, enemy->rockGrade);
	mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->rockName.c_str());
	mvwprintw(result_win,3,1, "Tug of war bar = %d ", tugOfWarBar);
	wrefresh(result_win);

  } else { // enemy has all weapons choose random
	p2 = rand() % 3; // computer chooses
	if (p2 == 0) { // computer chose rock
	  winner = (p1 - p2 + 3) % 3;
	  tugOfWarBar += fightLogic(playerGrade, winner, enemy->rockGrade);
	  mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	  mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->rockName.c_str());
	  mvwprintw(result_win,3,1, "Tug of war bar = %d ", tugOfWarBar);
	  wrefresh(result_win);
	} else if (p2 == 1) { // computer chose paper
	  winner = (p1 - p2 + 3) % 3;
	  tugOfWarBar += fightLogic(playerGrade, winner, enemy->paperGrade);
	  mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	  mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->paperName.c_str());
	  mvwprintw(result_win,3,1, "Tug of war bar = %d ", tugOfWarBar);
	  wrefresh(result_win);

	} else { // computer chose scissors
	  winner = (p1 - p2 + 3) % 3;
	  tugOfWarBar += fightLogic(playerGrade, winner, enemy->scissorsGrade);
	  mvwprintw(result_win,1,1, "%s used %s\n", player->name.c_str(), playerWeap.c_str());
	  mvwprintw(result_win,2,1, "%s used %s\n", enemy->name.c_str(), enemy->scissorsName.c_str());
	  mvwprintw(result_win,3,1, "Tug of war bar = %d ", tugOfWarBar);
	  wrefresh(result_win);
	}
  }

}

void Interact::buy(WINDOW *result_win, character* merchant, int choice) {
    int cost = 200; //(merchant->grade - 1) * 100;
    bool bought = FALSE;
	int rock = 1;
	int paper = 2;
	int scissors = 3;

  // Put weapons for player in an array

  // Put weapons for merchant in an array

  // Display the players weapons

  // Dislpay the merchant's weapons
    
  // Display question to user asking if they would like to buy anything
    mvwprintw(result_win,0,0, "Are you here to buy or waste my time?");
  // Get user input

  // Begin while loop while user input is y

  // Ask user what they would like to buy
    mvwprintw(result_win,0,0, "Select anything anything you'd like");
  // Get user input

  // Compare items cost to the money the user has
    if (cost > player->money) {
      mvwprintw(result_win,0,0, "Do I look like the charitable type to you? Don't come back without a bigger wallet.");
    }
  // If player has enough money confirm purchase
    else {
      mvwprintw(result_win,0,0, "Excellent choice, that should help you beat the others.");
        player->money - cost;
        bought = TRUE;
        if (choice == rock) {
            player->rockGrade = merchant->rockGrade;
            player->rockName = merchant->rockName;
        } else if (choice = paper) {
            player->paperGrade = merchant->paperGrade;
            player->paperName = merchant->paperName;
        } else {
            player->scissorsGrade = merchant->scissorsGrade;
            player->scissorsName = merchant->scissorsName;
        }
        //update player info on screen money and equipment
    }
  
  // If player does not have enough money display error message

  // If player  has enough money  confirm purchase and subtract cost of item from players purse and equip weapon

  // Ask user if they would like to make another purchase

  // Get user's input

  // End while loop

  // Display message to user indicating exiting store
    if (bought)
        mvwprintw(result_win,1,1, "Thanks for the money. I'll see you again");
    else
        mvwprintw(result_win,1,1, "Good luck winning without my help.");
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

void Interact::print_results(WINDOW *result_win) {
  box(result_win,0,0);
//  mvwprintw(result_win,1,1,"Greetings!\n");
  wrefresh(result_win);
}


void Interact::setOptions(character* options) {
  int price = (options->rockGrade -1) * 100;
  ostringstream priceString;
  priceString << price;
  
  if(options->type == "Vendor") {
	n_choices = 4;
	choices[0] = options->rockName +  "$" + priceString.str();
	choices[1] = options->paperName + "$" + priceString.str();
	choices[2] = options->scissorsName + "$" + priceString.str();
	choices[3] = "Exit";

  } else {
	n_choices = 3;
	choices[0] = player->rockName;
	choices[1] = player->paperName;
	choices[2] = player->scissorsName;
  }
}
