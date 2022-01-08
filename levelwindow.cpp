/***************************************************************************
 *   Copyright (C) 2004 by Johannes Schwagereit                            *
 *   mail@johannes-schwagereit.de                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Preferences.H>

#include "levelwindow.h"
#include "newwindow.h"

#include "options.h"
extern options option;
options leveloption;

#include "situation.h"
extern situation* actSit;


void return_l_cb(Fl_Widget *w, void *data) //return-button was pressed
{
  Fl_Window* e = (Fl_Window*)data;
  e->hide();
  delete e;
  
  option.modalWindow = false;
  //save value
  option.level  = leveloption.level;
  newWindow::save_preferences(); //this function is static!
  //option.currentMainWindow->redrawGame();
}

void cancel_l_cb(Fl_Widget *w, void *data) //cancel was pressed  or exit-button (corner up right)
{
  Fl_Window* e = (Fl_Window*)data;
  e->hide();
  delete e;
  option.modalWindow = false;

}

void size_l_cb(Fl_Widget *w, void *data) //size?
{
  Fl_Counter* e = (Fl_Counter*) w;
  leveloption.level = (unsigned char) (e->value());
}

levelWindow::levelWindow()
{
  //get values from options
  leveloption.level  = option.level;
//   leveloption.yhuman  = option.yhuman;
//   leveloption.ystarts = option.ystarts;
//   leveloption.pieRule = option.pieRule;
//   leveloption.xsize   = option.xsize;


  Fl_Window* pwin = new Fl_Window(413, 101, "Set computer level");


  { Fl_Counter* o = new Fl_Counter(228, 15, 160, 20, "Time per computer-move (sec.): ");
    o->labelsize(14);
    o->align(FL_ALIGN_LEFT);
    o->range(1,240); 
    o->value(leveloption.level);
//     o->value(5);
    o->step(1);
    o->lstep(5);
    o->callback((Fl_Callback *)size_l_cb, o);
    Fl_Box* o2 = new Fl_Box(140, 30, 100, 25, "(Default is 5)");
    o2->labelsize(12);
    o2->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  }
  {
    Fl_Return_Button* o = new Fl_Return_Button(230, 65, 80, 25, "OK");
    o->callback((Fl_Callback *)return_l_cb, pwin);
  }
  {
    Fl_Button* o = new Fl_Button(320, 65, 80, 25, "Cancel");
    o->callback((Fl_Callback *)cancel_l_cb, pwin);
  }
  pwin->end();
  pwin->callback((Fl_Callback *)cancel_l_cb,pwin);
  option.modalWindow = true;
  pwin->set_modal();
  pwin->show();
}


levelWindow::~levelWindow()
{
}


