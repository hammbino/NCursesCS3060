#include<string>
#include<iostream>
#include<vector>
#include"character.h"
using namespace std;

void populateCharacters(vector<character*> &people);
character* player = new character(1,1,1,"Player","Dude","Pebble","Recycled Paper","Safety Scissors", 0);

int main() {
  vector<character*> people;
  
  populateCharacters(people);

  return 0;

}

void populateCharacters(vector<character*> &people) {

  people.push_back(new character(5,5,5,0,"Professor","Wright","Obsidian","Plywood","Hedge Trimmers", 0));
  people.push_back(new character(0,1,0,0,"Student","Slim Steven","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(1,0,0,0,"Student","Rough Rodger","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(0,0,1,0,"Student","Pointy Pat","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(1,1,1,0,"Student","Loaded Larry","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Fred","Rock","Paper","Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Frank","Rock","Paper","Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Kristy","Rock","Paper","Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Hank","Rock","Paper","Scissors", 0));
  people.push_back(new character(0,3,0,0,"Student","Sharp Shawn","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(3,0,0,0,"Student","Sturdy Sid","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(0,0,3,0,"Student","Skinny Scott","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(3,3,3,0,"Student","Richie Rich","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(4,4,4,0,"Student","Clint","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(4,4,4,0,"Student","Curtis","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(4,4,4,0,"Student","Dave","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(4,4,4,0,"Student","Karine","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(2,2,2,0,"Vendor","Cheap Charlie","Rock","Paper","Scissors", 0));
  people.push_back(new character(3,3,3,0,"Vendor","Sly Sam","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(4,4,4,0,"Vendor","Sneaky Seth","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(5,5,5,0,"Vendor","Rocking Rocky","Obsidian","Plywood","Hedge Trimmers", 0));
  people.push_back(new character(6,6,6,0,"Vendor","Trafficing Trevor","Diamond","Steel Sheet","Steel Snips", 0));

}