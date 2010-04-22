/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "fieldtable.h"

#include "SireMol/moleculeview.h"
#include "SireMol/moleculegroup.h"
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
///////// Implementation of MolFieldTable
/////////

static const RegisterMetaType<MolFieldTable> r_moltable;

QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, 
                                      const MolFieldTable &moltable)
{
    writeHeader(ds, r_moltable, 1);
    
    SharedDataStream sds(ds);
    
    sds << moltable.molnum << moltable.moluid << moltable.ncgroups
        << moltable.cgidx_to_idx << static_cast<const PackedArray2D<Vector>&>(moltable);
        
    return ds;
}

QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, MolFieldTable &moltable)
{
    VersionID v = readHeader(ds, r_moltable);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> moltable.molnum >> moltable.moluid >> moltable.ncgroups
            >> moltable.cgidx_to_idx >> static_cast<PackedArray2D<Vector>&>(moltable);
    }
    else
        throw version_error( v, "1", r_moltable, CODELOC );
        
    return ds;
}

/** Null constructor */
MolFieldTable::MolFieldTable() : PackedArray2D<Vector>(), molnum(0), ncgroups(0)
{}

/** Construct to hold the field acting at the points of all of the atoms
    of all of the cutgroups viewed in 'molview' */
MolFieldTable::MolFieldTable(const MoleculeView &molview)
              : PackedArray2D<Vector>(),
                molnum(molview.data().number()),
                moluid(molview.data().info().UID()),
                ncgroups(molview.data().info().nCutGroups())
{
    //build arrays for each selected CutGroup
    AtomSelection selected_atoms = molview.selection();
    
    if (selected_atoms.selectedAllCutGroups())
    {
        QVector< QVector<Vector> > fields(ncgroups);
        QVector<Vector> *fields_array = fields.data();
    
        for (CGIdx i(0); i<ncgroups; ++i)
        {
            fields_array[i] = QVector<Vector>(molview.data().info().nAtoms(i), 
                                              Vector(0));
        }
        
        PackedArray2D<Vector>::operator=(fields);
    }
    else
    {
        QVector< QVector<Vector> > fields(selected_atoms.nSelectedCutGroups());
        cgidx_to_idx.reserve(selected_atoms.nSelectedCutGroups());
        
        QVector<Vector> *fields_array = fields.data();
        qint32 idx = 0;
        
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            fields_array[i] = QVector<Vector>(molview.data().info().nAtoms(i),
                                              Vector(0));
                                              
            cgidx_to_idx.insert(i, idx);
            ++idx;
        }
        
        PackedArray2D<Vector>::operator=(fields);
    }
}

/** Copy constructor */
MolFieldTable::MolFieldTable(const MolFieldTable &other)
              : PackedArray2D<Vector>(other),
                molnum(other.molnum), moluid(other.moluid),
                ncgroups(other.ncgroups), cgidx_to_idx(other.cgidx_to_idx)
{}

/** Destructor */
MolFieldTable::~MolFieldTable()
{}

/** Copy assignment operator */
MolFieldTable& MolFieldTable::operator=(const MolFieldTable &other)
{
    if (this != &other)
    {
        PackedArray2D<Vector>::operator=(other);
        molnum = other.molnum;
        moluid = other.moluid;
        ncgroups = other.ncgroups;
        cgidx_to_idx = other.cgidx_to_idx;
    }
    
    return *this;
}

/** Comparison operator */
bool MolFieldTable::operator==(const MolFieldTable &other) const
{
    return this == &other or
           (molnum == other.molnum and moluid == other.moluid and
            ncgroups == other.ncgroups and cgidx_to_idx == other.cgidx_to_idx and
            PackedArray2D<Vector>::operator==(other));
}

/** Comparison operator */
bool MolFieldTable::operator!=(const MolFieldTable &other) const
{
    return not MolFieldTable::operator==(other);
}

