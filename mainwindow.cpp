/***************************************************************************
                          mainwindow.cpp  -  main window
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
#include "options.h"
#include "newwindow.h"
#include "levelwindow.h" //set level = (time per computer-move)
#include "optionwindow.h"
#include <iostream>

#include "situation.h"

const int MENUBARHEIGHT = 25;

extern situation* actSit;

extern options option;

char ost[675]; //outputstring

void about_rb_cb(Fl_Widget *w, void *data) //About-Dialog - Return - Button
{
  Fl_Window* e = (Fl_Window*)data;
  e->hide();
  option.modalWindow = false;
  }

void option_cb(Fl_Widget *w, void *data)
{
  /*optionWindow* p =*/ new optionWindow(); //set options (color etc.)
}

void level_cb(Fl_Widget *w, void *data)
{
  /*levelWindow* p =*/ new levelWindow(); //set time per computer-move
}

void open_cb(Fl_Widget *w, void *data)
{  actSit->loadGame();  }

void save_cb(Fl_Widget *w, void *data)
{  actSit->saveGame();  }

void switch_cb(Fl_Widget *w, void *data)
{;}

void quit_cb(Fl_Widget *w, void *data)
{exit(0);}

void new_cb(Fl_Widget *w, void *data)
{
  /*newWindow* p =*/ new newWindow();  //open window for settings of new game
}



void about_cb(Fl_Widget *w, void *data)  //display an about-Dialog for T1
{

  strcpy(ost, "TwixT is a boardgame by Alex Randolph.\n\n");

  strcat(ost, "T1 is a program to play TwixT\n");
  strcat(ost, "(c) 2004, Johannes Schwagereit (mail@@johannes-schwagereit.de)\n");

  strcat(ost, "\nVersion 0.6.0\n");
  strcat(ost, "Date of this version: ");
  strcat(ost, __DATE__);

  strcat(ost, "\n\n");
  strcat(ost, "This program is free software; you can redistribute it and/or modify\n");
  strcat(ost, "it under the terms of the GNU General Public License as published by\n");
  strcat(ost, "the Free Software Foundation; either version 2 of the License, or   \n");
  strcat(ost, "(at your option) any later version.");
  
  //Fl::set_font(FL_FREE_FONT, "-*-bitstream vera sans-medium-o-*-*-*-120-*-*-*-*-*-*");

  Fl_Window* o = new Fl_Window(470, 272,"About T1");
  Fl_Box *box = new Fl_Box(20,20,430,180,ost);
  box->box(FL_NO_BOX);
  box->labelsize(13);
  //box->labelfont(FL_FREE_FONT);
  box->labelfont(FL_HELVETICA);
  box->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  Fl_Return_Button* arb = new Fl_Return_Button(o->w()/2 - 45, o->h() - 45, 90, 25, "OK");
  arb->callback((Fl_Callback *)about_rb_cb, o);
  o->callback((Fl_Callback *)about_rb_cb, o);
  o->end();
  o->set_modal();
  option.modalWindow = true;
  o->show();
}

void back_button_cb(Fl_Widget *w, void *data)
{
  actSit->back_move();
}

void forw_button_cb(Fl_Widget *w, void *data)
{
  actSit->forward_move();
}


MainWindow::MainWindow(int w, int h) : Fl_Double_Window(w,h)
{
  option.currentMainWindow = this;
  newWindow::load_preferences(); //this function is static!

  Fl_Menu_Item menuitems[] = {
  { "&File",              0, 0, 0, FL_SUBMENU,0,0,13 },
    { "&New Game",        FL_CTRL + 'N', (Fl_Callback *)new_cb,0,0,0,0,13 },
    { "&Load Game",       FL_CTRL + 'L', (Fl_Callback *)open_cb,0,0,0,0,13 },
    { "&Save Game",       FL_CTRL + 'S', (Fl_Callback *)save_cb,0,0,0,0,13 },
    { "E&xit",            FL_CTRL + 'X', (Fl_Callback *)quit_cb,0,0,0,0,13},
    { 0 },

  { "&Setup", 0, 0, 0,    FL_SUBMENU,0,0,13 },
    //{ "S&witch sides",    FL_CTRL + 'W', (Fl_Callback *)switch_cb, 0,FL_MENU_INACTIVE,0,0,13},
    { "O&ptions",         FL_CTRL + 'P', (Fl_Callback *)option_cb, 0,0,0,0,13},
    { "L&evel",           FL_CTRL + 'E', (Fl_Callback *)level_cb, 0,0,0,0,13},
    { "&About...",        FL_CTRL + 'A', (Fl_Callback *)about_cb, 0,0,0,0,13},
    { 0 },

  { 0 }
  };
  
  Fl_Menu_Bar *m = new Fl_Menu_Bar(0, 0, w, MENUBARHEIGHT);
  m->copy(menuitems);

  boart = new boardWidget(5, 5 + MENUBARHEIGHT,w - 115,h - MENUBARHEIGHT - 5*2,"TwixtBoard");
      
  actPos = new Fl_Box(w - 95,h - MENUBARHEIGHT - 5,60,22,"");
  actPos->box(FL_THIN_DOWN_BOX);
  actPos->labelsize(18);
  actPos->labeltype(FL_EMBOSSED_LABEL);

  back_button = new Fl_Button(w - 95, MENUBARHEIGHT + 15,30,30, "@<-");
  back_button->clear_visible_focus(); //no focus visible
  back_button->deactivate();
  back_button->callback((Fl_Callback *)back_button_cb, back_button);
  forw_button = new Fl_Button(w - 60, MENUBARHEIGHT + 15,30,30, "@->");
  forw_button->clear_visible_focus(); //no focus visible
  forw_button->deactivate();
  forw_button->callback((Fl_Callback *)forw_button_cb, forw_button);
                                                                                                    
  lastmove = new showMoveWidget(w - 102,110,95,90,"Show last move");
      
  end();

  resizable(boart);
  label("T1");
}
MainWindow::~MainWindow(){}


/** Set String for ActPos-Widget */
void MainWindow::setActPos(char str[5]){
  strcpy(actPosStr,str);
  actPos->label(actPosStr);
  actPos->redraw();
}

/** Called when new game starts or new colors*/
void MainWindow::redrawGame(){
  boart->redraw();
  lastmove->redraw();
}
