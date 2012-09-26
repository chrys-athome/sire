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

#ifndef SPIER_EVENTMONITOR_H
#define SPIER_EVENTMONITOR_H

#include <QPointer>

#include "renderview.h"

SIRE_BEGIN_HEADER

class QEvent;
class QFocusEvent;
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;

namespace Spier
{

/** This class is a Qt event filter that monitors and processes
    all of the events that are destined for a Spier::RenderView.
    This allows for different event filters to be easily swapped
    within a RenderView - and also allows all event monitoring
    code to be separated from the RenderView code
    
    @author Christopher Woods
*/
class SPIER_EXPORT EventMonitor : public QObject
{
    Q_OBJECT
    
public:
    EventMonitor(RenderView &parent);
    
    ~EventMonitor();
    
    static const char* typeName()
    {
        return "Spier::EventMonitor";
    }

protected:
    const RenderView& renderView() const;
    RenderView& renderView();

    bool inFocus() const;

    bool eventFilter(QObject *watched, QEvent *event);

    virtual bool keyEvent(QKeyEvent *event);
    virtual bool mouseEvent(QMouseEvent *event);
    virtual bool wheelEvent(QWheelEvent *event);

    virtual bool mouseEnterEvent();
    virtual bool mouseLeaveEvent();
    
    virtual bool focusEvent(QFocusEvent *event);

private:
    /** Pointer to the parent render view */
    QPointer<RenderView> render_view;
    
    /** The last known position of the mouse */
    QPoint mouse_global_pos;
    
    /** Whether or not the widget is currently focussed */
    bool in_focus;
};

}

SIRE_END_HEADER

#endif
