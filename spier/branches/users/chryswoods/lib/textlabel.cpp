/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include "textlabel.h"

#include <QPalette>
#include <QPixmap>
#include <QPicture>
#include <QPainter>

using namespace SireStream;
using namespace Spier;

/** Construct a new TextLabelBase */
TextLabelBase::TextLabelBase(GLWidget parent)
        : GLWidgetBase(parent), 
          labelfont("Courier", 12, QFont::Bold),
          pxmap(0), pcture(0), bordersize(5)
          
{
    txtopts.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    txtopts.setAlignment(Qt::AlignCenter);
    txtopts.setUseDesignMetrics(true);
}

/** Destructor */
TextLabelBase::~TextLabelBase()
{
    if (pxmap)
        delete pxmap;

    if (pcture)
        delete pcture;
}

/** Set the text of this label */
void TextLabelBase::setText(const QString &text)
{
    txt = text;
    
    //recalulate the preferred size...
    QFontMetricsF metrics(labelfont);
    
    if (maxsize.isValid())
    {
        labelrect.setTopLeft(QPointF(bordersize,bordersize));
        labelrect.setWidth(maxsize.width() - 2*bordersize);
        labelrect.setHeight(maxsize.height() - 2*bordersize);
        
        labelrect = metrics.boundingRect(labelrect, txtopts.flags(), txt);
    }
    else
    {
        labelrect = metrics.boundingRect(QRectF(0,0,1024,758), txtopts.flags(), txt);
    }
    
    //the preferred size is boundrect size plus two times border
    preferredsize.setWidth( labelrect.width() + 2*bordersize );
    preferredsize.setHeight( labelrect.height() + 2*bordersize );
    
    //translate the label rectangle by the border
    labelrect.setRect(bordersize, bordersize, labelrect.width(), labelrect.height());
    
    //if size policy is preferred, then resize to the preferred size
    if (this->sizePolicy().horizontalPolicy() == QSizePolicy::Preferred and 
        this->sizePolicy().verticalPolicy() == QSizePolicy::Preferred)
    {
        this->setSize(preferredsize);
    }
}

/** Clear the images from the label */
void TextLabelBase::clearImage()
{
    if (pxmap)
    {
        delete pxmap;
        pxmap = 0;
    }

    if (pcture)
    {
        delete pcture;
        pcture = 0;
    }
}

/** Set the picture to draw on the label (this will be below the text) */
void TextLabelBase::setPicture(const QPicture &picture)
{
    clearImage();

    pcture = new QPicture(picture);
    
    //recalulate the preferred size...
    
    //if size policy is preferred, then resize to the preferred size
}

/** Set the pixmap to draw on the label (this will below the text) */
void TextLabelBase::setPixmap(const QPixmap &pixmap)
{
    clearImage();

    pxmap = new QPixmap(pixmap);
    
    //recalulate the preferred size...
    
    //if size policy is preferred, then resize to the preferred size
}

/** Set the alignment of the text */
void TextLabelBase::setAlignment(Qt::Alignment align)
{
    txtopts.setAlignment(align);
}

/** Return the alignment of the text */
Qt::Alignment TextLabelBase::alignment() const
{
    return txtopts.alignment();
}

/** Set word wrapping to 'on' */
void TextLabelBase::setWordWrap(bool on)
{
    if (on)
        txtopts.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    else
        txtopts.setWrapMode(QTextOption::NoWrap);
}

/** Return whether or not word wrapping is enabled */
bool TextLabelBase::wordWrap() const
{
    return txtopts.wrapMode() & QTextOption::WrapAtWordBoundaryOrAnywhere;
}

/** Return the text in this label (a null string is returned if 
    there is no text in this label) */
QString TextLabelBase::text() const
{
    return txt;
}

/** Return the pixmap displayed in this label (a null pixmap is 
    returned if there is no pixmap in this label) */
QPixmap TextLabelBase::pixmap() const
{
    if (pxmap)
        return QPixmap(*pxmap);
    else
        return QPixmap();
}

/** Return the picture displayed in this label (a null picture is 
    returned if there is no picture in this label) */
QPicture TextLabelBase::picture() const
{
    if (pcture)
        return QPicture(*pcture);
    else
        return QPicture();
}

/** Actually paint the widget. The point (0,0) is at the top left 
    of this widget, with the bottom right corner at (width, height) */
void TextLabelBase::paintWidget(QPainter &painter)
{
    QBrush b(QColor(100,100,100,100));

    painter.setBrush(b);
    painter.setPen(QColor(0,0,0,100));
    painter.drawPath(clippath);

    //now draw the text
    if (not txt.isNull())
    {
        painter.setPen(QColor(255,255,255,200));
        painter.setFont(labelfont);
        painter.drawText(labelrect, txt, txtopts);
    }
}
