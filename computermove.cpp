/***************************************************************************
                computermove.cpp  -  calculate move for computer
                   this file adds procedures to class situation
                             -------------------
    begin                : Son Jun 15 2003
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
#include "situation.h"

#include <iostream>

#include <cstdlib>
#include <time.h>

#include "pattern.h"

#include "options.h"
extern options option;

#define FIRSTDEPTH 3
short currentDepth;

// just a constant
#define TIMEOVER -2131415161 

double timelimit;
clock_t start; // Zeitmessung
short maxRated; // number of rated moves before timelimit exceeded





/*** comp_moves: Compare two moves. (used in quicksort) ***/
int comp_moves(const void *a, const void *b)
{
  return ( ((move*)b)->val - ((move*)a)->val );
}


//{ //inverse sort order, max first
//  if (((move *)m1)->val <  ((move *)m1)->val) return 1;
//  return (((move *)m1)->val == ((move *)m2)->val) ? 0 : -1;
//}

short situation::evalStructureCrit(bool dir)
{
  unsigned char x1,y1,x2,y2;
  while (stackcnt > 0)
  {
    //handle top of stack
    stackcnt --;

    x1 = graphst_x[stackcnt];
    y1 = graphst_y[stackcnt];
    if (dir == XDIR)
    {
      if (x1 < smin)
        smin = x1;
    }
    else
    {
      if (y1 < smin)
        smin = y1;
    }
    crit[x1][y1] = 1;

    //add all the 8 neighbours
    x2 = x1 + 1; y2 = y1 - 2;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 + 2; y2 = y1 - 1;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 + 2; y2 = y1 + 1;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 + 1; y2 = y1 + 2;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}

    x2 = x1 - 1; y2 = y1 + 2;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 - 2; y2 = y1 + 1;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 - 2; y2 = y1 - 1;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
    x2 = x1 - 1; y2 = y1 - 2;
    if (connected(x1,y1,x2,y2) && !crit[x2][y2])
      {graphst_x[stackcnt] = x2; graphst_y[stackcnt]=y2; stackcnt++;}
  }
  return smin; //return smallest x or y found because re-check may be necessary
}

short situation::startStructureCrit(short x,short y, bool dir)
{ //eval a graph of pegs and bridges to mark critical pins
  graphst_x[0] = (unsigned char) x;
  graphst_y[0] = (unsigned char) y;
  stackcnt = 1;
  smin = 100;
  return evalStructureCrit(dir);
}

void situation::addToMoveCrit(short x, short y, int upleft)
{ // add position to list of critival moves
  short i;
  for (i = critMoveI; i >= 1; i--)   //check for duplicates
    if (critMove[i].x == x &&  critMove[i].y == y && critMove[i].val == upleft)
      return;
      
  critMoveI ++;
  critMove[critMoveI].x   = x;
  critMove[critMoveI].y   = y;
  critMove[critMoveI].val = upleft;
}
  
inline void situation::putCrit(short x1, short y1, short m1, short x2, short y2, short m2, bool dir)
{
  //if critical, but is pin and not marked as critical before, call recursive critical-marking
  if (crit[x1][y1] == 0 && pin[x1][y1])
    startStructureCrit(x1, y1, dir);

  crit[x1][y1] = 1;
  if (!m1 && m2 && testBridge(x1,y1,x2,y2))
    {
    addToMoveCrit(x1,y1,0); //this is a critical pin to the right/down
    }
  if (m1 && !m2 && testBridge(x1,y1,x2,y2))
    {
    addToMoveCrit(x2,y2,1); //this is a critical pin to the left/up
    }
}

int situation::critical_y(int minf) // evaluate critical path as seen by YPLAYER (top-bottom)
  //input is result of evaluation
{
  short i,j, v1, x2, y2, m1, m2;
  memset(crit, 0, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to false

  for (j=3; j<option.xsize+1; j++)    //x
    if (value[j][option.ysize+1] == minf) crit[j][option.ysize+1] = 1;

  for (j=option.ysize; j>=2; j--)    // y
    for (i=3; i<option.xsize+1; i++)      // x
    {
      v1 = value[i][j];
      m1 = (pin[i][j]) ? 0 : 1;
      // check if neighbour below is critical and own value okay
      x2 = i + 1; y2 = j + 2; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,YDIR);
      x2 = i + 2; y2 = j + 1; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,YDIR);
      x2 = i - 2; y2 = j + 1; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,YDIR);
      x2 = i - 1; y2 = j + 2; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,YDIR);
     }     
//      if (crit[x2][y2] && (v1 == v2 - 1 || v1 == v2 - 2)) crit[i][j] = 1;

  return 0;
}
  
