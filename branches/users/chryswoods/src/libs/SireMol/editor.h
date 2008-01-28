/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMOL_EDITOR_H
#define SIREMOL_EDITOR_H

#include "moleculeview.h"
#include "atomselection.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class provides the template independent part of Editor<T>.
    This class is not designed to be used on its own!
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT EditorBase
{
public:
    EditorBase();
    
    EditorBase(const EditorBase &other);

    ~EditorBase();
    
protected:
    void removeProperty(MoleculeData &moldata, const QString &key);
    void removeMetadata(MoleculeData &moldata, const QString &metakey);
    void removeMetadata(MoleculeData &moldata,
                        const QString &key, const QString &metakey);
};

}

SIRE_END_HEADER

#endif
