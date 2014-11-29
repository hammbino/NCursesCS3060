#include<string>
using namespace std;

#pragma once

class character {
public:
  int rockGrade;
  int paperGrade;
  int scissorsGrade;
  string type;
  string name;
  string rockName;
  string paperName;
  string scissorsName;
  int encounterDone;

  character(void);
  character(int rg, int pg, int sg, string t, string n, string rn, string pn, string sn, int ed);
  
};

character::character() {
  rockGrade = 0;
  paperGrade = 0;
  scissorsGrade = 0;
  type = "";
  name = "";
  rockName = "";
  paperName = "";
  scissorsName = "";
  encounterDone = 0;
}

character::character(int rg, int pg, int sg, string t, string n, string rn, string pn, string sn, int ed) {
  rockGrade = rg;
  paperGrade = pg;
  scissorsGrade = sg;
  type = t;
  name = n;
  rockName = rn;
  paperName = pn;
  scissorsName = sn;
  encounterDone = ed;
}


