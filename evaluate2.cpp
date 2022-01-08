/***************************************************************************
                   evaluate2.cpp  -  a new evaluation function
                   this file adds procedures to class situation
                             -------------------
    begin                : Fre Aug 8 2003
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

 #include "situation.h"

#include <iostream>

#include <cstdlib>

#include "options.h"
extern options option;

#define LEFT 1
#define RIGHT 2


int situation::evaluate2_x() // evaluate current situation as seen by XPLAYER (left-right)
{
  short i,j, sp = 0;
  short jmax = option.ysize+1;
  int res = option.xsize * 2 - 2; // result is never greater 46 = (boardsize * 2 -2)
  const unsigned char initval = 99;
  unsigned char tempval;

  memset(value, initval, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to 99
  //  std::memset(flags, 3      , (MAXSIZE+4)*(MAXSIZE+4));  //    are set to LEFT+RIGHT
  //first column without corners
  for (j=3; j<jmax; j++)
  {
    if (pin[2][j])
      {
      value[2][j] = 0;
      startStructure(2,j,0, XDIR);
      }
    else
      value[2][j] = 1;
  }

  for (i=3; i<option.xsize+2; i++)      // x
    for (j=3; j<jmax; j++)    // y
    {
      if (pin[i][j] == YPLAYER)
        value[i][j] = 60; // this pin is occupied by opponent
      else
      {
        // value is val of neighbour to the left
        tempval = value[i-1][j] + 1; 
        //Blocking bridge?
        if ((bridge[i - 1][j - 1][0] > 90) ||
            (bridge[i - 1][j + 1][3] > 90)) tempval = NOCONNECTION;
        
        if (pin[i][j]) // used by XPLAYER because Y already handled
        {
          if (value[i][j] > tempval)   // not already evaluated or now better?
            i = startStructure(i,j,tempval, XDIR); // to recognize backgoing structures
        }
        else //empty
        {
          if (i>3 && tempval < 80 && !(pin[i-1][j])) tempval++; //if empty and not 2nd column
          value[i][j] = tempval;
        }
      }
    }
  //get smallest of all fields of last line
  for (j=3; j<jmax; j++)
    if (value[option.xsize+1][j] < res)
    {
      res = value[option.xsize+1][j];
      sp = j;
    }
  
  return res * 100 + (abs(option.ysize / 2 - sp));   // je weiter in Mitte desto besser?
}


int situation::evaluate2_y() // evaluate current situation as seen by YPLAYER (top-bottom)
{
  short i,j,sp = 0;
  int res = option.ysize * 2 - 2; // result is never greater 46 = (boardsize * 2 -2)
  const unsigned char initval = 99;
  unsigned char tempval;

  memset(value, initval, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to 99
  //  std::memset(flags, 3      , (MAXSIZE+4)*(MAXSIZE+4));  //    are set to LEFT+RIGHT
  //first column without corners
  for (i=3; i<option.xsize+1; i++)
  {
    if (pin[i][2])
      {
      value[i][2] = 0;
      startStructure(i,2,0, YDIR);
      }
    else
      value[i][2] = 1;
  }
  for (j=3; j<option.ysize+2; j++)    // y
    for (i=3; i<option.xsize+1; i++)      // x
    {
      if (pin[i][j] == XPLAYER)
        value[i][j] = 60; // this pin is occupied by opponent
      else
      {
        // value is val of neighbour above
        tempval = value[i][j-1] + 1 ;  
        //Blocking bridge?
        if ((bridge[i - 1][j - 1][1] > 90) ||
            (bridge[i - 1][j    ][2] > 90)) tempval = NOCONNECTION;

        if (pin[i][j]) // used by YPLAYER because X already handled
        {
          if (value[i][j] > tempval)   // not already evaluated or now better?
            j = startStructure(i,j,tempval, YDIR); // to recognize backgoing structures
        }
        else //empty
        {
          if (j>3 && tempval < 80 && (!pin[i][j-1])) tempval++; //if empty and not 2nd column
          value[i][j] = tempval;
        }
      }
    }
  //get smallest of all fields of last line
  //std::memset(crit, 0, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to false

  for (j=3; j<option.xsize+1; j++)
    if (value[j][option.ysize+1] < res)
    {
      res = value[j][option.ysize+1];
      sp = j;
    }

  return res * 100 + (abs(option.xsize / 2 - sp));   // je weiter in Mitte desto besser?
}


