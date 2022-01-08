/***************************************************************************
            showmovewidget.cpp  -  show last move and color of next move
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

#include "showmovewidget.h"

#include "situation.h"
extern situation* actSit;

#include "options.h"
extern options option;


showMoveWidget::showMoveWidget(int x, int y, int w, int h, const char *label)
  : Fl_Box(x,y,w,h,label)
  {
  }

showMoveWidget::~showMoveWidget(){
}

void showMoveWidget::draw()
{
  fl_clip(x(),y(),w(),h());
  fl_color(FL_LIGHT2);
  fl_rectf(x(),y(),w(),h());

  //Display icon for next player to move
  short un = w()/20;
  short xo = x() + 4 * un -2;
  short yo = y() - un;

  if (actSit->getCurrentPlayer() == XPLAYER)
  {
    //draw arrow
    fl_color(option.XColor);
    fl_rectf(xo + un*6, yo + un*6, un*4, un*2);
    fl_polygon(xo + un* 3, yo + un* 7,
               xo + un* 6, yo + un* 4,
               xo + un* 6, yo + un*10);
    fl_polygon(xo + un*13, yo + un* 7,
               xo + un*10, yo + un* 4,
               xo + un*10, yo + un*10);
  }
  else // YPLAYER
  {
    //draw arrow
    fl_color(option.YColor);
    fl_rectf(xo + un*7, yo + un*5, un*2, un*4);
    fl_polygon(xo + un* 8, yo + un* 2,
               xo + un* 5, yo + un* 5,
               xo + un*11, yo + un* 5);
    fl_polygon(xo + un* 8, yo + un*12,
               xo + un* 5, yo + un* 9,
               xo + un*11, yo + un* 9);

  }

  //Display text of last move
  char st[15], st1[3], st2[3];
  if (last_x == 0 && last_y == 0)
    strcpy(lastmoveSt, " ");
  else
  {
    situation::setOutStr(st1, last_x, !option.letterdir);
    situation::setOutStr(st2, last_y, option.letterdir);
    if (option.letterdir)
      sprintf(st, "%s%s", st2, st1);
    else
      sprintf(st, "%s%s", st1, st2);
    strcpy(lastmoveSt, st);
  }
  fl_font(FL_HELVETICA,12);
  fl_color((actSit->getCurrentPlayer() == XPLAYER) ? option.YColor : option.XColor);
  fl_draw("Last Move: ", x() + 2, y() + h() - 3);
  fl_color(FL_BLACK);
  fl_draw(lastmoveSt, x() + 2 + (int)fl_width("Last Move: "), y() + h() - 3);
  
  fl_pop_clip();
}

/** Set String for lastmove-Widget*/
void showMoveWidget::setLastmove(unsigned char l_x, unsigned char l_y){
  last_x = l_x;
  last_y = l_y;
  redraw();
}

