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

#include "Conspire/GUI/optionbutton.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QLinearGradient>
#include <QColor>
#include <QPixmap>

#include <QStaticText>
#include <QTextOption>

#include <QGraphicsSceneMouseEvent>

using namespace Conspire;

/** Construct an empty OptionButton */
OptionButton::OptionButton(QGraphicsItem *parent) : Button(parent)
{
    build();
}

/** Construct a button representing an option called "key", which
    has sub-option values (so just displays an arrow, pointing
    to the sub-values) */
OptionButton::OptionButton(QString key, QGraphicsItem *parent) 
             : Button(parent)
{
    build();
    setText(key);
}

/** Construct a button representing an option called "key", with 
    the text representation of the value being "value" */
OptionButton::OptionButton(QString key, QString value, QGraphicsItem *parent)
             : Button(parent)
{
    build();
    setValue(value);
    setText(key);
} 

/** Destructor */
OptionButton::~OptionButton()
{
    delete txt;
    delete val;
}

/** Build the button */
void OptionButton::build()
{
    txt = 0;
    val = 0;
}

/** Return the text (if any) in the button */
QString OptionButton::text() const
{
    if (txt)
        return txt->text();
    else
        return QString::null;
}

/** Set the text to be rendered */
void OptionButton::setText(QString t)
{
    if (t == this->text())
        return;

    if (txt)
        delete txt;
    
    if (not t.isEmpty())
    {
        QSizeF button_size = this->size();

        QTextOption opt;
        
        if (val)
            opt.setAlignment( ::Qt::AlignRight );
        else
            opt.setAlignment( ::Qt::AlignCenter );
            
        opt.setWrapMode( QTextOption::WordWrap );        

        txt = new QStaticText(t);
        txt->setTextOption(opt);
        
        txt->setTextWidth( 0.4 * button_size.width() );
        txt->prepare( QTransform(), QFont("Helvetica [Cronyx]", 12) );
        
        update(this->geometry());
    }
}

/** Set the textual representation of the value of the option */
void OptionButton::setValue(QString v)
{
    if (v == this->value())
        return;

    if (val)
        delete val;
    
    if (not v.isEmpty())
    {
        QSizeF button_size = this->size();

        QTextOption opt;
        opt.setAlignment( ::Qt::AlignLeft );
        opt.setWrapMode( QTextOption::WordWrap );        

        val = new QStaticText(v);
        val->setTextOption(opt);
        
        val->setTextWidth( 0.4 * button_size.width() );
        val->prepare( QTransform(), QFont("Helvetica [Cronyx]", 12) );
        
        update(this->geometry());
    }
    
    QString t = this->text();
    delete txt;
    txt = 0;
    
    this->setText(t);
}

/** Return the text representation of the value of the option */
QString OptionButton::value() const
{
    if (val)
        return val->text();
    else
        return QString::null;
}

QSizeF OptionButton::getTextSize() const
{
    if (txt and val)
    {
        QSizeF button_size = this->size();
    
        return QSizeF( txt->size().width() + 0.1 * button_size.width() +
                       val->size().width(),
                       qMax(txt->size().height(), val->size().height()) );
    }
    else if (txt)
    {
        return QSizeF( txt->size() );
    }
    else
    {
        return QSizeF(0,0);
    }
}

void OptionButton::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);

    QSizeF button_size = this->size();

    if (txt)
    {

        txt->setTextWidth( 0.4 * button_size.width() );
        txt->prepare( QTransform(), QFont("Helvetica [Cronyx]", 12) );
    }
    
    if (val)
    {
        val->setTextWidth( 0.4 * button_size.width() );
        val->prepare( QTransform(), QFont("Helvetica [Cronyx]", 12) );
    }
        
    //this->setMinimumSize( 1.1 * getTextSize() );
}

void OptionButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
                         QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    
    QSizeF text_size = getTextSize();
    QSizeF button_size = this->size();
    QRectF button_rect = this->geometry();

    float m_roundness = 5;

    QColor button_color = QColor(220, 220, 220);
    QColor m_highlight = QColor(255, 255, 255);
    QColor m_shadow = QColor(180, 180, 180);
    
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

    QLinearGradient gradient;
    gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, button_color);
    gradient.setColorAt(0.3, m_shadow);
    gradient.setColorAt(0.7, m_shadow);
    gradient.setColorAt(1.0, button_color);
 
    QBrush brush(gradient);
    painter->setBrush(brush); 
    painter->setPen(QPen(QBrush(button_color), 2.0));
 
    //main button
    QPainterPath painter_path;
    painter_path.addRoundedRect(1, 1, button_rect.width() - 2, 
                                button_rect.height() - 2, m_roundness, m_roundness);
    painter->setClipPath(painter_path);

    if (val)
    {
        painter->setOpacity(0.7);

        painter->fillRect(0, 0,
                          0.5*button_rect.width(), button_rect.height(),
                          QColor(200, 240, 255));
        
        painter->fillRect(0.5*button_rect.width(), 0,
                          0.5*button_rect.width(), button_rect.height(),
                          QColor(200, 200, 255));
    }
 
    painter->setOpacity(0.4);
    painter->drawRoundedRect(1, 1, button_rect.width() - 2, 
                             button_rect.height() - 2, m_roundness, m_roundness);
 
    //glass highlight
    painter->setBrush(QBrush(::Qt::white));
    painter->setPen(QPen(QBrush(::Qt::white), 0.01));
    painter->setOpacity(0.2);
    painter->drawRect(1, 1, button_rect.width() - 2, (button_rect.height() / 2) - 2);
     
    painter->setOpacity(0.8);
    painter->setPen(QPen(QBrush(::Qt::black), 0.01));
    
    if (txt)
    {
        float dx = offset_x;
        float dy = offset_y + 0.5 * (text_size.height() - txt->size().height());

        painter->drawStaticText( ox+dx, ox+dy, *txt );
    }
    
    if (val)
    {
        float dx = button_size.width() - offset_x - val->size().width();
        float dy = offset_y + 0.5 * (text_size.height() - val->size().height());
        
        painter->drawStaticText( ox+dx, ox+dy, *val );
    }
}
