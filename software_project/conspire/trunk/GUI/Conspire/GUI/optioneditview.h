#ifndef CONSPIRE_OPTIONEDITVIEW_H
#define CONSPIRE_OPTIONEDITVIEW_H
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
class QGraphicsProxyWidget;
class QAbstractButton;

namespace Conspire
{
    class IndexEditView;

    /** This class is used to view and edit an Option object */
    class CONSPIRE_EXPORT OptionEditView : public EditView
    {
        Q_OBJECT
        
    public:
        OptionEditView(EditView *parent=0);
        OptionEditView(Option option, EditView *parent=0);
        
        ~OptionEditView();
        
        QString key() const;
        
        Option option() const;
        
        void setOption(Option option);
    
    protected slots:
        void showHelp() const;
    
    private:
        void build();
    
        /** The option being edited */
        Option opt;
        
        /** The set of editers for each index */
        QHash<int,IndexEditView*> *index_views;
        
        QGraphicsGridLayout *grid_layout;
        QAbstractButton *help_button;
        QGraphicsProxyWidget *help_button_proxy;
    };
}

CONSPIRE_END_HEADER

#endif
