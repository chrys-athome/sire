#ifndef CONSPIRE_WIDGETSTACK_H
#define CONSPIRE_WIDGETSTACK_H
/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include <QGraphicsWidget>
#include <QPointer>
#include <QList>

#include "Conspire/conspire.h"

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This widget provides a stack which can display
        one widget at a time */
    class WidgetStack : public QGraphicsWidget
    {
        Q_OBJECT
        
    public:
        WidgetStack(QGraphicsItem *parent=0);
        ~WidgetStack();
        
        void addWidget(QGraphicsWidget *page);
        void addWidget(QWidget *page);
        
        void switchTo(int idx);
        
    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);
        
    private:
        QList< QPointer<QGraphicsWidget> > pgs;
        
        QPointer<QGraphicsWidget> current_page;
    };

}

CONSPIRE_END_HEADER

#endif
