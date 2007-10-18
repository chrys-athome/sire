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

#ifndef SPIER_INPUTINTERPRETER_H
#define SPIER_INPUTINTERPRETER_H

#include <boost/shared_ptr.hpp>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include <QObject>

#include "glwidgetset.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This class is the base class of all input interpreters. An input interpreter is a class that interprets mouse and keyboard events and performs the requested actions. This class is designed to be used with the GLCanvas, as there are many different input modes, which are best handled using polymorphic input interpreter class, rather than multiple input modes.
 
@author Christopher Woods
*/
class SPIER_EXPORT InputInterpreterBase : public QObject
{

Q_OBJECT

public:
    InputInterpreterBase();
    virtual ~InputInterpreterBase();
    
    const QPoint& pos() const;
    const QPoint& globalPos() const;
    
    const GLWidgetSet& widgets() const;
    
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseClickEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    
    virtual void wheelEvent(QWheelEvent *e);
    
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);

public slots:
    virtual void setTranslationMode();
    virtual void setRotationMode();

protected:

    /** The current position of the mouse */
    QPoint currentpos;
    /** The global coordinates of the current known position of the mouse */
    QPoint currentpos_g;
    
    /** The (optional) set of GLWidgets associated with this view */
    GLWidgetSet interpreterwidgets;
};

/** Return the current position of the mouse */
inline const QPoint& InputInterpreterBase::pos() const
{
    return currentpos;
}

/** Return the global coordinates of the current position of the mouse */
inline const QPoint& InputInterpreterBase::globalPos() const
{
    return currentpos_g;
}

/** Return the set of widgets associated with this interpreter */
inline const GLWidgetSet& InputInterpreterBase::widgets() const
{
    return interpreterwidgets;
}

/** The InputInterpreterBase is wrapped up by the shared pointer 'InputInterpreter' class. 

    @author Christopher Woods
*/
class InputInterpreter : private boost::shared_ptr<InputInterpreterBase>
{
public:
    InputInterpreter();
    InputInterpreter(InputInterpreterBase *ptr);
    InputInterpreter(const InputInterpreter &other);

    ~InputInterpreter();
    
    InputInterpreter& operator=(const InputInterpreter &other);
    InputInterpreter& operator=(InputInterpreterBase *ptr);
    
    QPoint pos() const;
    QPoint globalPos() const;
    
    GLWidgetSet widgets() const;
    
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseClickEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    
    void wheelEvent(QWheelEvent *e);
    
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    
    void setTranslationMode();
    void setRotationMode();
};

/** Handle mouse move events */
inline void InputInterpreter::mouseMoveEvent(QMouseEvent *e)
{
    if (this->get())
        this->get()->mouseMoveEvent(e);
}

/** Handle mouse press events */
inline void InputInterpreter::mousePressEvent(QMouseEvent *e)
{
    if (this->get())
        this->get()->mousePressEvent(e);
}

/** Handle mouse release events */
inline void InputInterpreter::mouseReleaseEvent(QMouseEvent *e)
{
    if (this->get())
        this->get()->mouseReleaseEvent(e);
}

/** Handle mouse click events */
inline void InputInterpreter::mouseClickEvent(QMouseEvent *e)
{
    if (this->get())
        this->get()->mouseClickEvent(e);
}

/** Handle mouse double-click events */
inline void InputInterpreter::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (this->get())
        this->get()->mouseDoubleClickEvent(e);
}

/** Handle mouse wheel events */
inline void InputInterpreter::wheelEvent(QWheelEvent *e)
{
    if (this->get())
        this->get()->wheelEvent(e);
}

/** Handle key press events */
inline void InputInterpreter::keyPressEvent(QKeyEvent *e)
{
    if (this->get())
        this->get()->keyPressEvent(e);
}

/** Handle mouse move events */
inline void InputInterpreter::keyReleaseEvent(QKeyEvent *e)
{
    if (this->get())
        this->get()->keyReleaseEvent(e);
}

/** Return the current position of the mouse */
inline QPoint InputInterpreter::pos() const
{
    if (this->get())
        return this->get()->pos();
    else
        return QPoint();
}

/** Return the current global position of the mouse */
inline QPoint InputInterpreter::globalPos() const
{
    if (this->get())
        return this->get()->globalPos();
    else
        return QPoint();
}

/** Return the set of widgets associated with this interpreter */
inline GLWidgetSet InputInterpreter::widgets() const
{
    if (this->get())
        return this->get()->widgets();
    else
        return GLWidgetSet();
}

/** Set translation mode */
inline void InputInterpreter::setTranslationMode()
{
    if (this->get())
        this->get()->setTranslationMode();
}

/** Set rotation mode */
inline void InputInterpreter::setRotationMode()
{
    if (this->get())
        this->get()->setRotationMode();
}

}

SIRE_END_HEADER

#endif