/** Add the fields in the passed table onto this table */
MolFieldTable& MolFieldTable::operator+=(const MolFieldTable &other)
{
    this->add(other);
    return *this;
}

/** Subtract the fields in the passed table from this table */
MolFieldTable& MolFieldTable::operator-=(const MolFieldTable &other)
{
    this->subtract(other);
    return *this;
}

/** Return the sum of the fields of this table with 'other' */
MolFieldTable MolFieldTable::operator+(const MolFieldTable &other) const
{
    MolFieldTable ret(*this);
    ret += other;
    return ret;
}

/** Return the difference of the fields of this table with 'other' */
MolFieldTable MolFieldTable::operator-(const MolFieldTable &other) const
{
    MolFieldTable ret(*this);
    ret -= other;
    return *this;
}

const char* MolFieldTable::typeName()
{
    return QMetaType::typeName( qMetaTypeId<MolFieldTable>() );
}

/** Initialise this table - this clears all of the fields, resetting them to zero */
void MolFieldTable::initialise()
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

/** Return all of the fields in this table in a single array */
QVector<Vector> MolFieldTable::toVector() const
{
    return PackedArray2D<Vector>::toQVector();
}


/** Return an array of all of the fields at the location of 
    the atoms selected in 'selection'

    \throw SireError::incompatible_error
*/
QVector<Vector> MolFieldTable::toVector(const AtomSelection &selection) const
{
    this->assertCompatibleWith(selection);

    if (selection.selectedAll())
    {
        if (not this->selectedAll())
            throw SireMol::missing_atom( QObject::tr(
                "Cannot return the forces on all atoms as not all of the atoms "
                "are selected in this forcetable."), CODELOC );
        
        return this->toVector();
    }

    QVector<Vector> vals( selection.nSelected() );
    Vector *value = vals.data();

    if (this->selectedAll())
    {   
        if (selection.selectedAllCutGroups())
        {
            const int ncg = selection.nCutGroups();
        
            for (CGIdx i(0); i<ncg; ++i)
            {
                const Vector *groupfields = PackedArray2D<Vector>::constData(i);
            
                if (selection.selectedAll(i))
                {
                    const int nats = PackedArray2D<Vector>::nValues(i);
                    
                    quickCopy<Vector>(value, groupfields, nats);
                    value += nats;
                }
                else
                {
                    QList<Index> idxs = selection.selectedAtoms(i).toList();
                    qSort(idxs);
                    
                    foreach (Index idx, idxs)
                    {
                        *value = groupfields[idx];
                        ++value;
                    }
                }
            }
        }
        else
        {
            QList<CGIdx> cgidxs = selection.selectedCutGroups();
            qSort(cgidxs);
            
            foreach (CGIdx i, cgidxs)
            {
                const Vector *groupfields = PackedArray2D<Vector>::constData(i);
            
                if (selection.selectedAll(i))
                {
                    const int nats = PackedArray2D<Vector>::nValues(i);
                    
                    quickCopy<Vector>(value, groupfields, nats);
                    value += nats;
                }
                else
                {
                    QList<Index> idxs = selection.selectedAtoms(i).toList();
                    qSort(idxs);
                    
                    foreach (Index idx, idxs)
                    {
                        *value = groupfields[idx];
                        ++value;
                    }
                }
            }
        }
    }
    else
    {
        if (selection.selectedAllCutGroups())
            throw SireMol::missing_atom( QObject::tr(
                "Cannot return the forces as while all CutGroups are selected, "
                "not all CutGroups are present in the forcetable."), CODELOC );

        QList<CGIdx> cgidxs = selection.selectedCutGroups();
        qSort(cgidxs);
        
        foreach (CGIdx cgidx, cgidxs)
        {
            int i = cgidx_to_idx.value(cgidx, -1);
            
            if (i == -1)
                throw SireMol::missing_atom( QObject::tr(
                    "Cannot return the forces as while atoms in CutGroup %1 "
                    "are selected, this CutGroup is not present in the forcetable.")
                        .arg(cgidx), CODELOC );

            const Vector *groupfields = PackedArray2D<Vector>::constData(i);
        
            if (selection.selectedAll(cgidx))
            {
                const int nats = PackedArray2D<Vector>::nValues(i);
                
                quickCopy<Vector>(value, groupfields, nats);
                value += nats;
            }
            else
            {
                QList<Index> idxs = selection.selectedAtoms(cgidx).toList();
                qSort(idxs);
                
                foreach (Index idx, idxs)
                {
                    *value = groupfields[idx];
                    ++value;
                }
            }
        }
    }
    
    return vals;
}

