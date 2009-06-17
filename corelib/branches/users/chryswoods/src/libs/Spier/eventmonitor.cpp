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

#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "eventmonitor.h"
#include "cameracommand.h"

#include "SireUnits/units.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace Spier;
using namespace SireUnits;

/** Constructor */
EventMonitor::EventMonitor(RenderView &parent) 
             : QObject( &parent),
               render_view( &parent ),
               in_focus(false)
{}

/** Destructor */
EventMonitor::~EventMonitor()
{}

/** Return the render view whose events are being monitored */
const RenderView& EventMonitor::renderView() const
{
    if (render_view.isNull())
        throw SireError::nullptr_error( QObject::tr(
            "Trying to return a null RenderView!"), CODELOC );
            
    return *render_view;
}

/** Return the render view whose events are being monitored */
RenderView& EventMonitor::renderView()
{
    if (render_view.isNull())
        throw SireError::nullptr_error( QObject::tr(
            "Trying to return a null RenderView!"), CODELOC );
            
    return *render_view;
}

/** Does this widget hold focus? */
bool EventMonitor::inFocus() const
{
    return in_focus;
}

/** Process a key event */
bool EventMonitor::keyEvent(QKeyEvent *event)
{
    qDebug() << "GOT A KEY EVENT!";
    return true;
}

/** Process a mouse event */
bool EventMonitor::mouseEvent(QMouseEvent *event)
{
    if (renderView().hasFocus())
        in_focus = true;
    
    qDebug() << "GOT A MOUSE EVENT!";
    
    if (event->buttons() & Qt::LeftButton)
    {
        if (not mouse_global_pos.isNull())
        {
            renderView().execute( RotateCamera(
                             -0.5*(event->globalX() - mouse_global_pos.x()) * degrees,
                             -0.5*(event->globalY() - mouse_global_pos.y()) * degrees,
                             0 * degrees ) );
        }
    }
    
    if (event->buttons() == Qt::NoButton)
    {
        //forget the mouse position
        mouse_global_pos = QPoint();
    }
    else
    {
        //record the current position of the mouse
        mouse_global_pos = event->globalPos();
    }
    
    return true;
}

/** Process a mouse wheel event */
bool EventMonitor::wheelEvent(QWheelEvent *event)
{
    if (in_focus)
    {
        qDebug() << "GOT A WHEEL EVENT while in focus!";
        return true;
    }
    else
    {
        qDebug() << "GOT A WHEEL EVENT while not in focus!";
        //get rid of accidental focus caused by wheel view
        renderView().clearFocus();
        
        return false;
    }
}

/** Process a mouse entering the widget event */
bool EventMonitor::mouseEnterEvent()
{
    qDebug() << "MOUSE ENTERED THE WIDGET";
    return true;
}

/** Process a mouse leaving the widget event */
bool EventMonitor::mouseLeaveEvent()
{
    qDebug() << "MOUSE LEAVING THE WIDGET";

    //lose focus once the mouse leaves the widget
    renderView().clearFocus();
    
    return true;
}
    
/** Process the widget gaining edit focus */
bool EventMonitor::focusEvent(QFocusEvent *event)
{
    if (event->gotFocus())
    {
        qDebug() << "GAINED EDIT FOCUS" << event->reason();
        
        //only get focus when we next click in the window
        in_focus = false;
    }
    else
    {
        qDebug() << "LOST EDIT FOCUS" << event->reason();
        in_focus = false;
    }
        
    return true;
}

/** Event filter function */
bool EventMonitor::eventFilter(QObject *watched, QEvent *event)
{
    if (not event)
        return false;

    #if QT_VERSION >= 0x040400
        BOOST_ASSERT( watched == render_view.data() );
    #else
        BOOST_ASSERT( watched == render_view.operator->() );
    #endif

    switch( event->type() )
    {
        case QEvent::Enter:
            return this->mouseEnterEvent();
        
        case QEvent::Leave:
            return this->mouseLeaveEvent();
    
        case QEvent::FocusIn:
        case QEvent::FocusOut:
            return this->focusEvent( static_cast<QFocusEvent*>(event) );
    
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
            return this->keyEvent( static_cast<QKeyEvent*>(event) );
            
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
            return this->mouseEvent( static_cast<QMouseEvent*>(event) );
            
        case QEvent::Wheel:
            return this->wheelEvent( static_cast<QWheelEvent*>(event) );
            
        default:
            return false;
    }
}
