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

#include "Conspire/GUI/editer.h"
#include "Conspire/GUI/stringediter.h"

using namespace Conspire;

/** Constructor */
Editer::Editer(QGraphicsItem *parent) : QGraphicsWidget(parent)
{}

/** Construct the object, with the edited object in 'object' */
Editer::Editer(Obj object, QGraphicsItem *parent) : QGraphicsWidget(parent), obj(object)
{}

/** Destructor */
Editer::~Editer()
{}

/** Return the object being edited */
Obj Editer::object() const
{
    return obj;
}

/** Set the object to be edited */
void Editer::setObject(Obj object)
{
    obj = object;
}

/** Internal function called to set the new value of the edited object */
void Editer::update(Obj new_obj)
{
    if (new_obj == obj)
        return;

    Obj old_obj = obj;
    
    try
    {
        obj = new_obj;
        emit edited(obj);
    }
    catch(...)
    {
        obj = old_obj;
        throw;
    }
}

/** Create an appropriate editer for the passed object */
Editer* Editer::create(Obj object, QGraphicsItem *parent)
{
    return new StringEditer(object, parent);
}
