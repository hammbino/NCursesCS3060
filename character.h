// Copyright Richard Larsen
#include<string>
#include<iostream>

#pragma once

class character {
public:
  std::string type;
  std::string name;
  std::string rockName;
  std::string paperName;
  std::string scissorsName;
  int rockGrade;
  int paperGrade;
  int scissorGrade;
  int money;
  int hitPoints;// Are we wanting to have the hit points calculated in fight by the grade?
   
  character(void);
  void characterP(std::string t, std::string n, std::string rn, std::string pn, std::string sn, int rg, int pg, int sg, int m, int hp);
  void populateCharacters();
  
};

void character::populateCharacters() {
  character wright;
  character steven;
  character fred;
  character scott;
  character clint;
  character trevor;

  wright.characterP("Professor","Wright","Obsidian","Plywood","Hedge Trimmers", 5, 5, 5, 50000, 50);
  steven.characterP("Freshman","Slim Steven","Pebble","Recycled Paper","Safety Scissors", 4, 4, 4, 40000, 40);
  fred.characterP("Sophmore","Fred","Rock","Paper","Scissors", 3, 3, 3, 30000, 30);
  scott.characterP("Junior","Skinny Scott","Boulder","Cardboard","Cardboard Cutter", 2, 2, 2, 20000, 20);
  clint.characterP("Senior","Clint","Granite","Vinyl","Exact-o Knife", 1, 1, 1, 10000, 10);
  trevor.characterP(6,"Vendor","TrafficingTrevor","Diamond","Steel Sheet","Steel Snips", 6, 6, 6, 60000, 60);
}
