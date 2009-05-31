/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "inputinterpreter.h"

using namespace Spier;

/** Empty class has an empty constructor. */
InputInterpreter::InputInterpreter() : QSharedData()
{}

/** Destructor */
InputInterpreter::~ InputInterpreter()
{}

/** Default implementation does nothing */
void InputInterpreter::setTranslationMode()
{}

/** Default implementation does nothing */
void InputInterpreter::setRotationMode()
{}

/** Default implementation just updates the current position of the mouse */
void InputInterpreter::mouseMoveEvent(QMouseEvent *e)
{
    if (e)
    {
        currentpos = e->pos();
        currentpos_g = e->globalPos();
    }
}

/** Default implementation just updates the current position of the mouse */
void InputInterpreter::mousePressEvent(QMouseEvent *e)
{
    if (e)
    {
        currentpos = e->pos();
        currentpos_g = e->globalPos();
        
        e->accept();
    }
}

/** Default implementation ignores this event */
void InputInterpreter::mouseReleaseEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreter::mouseClickEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreter::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}
    
/** Default implementation ignores this event */
void InputInterpreter::wheelEvent(QWheelEvent *e)
{
    if (e)
        e->accept();
}
    
/** Default implementation ignores this event */
void InputInterpreter::keyPressEvent(QKeyEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreter::keyReleaseEvent(QKeyEvent *e)
{
    if (e)
        e->accept();
}

/** Return the current position of the mouse */
const QPoint& InputInterpreter::pos() const
{
    return currentpos;
}

/** Return the global coordinates of the current position of the mouse */
const QPoint& InputInterpreter::globalPos() const
{
    return currentpos_g;
}
