/***************************************************************************
          boardwidget.cpp  -  draw board and process mouse-clicks
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

#include "mainwindow.h"
#include "boardwidget.h"

#include "options.h"
extern options option;

#include "situation.h"
extern situation* actSit;

#include <iostream>

// needed in markLastPin(): Circles are different bitween Linux and Win
#ifdef __GNUC__
  #define LASTMOVEOFFSET 1
#else
  #define LASTMOVEOFFSET 2
#endif


const int YBORDER = 20; //border for letters on top
const int XBORDER = 30; //border for letters on left
const int DISTFROMDOTS = 8; //distance of letters from dots


boardWidget::boardWidget(int x, int y, int w, int h, const char *label)
  : Fl_Box(x,y,w,h,label)
  {
  }

void boardWidget::draw()
{
  fl_clip(x(),y(),w(),h());
  fl_color(FL_WHITE);
  fl_rectf(x(),y(),w(),h());

  short xiw = w() - XBORDER; //room for field in xdir
  short yiw = h() - YBORDER; //room for field in ydir

  iw = MIN((xiw - 9) / (option.xsize - 1), (yiw - 9) / (option.ysize - 1)); //Distance of points   (min.)
  dotsz = 2; //dotsize
  pinsz = (iw < 18) ? ((iw < 10) ? 5 : 7) : 9; //pinsize
  xoff = x() + XBORDER;
  yoff = y() + YBORDER;
      
  if (option.showAreaLines) { // thanks to abraendle@gmx.de for this section 
    int rand=0; // CHANGE THIS to get shorter lines
    int tab1[4][2]=
    {{1,1},
    {99,1},
    {1,99},
    {99,99}};
    tab1[1][0]=option.xsize-2;
    tab1[2][1]=option.ysize-2;
    tab1[3][0]=option.xsize-2;
    tab1[3][1]=option.ysize-2;

    int tab2[][2]=
    {{2,1},{1,2},
    {-2,1},{-1,2},
    {2,-1},{1,-2},
    {-2,-1},{-1,-2}};
    
    for (int i=0; i<8;i++) {
      uchar xv=tab1[i/2][0], yv=tab1[i/2][1];
      uchar x2=xv, y2=yv;
      
      x2+=tab2[i][0]; y2+=tab2[i][1]; 
      x2+=tab2[i][0]; y2+=tab2[i][1]; 
      x2+=tab2[i][0]; y2+=tab2[i][1]; 
      while (
        x2>=2+rand &&
        y2>=2+rand && 
        x2<=option.xsize-3-rand &&
        y2<=option.ysize-3-rand)
        
      {
        x2+=tab2[i][0]; 
        y2+=tab2[i][1]; 
      }

      fl_color(fl_rgb_color(0, 158, 0));
      char d_style[]={11,11,0};
      fl_line_style(FL_DASH, 0, d_style);
      fl_line (xv*iw + xoff, yv*iw + yoff, x2*iw + xoff, y2*iw + yoff);
  
    }  
  }
  //********************

  
  //draw dots or pins and bridges
  uchar i,j;
  fl_line_style(FL_SOLID,(iw < 17) ? 1 : 2);
  for (i=1;i<option.xsize - 1;i++)
    for (j=0;j<option.ysize;j++)
      drawPin(i,j);
  for (j=1;j<option.ysize - 1;j++)
    drawPin(0,j);
  for (j=1;j<option.ysize - 1;j++)
    drawPin(option.xsize - 1,j);  
    
  fl_line_style(FL_SOLID,1);
  markLastPin(actSit->moveHistory[actSit->getMoveNo()].x, actSit->moveHistory[actSit->getMoveNo()].y);

  fl_font(FL_HELVETICA,8);
  fl_font(FL_COURIER,9); //as a test (debug!)

  char l[3];
  //letters or numbers on upper border
  fl_color(FL_DARK3);
  //fl_color(FL_BLACK);
  for (i=0;i<option.xsize;i++)
  {
    situation::setOutStr(l ,i, !option.letterdir);
    fl_draw(l, (int) (i*iw + xoff - fl_width(l)/2), yoff - DISTFROMDOTS);
  }

  for (i=0;i<option.ysize;i++)
  {
    situation::setOutStr(l ,i, option.letterdir);
    fl_draw(l, (int) (xoff - fl_width(l) - DISTFROMDOTS - 3), i * iw + yoff + 4);
  }

  // Lines before Players
  fl_color(option.XColor);     //Vertical
  fl_line_style(FL_SOLID,1); // line style _after_ color necessary for win32
  fl_line (xoff + iw/2 +1, yoff + iw,
           xoff + iw/2 +1, yoff + iw * (option.ysize - 2)+ dotsz);
  fl_line (xoff + iw * (option.xsize - 1) - iw/2 , yoff +iw,
           xoff + iw * (option.xsize - 1) - iw/2 , yoff +iw * (option.ysize - 2)+ dotsz);

  fl_color(option.YColor);   //horizontal
  fl_line_style(FL_SOLID,1); // line style _after_ color necessary for win32
  fl_line (xoff + iw,                      yoff + iw/2 +1,
           xoff + iw * (option.xsize - 2)+ dotsz, yoff + iw/2 +1);
  fl_line (xoff + iw                     , yoff + iw/2 +iw * (option.ysize - 2)+1,
           xoff + iw * (option.xsize - 2)+ dotsz, yoff + iw/2 +iw * (option.ysize - 2)+1);

  fl_pop_clip();


}

bool boardWidget::setPosStr(char st[5], int x, int y, bool letter)
{
       char st1[3], st2[3];
       int  xko = xclickToPos(x);
       int  yko = yclickToPos(y);
       if (xko < 0 || yko < 0 || xko >= option.xsize || yko >= option.ysize ||
         ((xko == 0 || xko == option.xsize - 1) && (yko == 0 || yko == option.ysize - 1))) //the corners
       {
         st[0] = '\0';
         return false;  // no usable Position
       }
       situation::setOutStr(st1, xko, !letter);
       situation::setOutStr(st2, yko, letter);
       if (letter)
         sprintf(st, "%s%s", st2, st1);
       else
         sprintf(st, "%s%s", st1, st2);
       return true; // legal Position
}

int boardWidget::handle(int event)
{
  MainWindow* mw = (MainWindow *) this->parent();
  if (event == FL_RELEASE && !option.modalWindow && !option.gameOver)
    // mousebutton released without open modalwindow and game not over
  {
    char st[5];
    mx = Fl::event_x();
    my = Fl::event_y();
    short xp = xclickToPos(mx);
    short yp = yclickToPos(my);

    if (setPosStr(st, mx, my, option.letterdir) &&
        // no player behind opponent lines
        ((actSit->getCurrentPlayer() == XPLAYER && yp > 0 && yp < option.ysize - 1) ||
         (actSit->getCurrentPlayer() == YPLAYER && xp > 0 && xp < option.xsize - 1)) )
    {
      mw->setActPos(st); // Anzeige der aktuellen Position
      if (actSit->setPin(xp,yp,actSit->getCurrentPlayer()) >= 0)
      { //only if move was legal
        actSit->setLastMove(xp, yp);
        actSit->setMoveNo(actSit->getMoveNo() + 1);
        actSit->setMaxMoveNo(actSit->getMoveNo());  //set to same value to deactivate forw-button
        actSit->moveHistory[actSit->getMoveNo()].x = xp;
        actSit->moveHistory[actSit->getMoveNo()].y = yp;
        actSit->control();
        if (!option.gameOver)
          mw->cursor(FL_CURSOR_CROSS,(actSit->getCurrentPlayer() == XPLAYER)
            ? option.XColor :option.YColor);
      }
      redraw();
    }
  }
  if (event == FL_ENTER && !option.gameOver)
  {
    // mousecursor gets color of next player
    mw->cursor(FL_CURSOR_CROSS,(actSit->getCurrentPlayer() == XPLAYER) ? option.XColor : option.YColor);
    return 1;
  }
  if (event == FL_LEAVE)
  {
    mw->cursor(FL_CURSOR_DEFAULT);
    char st[5] = "";
    mw->setActPos(st);
    return 1;
  }

  if (event == FL_MOVE)
  {
    char st[5];
    if (iw > 0)
    {
      setPosStr(st, Fl::event_x(), Fl::event_y(), option.letterdir);
      mw->setActPos(st);
    }
    //return 1;
  }

    
  return Fl_Widget::handle(event);
}

boardWidget::~boardWidget(){
}

/** Draw a pin or dot */
void boardWidget::drawPin(uchar xv, uchar yv){
  uchar ph = pinsz / 2;
  uchar pi = actSit->getPin(xv,yv);
  if (pi == XPLAYER)
  {
    fl_color(option.XColor);
    fl_pie(xv*iw + xoff - ph, yv*iw + yoff - ph,pinsz,pinsz,0,360);
    drawBridges(xv,yv);
  }
  else if (pi == YPLAYER)
  {
    fl_color(option.YColor);
    fl_pie(xv*iw + xoff - ph, yv*iw + yoff - ph,pinsz,pinsz,0,360);
    drawBridges(xv,yv);
  }
  else
  {
    // fl_color(FL_DARK2); not visible on (win xp)
    fl_color(FL_DARK_BLUE);
    fl_rectf(xv*iw + xoff, yv*iw + yoff,dotsz,dotsz);
    drawBridges(xv,yv);
  }
}

