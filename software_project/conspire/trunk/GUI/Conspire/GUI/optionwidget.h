#ifndef CONSPIRE_OPTIONWIDGET_H
#define CONSPIRE_OPTIONWIDGET_H
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

#include "Conspire/GUI/pagewidget.h"

#include "Conspire/option.h"

CONSPIRE_BEGIN_HEADER

class QLabel;
class QLineEdit;

namespace Conspire
{
    /** This page is used to change the value of a specific option */
    class CONSPIRE_EXPORT OptionWidget : public PageWidget
    {
        Q_OBJECT
    
    public:
        OptionWidget(QGraphicsItem *parent=0);
        OptionWidget(Option option, QString root_key,
                     QGraphicsItem *parent=0);
                     
        ~OptionWidget();
        
        Option option() const;
        QString rootKey() const;
        
    private:
        void build();
        void setOption(Option option, QString key);
    
        Option opt;
        QString root_key;
        
        QLabel *help_box;
        QLabel *key_box;
        QLineEdit *value_edit;
    };

}

CONSPIRE_END_HEADER

#endif
