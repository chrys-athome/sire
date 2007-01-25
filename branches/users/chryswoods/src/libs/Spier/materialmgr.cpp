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

#include "materialmgr.h"
#include "material.h"

using namespace Spier;
using namespace SireMol;

MaterialMgr::MaterialMgr() 
{}

MaterialMgr::~MaterialMgr()
{}

MaterialPtr MaterialMgr::getMaterial(const char *name)
{
    return this->getMaterial(QString(name));
}

MaterialPtr MaterialMgr::getMaterial(const QString &name)
{
    if (mats.contains(name))
        return mats[name];
    else
    {
        //create a default material (orange)
        MaterialPtr mat( new Material() );
        
        mat->setMaterial( GLMaterial( Color(255, 162, 13) ) );
        
        mats.insert(name, mat);
        
        return mat;
    }
}

MaterialPtr MaterialMgr::getMaterial(const Element &elmnt)
{
    if (mats.contains(elmnt.name()))
        return mats[elmnt.name()];
    else
    {
        //create a new material that is based on the 
        //color specified by the element
        MaterialPtr mat( new Material() );
        
        mat->setMaterial( GLMaterial(Vector(elmnt.red(), elmnt.green(), elmnt.blue())) );
        
        mats.insert(elmnt.name(), mat);
        
        return mat;
    }
}
