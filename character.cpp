#include"character.h"
#include<string>
#include<vector>
using namespace std;

character* player; 
vector<character*> people;

character::character() {
  rockGrade = 0;
  paperGrade = 0;
  scissorsGrade = 0;
  money = 0;
  type = "";
  name = "";
  rockName = "";
  paperName = "";
  scissorsName = "";
  encounterDone = 0;
}

character::character(int rg, int pg, int sg, int m, string t, string n, string rn, string pn, string sn, int ed) {
  rockGrade = rg;
  paperGrade = pg;
  scissorsGrade = sg;
  money = m;
  type = t;
  name = n;
  rockName = rn;
  paperName = pn;
  scissorsName = sn;
  encounterDone = ed;
}

void populateCharacters(vector<character*> &people) {
  people.push_back(new character(5,5,5,0,"Professor","Wright","Obsidian","Plywood","Hedge Trimmers", 0));
  people.push_back(new character(0,1,0,0,"Student","Slim Steven","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(1,0,0,0,"Student","Rough Rodger","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(0,0,1,0,"Student","Pointy Pat","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(1,1,1,0,"Student","Loaded Larry","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(0,1,0,0,"Student","Thin Theresa","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(1,0,0,0,"Student","Hard Headed Harvey","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(0,0,1,0,"Student","Pokey Paul","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(1,1,1,0,"Student","Spoiled Samantha","Pebble","Recycled Paper","Safety Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Fred","Rock","Paper","Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Frank","Rock","Paper","Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Kristy","Rock","Paper","Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Precila","Rock","Paper","Scissors", 0));
  people.push_back(new character(2,2,2,0,"Student","Hank","Rock","Paper","Scissors", 0));
  people.push_back(new character(0,0,3,0,"Student","Sharp Shawn","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(3,0,0,0,"Student","Sturdy Sid","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(0,3,0,0,"Student","Skinny Scott","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(3,3,3,0,"Student","Richie Rich","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(0,0,3,0,"Student","Spikey Spike","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(3,0,0,0,"Student","Stoney Stoner","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(0,3,0,0,"Student","Lanky Loyd","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(3,3,3,0,"Student","Tons o' money Tyler","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(4,4,4,0,"Student","Clint","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(4,4,4,0,"Student","Curtis","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(4,4,4,0,"Student","Dave","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(4,4,4,0,"Student","Karine","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(2,2,2,0,"Vendor","Cheap Charlie","Rock","Paper","Scissors", 0));
  people.push_back(new character(3,3,3,0,"Vendor","Sly Sam","Boulder","Cardboard","Cardboard Cutter", 0));
  people.push_back(new character(4,4,4,0,"Vendor","Sneaky Seth","Granite","Vinyl","Exact-o Knife", 0));
  people.push_back(new character(5,5,5,0,"Vendor","Shifty Sheldon","Obsidian","Plywood","Hedge Trimmers", 0));
  people.push_back(new character(6,6,6,0,"Vendor","Trafficing Trevor","Diamond","Steel Sheet","Steel Snips", 0));
}
