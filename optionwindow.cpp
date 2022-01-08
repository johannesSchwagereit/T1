/***************************************************************************
       optionwindow.cpp  -  set  option which can be changed during game
                             -------------------
    begin                : Wed Feb 19 2003
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


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>

#include <iostream>

#include "optionwindow.h"
#include "newwindow.h"

#include "options.h"
extern options option;


void return_opt_cb(Fl_Widget *w, void *data) //return was pressed
{
  optionWindow* ow = (optionWindow*)data;
  Fl_Window* e = ow->pwin;
  e->hide();
  delete e;
  option.modalWindow = false;
  //save value
  option.XColor  = ow->colorList[ow->choice2->value()];
  option.YColor  = ow->colorList[ow->choice1->value()];

  option.letterdir = ow->letterVal;
  option.showAreaLines = ow->areaVal;
  

  newWindow::save_preferences(); //this function is static!
  option.currentMainWindow->redrawGame();
}

void cancel_opt_cb(Fl_Widget *w, void *data) //cancel was pressed  or exit-button (corner up right)
{
  Fl_Window* e = (Fl_Window*)data;
  e->hide();
  delete e;
  option.modalWindow = false;
}

void check_letter_cb(Fl_Widget *w, void *data) //check-button callback
{
  optionWindow* ow = (optionWindow*)data;
  ow->letterVal = !ow->letterVal;
}

void check_area_cb(Fl_Widget *w, void *data) //check-button callback
{
  optionWindow* ow = (optionWindow*)data;
  ow->areaVal = !ow->areaVal;
}

optionWindow::optionWindow(){
  uchar i;
  colorList[0] = FL_RED;
  colorList[1] = FL_MAGENTA;
  colorList[2] = FL_DARK_YELLOW;
  colorList[3] = FL_BLUE;
  colorList[4] = FL_GREEN;
  colorList[5] = FL_DARK_CYAN;
  colorList[6] = FL_DARK3;
  colorList[7] = FL_BLACK;

  color1 = 0;
  color2 = 6;
  for (i=0; i<8; i++)
  {
    if (option.YColor == colorList[i])
      color1 = i;
    if (option.XColor == colorList[i])
      color2 = i;
  }

  Fl_Menu_Item menu_first[] = {
    {"Red",         0, 0, 0, 0, 0, 0, 12, 56},
    {"Magenta",     0, 0, 0, 0, 0, 0, 12, 56},
    {"Dark Yellow", 0, 0, 0, 0, 0, 0, 12, 56},
    {"Blue",        0, 0, 0, 0, 0, 0, 12, 56},
    {"Green",       0, 0, 0, 0, 0, 0, 12, 56},
    {"Cyan",        0, 0, 0, 0, 0, 0, 12, 56},
    {"Gray",        0, 0, 0, 0, 0, 0, 12, 56},
    {"Black",       0, 0, 0, 0, 0, 0, 12, 56},
    {0}
  };

  for (i=0; i<9; i++)
    menu_Auswahl[i] = menu_first[i];

  pwin = new Fl_Window(304, 251, "Set Options");
  choice1 = new Fl_Choice(120, 25, 110, 25, "Color Player 1:");
  choice1->down_box(FL_BORDER_BOX);
  choice1->labelsize(12);
  choice1->textsize(12);
  choice1->menu(menu_Auswahl);
  choice1->value(color1);
  choice2 = new Fl_Choice(120, 65, 110, 25, "Color Player 2:");
  choice2->down_box(FL_BORDER_BOX);
  choice2->labelsize(12);
  choice2->textsize(12);
  choice2->menu(menu_Auswahl);
  choice2->value(color2);


  //Checkbox to switch between "Letters on Columns" and "Letters on Rows"
  check = new Fl_Check_Button(37,105,127,20,"Letters on Columns");
  check->callback((Fl_Callback *)check_letter_cb, this);
  check->labelsize(12);
  letterVal = option.letterdir;
  check->value((option.letterdir) ? 0 : 1);

  //Checkbox to switch if diagonal lines ("area lines") are shown
  check = new Fl_Check_Button(37,125,127,20,"Show area lines");
  check->callback((Fl_Callback *)check_area_cb, this);
  check->labelsize(12);
  areaVal = option.showAreaLines;
  check->value((option.showAreaLines) ? 1 : 0);
  
  //Return- and Cancel-Button
  {
    Fl_Return_Button* o = new Fl_Return_Button(120, 215, 80, 25, "OK");
    o->callback((Fl_Callback *)return_opt_cb, this);
  }
  {
    Fl_Button* o = new Fl_Button(210, 215, 80, 25, "Cancel");
    o->callback((Fl_Callback *)cancel_opt_cb, pwin);
  }
  pwin->end();
  pwin->callback((Fl_Callback *)cancel_opt_cb,pwin);
  option.modalWindow = true;
  pwin->set_modal();
  pwin->show();

}

optionWindow::~optionWindow(){
}
