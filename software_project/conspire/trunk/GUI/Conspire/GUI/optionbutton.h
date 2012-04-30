#ifndef CONSPIRE_OPTIONBUTTON_H
#define CONSPIRE_OPTIONBUTTON_H
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

CONSPIRE_BEGIN_HEADER

class QSizeF;

namespace Conspire
{
    /** This is a specialised button that is used to display
        the name and value of an Option. It is used by the OptionsPage
        widget to provide the buttons that access each Option
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT OptionButton : public Button
    {
        Q_OBJECT
    
    public:
        OptionButton(QGraphicsItem *parent=0);
        OptionButton(QString key, QGraphicsItem *parent=0);
        OptionButton(QString key, QString value, QGraphicsItem *parent=0);
        
        ~OptionButton();
        
        void setText(QString key);
        void setValue(QString value);
        
        QString text() const;
        QString value() const;
        
        void paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem *option, 
                   QWidget *widget);

    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *event);

    private:
        void build();
        
        QSizeF getTextSize() const;
        
        /** The text representation of the key */
        QStaticText *txt;
        
        /** The text representation of the value of the option. 
            This is null if this option has sub-options */
        QStaticText *val;
    };

}

CONSPIRE_END_HEADER

#endif