/** Draw bridges from 1 point to the right*/
void boardWidget::drawBridges(uchar xv, uchar yv){
  uchar x2 = 0, y2 = 0;
  for (uchar i = 0; i < 4; i++)
    if (actSit->getBridge(xv,yv,i) > 90) // > 250
    {
      switch(i) {
        case UH:
          x2 = xv + 1;
          y2 = yv + 2;
          break;
        case UL:
          x2 = xv + 2;
          y2 = yv + 1;
          break;
        case DL:
          x2 = xv + 2;
          y2 = yv - 1;
          break;
        case DH:
          x2 = xv + 1;
          y2 = yv - 2;
          break;
        }
      fl_line (xv*iw + xoff, yv*iw + yoff, x2*iw + xoff, y2*iw + yoff);
    }
}      
    
void boardWidget::markLastPin(uchar xv, uchar yv)
{ //draw extra circle around last pin set  
  if (xv > 0 || yv > 0)
  { //mark last pin set
    uchar ph = pinsz / 2 + LASTMOVEOFFSET;
    uchar pi = actSit->getPin(xv,yv);
    fl_color((pi == XPLAYER) ? option.XColor : option.YColor);
    fl_arc(xv*iw + xoff - ph, yv*iw + yoff - ph,pinsz + 2 + LASTMOVEOFFSET,pinsz + 2 + LASTMOVEOFFSET,0,360);
  }

}
