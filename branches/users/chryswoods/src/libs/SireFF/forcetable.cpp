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

#include "forcetable.h"

#include "SireMol/moleculeview.h"
#include "SireMol/molgroup.h"
#include "SireMol/atomselection.h"

#include "SireMol/mover.hpp"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of MolForceTable
/////////

static const RegisterMetaType<MolForceTable> r_molforcetable;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const MolForceTable &molforcetable)
{
    writeHeader(ds, r_molforcetable, 1);
    
    SharedDataStream sds(ds);
    
    sds << molforcetable.molnum << molforcetable.ncgroups
        << molforcetable.cgidx_to_idx
        << static_cast<const PackedArray2D<Vector>&>(molforcetable);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      MolForceTable &molforcetable)
{
    VersionID v = readHeader(ds, r_molforcetable);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> molforcetable.molnum >> molforcetable.ncgroups
            >> molforcetable.cgidx_to_idx
            >> static_cast<PackedArray2D<Vector>&>(molforcetable);
    }
    else
        throw version_error(v, "1", r_molforcetable, CODELOC);
        
    return ds;
}

/** Constructor */
MolForceTable::MolForceTable() : PackedArray2D<Vector>(),
                                 molnum(0), ncgroups(0)
{}

/** Construct a table to hold the forces on all of the CutGroups that
    are selected in 'molview'. The forces are initialised to zero */
MolForceTable::MolForceTable(const MoleculeView &molview)
              : PackedArray2D<Vector>(),
                molnum(molview.data().number()),
                ncgroups(molview.data().info().nCutGroups())
{
    //build arrays for each selected CutGroup
    AtomSelection selected_atoms = molview.selection();
    
    if (selected_atoms.selectedAllCutGroups())
    {
        QVector< QVector<Vector> > forces(ncgroups);
        QVector<Vector> *forces_array = forces.data();
    
        for (CGIdx i(0); i<ncgroups; ++i)
        {
            forces_array[i] = QVector<Vector>(molview.data().info().nAtoms(i), 
                                              Vector(0));
        }
        
        PackedArray2D<Vector>::operator=(forces);
    }
    else
    {
        QVector< QVector<Vector> > forces(selected_atoms.nSelectedCutGroups());
        cgidx_to_idx.reserve(selected_atoms.nSelectedCutGroups());
        
        QVector<Vector> *forces_array = forces.data();
        qint32 idx = 0;
        
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            forces_array[i] = QVector<Vector>(molview.data().info().nAtoms(i),
                                              Vector(0));
                                              
            cgidx_to_idx.insert(i, idx);
            ++idx;
        }
        
        PackedArray2D<Vector>::operator=(forces);
    }
}

/** Copy constructor */
MolForceTable::MolForceTable(const MolForceTable &other)
              : PackedArray2D<Vector>(other),
                molnum(other.molnum), ncgroups(other.ncgroups),
                cgidx_to_idx(other.cgidx_to_idx)
{}

/** Destructor */
MolForceTable::~MolForceTable()
{}

/** Copy assignment operator */
MolForceTable& MolForceTable::operator=(const MolForceTable &other)
{
    if (this != &other)
    {
        PackedArray2D<Vector>::operator=(other);
        molnum = other.molnum;
        ncgroups = other.ncgroups;
        cgidx_to_idx = other.cgidx_to_idx;
    }
    
    return *this;
}

/** Comparison operator */
bool MolForceTable::operator==(const MolForceTable &other) const
{
    return molnum == other.molnum and cgidx_to_idx == other.cgidx_to_idx
             and PackedArray2D<Vector>::operator==(other);
}

/** Comparison operator */
bool MolForceTable::operator!=(const MolForceTable &other) const
{
    return not this->operator==(other);
}

/** Initialise this table - this resets all of the forces back to zero */
void MolForceTable::initialise()
{
    int nvals = PackedArray2D<Vector>::nValues();

    if (nvals > 0)
    {
        Vector *vals = PackedArray2D<Vector>::valueData();
        
        for (int i=0; i<nvals; ++i)
        {
            vals[i] = Vector(0);
        }
    }
}

////////
//////// Implementation of ForceTable
////////

static const RegisterMetaType<ForceTable> r_forcetable;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const ForceTable &forcetable)
{
    writeHeader(ds, r_forcetable, 1);
    
    SharedDataStream sds(ds);
    
    sds << forcetable.tables_by_idx << forcetable.molnum_to_idx;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      ForceTable &forcetable)
{
    VersionID v = readHeader(ds, r_forcetable);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> forcetable.tables_by_idx >> forcetable.molnum_to_idx;
    }
    else
        throw version_error(v, "1", r_forcetable, CODELOC);
        
    return ds;
}

/** Constructor */
ForceTable::ForceTable()
{}

/** Construct a table that holds all of the forces on all of the atoms
    for all of the CutGroups viewed in all of the molecules in the passed
    molecule group */
ForceTable::ForceTable(const MolGroup &molgroup)
{
    if (molgroup.isEmpty())
        return;
        
    int nmols = molgroup.nMolecules();
    
    tables_by_idx = QVector<MolForceTable>(nmols);
    molnum_to_idx.reserve(nmols);
    
    MolForceTable *tables_by_idx_array = tables_by_idx.data();
    
    quint32 i = 0;
    
    for (MolGroup::const_iterator it = molgroup.constBegin();
         it != molgroup.constEnd();
         ++it)
    {
        tables_by_idx_array[i] = MolForceTable(*it);
        molnum_to_idx.insert(it->data().number(), i);
        ++i;
    }
}

/** Copy constructor */
ForceTable::ForceTable(const ForceTable &other)
           : tables_by_idx(other.tables_by_idx),
             molnum_to_idx(other.molnum_to_idx)
{}

/** Destructor */
ForceTable::~ForceTable()
{}

/** Copy assignment operator */
ForceTable& ForceTable::operator=(const ForceTable &other)
{
    tables_by_idx = other.tables_by_idx;
    molnum_to_idx = other.molnum_to_idx;
    
    return *this;
}

/** Comparison operator */
bool ForceTable::operator==(const ForceTable &other) const
{
    return tables_by_idx == other.tables_by_idx;
}

/** Comparison operator */
bool ForceTable::operator!=(const ForceTable &other) const
{
    return tables_by_idx != other.tables_by_idx;
}

/** Initialise all of the tables - this resets all of the forces 
    back to zero */
void ForceTable::initialiseTables()
{
    int nmols = tables_by_idx.count();
    
    if (nmols > 0)
    {
        MolForceTable *tables_by_idx_array = tables_by_idx.data();
        
        for (int i=0; i<nmols; ++i)
        {
            tables_by_idx_array[i].initialise();
        }
    }
}

/** Initialise all of the forces for the table for the molecule
    with number 'molnum'
    
    \throw SireMol::missing_molecule
*/
void ForceTable::initialiseTable(MolNum molnum)
{
    this->getTable(molnum).initialise();
}

/** Assert that this forcetable contains a table for the 
    forces for the molecule at number 'molnum'
    
    \throw SireMol::missing_molecule
*/
void ForceTable::assertContainsTableFor(MolNum molnum) const
{
    if (not this->containsTable(molnum))
        throw SireMol::missing_molecule( QObject::tr(
            "This force table does not contain a table for the "
            "molecule with number %1.")
                .arg(molnum), CODELOC );
}
