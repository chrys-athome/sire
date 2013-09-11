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

#include "Conspire/GUI/slidelabel.h"

#include <QPainter>
#include <QRectF>

using namespace Conspire;

void SlideLabel::build()
{}

/** Constructor */
SlideLabel::SlideLabel(QGraphicsItem *parent) : QGraphicsWidget(parent), w(0)
{}

/** Construct, providing the text contents of the label */
SlideLabel::SlideLabel(QString text, QGraphicsItem *parent)
           : QGraphicsWidget(parent), txt(text), w(0)
{}

/** Destructor */
SlideLabel::~SlideLabel()
{}

/** Set the widget to be displayed in the SlideLabel */
void SlideLabel::setWidget(QGraphicsWidget *widget)
{
    if (w)
    {
        delete w.data();
        w = 0;
    }
    
    w = widget;
    
    if (w)
    {
        w->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
        w->setParent(this);
        w->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    }
}

/** Set the text to be displayed in the label */
void SlideLabel::setText(QString text)
{
    txt = text;
}

/** Return the text to be displayed in the label */
QString SlideLabel::text() const
{
    return txt;
}

void SlideLabel::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget)
{
    QGraphicsWidget::paint(painter, option, widget);

    //paint the slider (if viewed)
    painter->drawRect( QRectF(10, 10, geometry().width()-20, geometry().height()-20) );
}

void SlideLabel::focusInEvent(QFocusEvent *event)
{
    QGraphicsWidget::focusInEvent(event);
}

void SlideLabel::focusOutEvent(QFocusEvent *event)
{
    QGraphicsWidget::focusOutEvent(event);
}

void SlideLabel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mousePressEvent(event);
}

void SlideLabel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseReleaseEvent(event);
}

void SlideLabel::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);

    if (w)
        w->setGeometry(0, 0, geometry().width(), geometry().height());
}
