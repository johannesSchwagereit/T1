/***************************************************************************
              newwindow.cpp  -  window to set data for new game
                             -------------------
    begin                : Tue Nov 12 2002
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
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Preferences.H>

#include "newwindow.h"

#include "options.h"
extern options option;
options newwinoption;

#include "situation.h"
extern situation* actSit;


void return_cb(Fl_Widget *w, void *data) //return-button was pressed
{
  Fl_Window* e = (Fl_Window*)data;
  e->hide();
  delete e;

  //Two Computerplayer are not possible at the moment.
  if (!newwinoption.xhuman && !newwinoption.yhuman)
  {
    fl_message("Only one Computer-player allowed.\nPlayer 2 is set to human.");
    newwinoption.xhuman = true;
  }
  
  option.modalWindow = false;
  option.gameOver = 0; //false
  //save value
  option.xhuman  = newwinoption.xhuman;
  option.yhuman  = newwinoption.yhuman;
  option.ystarts = newwinoption.ystarts;
  option.pieRule = newwinoption.pieRule;
  option.xsize   = newwinoption.xsize;
  option.ysize   = newwinoption.ysize;

  newWindow::save_preferences(); //this function is static!
  option.currentMainWindow->redrawGame();
  if (option.ystarts) //new game starts...
    actSit->initialize(YPLAYER);
  else
    actSit->initialize(XPLAYER);

}

void cancel_cb(Fl_Widget *w, void *data) //cancel was pressed  or exit-button (corner up right)
{
  Fl_Window* e = (Fl_Window*)data;
  e->hide();
  delete e;
  option.modalWindow = false;

}


void p1h_cb(Fl_Widget *w, void *data) //player 1 is human
{
  Fl_Round_Button* e = (Fl_Round_Button*) w;
  newwinoption.yhuman = true;
  e->setonly();
}

void p1c_cb(Fl_Widget *w, void *data) //player 1 is computer
{
  Fl_Round_Button* e = (Fl_Round_Button*) w;
  newwinoption.yhuman = false;
  e->setonly();
}

void p2h_cb(Fl_Widget *w, void *data) //player 2 is human
{
  Fl_Round_Button* e = (Fl_Round_Button*) w;
  newwinoption.xhuman = true;
  e->setonly();
}

void p2c_cb(Fl_Widget *w, void *data) //player 2 is computer
{
  Fl_Round_Button* e = (Fl_Round_Button*) w;
  newwinoption.xhuman = false;
  e->setonly();
}

void pie_cb(Fl_Widget *w, void *data) //pie-rule?
{
  Fl_Check_Button* e = (Fl_Check_Button*) w;
  newwinoption.pieRule = (e->value()) ? true : false;
}

void first_cb(Fl_Widget *w, void *data) //Player 1 sets 1st pin?
{
  Fl_Check_Button* e = (Fl_Check_Button*) w;
  newwinoption.ystarts = (e->value()) ? true : false;
}

void size_x_cb(Fl_Widget *w, void *data) //size?
{
  Fl_Counter* e = (Fl_Counter*) w;
  newwinoption.xsize = (unsigned char) (e->value());
}

void size_y_cb(Fl_Widget *w, void *data) //size?
{
  Fl_Counter* e = (Fl_Counter*) w;
  newwinoption.ysize = (unsigned char) (e->value());
}

newWindow::newWindow()
{
  //get values from options
  newwinoption.xhuman  = option.xhuman;
  newwinoption.yhuman  = option.yhuman;
  newwinoption.ystarts = option.ystarts;
  newwinoption.pieRule = option.pieRule;
  newwinoption.xsize   = option.xsize;
  newwinoption.ysize   = option.ysize;


  Fl_Window* pwin = new Fl_Window(533, 271, "New game");

  { Fl_Group* gr = new Fl_Group(15, 20, 245, 135);

    Fl_Box* q = new Fl_Box(FL_ENGRAVED_BOX, gr->x(), gr->y(), gr->w(), gr->h(),
	"Player 1 (top <-> bottom)");
    q->labelsize(14);
    q->align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE);
    { Fl_Round_Button* o = new Fl_Round_Button(gr->x()+20, gr->y()+30, 90, 20, "Computer");
      o->type(FL_RADIO_BUTTON);
      o->labelsize(14);
      o->callback((Fl_Callback *)p1c_cb, pwin);
      if (!newwinoption.yhuman) o->setonly();
    }
    { Fl_Round_Button* o = new Fl_Round_Button(gr->x()+20, gr->y()+55, 90, 20, "Human");
      o->type(FL_RADIO_BUTTON);
      o->labelsize(14);
      o->callback((Fl_Callback *)p1h_cb, pwin);
      if (newwinoption.yhuman) o->setonly();
    }
    gr->end();
  }
  { Fl_Group* gr = new Fl_Group(275, 20, 245, 135);

    Fl_Box* q = new Fl_Box(FL_ENGRAVED_BOX, gr->x(), gr->y(), gr->w(), gr->h(),
	"Player 2 (left <-> right)");
    q->labelsize(14);
    q->align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE);
    { Fl_Round_Button* o = new Fl_Round_Button(gr->x()+20, gr->y()+30, 90, 20, "Computer");
      o->type(FL_RADIO_BUTTON);
      o->labelsize(14);
      o->callback((Fl_Callback *)p2c_cb, o);
      if (!newwinoption.xhuman) o->setonly();
    }
    { Fl_Round_Button* o = new Fl_Round_Button(gr->x()+20, gr->y()+55, 90, 20, "Human");
      o->type(FL_RADIO_BUTTON);
      o->labelsize(14);
      o->callback((Fl_Callback *)p2h_cb, o);
      if (newwinoption.xhuman) o->setonly();
    }
    gr->end();
  }
  { Fl_Check_Button* o = new Fl_Check_Button(20, 175, 190, 20, "Player 1 sets first pin");
    o->labelsize(14);
    o->value(option.ystarts ? 1 : 0);
    o->callback((Fl_Callback *)first_cb, o);
  }
  { Fl_Check_Button* o = new Fl_Check_Button(20, 200, 190, 20, "Pie-Rule is used");
    o->labelsize(14);
    o->value(option.pieRule ? 1 : 0);
    o->callback((Fl_Callback *)pie_cb, o);
  }
  { Fl_Counter* o = new Fl_Counter(76, 105, 160, 20, "Size: ");
    o->labelsize(14);
    o->align(FL_ALIGN_LEFT);
    o->range(12,36); // set 2nd value like MAXSIZE
    o->value(newwinoption.xsize);
    o->step(1);
    o->lstep(4);
    o->callback((Fl_Callback *)size_x_cb, o);
  }
  { Fl_Counter* o = new Fl_Counter(336, 105, 160, 20, "Size: ");
    o->labelsize(14);
    o->align(FL_ALIGN_LEFT);
    o->range(12,36); // set 2nd value like MAXSIZE
    o->value(newwinoption.ysize);
    o->step(1);
    o->lstep(4);
    o->callback((Fl_Callback *)size_y_cb, o);
  }
  {
    Fl_Box* o2 = new Fl_Box(90, 125, 100, 25, "(Official size is 24)");
    o2->labelsize(12);
    o2->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    Fl_Box* o3 = new Fl_Box(350, 125, 100, 25, "(Official size is 24)");
    o3->labelsize(12);
    o3->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  }
  {
    Fl_Return_Button* o = new Fl_Return_Button(350, 235, 80, 25, "OK");
    o->callback((Fl_Callback *)return_cb, pwin);
  }
  {
    Fl_Button* o = new Fl_Button(440, 235, 80, 25, "Cancel");
    o->callback((Fl_Callback *)cancel_cb, pwin);
  }
  pwin->end();
  pwin->callback((Fl_Callback *)cancel_cb,pwin);
  option.modalWindow = true;
  pwin->set_modal();
  pwin->show();

}
newWindow::~newWindow(){
}

/** Load preferences from file (static) */
void newWindow::load_preferences(){
  int zz;
  Fl_Preferences* pref = new Fl_Preferences(Fl_Preferences::USER,
      "mail@johannes-schwagereit.de", "T1");
  pref->get("xhuman", zz, 0);
  option.xhuman = zz ? true : false;
  pref->get("yhuman", zz, 1);
  option.yhuman = zz ? true : false;
  pref->get("ystarts", zz, 0);
  option.ystarts = zz ? true : false;
  pref->get("pieRule", zz, 1);
  option.pieRule = zz ? true : false;
  pref->get("xsize", zz, 24)   ;
  option.xsize = zz;
  pref->get("ysize", zz, 24)   ;
  option.ysize = zz;
  pref->get("level", zz, 5)   ;
  option.level = zz;
  pref->get("xcolor", zz, FL_BLACK)   ;
  option.XColor = (Fl_Color) zz;
  pref->get("ycolor", zz, FL_RED)   ;
  option.YColor = (Fl_Color) zz;
  pref->get("letterdir", zz, 0);
  option.letterdir = zz ? true : false;                                    
  pref->get("showAreaLines", zz, 0);
  option.showAreaLines = zz ? true : false;                                    
  delete pref;
}

/** Save preferences to file (static) */
void newWindow::save_preferences(){
  Fl_Preferences* pref = new Fl_Preferences(Fl_Preferences::USER,
      "mail@johannes-schwagereit.de", "T1");
  pref->set("xhuman", option.xhuman)  ;
  pref->set("yhuman", option.yhuman)  ;
  pref->set("ystarts", option.ystarts) ;
  pref->set("pieRule", option.pieRule) ;
  pref->set("xsize", option.xsize)   ;
  pref->set("ysize", option.ysize)   ;
  pref->set("level", option.level)   ;
  pref->set("xcolor", option.XColor)   ;
  pref->set("ycolor", option.YColor)   ;
  pref->set("letterdir", option.letterdir)   ;
  pref->set("showAreaLines", option.showAreaLines)   ;
  delete pref;
}

