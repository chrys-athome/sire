/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include "viewobj.h"
#include "glcanvas.h"

using namespace Spier;

ViewObj::ViewObj() : cnvs(0)
{}

ViewObj::~ViewObj()
{}

/** Set the canvas that on which this object will be drawn */
void ViewObj::setCanvas(GLCanvas *canvas)
{
    if (cnvs == canvas)
        return;

    if (cnvs)
        //remove this object from the old canvas
        cnvs->removeFromDisplay(self());
    
    cnvs = canvas;
    
    if (cnvs)
    {
        cnvs->addToDisplay(self());
        
        //(re)build this representation for the new canvas
        this->buildRep();
    }
}

/** Orphan this object (this sets the canvas to 0) */
void ViewObj::orphan()
{
    if (cnvs)
        cnvs->removeFromDisplay(self());
        
    cnvs = 0;
}

/** Return the shared_ptr pointer to this object. Note that this object must
    have been created via the 'new' (i.e. on the heap) or else undefined
    things will happen! */
ViewObjPtr ViewObj::self() const
{
    ViewObjPtr ptr = selfptr.lock();
    
    if (ptr.get() == 0)
    {
        ViewObj *nonconst_this = const_cast<ViewObj*>(this);
        ptr.reset(nonconst_this);
        nonconst_this->selfptr = ptr;
    }
    
    return ptr;
}
