// Copyright Richard Larsen
#include<string>
#include<iostream>
using namespace std;

#pragma once

class character {
public:
  int grade;
  string type;
  string name;
  string rockName;
  string paperName;
  string scissorsName;

  character(void);
  void characterP(int g, string t, string n, string r, string p, string s);
  void populateCharacters();
  
};

void character::populateCharacters() {
  character wright;
  character steven;
  character fred;
  character scott;
  character clint;
  character trevor;

  wright.characterP(5,"Professor","Wright","Obsidian","Plywood","Hedge Trimmers");
  steven.characterP(1,"Freshman","Slim Steven","Pebble","Recycled Paper","Safety Scissors");
  fred.characterP(2,"Sophmore","Fred","Rock","Paper","Scissors");
  scott.characterP(3,"Junior","Skinny Scott","Boulder","Cardboard","Cardboard Cutter");
  clint.characterP(4,"Senior","Clint","Granite","Vinyl","Exact-o Knife");
  trevor.characterP(6,"Vendor","TrafficingTrevor","Diamond","Steel Sheet","Steel Snips");
}
