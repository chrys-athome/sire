#ifndef CONSPIRE_BUTTON_H
#define CONSPIRE_BUTTON_H
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

class QStaticText;

namespace Conspire
{
    /** This is a simple button */
    class CONSPIRE_EXPORT Button : public QGraphicsWidget
    {
        Q_OBJECT
        
    public:
        Button(QGraphicsItem *parent=0);
        Button(QString text, QGraphicsItem *parent=0);
        
        ~Button();
        
        void setText(QString text);

        QString text() const;
        
    signals:
        void clicked();
        
    protected:
        void paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem *option, 
                   QWidget *widget);
    
        void focusInEvent(QFocusEvent *event);
        void focusOutEvent(QFocusEvent *event);    

        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

        void resizeEvent(QGraphicsSceneResizeEvent *event);

    private:
        void build();
    
        /** The text written onto the button */
        QStaticText *txt;

        /** Offset for the text */
        float offset_x, offset_y;

        /** Whether or not the button has focus */
        bool has_focus;
        
        /** Whether or not the button is primed */
        bool is_primed;
        
        /** Whether or not the button is checked */
        bool is_checked;
    };
}

CONSPIRE_END_HEADER

#endif
