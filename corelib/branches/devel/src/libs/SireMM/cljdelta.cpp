/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "cljdelta.h"

#include "SireMol/moleculeview.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<CLJDelta> r_delta(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJDelta &delta)
{
    writeHeader(ds, r_delta, 1);
    
    SharedDataStream sds(ds);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJDelta &delta)
{
    VersionID v = readHeader(ds, r_delta);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
    }
    else
        throw version_error(v, "1", r_delta, CODELOC);
    
    return ds;
}

/** Construct from the passed data, but re-using the memory already allocated
    for this delta */
void CLJDelta::buildFrom(const CLJBoxes &boxes, const QVector<CLJBoxIndex> &indicies,
                         const MoleculeView &new_molecule, CLJAtoms::ID_SOURCE source,
                         const PropertyMap &map)
{
    CLJBoxIndex min_box;
    CLJBoxIndex max_box;

    //how many changed atoms are there?
    const int nold = indicies.count();
    
    //construct the new molecule
    new_atoms.reconstruct(new_molecule, source, map);

    const int nnew = new_atoms.nAtoms();

    changed_atoms.resize(nold + nnew);
    changed_atoms.copyIn(new_atoms);

    if (not indicies.isEmpty())
    {
        //find the range of boxes occupied by the atoms and copy out the data
        const CLJBoxIndex *indicies_array = indicies.constData();

        for (int i=0; i<indicies.count(); ++i)
        {
            const CLJBoxIndex &index = indicies_array[i];
        
            if (min_box.isNull())
            {
                min_box = index;
                max_box = min_box;
            }
            else
            {
                min_box = min_box.min(index);
                max_box = max_box.max(index);
            }
        }
    }

    old_indicies = indicies;
}

/** Null constructor */
CLJDelta::CLJDelta() : nbox_x(0), nbox_y(0), nbox_z(0), is_single_box(0)
{}

/** Construct the delta that changes from the atoms at indicies 'old_atoms' in the 
    CLJBoxes 'boxes' to the atoms in view 'new_atoms', using the optionally supplied
    property map to extract the necessary properties */
CLJDelta::CLJDelta(const CLJBoxes &boxes, const QVector<CLJBoxIndex> &old_atoms,
                   const MoleculeView &new_atoms, const PropertyMap &map)
         : nbox_x(0), nbox_y(0), nbox_z(0), is_single_box(0)
{
    this->buildFrom(boxes, old_atoms, new_atoms, CLJAtoms::USE_MOLNUM, map);
}

/** Construct the delta that changes from the atoms at indicies 'old_atoms' in the 
    CLJBoxes 'boxes' to the atoms in view 'new_atoms', using the optionally supplied
    property map to extract the necessary properties */
CLJDelta::CLJDelta(const CLJBoxes &boxes, const QVector<CLJBoxIndex> &old_atoms,
                   const MoleculeView &new_atoms, CLJAtoms::ID_SOURCE source,
                   const PropertyMap &map)
         : nbox_x(0), nbox_y(0), nbox_z(0), is_single_box(0)
{
    this->buildFrom(boxes, old_atoms, new_atoms, source, map);
}

/** Copy constructor */
CLJDelta::CLJDelta(const CLJDelta &other)
         : new_atoms(other.new_atoms), changed_atoms(other.changed_atoms),
           box_index(other.box_index), old_indicies(other.old_indicies),
           nbox_x(other.nbox_x), nbox_y(other.nbox_y), nbox_z(other.nbox_z),
           is_single_box(other.is_single_box)
{}

/** Destructor */
CLJDelta::~CLJDelta()
{}

/** Copy assignment operator */
CLJDelta& CLJDelta::operator=(const CLJDelta &other)
{
    if (this != &other)
    {
        new_atoms = other.new_atoms;
        changed_atoms = other.changed_atoms;
        box_index = other.box_index;
        old_indicies = other.old_indicies;
        nbox_x = other.nbox_x;
        nbox_y = other.nbox_y;
        nbox_z = other.nbox_z;
        is_single_box = other.is_single_box;
    }
    
    return *this;
}

/** Comparison operator */
bool CLJDelta::operator==(const CLJDelta &other) const
{
    return this == &other or
           (new_atoms == other.new_atoms and changed_atoms == other.changed_atoms and
            box_index == other.box_index and old_indicies == other.old_indicies and
            nbox_x == other.nbox_x and nbox_y == other.nbox_y and nbox_z == other.nbox_z and
            is_single_box == other.is_single_box);
}

/** Comparison operator */
bool CLJDelta::operator!=(const CLJDelta &other) const
{
    return not operator==(other);
}

const char* CLJDelta::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJDelta>() );
}

const char* CLJDelta::what() const
{
    return CLJDelta::typeName();
}

QString CLJDelta::toString() const
{
    return QObject::tr("CLJDelta( nChanged() = %1, nBoxes() = %2 )")
                .arg(changedAtoms().count()).arg(nBoxes());
}

/** Return the total number of boxes covered by these atoms */
int CLJDelta::nBoxes() const
{
    return int(nbox_x) * int(nbox_y) * int(nbox_z);
}

/** Return the old version of the changed atoms */
CLJAtoms CLJDelta::oldAtoms() const
{
    //the old atoms are the ones after the new atoms in 'changed_atoms'
    if (changed_atoms.isEmpty())
        return QVector<CLJAtom>();
    
    QVector<CLJAtom> old_atoms( old_indicies.count() );
    
    if (changed_atoms.nAtoms() - new_atoms.count() != old_indicies.count())
        throw SireError::program_bug( QObject::tr(
                "Something wrong? %1 - %2 != %3")
                    .arg(changed_atoms.nAtoms())
                    .arg(new_atoms.count())
                    .arg(old_indicies.count()), CODELOC );
    
    int idx = 0;
    
    for (int i=new_atoms.count(); i<changed_atoms.nAtoms(); ++i)
    {
        old_atoms[idx] = changed_atoms[i].negate();
    }

    return CLJAtoms(old_atoms);
}
