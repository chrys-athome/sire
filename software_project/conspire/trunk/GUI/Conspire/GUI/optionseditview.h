#ifndef CONSPIRE_OPTIONSEDITVIEW_H
#define CONSPIRE_OPTIONSEDITVIEW_H
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

class QAbstractButton;
class QGraphicsGridLayout;

namespace Conspire
{
    class OptionEditView;

    /** This is a QGraphicsWidget that can be used to view
        and edit the Options in the passed Option object */
    class CONSPIRE_EXPORT OptionsEditView : public EditView
    {
        Q_OBJECT
    
    public:
        OptionsEditView(EditView *parent=0);
        OptionsEditView(Options options, EditView *parent=0);
        
        ~OptionsEditView();
        
        Options options() const;
        
        void setOptions(Options options);

    signals:
        void setCanAddOptions(bool);
        void hasParentOptions(bool);
        
    private:
        void build();
    
        /** The options being viewed and edited */
        Options opts;
        
        /** The set of OptionEditView objects for the set options */
        QHash<QString,OptionEditView*> *option_views;
        
        /** The layout used to arrange items in this widget */
        QGraphicsGridLayout *grid_layout;
    };

}

CONSPIRE_END_HEADER

#endif
