/***************************************************************************
                          pattern.cpp  -  pattern-administration
                             -------------------
    begin                : Son Nov 30 2003
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
#include <string>

#include "situation.h"
#include "pattern.h"

#include "options.h"
extern options option;

#define FILE "patterns.txt"




//a singleton pattern
pattern * pattern::_instance = NULL;

pattern * pattern::instance()
{
    if (_instance == NULL) {
        _instance = new pattern ;
    }
    return _instance ;
}

//cons'tor
pattern::pattern()
{
  std::string line;
  enum pTypes {off, def, unknown} pType = unknown;
  pat* off_pat_tail = NULL; //tail of list of offensive patterns
  pat* def_pat_tail = NULL; //tail of list of defensive patterns
  pat* cur_pat = NULL; //current pattern
  cond* cur_cond;
  cond* cond_tail = NULL; //tail of list of conditions of one pattern

  off_pat = NULL; 
  def_pat = NULL;

  infile.open(FILE);  
  if (!infile)
  {
    printf("ERROR: File '%s' not found.\n", FILE);
    return;
  }

  
  do
  {
    line = getl();
    if (line[0] == 'O')   // 
    {
      cur_pat = new pat();
      if (off_pat_tail != NULL) off_pat_tail->next = cur_pat;
      off_pat_tail = cur_pat;
      if (off_pat == NULL) off_pat = cur_pat;
      cond_tail = NULL;
      pType = off;
    }
    else if (line[0] == 'D')  // 
    {
      cur_pat = new pat();
      if (def_pat_tail != NULL) def_pat_tail->next = cur_pat;
      def_pat_tail = cur_pat;
      if (def_pat == NULL) def_pat = cur_pat;
      cond_tail = NULL;
      pType = def;
    }
    else
    {
      if (pType == unknown)
      {
        printf("ERROR while reading File '%s': Illegal syntax.\n", FILE);
        return;
      }
      short args = -1;
      switch (line[0]) {
        case 'R':
          args = 1;
          break;
        case 'F':
        case 'E':
        case 'G':
        case 'S':
          args = 2;
          break;
        case 'B':
        case 'V':
          args = 4;
          break;
        default:
          if (line[0])
            printf("ERROR while reading File '%s': '%c' is illegal command.\n", FILE, line[0]);
      } // switch
      if (args >= 0) //no error found
      {
        cur_cond = new cond;
        sscanf(line.c_str(), "%c %hd %hd %hd %hd",
          &(cur_cond->type), &(cur_cond->x1), &(cur_cond->y1), &(cur_cond->x2), &(cur_cond->y2));
        cur_cond->next = NULL;

        if (cond_tail != NULL) cond_tail->next = cur_cond;
        cond_tail = cur_cond;
        if (cur_pat->condList == NULL) cur_pat->condList = cur_cond;                
      }
    } // else
  }
  while (line != "");

}

std::string pattern::getl() //
{
  std::string line = "";
  void* fstatus;
  unsigned int pos;

  do
  {
    fstatus = std::getline(infile,line,'\n');
    if (fstatus)
    {
      pos = line.find("#", 0);
      if (pos != std::string::npos)
        line = line.substr(0,pos);
    }
  } while (fstatus && line == "");
  return line;

}

bool situation::checkPattern(unsigned char thePlayer, unsigned char x,unsigned char y,int dir,
                    move possMove[], bool offensive)
{
  pattern* muster = pattern::instance();
  pat* cur_pat = NULL; //current pattern
  cond* cc;
  unsigned short xf, yf, zx1, zy1, zx2, zy2;

  //check one pattern after the other
  cur_pat = offensive ? muster->off_pat : muster->def_pat;


  while (cur_pat != NULL)
  {
    cc = cur_pat->condList;
    xf = 0; yf = 0;
      
    while (cc != NULL) //check the conditions
      {
    if (thePlayer == YPLAYER)
    {
      if (offensive)
      {
        if (dir)
          {zx1 = x + cc->x1; zy1 = y - cc->y1; zx2 = x + cc->x2; zy2 = y - cc->y2;}
        else
          {zx1 = x + cc->x1; zy1 = y + cc->y1; zx2 = x + cc->x2; zy2 = y + cc->y2;}
      }
      else
      {
        if (!dir)
          {zx1 = x + cc->y1; zy1 = y - cc->x1; zx2 = x + cc->y2; zy2 = y - cc->x2;}
        else //
          {zx1 = x - cc->y1; zy1 = y + cc->x1; zx2 = x - cc->y2; zy2 = y + cc->x2;}
      }
    }
    else //XPLAYER
    {
      if (offensive)
      {
        if (dir)
          {zx1 = x - cc->y1; zy1 = y + cc->x1; zx2 = x - cc->y2; zy2 = y + cc->x2;}
        else // !dir
          {zx1 = x + cc->y1; zy1 = y + cc->x1; zx2 = x + cc->y2; zy2 = y + cc->x2;}
      }
      else
      {
        if (dir)
          {zx1 = x + cc->x1; zy1 = y - cc->y1; zx2 = x + cc->x2; zy2 = y - cc->y2;}
        else // !dir
          {zx1 = x - cc->x1; zy1 = y + cc->y1; zx2 = x - cc->x2; zy2 = y + cc->y2;}
      }
    }

    //check boardsize
    if (zx1 < 2 || zx1 > option.xsize + 1 || zy1 < 2 || zy1 > option.ysize + 1) //not legal
      if (cc->type != 'R') goto nextpattern;
   
    switch (cc->type) {
        case 'R':
          //check the row  --- only offensive
          if (thePlayer == XPLAYER)
          {
            if (dir && x - 2 == cc->x1 - 1 || !dir && x - 2 == option.xsize - cc->x1 + 0)
            {
//              char st[5];
//              posStr1(st, x-2, y-2);
//              printf("found x on %s\n", st);
            }
            else goto nextpattern;
          }
          else // YPLAYER
          {
            if (dir && y - 2 == cc->x1 - 1 || !dir && y - 2 == option.ysize - cc->x1 + 0)
            {
//              char st[5];
//              posStr1(st, x-2, y-2);
//              printf("found y on %s\n", st);
            }
            else goto nextpattern;
          }
          break;
        case 'S':
          //set pin
          if (thePlayer == XPLAYER)
          {
            if (!pin[zx1][zy1])
              {
                xf = zx1;
                yf = zy1;
                if (xf < 2 || xf > option.xsize + 1 || yf < 3 || yf > option.ysize) //not legal
                  goto nextpattern;
                break;
              }
          }
          else // YPLAYER
          {
            if (!pin[zx1][zy1])
              {
                xf = zx1;
                yf = zy1;
                if (xf < 3 || xf > option.xsize || yf < 2 || yf > option.ysize + 1) //not legal
                  goto nextpattern;
                break;
              }
          }
          goto nextpattern;                   // no free pin found
          break;
        case 'E':
          //own pin
          if (thePlayer == XPLAYER)
          {
//                char st[5],st1[5];
//                posStr1(st, x-2, y-2);
//                posStr1(st1, zx1 - 2, zy1 - 2);
            if (pin[zx1][zy1] == XPLAYER)
              {
//                char st[5],st1[5];
//                posStr1(st, x-2, y-2);
//                posStr1(st1, zx1 - 2, zy1 - 2);
                break;   //eigentlich fehlt hier Check auf Feldgrenzen
              }
          }
          else // YPLAYER
          {
            if (pin[zx1][zy1] == YPLAYER)
              {
                break;
              }
          }
          goto nextpattern;                   // no correct pin found
          break;        
        case 'G':
          //opponent pin
          if (thePlayer == XPLAYER)
          {
            if (pin[zx1][zy1] == YPLAYER)
              {
                break;   //eigentlich fehlt hier Check auf Feldgrenzen
              }
          }
          else // YPLAYER
          {
            if (pin[zx1][zy1] == XPLAYER)
              {
                break;
              }
          }
          goto nextpattern;                   // no correct pin found
          break;
        case 'F':
          //free pin
          if (!pin[zx1][zy1])
              {
                break;   // 
              }
          goto nextpattern;                   // no empty pin found
          break;

        case 'B':
          //test if bridge allowed
          if (!testBridge(zx1, zy1, zx2, zy2))
              goto nextpattern; // Bridge not allowed
//          printf("B-Ok.");
          break;

        case 'V':
          //test if bridge exist
          if (!connected(zx1, zy1, zx2, zy2))
              goto nextpattern; // Bridge doesnt exist
//          printf("V-Ok.");
          break;
          
      } //switch
      cc = cc->next; //next condition
      } //while
    //pattern sucessfully found:
    if (xf > 0 && yf > 0) //moves found
    {
      addToMovePoss(xf, yf, 0, possMove);
      return true; //move found
    }
    nextpattern: //label for goto above
    cur_pat = cur_pat->next;
  }

  return false; //nothing found
}

