#include<string>
#include<iostream>
#include<vector>
#include"character.h"
using namespace std;

void populateCharacters();
vector<character*> people;

int main() {

  character* person;
  
  populateCharacters();
  person = people.at(0);

  cout << person->rockGrade << person->paperGrade << person->scissorsGrade << person->type 
	  << person->name << person->rockName << person->paperName << person->scissorsName << person->encounterDone << endl;

  system("Pause");
  return 0;

}

void populateCharacters() {

  people.push_back(new character(1,1,1,"Player","Dude","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(5,5,5,"Professor","Wright","Obsidian","Plywood","Hedge Trimmers", 0));
  people.push_back(new character(0,1,0,"Student","Slim Steven","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(2,2,2,"Student","Fred","Rock","Paper","Scissors", 0));
  people.push_back(new character(0,0,3,"Student","Sharp Shawn","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(4,4,4,"Student","Clint","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(6,6,6,"Vendor","Trafficing Trevor","Diamond","Steel Sheet","Steel Snips", 0));

}

