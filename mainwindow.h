/***************************************************************************
                          mainwindow.h  -  main window
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstring>
#include <stdlib.h>


#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Return_Button.H>

#include "boardwidget.h"
#include "showmovewidget.h"

/**
  *@author Johannes Schwagereit
  */

class MainWindow : public Fl_Double_Window  {
  Fl_Box *actPos; //displaybox
  char actPosStr[5];
public:
  MainWindow(int,int);
	~MainWindow();
  /** Set String for ActPos-Widget */
  void setActPos(char str[5]);
  /** Called when new game starts or new colors*/
  void redrawGame();
  showMoveWidget *lastmove;    //displaybox
  boardWidget *boart;
  Fl_Button *back_button;
  Fl_Button *forw_button;
};

#endif
