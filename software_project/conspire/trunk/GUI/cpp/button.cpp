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

static QFont starting_font("LucidaGrande", 10);

void Button::scaleTextToFit()
{
    if (txt)
    {
        QSizeF sz = 0.9 * this->size();
    
        txt->setTextWidth(sz.width());
        txt->prepare(QTransform(), starting_font);
    
        float w_factor = sz.width() / txt->size().width();
        float h_factor = sz.height() / txt->size().height();
    
        float factor = 1.5 * qMin(w_factor, h_factor);

        button_font = starting_font;

        if (factor != 1)
        {
            QFont scaled_font = button_font;
            scaled_font.setPointSizeF(button_font.pointSizeF() * factor);
            txt->prepare(QTransform(), scaled_font);
            button_font = scaled_font;
        }

        QSizeF text_size = txt->size();
        
        offset_x = 0.5 * (this->size().width() - text_size.width());
        offset_y = 0.5 * (this->size().height() - text_size.height());
    }
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

        QTextOption opt;
        opt.setAlignment( ::Qt::AlignCenter );
        opt.setWrapMode( QTextOption::WordWrap );        
        txt->setTextOption(opt);

        scaleTextToFit();
        
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
    scaleTextToFit();
}

bool Button::hasFocus() const
{
    return has_focus;
}

bool Button::isPrimed() const
{
    return is_primed;
}

bool Button::isChecked() const
{
    return is_checked;
}

/** Paint the button */
void Button::paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem*, QWidget*)
{
    QSizeF text_size;
    
    if (txt)
        text_size = txt->size();
    
    QSizeF button_size = this->size();
    QRectF button_rect = this->geometry();

    float m_roundness = 5;

    QColor button_color = QColor(225, 225, 225);
    QColor m_highlight = QColor(255, 255, 255);
    QColor m_shadow = QColor(200, 200, 200);
    
    float ox = 0;
    float oy = 0;
    float offset_x = 0.5 * (button_size.width() - text_size.width());
    float offset_y = 0.5 * (button_size.height() - text_size.height());

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);

    painter->setPen(QPen(QBrush(::Qt::black), 2.0));
    
    QPainterPath outline;
    outline.addRoundedRect(0, 0, button_rect.width(), button_rect.height(), 
                           m_roundness, m_roundness);
    painter->setOpacity(0.5);
    painter->drawPath(outline);

    QBrush brush( ::Qt::SolidPattern );
    brush.setColor( QColor(150,150,150) );
    painter->setBrush(brush); 
    painter->setPen(QPen(QColor(50,50,50), 2.0));
 
    //main button
    QPainterPath painter_path;
    painter_path.addRoundedRect(1, 1, button_rect.width() - 2, 
                                button_rect.height() - 2, m_roundness, m_roundness);
    painter->setClipPath(painter_path);

    painter->setOpacity(0.6);
    painter->drawRoundedRect(1, 1, button_rect.width() - 2, 
                             button_rect.height() - 2, m_roundness, m_roundness);
 
    //glass highlight
    painter->setBrush(QBrush(::Qt::white));
    painter->setPen(QPen(QBrush(::Qt::white), 0.01));
    painter->setOpacity(0.4);
    painter->drawRect(1, 1, button_rect.width() - 2, (button_rect.height() / 2) - 2);
     
    painter->setOpacity(0.9);
    painter->setPen(QPen(QBrush(::Qt::black), 0.01));
     
    if (txt)
    {
        float dx = offset_x;
        float dy = offset_y + 0.5 * (text_size.height() - txt->size().height());

        painter->setFont(button_font);
        painter->drawStaticText( ox+dx, ox+dy, *txt );
    }
}
