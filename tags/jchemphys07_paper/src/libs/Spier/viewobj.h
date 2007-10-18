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

#ifndef SPIER_VIEWOBJ_H
#define SPIER_VIEWOBJ_H

#include "materialptr.h"
#include "selectid.h"
#include "viewobjptr.h"

#include "SireMaths/sphere.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Sphere;

class GLCanvas;

/**
This is the base class of all viewable objects.
 
@author Christopher Woods
*/
class SPIER_EXPORT ViewObj
{

friend class GLCanvas; // so can call drawing and reparenting functions

public:
    ViewObj();
    ViewObj(const ViewObj &other);
    virtual ~ViewObj();

    ViewObjPtr self() const;

    virtual void setCanvas(GLCanvas *canvas);
    virtual void orphan();
    
    const GLCanvas* canvas() const;

    bool isOrphan() const;

    const Sphere& boundingSphere() const;

protected:

    GLCanvas* canvas();

    virtual void draw()=0;
    virtual void selectDraw()=0;
    virtual void buildRep()=0;
    virtual SelectID select(int x, int y)=0;

    Sphere boundsphere;

private:
    
//     virtual void orphan();
//     virtual void reparent(GLCanvas *newparent);
//     
//     virtual ViewObjPtr duplicate(GLCanvas *newparent) const=0;

    ViewObjWeakPtr selfptr;
    
    GLCanvas *cnvs;
};

/** Return the canvas on which this object is currently being drawn */
inline const GLCanvas* ViewObj::canvas() const
{
    return cnvs;
}

/** Return a nonconst pointer to the GLCanvas on which this object is currently
    being drawn - this is only available to derived classes of ViewObj */
inline GLCanvas* ViewObj::canvas()
{
    return cnvs;
}

/** Return whether or not this is an orphaned ViewObj (has no GLCanvas parent) */
inline bool ViewObj::isOrphan() const
{
    return cnvs == 0;
}

/** Return the bounding sphere for this representation */
inline const Sphere& ViewObj::boundingSphere() const
{
    return boundsphere;
}

}

SIRE_END_HEADER

#endif