int situation::critical_x(int minf) // evaluate critical path as seen by XPLAYER (left-right)
  //input is result of evaluation
{
  short i,j, v1, x2, y2, m1, m2;
  memset(crit, 0, (MAXSIZE+4)*(MAXSIZE+4));  //    are set to false

  for (j=3; j<option.xsize+1; j++)    //y
    if (value[option.xsize+1][j] == minf) crit[option.xsize+1][j] = 1;

  for (i=option.xsize; i>=2; i--)    // x
    for (j=3; j<option.xsize+1; j++)      // y
    {
      v1 = value[i][j];
      m1 = (pin[i][j]) ? 0 : 1;
      // check if neighbour to the right is critical and own value okay
      x2 = i + 1; y2 = j + 2; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,XDIR);
      x2 = i + 2; y2 = j + 1; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,XDIR);
      x2 = i + 2; y2 = j - 1; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,XDIR);
      x2 = i + 1; y2 = j - 2; m2 = (pin[x2][y2]) ? 0 : 1;
      if (crit[x2][y2] && value[x2][y2] - m1 - m2 == v1) putCrit(i,j,m1,x2,y2,m2,XDIR);
     }

  return 0;
}

void situation::addToMovePoss(short x, short y, int val, move possMove[])
{ //add a move to the list of possible moves
  short i;

  //Check if legal Field
  if (movingPlayer == XPLAYER)
    {if (x < 2 || x > option.xsize + 1 || y < 3 || y > option.ysize    ) return;}
  else //YPLAYER
    {if (x < 3 || x > option.xsize     || y < 2 || y > option.ysize + 1) return;}

  //hole has to be empty
  if (pin[x][y]) return;
  
  for (i = possMoveI; i >= 1; i--)  //check for duplicates
    if (possMove[i].x == x &&  possMove[i].y == y)
      return;
      
  possMoveI ++;
  possMove[possMoveI].x   = x;
  possMove[possMoveI].y   = y;
}


void situation::addDefense(unsigned char thePlayer, move possMove[])
//add all defensive moves to list of possible moves
{
  short i, j;
  for (i=1; i <= critMoveI; i++)  // for all critical pins
    if (!checkPattern(3 - thePlayer, critMove[i].x, critMove[i].y, critMove[i].val, possMove, false))
      for (j = 0; j <= 5; j++)         //  for all defensive moves
        if (thePlayer == YPLAYER)
          if (critMove[i].val)  // val gives direction
            addToMovePoss(critMove[i].x + defense[j].y, critMove[i].y - defense[j].x,
              defense[j].val, possMove);
          else
            addToMovePoss(critMove[i].x + defense[j].y, critMove[i].y + defense[j].x,
              defense[j].val, possMove);
        else // (thePlayer == XPLAYER)
          if (critMove[i].val)
            addToMovePoss(critMove[i].x - defense[j].x, critMove[i].y + defense[j].y,
              defense[j].val, possMove);
          else
            addToMovePoss(critMove[i].x + defense[j].x, critMove[i].y + defense[j].y,
              defense[j].val, possMove);
}

void situation::addOffense(unsigned char thePlayer, move possMove[])
//add all offensive moves to list
{
  short i, j;
  for (i=1; i <= critMoveI; i++)  // for all critical pins
  {
    if (!checkPattern(thePlayer, critMove[i].x, critMove[i].y, critMove[i].val, possMove, true))
      //the check for rules returned false, so the normal moves are taken.
      for (j = 0; j <= 9; j++)         //  for all offensive moves
        if (thePlayer == YPLAYER)
          if (critMove[i].val)
            addToMovePoss(critMove[i].x + offense[j].y, critMove[i].y - offense[j].x,
              offense[j].val, possMove);
          else
            addToMovePoss(critMove[i].x + offense[j].y, critMove[i].y + offense[j].x,
              offense[j].val, possMove);
          else // (thePlayer == XPLAYER)
            if (critMove[i].val)
              addToMovePoss(critMove[i].x - offense[j].x, critMove[i].y + offense[j].y,
                offense[j].val, possMove);
            else
              addToMovePoss(critMove[i].x + offense[j].x, critMove[i].y + offense[j].y,
                offense[j].val, possMove);
  }
}

