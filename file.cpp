/***************************************************************************
                file.cpp  -  input and output of files
                   this file adds procedures to class situation
                             -------------------
    begin                : Son Jun 15 2003
    copyright            : (C) 2003 by Johannes Schwagereit
    email                : mail@johannes-schwagereit.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <FL/Fl_File_Chooser.H>

#include "situation.h"
#include "options.h"
extern options option;


std::string getlf(std::ifstream* inf) //my version of getline
{
  std::string line = "";
  void* fstatus;
  unsigned int pos;

  do
  {
    fstatus = std::getline(*inf,line,'\n');
    if (fstatus)
    {
      pos = line.find("#", 0);
      if (pos != std::string::npos)
        line = line.substr(0,pos);
    }
  } while (fstatus && line == "");
  return line;
}


void situation::loadGame()
{
  std::string line;
  std::ifstream infile;
  int i;
  
  char* fname = fl_file_chooser("Load T1-File", "*.T1", NULL);
  if (!fname)
    return; //Cancel was pressed
  
  infile.open(fname);
  if (!infile)
  {
    printf("ERROR: File '%s' not found.\n", fname);
    return;
  }
  
  getlf(&infile); // version of file-format
  getlf(&infile); // Name 1
  getlf(&infile); // Name 2

  sscanf(getlf(&infile).c_str(), "%i", &i);
  option.ysize = i;
  sscanf(getlf(&infile).c_str(), "%i", &i);
  option.xsize = i;

  option.yhuman    = (getlf(&infile)[0] == 'H') ? true : false;
  option.xhuman    = (getlf(&infile)[0] == 'H') ? true : false;

  option.ystarts   = (getlf(&infile)[0] == '1') ? true : false;
  currentPlayer    = (option.ystarts) ? YPLAYER : XPLAYER;

  option.letterdir = (getlf(&infile)[0] == 'H') ? true : false;
  option.pieRule   = (getlf(&infile)[0] == 'Y') ? true : false;
  option.gameOver  = (getlf(&infile)[0] == 'Y') ? true : false;
  
  lastmove_x = 0;
  lastmove_y = 0;
  setMoveNo(0);
  memset(pin,    0, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to 0
  memset(bridge, 0, (MAXSIZE+4)*(MAXSIZE+4)*4);  //    are set to 0

  bool gs;
  int x,y;
  do
  {
    line = getlf(&infile);
    if (line == "") break;
    gs = (line[1] >= 'A' && line[1] <= 'Z'); //2nd character used (column > 26)
    if (gs) x = 26 + ((int) line[1]) - 65; else x = ((int) line[0]) - 65; //value of letter

    sscanf(line.substr(gs ? 2 : 1).c_str(), "%i", &y);
    y--;
    setMoveNo(getMoveNo() + 1);
    setPin(x,y,currentPlayer);
    moveHistory[getMoveNo()].x = x;
    moveHistory[getMoveNo()].y = y;
    switchCurrentPlayer();
  }
  while (line != "");
  update_gui();

}

void situation::saveGame()
{
  char st[15], st1[3], st2[3];
  std::ofstream ofile; // the output-file

  char* fname = fl_file_chooser("Save T1-File", "*.T1", NULL);
  if (!fname)
    return; //Cancel was pressed
  
  ofile.open(fname);
  if (!ofile)
  {
    printf("ERROR: Couldn't open '%s' for output.\n", fname);
    return;
  }
  ofile << "# File created by T1\n# T1 is a program to play TwixT (mail@johannes-schwagereit.de)\n";
  ofile << "1 # version of file-format\n";
  ofile << "Player 1# Name of Player 1\n";
  ofile << "Player 2# Name of Player 2\n";
  ofile << (int) option.ysize << "# y-size of board\n";
  ofile << (int) option.xsize << "# x-size of board\n";
  ofile << (option.yhuman ? "H" : "C") << "# player 1 human or computer\n";
  ofile << (option.xhuman ? "H" : "C") << "# player 2 human or computer\n";
  ofile << (option.ystarts ? "1" : "2") << "# starting player\n";
  ofile << (option.letterdir ? "H" : "V") << "# direction of letters \n";
  ofile << (option.pieRule ? "Y" : "N") << "# pierule? \n";
  ofile << (option.gameOver ? "Y" : "N") << "# game already over? \n";

  for(int i=1; i<=getMoveNo(); i++)
  {
    situation::setOutStr(st1, moveHistory[i].x, !option.letterdir);
    situation::setOutStr(st2, moveHistory[i].y, option.letterdir);
    if (option.letterdir)
      sprintf(st, "%s%s", st2, st1);
    else
      sprintf(st, "%s%s", st1, st2);
    ofile << st << std::endl;
  }  
  ofile.close();
}
