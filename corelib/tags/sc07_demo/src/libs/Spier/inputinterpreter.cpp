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

#include "inputinterpreter.h"

using namespace Spier;

/** Create a null InputInterpreter. This ignores everything. */
InputInterpreter::InputInterpreter()
{}

/** Create an InputInterpreter from the passed pointer. This will take over
    ownership of the pointer, so you should not delete it. */
InputInterpreter::InputInterpreter(InputInterpreterBase *base)
                 : boost::shared_ptr<InputInterpreterBase>(base)
{}

/** Copy constructor. This class is explicitly shared, so copying
    the object will mean that both copies will point to the same
    underlying InputInterpreterBase class. */
InputInterpreter::InputInterpreter(const InputInterpreter &other)
                 : boost::shared_ptr<InputInterpreterBase>(other)
{}

/** Destructor */
InputInterpreter::~InputInterpreter()
{}

/** Assignment operator. This class is explicitly shared, so copying
    the object will mean that both copies will point to the same
    underlying InputInterpreterBase class. */
InputInterpreter& InputInterpreter::operator=(const InputInterpreter &other)
{
    boost::shared_ptr<InputInterpreterBase>::operator=(other);
    return *this;
}

/** Assignment operator. This takes over ownership of the passed pointer, so you 
    should not delete it. */
InputInterpreter& InputInterpreter::operator=(InputInterpreterBase *ptr)
{
    boost::shared_ptr<InputInterpreterBase>::reset(ptr);
    return *this;
}

//////////////
////////////// Implementation of InputInterpreterBase
//////////////

/** Empty class has an empty constructor. */
InputInterpreterBase::InputInterpreterBase() : QObject()
{}

/** Destructor */
InputInterpreterBase::~ InputInterpreterBase()
{}

/** Default implementation does nothing */
void InputInterpreterBase::setTranslationMode()
{}

/** Default implementation does nothing */
void InputInterpreterBase::setRotationMode()
{}

/** Default implementation just updates the current position of the mouse */
void InputInterpreterBase::mouseMoveEvent(QMouseEvent *e)
{
    if (e)
    {
        currentpos = e->pos();
        currentpos_g = e->globalPos();
    }
}

/** Default implementation just updates the current position of the mouse */
void InputInterpreterBase::mousePressEvent(QMouseEvent *e)
{
    if (e)
    {
        currentpos = e->pos();
        currentpos_g = e->globalPos();
        
        e->accept();
    }
}

/** Default implementation ignores this event */
void InputInterpreterBase::mouseReleaseEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreterBase::mouseClickEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreterBase::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}
    
/** Default implementation ignores this event */
void InputInterpreterBase::wheelEvent(QWheelEvent *e)
{
    if (e)
        e->accept();
}
    
/** Default implementation ignores this event */
void InputInterpreterBase::keyPressEvent(QKeyEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreterBase::keyReleaseEvent(QKeyEvent *e)
{
    if (e)
        e->accept();
}
