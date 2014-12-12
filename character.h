#pragma once

#include<string>
#include<vector>
using namespace std;

class character {
public:
  int rockGrade;
  int paperGrade;
  int scissorsGrade;
  int money;
  string type;
  string name;
  string rockName;
  string paperName;
  string scissorsName;
  int encounterDone;

  character(void);
  character(int rg, int pg, int sg, int m, string t, string n, string rn, string pn, string sn, int ed);
  
};

void populateCharacters(vector<character*> &people);

extern character* player; 
extern vector<character*> people;