/** take last oppenent pin as only critical pin, direction is opposite to bridge layed
	\return critical pin found?
*/
void situation::criticalSingle(unsigned char thePlayer, short x, short y)
{ 
  x += 2;
  y += 2;
  if (thePlayer == YPLAYER)
  {
    if (connected(x,y,x-1,y+2) || connected(x,y,x+1,y+2) ||
        connected(x,y,x-2,y+1) || connected(x,y,x+2,y+1))
			/*std::cout << "S"*/;  
		else
			addToMoveCrit(x, y, 0);
    
		if (connected(x,y,x-1,y-2) || connected(x,y,x+1,y-2) ||
        connected(x,y,x-2,y-1) || connected(x,y,x+2,y-1))
			/*std::cout << "N"*/;  
		else
			addToMoveCrit(x, y, 1);
  }
  
	if (thePlayer == XPLAYER)
  {
    if (connected(x,y,x+1,y+2) || connected(x,y,x+1,y-2) ||
        connected(x,y,x+2,y+1) || connected(x,y,x+2,y-1))
			/*std::cout << "O"*/;  
		else
			addToMoveCrit(x, y, 0);
    
		if (connected(x,y,x-1,y+2) || connected(x,y,x-1,y-2) ||
        connected(x,y,x-2,y+1) || connected(x,y,x-2,y-1))
			/*std::cout << "W"*/;  
		else
			addToMoveCrit(x, y, 1);
  }
//   std::cout << "-" << ((critMoveI == 0) ? "nothing" : "found") << " - " << st << "\n";
  return;
}

short situation::generateMoves(unsigned char player, move possMove[], short depth)
{ //compute moves resulting from the critical positions

  //depending on depth only moves from depthMove are taken into consideration

  if (player == XPLAYER)
  {
    critMoveI = 0; // reset list of critical positions    
    if (depth < currentDepth)
      // last opponent move is critical
      criticalSingle(YPLAYER, depthMove[depth + 1].x, depthMove[depth + 1].y);
    else
	    critical_y(evaluate_y());
    addDefense(YPLAYER, possMove);
    
    critMoveI = 0; // reset list of critical positions
    if (depth < currentDepth - 1)
      // last own move is critical for offense
      criticalSingle(XPLAYER, depthMove[depth + 2].x, depthMove[depth + 2].y);
    else
      critical_x(evaluate_x());
    addOffense(XPLAYER, possMove);
  }
  else //YPLAYER
  {
    critMoveI = 0; // reset list of critical positions
    if (depth < currentDepth)
      criticalSingle(XPLAYER, depthMove[depth + 1].x, depthMove[depth + 1].y);
    else
	    critical_x(evaluate_x());    
    addDefense(XPLAYER, possMove);
    
    critMoveI = 0; // reset list of critical positions
    if (depth < currentDepth - 1)
      criticalSingle(YPLAYER, depthMove[depth + 2].x, depthMove[depth + 2].y);
    else
      critical_y(evaluate_y());
    addOffense(YPLAYER, possMove);
  }
  
  if (possMoveI == 0) //no move found, then generate one by random
  {
    short x, y;
    //srand((unsigned int)time((time_t *)NULL));
    do {
      x = (rand()%22 + 3);
      y = (rand()%22 + 3);
    } while (pin[x][y]);
    possMoveI ++;
    possMove[possMoveI].x   = x;
    possMove[possMoveI].y   = y;
  }
  return possMoveI;
}


short situation::getMoves(short depth, unsigned char player, move possMove[])
{
  possMoveI = 0; // reset list of possible moves


  if (depth == currentDepth && depth > FIRSTDEPTH)
  { //take the precomputed list of moves
    short i,j;
    short maxMoves = 35 - (currentDepth * 5);
    if (maxMoves < 12) maxMoves = 12;
    
    j = topMoveI;
    for (i = 1; i <= topMoveI; i++)
    {
      if (i > maxMoves || (topMove[i].val == -999999999 && i > 1)) //no more than this value
        {
          j = i - 1;
          i = topMoveI + 1;
        }
      else
        possMove[i] = topMove[i];
    }
    possMoveI = j;
    topMoveI = j;
    return possMoveI;
  }


  //generateMoves
  possMoveI = generateMoves(player, possMove, depth);
  
  //list is saved to be sorted and used later
  if (depth == currentDepth && depth == FIRSTDEPTH)
  { //save this list of moves
    int i;
    for (i = 1; i <= possMoveI; i++)
    {
      topMove[i] = possMove[i];
      topMove[i].val = -999999999;
    }
    topMoveI = possMoveI;
  }

  return possMoveI;
}

