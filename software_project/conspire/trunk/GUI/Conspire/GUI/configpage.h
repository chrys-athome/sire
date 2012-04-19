#ifndef CONSPIRE_CONFIGPAGE_H
#define CONSPIRE_CONFIGPAGE_H
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

#include "Conspire/GUI/page.h"

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This is the base class of all pages that are used 
        to view and edit Options objects */
    class CONSPIRE_EXPORT ConfigPage : public Page
    {
        Q_OBJECT
    
    public:
        ConfigPage(Page *parent=0);
        
        virtual ~ConfigPage();
    
    public slots:
        virtual void reread(Options options);
                
    signals:
        /** Signal emitted when this page requests that the key "full_key"
            is added to the GUI's Options object */
        void add(QString full_key);

        /** Signal emitted when this page requests that the key "full_key"
            is removed from the GUI's Options object */
        void remove(QString full_key);

        /** Signal emitted when this page requests that the key "full_key"
            in the GUI's Options object should be set to have the 
            value "new_value" */
        void update(QString full_key, Obj new_value);
    };

}

CONSPIRE_END_HEADER

#endif
