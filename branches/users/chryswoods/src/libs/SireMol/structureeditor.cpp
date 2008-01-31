/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "structureeditor.h"

#include "atom.h"
#include "cutgroup.h"
#include "residue.h"
#include "chain.h"
#include "segment.h"
#include "molecule.h"
#include "mover.hpp"
#include "selector.hpp"

#include "atomname.h"
#include "atomidx.h"
#include "atomnum.h"

#include "cgname.h"
#include "cgidx.h"

#include "resname.h"
#include "resnum.h"
#include "residx.h"

#include "chainname.h"
#include "chainidx.h"

#include "segname.h"
#include "segidx.h"

#include "molname.h"
#include "molnum.h"

#include "SireBase/properties.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of detail::EditMolData
/////////
namespace detail
{

/** This class holds the editable data of an Atom */
class EditAtomData
{
public:
    EditAtomData();
    EditAtomData(const EditAtomData &other);
    
    ~EditAtomData();
    
    AtomName name;
    AtomNum number;
    
    quint32 cg_parent;
    quint32 res_parent;
    quint32 seg_parent;
    
    QHash<QString,QVariant> properties;
};

/** This class holds the editable data of a CutGroup */
class EditCGData
{
public:
    EditCGData();
    EditCGData(const EditCGData &other);
    
    ~EditCGData();
    
    CGName name;
    
    QList<quint32> atoms;
};

/** This private class is used to hold the explicitly shared
    data of the StructureEditor. */
class EditMolData
{
public:
    EditMolData();
    EditMolData(const EditMolData &other);
    
    ~EditMolData();
    
    MolName molname;
    MolNum molnum;
    
    QHash<quint32, EditAtomData> atoms;
    QHash<quint32, EditCGData> cutgroups;
    
    Properties properties;
};

} // end of namespace detail

/////////
///////// Implementation of StructureEditor
/////////

static const RegisterMetaType<StructureEditor> r_structeditor(MAGIC_ONLY,
                                                  "SireMol::StructureEditor");
                                                  
/** Serialise to a binary datastream */

