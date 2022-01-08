/***************************************************************************
                          options.h  -  public game-options
                             -------------------
    begin                : Sun Oct 20 2002
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

#define XDIR true
#define YDIR false


struct options
{
  unsigned char xsize; //size in x-dir
  unsigned char ysize; //size in y-dir
  bool letterdir; //Beschriftung: letter from left to right or up to down
  bool showAreaLines; //Show area lines (diagonal)
  bool xhuman; //x-player played by human
  bool yhuman; //y-player played by human
  bool pieRule; //pierule is used
  bool ystarts; //y-player starts
  unsigned char level; //time (sec) per computer move
  Fl_Color XColor; // Color for Player on x-axis (Player 2)
  Fl_Color YColor; // Color for Player on y-axis (Player 1)
  bool modalWindow; //True, if a modal window is currently open
  short gameOver; // >0, if current game is over
  MainWindow* currentMainWindow; //The mainwindow
};
