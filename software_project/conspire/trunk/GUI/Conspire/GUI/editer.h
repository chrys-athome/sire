#ifndef CONSPIRE_EDITER_H
#define CONSPIRE_EDITER_H
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

namespace Conspire
{
    /** This is the base class of all editers. These are widgets
        that can display and edit the values of Conspire::Value 
        objects */
    class CONSPIRE_EXPORT Editer : public QGraphicsWidget
    {
        Q_OBJECT
        
    public:
        Editer(QGraphicsItem *parent=0);
        Editer(Obj object, QGraphicsItem *parent=0);
        
        virtual ~Editer();
        
        Obj object() const;
        
        static Editer* create(Obj object, QGraphicsItem *parent=0);
        
        virtual void setObject(Obj object);
        
    signals:
        void edited(Obj obj);
        
    protected:
        void update(Obj new_obj);
        
    private:
        Obj obj;
    };
}

CONSPIRE_END_HEADER

#endif
