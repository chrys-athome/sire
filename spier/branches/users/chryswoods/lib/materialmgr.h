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

#ifndef SPIER_MATERIALMGR_H
#define SPIER_MATERIALMGR_H

#include <QHash>

#include "materialptr.h"

#include "SireMol/atom.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMol::Element;

class Material;
class GLMaterial;
class GLTexture;
class GLShader;

/**
This maintains a library of materials.
 
@author Christopher Woods
*/
class SPIER_EXPORT MaterialMgr
{
public:
    MaterialMgr();
    ~MaterialMgr();

    MaterialPtr getMaterial(const char *name);
    MaterialPtr getMaterial(const QString &name);
    MaterialPtr getMaterial(const Element &elmnt);

private:

    /** Hash that stores different materials indexed name */
    QHash<QString, MaterialPtr> mats;
};

}

SIRE_END_HEADER

#endif