/** Add the field 'field' onto this table - this returns whether or not the
    atom is in this table
    
    \throw SireError::invalid_index
*/
bool MolForceTable::add(const CGAtomIdx &cgatomidx, const Vector &field)
{
    CGIdx cgidx( cgatomidx.cutGroup().map(this->nCutGroups()) );
    
    int i = -1;
    
    if (this->selectedAll())
    {
        i = cgidx;
    }
    else if (cgidx_to_idx.contains(cgidx))
    {
        i = cgidx_to_idx.value(cgidx);
    }
    else
    {
        return false;
    }
    
    int j = cgatomidx.atom().map( this->nValues(i) );
    
    this->operator()(i, j) += field;
    
    return true;
}

/** Subtract the field 'field' from this table - this returns whether or not the
    atom is in this table
    
    \throw SireError::invalid_index
*/
bool MolFieldTable::subtract(const CGAtomIdx &cgatomidx, const Vector &field)
{
    return this->add( cgatomidx, -force );
}

static void addField(const Vector &field, Vector *fields, const int nats)
{
    for (int i=0; i<nats; ++i)
    {
        fields[i] += field;
    }
}

/** Add the field 'field' onto this table for all of the atoms
    in 'selected_atoms - this returns whether 
    or not any selected atoms are in this table
    
    \throw SireError::incompatible_error
*/
bool MolFieldTable::add(const AtomSelection &selected_atoms, const Vector &field)
{
    this->assertCompatibleWith(selected_atoms);

    if (selected_atoms.selectedNone() or this->isEmpty())
        return false;

    bool changed_atoms = false;

    if (selected_atoms.selectedAll())
    {
        //this is easy - all atoms are selected for updating,
        //so just update all of the forces in this table
        ::addField(field, this->valueData(), this->nValues());
        
        changed_atoms = true;
    }
    else if (this->selectedAll())
    {
        //easy(ish) case - all atoms are in this forcetable,
        //so we only need to update the forces of the selected atoms
    
        if (selected_atoms.selectedAllCutGroups())
        {
            for (CGIdx i(0); i<ncgroups; ++i)
            {
                if (selected_atoms.selectedAll(i))
                {
                    ::addField(field, this->data(i), this->nValues(i));
                    changed_atoms = true;
                }
                else
                {
                    QSet<Index> idxs = selected_atoms.selectedAtoms(i);
                    
                    Vector *atomfields = this->data(i);
                    const int nats = this->nValues(i);
                    
                    foreach (Index idx, idxs)
                    {
                        BOOST_ASSERT( idx >= 0 and idx < nats );
                        atomfields[idx] += field;
                    }
                    
                    changed_atoms = true;
                }
            }
        }
        else
        {
            QList<CGIdx> cgidxs = selected_atoms.selectedCutGroups();
            
            foreach (CGIdx i, cgidxs)
            {
                if (selected_atoms.selectedAll(i))
                {
                    ::addField(field, this->data(i), this->nValues(i));
                    changed_atoms = true;
                }
                else
                {
                    QSet<Index> idxs = selected_atoms.selectedAtoms(i);
                    
                    Vector *atomfields = this->data(i);
                    const int nats = this->nValues(i);
                    
                    foreach (Index idx, idxs)
                    {
                        BOOST_ASSERT( idx >= 0 and idx < nats );
                        atomfields[idx] += field;
                    }
                    
                    changed_atoms = true;
                }
            }
        }
    }
    else
    {
        //harder case - not all atoms are in this fieldtable
        //and not all atoms are selected for updating

        if (selected_atoms.selectedAllCutGroups())
        {
            for (QHash<CGIdx,qint32>::const_iterator it = cgidx_to_idx.constBegin();
                 it != cgidx_to_idx.constEnd();
                 ++it)
            {
                const CGIdx cgidx = it.key();
                const int i = it.value();
                
                if (selected_atoms.selectedAll(cgidx))
                {
                    ::addField(field, this->data(i), this->nValues(i));
                    changed_atoms = true;
                }
                else
                {
                    QSet<Index> idxs = selected_atoms.selectedAtoms(cgidx);
                    
                    Vector *atomfields = this->data(i);
                    const int nats = this->nValues(i);
                    
                    foreach (Index idx, idxs)
                    {
                        BOOST_ASSERT( idx >= 0 and idx < nats );
                        atomfields[idx] += force;
                    }
                }
                
                changed_atoms = true;
            }
        }
        else
        {
            for (QHash<CGIdx,qint32>::const_iterator it = cgidx_to_idx.constBegin();
                 it != cgidx_to_idx.constEnd();
                 ++it)
            {
                const CGIdx cgidx = it.key();
                const int i = it.value();
                
                if (selected_atoms.selectedAll(cgidx))
                {
                    ::addField(field, this->data(i), this->nValues(i));
                    changed_atoms = true;
                }
                else if (selected_atoms.selected(cgidx))
                {
                    QSet<Index> idxs = selected_atoms.selectedAtoms(cgidx);
                    
                    Vector *atomfields = this->data(i);
                    const int nats = this->nValues(i);
                    
                    foreach (Index idx, idxs)
                    {
                        BOOST_ASSERT( idx >= 0 and idx < nats );
                        atomfields[idx] += force;
                    }
                    
                    changed_atoms = true;
                }
            }
        }
    }
    
    return changed_atoms;
}

