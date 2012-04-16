#ifndef CONSPIRE_CONFIGVIEW_H
#define CONSPIRE_CONFIGVIEW_H
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

#include "Conspire/GUI/pageview.h"

#include "Conspire/option.h"

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This class holds a view of a set of ConfigPage pages. This
        is an extension of PageView that adds additional signals
        if the Options object is edited, and provides slots that
        allow all of the pages to be updated if the Options are
        edited */
    class CONSPIRE_EXPORT ConfigView : public PageView
    {
        Q_OBJECT
        
    public:
        ConfigView(QGraphicsItem *parent=0);
        ConfigView(PagePointer top_page, QGraphicsItem *parent=0);
        
        ~ConfigView();
        
    public slots:
        void push(PagePointer page, bool new_tab=false);
        void update(Options options);

    signals:
        void updateOptions(Options);
        
        void add(QString full_key);
        void remove(QString full_key);
        void update(QString full_key, Obj value);
    };

}

CONSPIRE_END_HEADER

#endif
