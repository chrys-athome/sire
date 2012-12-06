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

#include "Conspire/GUI/widgetrack.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

using namespace Conspire;

/** Constructor - by default, WidgetRacks are vertical */
WidgetRack::WidgetRack(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
    build( ::Qt::Vertical );
}

/** Construct a WidgetRack of specified orientation */
WidgetRack::WidgetRack(::Qt::Orientation orient, QGraphicsItem *parent)
           : QGraphicsWidget(parent)
{
    build(orient);
}

/** Destructor */
WidgetRack::~WidgetRack()
{}

/** Add the passed widget to the rack. The rack will take over 
    ownership of the widget and may delete it at any time */
void WidgetRack::addWidget(QGraphicsWidget *widget)
{
    if (widget)
    {
        QGraphicsLinearLayout *l = dynamic_cast<QGraphicsLinearLayout*>(
                                        scrollview->layout());
    
        l->addItem(widget);

        if (l->orientation() == ::Qt::Horizontal)
        {
            this->setMinimumHeight( scrollview->minimumHeight() );
        }
        else
        {
            this->setMinimumWidth( scrollview->minimumWidth() );
        }
    }
}

/** Add the passed widget to the rack. The rack will take over 
    ownership of the widget and may delete it at any time */
void WidgetRack::addWidget(QWidget *widget)
{
    if (widget)
    {
        QGraphicsProxyWidget *widget_proxy = new QGraphicsProxyWidget();
        widget_proxy->setWidget(widget);
        this->addWidget(widget_proxy);
    }
}

/** Add a divider to the rack */
void WidgetRack::addDivider()
{}

/** Resize the widget */
void WidgetRack::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    QGraphicsWidget::resizeEvent(e);
    scrollview->resize(this->size());
}

void WidgetRack::build(::Qt::Orientation orient)
{
    scrollview = new QGraphicsWidget(this);
    scrollview->setContentsMargins(0,0,0,0);
    scrollview->setLayout( new QGraphicsLinearLayout(orient) );
    scrollview->setContentsMargins(0,0,0,0);

    if (orient == ::Qt::Horizontal)
    {
        scrollview->layout()->setContentsMargins(0,0,0,0);
        scrollview->setPos(0,0);
        this->setMinimumHeight(scrollview->minimumHeight());
        //scrollview->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
        //this->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    }
    else
    {
        scrollview->setPos(0,0);
        scrollview->resize(this->size());
        this->setMinimumWidth(scrollview->minimumWidth());
        scrollview->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
        this->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
    }
}