/** Subtract the field 'field' from this table for all of the atoms
    in 'selected_atoms' - this returns whether 
    or not any selected atoms are in this table
    
    \throw SireError::incompatible_error
*/
bool MolFieldTable::subtract(const AtomSelection &selected_atoms, const Vector &field)
{
    return MolForceTable::add( selected_atoms, -field );
}

void MolFieldTable::assertCompatibleWith(const AtomSelection &selection) const
{
    if (not selection.selectedAll())
    {
        AtomSelection new_selection(selection);
        new_selection.selectAll();
        this->assertCompatibleWith(new_selection);
        return;
    }

    bool compatible = true;
    
    if (selection.nCutGroups() != ncgroups)
    {
        compatible = false;
    }
    else if (this->selectedAll())
    {
        for (CGIdx i(0); i<ncgroups; ++i)
        {
            if (selection.nSelected(i) != PackedArray2D<Vector>::nValues(i))
            {
                compatible = false;
                break;
            }
        }
    }
    else
    {
        for (QHash<CGIdx,qint32>::const_iterator it = cgidx_to_idx.constBegin();
             it != cgidx_to_idx.constEnd();
             ++it)
        {
            if (selection.nSelected(it.key()) != PackedArray2D<Vector>::nValues(it.key()))
            {
                compatible = false;
                break;
            }
        }
    }
    
    if (not compatible)
        throw SireError::incompatible_error( QObject::tr(
            "This MolForceTable is incompatible with the passed atom selection."),
                CODELOC );
}

/** Add the fields contained in 'other' onto this field table. This will only
    add the fields for CutGroups that are in both tables */
void MolFieldTable::add(const MolFieldTable &other)
{
    if (this == &other)
    {
        //just double everything
    }
}

void MolFieldTable::subtract(const MolFieldTable &other);