int situation::alphaBeta(short depth, int alpha, int beta, unsigned char player)
{
  move  possMove[MAXMOVES]; //list of possible next Moves
  short mv, i;
  char nb;
  int valu;
  unsigned char x1, y1;
  bool moveFound = false;
 
  if (depth == 0)
  {
    return evaluate(computerPlayer) * 10 + (rand()%10); //  some random for equally good moves
  }
  movingPlayer = player;
  mv = getMoves(depth, player, possMove);

  if (player == computerPlayer) //max-node
  {
    
    for (i=1; i <= mv; i++)  // for all possible moves
    {
      x1 = possMove[i].x-2;
      y1 = possMove[i].y-2;
      nb = setPin(x1,y1,player);
      if (nb >= 0) // returns no of bridges
      {                 
//        if (nb == 0 && depth <= 2 && (i != mv || moveFound))
          //**Fehler: possMove.val hat nicht immer den richigen Wert
        if (nb == 0 && (depth <= 2 /*|| possMove[i].val*/) && moveFound)
           // pins without bridges are not interesting in both lowest levels except if last move
           // and no other move was found
          removePin(x1,y1,player);
        else
        {
          moveFound = true;
          depthMove[depth].x = x1; //save move to be used in deeper levels
          depthMove[depth].y = y1;
          if (depth == currentDepth && depth == FIRSTDEPTH)
            //1st depth without alphabeta to allow sorting for higher depths
            {
              alpha = -2147483647;
              valu = alphaBeta(depth - 1, alpha, beta, 3 - player);
            }
          else
            valu = alphaBeta(depth - 1, alpha, beta, 3 - player);
          removePin(x1,y1,player);
          
          //check the time limit
          if ((clock()-start) >= timelimit || valu == TIMEOVER) return TIMEOVER;

          if (depth == currentDepth)
            { topMove[i].val = valu;
              maxRated = i;
            }
          if (valu > alpha) //new best found
          {
            alpha = valu;
          }
          if (alpha >= beta)   //cutoff
          {
            //take move as killermove if different than other killermove
            /*if (killer[depth][1 - killerI[depth]].x != x1 + 2 ||
                killer[depth][1 - killerI[depth]].y != y1 + 2)
            {
              killer[depth][killerI[depth]].x = x1 + 2;
              killer[depth][killerI[depth]].y = y1 + 2;
              killerI[depth] = 1 - killerI[depth];
            }*/
            return alpha; //cutoff
          }
        }
      }
    }
    return alpha;  
  }
  else
  { //min-node
    for (i=1; i <= mv; i++)  // for all possible moves
    {
      x1 = possMove[i].x-2;
      y1 = possMove[i].y-2;
      nb = setPin(x1,y1,player);
      if (nb >= 0) // returns no of bridges
      {
        if (nb == 0 && depth <= 2 && (i != mv || moveFound))
           // pins without bridges are not interesting in both lowest levels except if last move
           // and no other move was found
          removePin(x1,y1,player);
        else
        {
          moveFound = true;
          depthMove[depth].x = x1; //save move to be used in deeper levels
          depthMove[depth].y = y1;
          valu = alphaBeta(depth - 1, alpha, beta, 3 - player);
          removePin(x1,y1,player);
          //check the time limit
          if (valu == TIMEOVER) return TIMEOVER;
          
          if (depth == currentDepth)
            { topMove[i].val = valu;
            }
          beta = MIN(beta, valu);
          if (alpha >= beta)
          {
            //take move as killermove if different than other killermove
            /*if (killer[depth][1 - killerI[depth]].x != x1 + 2 ||
                killer[depth][1 - killerI[depth]].y != y1 + 2)
            {
              killer[depth][killerI[depth]].x = x1 + 2;
              killer[depth][killerI[depth]].y = y1 + 2;
              killerI[depth] = 1 - killerI[depth];
            }
            */
            return beta; //cutoff
          }
        }
      }
    }
    return beta;
  }
  return 0; //dummy, never used
}  


void situation::startAlphaBeta()
{
  srand((unsigned int)time((time_t *)NULL));
  timelimit = option.level * CLOCKS_PER_SEC; //sec.
  winningMove = false;
  topMoveI = 0;
  evalNumber = 0;
  
  start = clock();

  //reset lowlevel-moves (Moves of depth 1 and 2 are precomputed at level 2 resp. 3)
  llMove[0][1].val = -2147483647;

  //is there already a gameover-situation?
  currentDepth = 2;
  alphaBeta(2, -2147483647, 2147483647, computerPlayer);
  
  if (!winningMove)
  { //game not already over
  
      
    topMoveI = 0;
    
    currentDepth = FIRSTDEPTH;
    do{     //iterative deepening
    
      maxRated = 0; //reset counter
      
      alphaBeta(currentDepth, -2147483647, 2147483647, computerPlayer);

      //sort the moves by values as computed before
      qsort(
          topMove + 1,       /* Pointer to elements		*/
          maxRated,          /* Number of elements		*/
          sizeof(move),     /* size of one element.		*/
          comp_moves        /* Pointer to comparison function */
        );

      
      currentDepth += 2;
    } while((clock()-start) < timelimit && currentDepth <= 15 && !winningMove);

      
    bestX = topMove[1].x - 2; 
    bestY = topMove[1].y - 2;
  }
  
}



