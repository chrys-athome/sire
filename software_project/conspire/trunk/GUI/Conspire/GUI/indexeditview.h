#ifndef CONSPIRE_INDEXEDITVIEW_H
#define CONSPIRE_INDEXEDITVIEW_H
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
#include "Conspire/GUI/editview.h"

CONSPIRE_BEGIN_HEADER

class QGraphicsGridLayout;
class QLabel;
class QAbstractButton;

namespace Conspire
{
    class Editer;

    /** This class is used to view and edit an individual
        indexed value of an Option */
    class CONSPIRE_EXPORT IndexEditView : public EditView
    {
        Q_OBJECT
        
    public:
        IndexEditView(EditView *parent=0);
        IndexEditView(Option option, EditView *parent=0);
        
        ~IndexEditView();
        
        QString key() const;
        QString rootKey() const;
        
        int index() const;
        
        Option option() const;
        
        void setOption(Option option);
        
    private:
        void build();
        
        /** This option being edited */
        Option opt;
        
        QGraphicsGridLayout *grid_layout;
        QLabel *label;
        Editer *editer;
        QAbstractButton *del_button;
    };

}

CONSPIRE_END_HEADER

#endif
