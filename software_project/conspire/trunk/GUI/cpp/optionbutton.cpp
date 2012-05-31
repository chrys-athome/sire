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
#include <QFontMetrics>

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
    
    txt = 0;
    val = 0;
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

static QFont starting_font("LucidaGrande", 14);

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

void OptionButton::scaleTextToFit()
{
    QSizeF sz = this->size();
    
    int max_height = 0.8 * sz.height();
    int max_width = 0;
    
    if (txt and val)
        max_width = 0.4 * sz.width();
    else if (txt or val)
        max_width = 0.8 * sz.width();
    
    if (txt)
    {
        if (val)
            txt->setTextWidth(0.4 * sz.width());
        else
            txt->setTextWidth(sz.width());
        
        txt->prepare(QTransform(), starting_font);
    }
    
    if (val)
    {
        if (txt)
            val->setTextWidth(0.4 * sz.width());
        else
            val->setTextWidth(sz.width());
            
        val->prepare(QTransform(), starting_font);
    }

    QFontMetrics metrics(starting_font);
    
    int text_width = 0;
    int text_height = 0;
    
    if (txt)
    {
        QRect txt_sz = metrics.boundingRect(0,0,max_width,max_height,
                                            ::Qt::AlignRight|::Qt::AlignVCenter,
                                            txt->text());
                                            
        text_width += txt_sz.width();
        text_height = qMax(text_height, txt_sz.height());
    }
    
    if (val)
    {
        QRect txt_sz = metrics.boundingRect(0,0,max_width,max_height,
                                            ::Qt::AlignRight|::Qt::AlignVCenter,
                                            val->text());
                                            
        text_width += txt_sz.width();
        text_height = qMax(text_height, txt_sz.height());
    }
    
    QSizeF text_size = QSizeF(text_width + 0.1*sz.width(), text_height); //this->getTextSize();
    
    if (text_size.width() <= 0 or text_size.height() <= 0)
        return;
    
    float w_factor = sz.width() / text_size.width();
    float h_factor = sz.height() / text_size.height();
    
    float factor = qMin(w_factor, h_factor);

    conspireDebug() << "Factor =" << factor << w_factor << h_factor;

    button_font = starting_font;

    int ntries = 1;

    while (factor > 1.1 or factor < 0.9)
    {
        QFont scaled_font = button_font;
        
        conspireDebug() << button_font.pointSizeF()
                        << factor << int(button_font.pointSizeF() * factor);
        
        scaled_font.setPointSize( int(button_font.pointSizeF() * factor) );

        if (txt)
            txt->prepare(QTransform(), scaled_font);
        
        if (val)
            val->prepare(QTransform(), scaled_font);
            
        button_font = scaled_font;
        
        text_size = this->getTextSize();
        
        w_factor = 0.8*sz.width() / text_size.width();
        h_factor = 0.8*sz.height() / text_size.height();
        
        factor = qMin(w_factor, h_factor);
        
        ++ntries;
        
        if (ntries > 1)
            break;
    }
    
    conspireDebug() << "Scale text" << button_font.pointSizeF()
                    << w_factor << h_factor << text_size << sz;
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

        this->scaleTextToFit();

        update(this->geometry());
    }
}

/** Set the textual representation of the value of the option */
void OptionButton::setValue(QString v)
{
    if (v == this->value())
        return;

    if (val)
    {
        delete val;
        val = 0;
    }
    
    if (not v.isEmpty())
    {
        QSizeF button_size = this->size();

        QTextOption opt;
        opt.setAlignment( ::Qt::AlignLeft );
        opt.setWrapMode( QTextOption::WordWrap );        

        val = new QStaticText(v);
        val->setTextOption(opt);
        
        this->scaleTextToFit();
        
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

void OptionButton::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);

    scaleTextToFit();
        
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
    /*painter->setBrush(QBrush(::Qt::white));
    painter->setPen(QPen(QBrush(::Qt::white), 0.01));
    painter->setOpacity(0.2);
    painter->drawRect(1, 1, button_rect.width() - 2, (button_rect.height() / 2) - 2);
     */
     
    painter->setOpacity(0.8);
    painter->setPen(QPen(QBrush(::Qt::black), 0.01));
    painter->setFont(button_font);
    
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
