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

#include "Conspire/GUI/button.h"

#include <QPainter>
#include <QColor>
#include <QStaticText>

#include <QGraphicsSceneMouseEvent>

using namespace Conspire;

/** Constructor */
Button::Button(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
    build();
}

/** Constructor, specifying the text to be written on the button */
Button::Button(QString text, QGraphicsItem *parent) : QGraphicsWidget(parent)
{
    build();
    setText(text);
}

/** Destructor */
Button::~Button()
{
    delete txt;
}

/** Build this button */
void Button::build()
{
    txt = 0;
    has_focus = false;
    is_primed = false;
    is_checked = false;
    
    this->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}

/** Return the text (if any) in the button */
QString Button::text() const
{
    if (txt)
        return txt->text();
    else
        return QString::null;
}

/** Set the text to be rendered */
void Button::setText(QString t)
{
    if (t == this->text())
        return;

    if (txt)
        delete txt;
    
    if (not t.isEmpty())
    {
        QSizeF button_size = this->size();

        txt = new QStaticText(t);
        txt->setTextWidth( 0.8 * button_size.width() );
        txt->prepare( QTransform(), QFont("Helvetica [Cronyx]", 12) );
        
        QSizeF text_size = txt->size();
        
        offset_x = 0.5 * (button_size.width() - text_size.width());
        offset_y = 0.5 * (button_size.height() - text_size.height());
        
        update(this->geometry());
    }
}

void Button::focusInEvent(QFocusEvent *event)
{
    QGraphicsWidget::focusInEvent(event);
    conspireDebug() << "Button" << this->text() << "got focus :-)";
}

void Button::focusOutEvent(QFocusEvent *event)
{
    QGraphicsWidget::focusOutEvent(event);
    conspireDebug() << "Button" << this->text() << "lost focus :-)";
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mousePressEvent(event);
    conspireDebug() << "Button" << this->text() << "mouse pressed :-)";

    is_primed = true;
        
    event->accept();
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseReleaseEvent(event);
    conspireDebug() << "Button" << this->text() << "mouse released :-)";
    
    if (is_primed)
    {
        is_primed = false;
        emit( clicked() );
    }

    event->accept();
}

void Button::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);

    if (txt)
    {
        QSizeF button_size = this->size();

        txt->setTextWidth( 0.8 * button_size.width() );
        txt->prepare( QTransform(), QFont("Helvetica [Cronyx]", 14) );
        
        QSizeF text_size = txt->size();
        
        offset_x = 0.5 * (button_size.width() - text_size.width());
        offset_y = 0.5 * (button_size.height() - text_size.height());
        
        // COULD ADD CODE HERE TO AUTOMATICALLY SCALE DOWN THE FONT
        // IF THE BUTTON TEXT IS TOO BIG
        
        this->setMinimumSize( 1.2 * text_size );
    }
}

/** Paint the button */
void Button::paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem*, QWidget*)
{
    float w = this->size().width();
    float h = this->size().height();

    painter->fillRect(0, 0, w, h, QColor::fromRgbF(0.8, 0.8, 0.8, 0.5) );
    painter->drawRoundedRect(0, 0, size().width(), size().height(), 5, 5);

    if (txt)
    {
        QSizeF sz = txt->size();
        painter->drawStaticText( offset_x, offset_y, *txt );
    }
}
