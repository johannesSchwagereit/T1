/***************************************************************************
                  main.cpp  -  main of T1 (A program to play Twixt)
                             -------------------
    begin                : Die Okt  8 22:20:48 CEST 2002
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
#include <FL/Fl_Box.H>
#include <FL/Fl_Light_Button.H>

#include "mainwindow.h"
#include "options.h"
options option;

#include "situation.h"
situation* actSit; //current situation on the board

MainWindow *window;

int main(int argc, char **argv) {

  //set default for options
  option.xsize = 24; // 12-32
  option.ysize = 24; // 12-32
  option.letterdir = YDIR;
  option.XColor = FL_GREEN;
  option.YColor = FL_RED;
  option.modalWindow = false;
  option.gameOver = 0; // false

  window = new MainWindow(506,410);
  window->show(argc, argv);

  actSit = new situation(window);

  if (option.ystarts)
    actSit->initialize(YPLAYER);
  else
    actSit->initialize(XPLAYER);

  return Fl::run();
}
