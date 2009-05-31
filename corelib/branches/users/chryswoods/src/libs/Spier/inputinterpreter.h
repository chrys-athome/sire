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

#ifndef SPIER_INPUTINTERPRETER_H
#define SPIER_INPUTINTERPRETER_H

#include <QSharedData>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

SIRE_BEGIN_HEADER

namespace Spier
{

/** This class is the base class of all input interpreters. 
    An input interpreter is a class that interprets mouse and keyboard
    events and performs the requested actions. This class is designed 
    to be used with the GLCanvas, as there are many different input modes, 
    which are best handled using polymorphic input interpreter class, rather 
    than multiple input modes.
 
    @author Christopher Woods
*/
class SPIER_EXPORT InputInterpreter : public QSharedData
{

Q_OBJECT

public:
    InputInterpreter();
    virtual ~InputInterpreter();
    
    static const char* typeName()
    {
        return "Spier::InputInterpreter";
    }
    
    virtual const char* what() const=0;
    
    virtual InputInterpreter* clone() const=0;
    
    const QPoint& pos() const;
    const QPoint& globalPos() const;
    
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseClickEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    
    virtual void wheelEvent(QWheelEvent *e);
    
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);

    virtual void setTranslationMode();
    virtual void setRotationMode();

protected:
    /** The current position of the mouse */
    QPoint currentpos;
    
    /** The global coordinates of the current known position of the mouse */
    QPoint currentpos_g;
};

}

SIRE_END_HEADER

#endif
