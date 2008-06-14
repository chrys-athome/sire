/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "mouseclickhelper.h"

using namespace Spier;

MouseClickHelper::MouseClickHelper() 
                 : maxinterval(250), maxdelta(3), click_in_progress(false),
                   startevent(QEvent::MouseMove, QPoint(), Qt::NoButton, Qt::NoButton, Qt::NoModifier)
{
    timer.start();
}

MouseClickHelper::~MouseClickHelper()
{}

/** This function is called when a mouse button is pressed. 
    If the left mouse button is pressed then a mouse click may be in progress...
*/
void MouseClickHelper::press(QMouseEvent *e)
{
    if (e and e->button() == Qt::LeftButton)
    {
        //we have the start of a possible click - copy the initial event
        startevent = *e;
        
        click_in_progress = true;
        
        //start the timer
        timer.restart();
    }
}

/** This function is called whenever the mouse moves - this is used to 
    check if a click turns into a drag. This returns whether or not the 
    mouse click is in progress after the move event. */
bool MouseClickHelper::move(QMouseEvent *e)
{
    if (e and click_in_progress)
    {
        if (timer.elapsed() > maxinterval)
            click_in_progress = false;
        else
        {            
            QPoint del = e->globalPos() - startevent.globalPos();
    
            if (qAbs(del.x()) > maxdelta or qAbs(del.y()) > maxdelta)
                click_in_progress = false;
        }
    }
    
    return click_in_progress;
}

/** This function is called whenever the mouse button is released - 
    this checks to see if there has been a click. This returns the 
    mouseevent at the start of the click (the copy taken during 'press')
    if there was a valid mouse click, or 0 if there was no mouse click 
    
    \warning Do not delete the returned QMouseEvent pointer as it does
             not belong to you!    
*/
QMouseEvent* MouseClickHelper::release(QMouseEvent *e)
{
    if (e and click_in_progress)
    {
        if (e->button() == Qt::LeftButton)
        {
            int ms = timer.elapsed();
            click_in_progress = false;
    
            if (ms <= maxinterval)
                return &startevent;
        }
    }
    
    return 0;
}
