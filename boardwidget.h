/***************************************************************************
              boardwidget.h  -  draw board and process mouse-clicks
                             -------------------
    begin                : Thu Oct 10 2002
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

#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <cstdio>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>


class boardWidget : public Fl_Box  {
  void draw();
  /** Draw a pin or dot */
  void drawPin(uchar x, uchar y);
  /** Draw bridges from 1 point to the right*/
  void drawBridges(uchar x, uchar y);
  void markLastPin(uchar xv, uchar yv); /*mark last pin set */

  bool setPosStr(char st[5], int x, int y, bool letter);
  short xclickToPos(int x) {return (x - xoff + iw/2) / iw;} // Convert x-click to real pos
  short yclickToPos(int y) {return (y - yoff + iw/2) / iw;} // Convert y-click to real pos

  short iw;   //Distance of  points
  short dotsz; //dotsize
  short pinsz; //pinsize
  short xoff; //Xoffset
  short yoff; //Yoffset

  
public: 
	boardWidget(int x, int y, int w, int h, const char *label = 0);

  int  handle(int event);

	~boardWidget();
  int mx,my;
};

#endif
