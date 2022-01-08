/***************************************************************************
                          newwindow.h  - set data for new game
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

#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <FL/Fl_Double_Window.H>
#include "mainwindow.h"

/**Set options for new game
  *@author Johannes Schwagereit
  */

class newWindow {
private:

public: 
  newWindow();
  ~newWindow();
  /** Load preferences from file */
  static void load_preferences();
  /** Save preferences to file */
  static void save_preferences();
};

#endif
