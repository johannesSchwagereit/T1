/***************************************************************************
                          pattern.h  -  pattern-administration
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

#ifndef PATTERN_H
#define PATTERN_H

#include <fstream>

/**Read and store patterns
  *@author Johannes Schwagereit
  */

//a linked list of conditions for patterns
class cond
{
   public:
      char type;
      short x1, y1, x2, y2;
      cond* next;
      cond() {x1=0, y1=0, x2=0, y2=0, next=NULL;}
};
  
//a linked list of pattern
class pat
{
   public:
      cond* condList;
      pat* next;
      pat(){condList = NULL; next = NULL;}
};


//this class is implemented as singleton-pattern  
class pattern
{
  public:
    static pattern* instance() ;
    pat* off_pat; //list of offensive patterns
    pat* def_pat; //list of defensive patterns
  protected:
    pattern();
  private:
    static pattern* _instance ;
    std::ifstream infile; // the pattern-file
    std::string getl(); // readline
    
};

#endif
