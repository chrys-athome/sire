#ifndef CONSPIRE_WIDGETRACK_H
#define CONSPIRE_WIDGETRACK_H
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

#include "Conspire/conspire.h"

#include <QGraphicsWidget>

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This provides a rack on which widgets can be stacked.
        Widgets can either be racked horizontally or vertically.
        If there is not enough space to view the whole rack, then
        the widgets can be scrolled */
    class CONSPIRE_EXPORT WidgetRack : public QGraphicsWidget
    {
        Q_OBJECT
    
    public:
        WidgetRack(QGraphicsItem *parent=0);
        WidgetRack(::Qt::Orientation orient, QGraphicsItem *parent=0);
        
        ~WidgetRack();
        
        void addWidget(QWidget *widget);
        void addWidget(QGraphicsWidget *widget);

        void addDivider();

    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *e);

    private:
        void build(::Qt::Orientation orient);
        
        QGraphicsWidget *scrollview;
    };

}

CONSPIRE_END_HEADER

#endif
