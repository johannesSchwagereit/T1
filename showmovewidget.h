/***************************************************************************
              showmovewidget.h  -  show last move and color of next move
                             -------------------
    begin                : Sat Nov 2 2002
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

#ifndef SHOWMOVEWIDGET_H
#define SHOWMOVEWIDGET_H

#include <cstring>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

/**Show last and next move
  *@author Johannes Schwagereit
  */

class showMoveWidget : public Fl_Box  {
  void draw();
  unsigned char last_x;
  unsigned char last_y;
public: 
  char lastmoveSt[18];
	showMoveWidget(int x, int y, int w, int h, const char *label = 0);
	~showMoveWidget();
  /** Set String for lastmove-Widget*/
  void setLastmove(unsigned char, unsigned char);
};

#endif
