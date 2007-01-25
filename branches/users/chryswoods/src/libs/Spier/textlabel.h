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

#ifndef SPIER_TEXTLABEL_H
#define SPIER_TEXTLABEL_H

#include "wrapper.hpp"
#include "glwidgetbase.h"

#include <QTextOption>

class QPixmap;
class QPicture;

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This is a label GLWidget, used to display text (:-)
 
@author Christopher Woods
*/
class SPIER_EXPORT TextLabelBase : public GLWidgetBase
{

friend class Wrapper<TextLabelBase>;

Q_OBJECT

public:
    ~TextLabelBase();

    void setAlignment(Qt::Alignment align);
    Qt::Alignment alignment() const;
    
    void setWordWrap(bool on);
    bool wordWrap() const;

    QString text() const;
    QPixmap pixmap() const;
    QPicture picture() const;

public slots:

    void setText(const QString &text);
    void setPicture(const QPicture &picture);
    void setPixmap(const QPixmap &pixmap);

protected:
    TextLabelBase(GLWidget parent = GLWidget());

    void paintWidget(QPainter &painter);
    
    void clearImage();
    
    /** The text displayed in the label */
    QString txt;
    
    /** The font of the label */
    QFont labelfont;
    
    /** Text options */
    QTextOption txtopts;
    
    /** The pixmap that may be displayed in the label */
    QPixmap *pxmap;
    
    /** The picture that may be displayed in the label */
    QPicture *pcture;

    /** The border size */
    qreal bordersize;
    
    /** The rectangle that will contain the text/image */
    QRectF labelrect;
    
    /** The preferred size of the label */
    QSizeF preferredsize;
};

typedef Wrapper<TextLabelBase> TextLabel;

}

SIRE_END_HEADER

#endif
