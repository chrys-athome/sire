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

#include "atomselection.h"
#include "moleculeview.h"
#include "moleculedata.h"

#include "moleculeinfodata.h"

#include "SireError/errors.h"
#include "SireMol/errors.h"

using namespace SireMol;
using namespace SireBase;

/** Null constructor */
AtomSelection::AtomSelection() 
              : ConcreteProperty<AtomSelection,MoleculeProperty>(),
                nselected(0)
{}

/** Return the info object for the molecule whose atoms
    are being selected */
const MoleculeInfoData& AtomSelection::info() const
{
    return *d;
}

/** Construct a selection of all of the atoms in the view 
    'molecule' */
AtomSelection::AtomSelection(const MoleculeView &molecule)
              : ConcreteProperty<AtomSelection,MoleculeProperty>()
{
    this->operator=(molecule.selection());
}

/** Construct a selection of all of the atoms in the 
    molecule whose data is in 'moldata' */
AtomSelection::AtomSelection(const MoleculeData &moldata)
              : ConcreteProperty<AtomSelection,MoleculeProperty>(),
                d(moldata.info()), nselected(moldata.info().nAtoms())
{}              

/** Construct a selection of all of the atoms in the
    molecule that is described by the info object 'molinfo' */
AtomSelection::AtomSelection(const MoleculeInfoData &molinfo)
              : ConcreteProperty<AtomSelection,MoleculeProperty>(),
                d(molinfo), nselected(molinfo.nAtoms())
{}

/** Copy constructor */
AtomSelection::AtomSelection(const AtomSelection &other)
              : ConcreteProperty<AtomSelection,MoleculeProperty>(other),
                selected_atoms(other.selected_atoms),
                d(other.d), nselected(other.nselected)
{}

/** Destructor */
AtomSelection::~AtomSelection()
{}

/** Copy assignment operator */
AtomSelection& AtomSelection::operator=(const AtomSelection &other)
{
    MoleculeProperty::operator=(other);

    selected_atoms = other.selected_atoms;
    d = other.d;
    nselected = other.nselected;
    
    return *this;
}

/** Comparison operator */
bool AtomSelection::operator==(const AtomSelection &other) const
{
    return this == &other or
           ( nselected == other.nselected and 
             (d == other.d or *d == *(other.d)) and
             selected_atoms == other.selected_atoms );
}

/** Comparison operator */
bool AtomSelection::operator!=(const AtomSelection &other) const
{
    return this != &other and
           ( nselected != other.nselected or
             (d != other.d and *d != *(other.d)) or
             selected_atoms != other.selected_atoms );
}

/** Return wheter no atoms are selected */
bool AtomSelection::isEmpty() const
{
    return nselected == 0;
}

/** Return the number of selected atoms */
int AtomSelection::nSelected() const
{
    return nselected;
}

bool AtomSelection::_pvt_selected(const CGAtomIdx &cgatomidx) const
{
    return nselected > 0 and 
           ( (not selected_atoms.contains(cgatomidx.cutGroup())) or
             (selected_atoms.find(cgatomidx.cutGroup())->contains(cgatomidx.atom())) );
}

/** Return whether or not the atom at index 'cgatomidx' has
    been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const CGAtomIdx &cgatomidx) const
{
    CGAtomIdx sane_idx( CGIdx(cgatomidx.cutGroup().map(info().nAtoms())),
                        Index(cgatomidx.atom()
                                        .map(info().nAtoms(cgatomidx.cutGroup()))) );
                                              
    return this->_pvt_selected(sane_idx);
}

/** Return whether the atom at index 'atomidx' has been selected 

    \throw SireError::invalid_index
*/
bool AtomSelection::selected(AtomIdx atomidx) const
{
    return this->_pvt_selected( info().cgAtomIdx(atomidx) );
}

/** Return whether any of the atom(s) identified by the ID 'atomid'
    have been selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const AtomID &atomid) const
{
    foreach (const CGAtomIdx &atomidx, info().cgAtomIdxs(atomid))
    {
        if (this->_pvt_selected(atomidx))
            return true;
    }
    
    return false;
}

/** Return whether or not any atom in the CutGroup
    at index 'cgidx' has been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(CGIdx cgidx) const
{
    cgidx = CGIdx( cgidx.map(info().nCutGroups()) );
    
    return this->selectedAll() or selected_atoms.contains(cgidx);
}

/** Return whether or not any atoms in the residue
    at index 'residx' has been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(ResIdx residx) const
{
    residx = ResIdx( residx.map(info().nResidues()) );
    
    if (selected_atoms.isEmpty())
        return nselected > 0;
    else
    {
        foreach( const CGAtomIdx &atomidx, info().cgAtomIdxs(residx) )
        {
            if (this->_pvt_selected(atomidx))
                return true;
        }
        
        return false;
    }
}

/** Return whether or not any atoms in the chain
    at index 'chainidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(ChainIdx chainidx) const
{
    chainidx = ChainIdx( chainidx.map(info().nChains()) );
    
    if (selected_atoms.isEmpty())
        return nselected > 0;
    else
    {
        foreach( ResIdx residx, info().getResiduesIn(chainidx) )
        {
            foreach( const CGAtomIdx &atomidx, info().cgAtomIdxs(residx) )
            {
                if (this->_pvt_selected(atomidx))
                    return true;
            }
        }
        
        return false;
    }
}

/** Return whether or not any atoms in the segment at 
    index 'segidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(SegIdx segidx) const
{
    segidx = SegIdx( segidx.map(info().nSegments()) );
    
    if (selected_atoms.isEmpty())
        return nselected > 0;
    else
    {
        foreach( const CGAtomIdx &atomidx, info().cgAtomIdxs(segidx) )
        {
            if (this->_pvt_selected(atomidx))
                return true;
        }
        
        return false;
    }
}

/** Return whether any atoms in the CutGroup(s) identified
    by 'cgid' have been selected 
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const CGID &cgid) const
{
    foreach (CGIdx cgidx, cgid.map(info()))
    {
        if (this->selected(cgidx))
            return true;
    }
    
    return false;
}

/** Return whether any atoms in the residue(s) identified
    by 'resid' have been selected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const ResID &resid) const
{
    foreach (ResIdx residx, resid.map(info()))
    {
        if (this->selected(residx))
            return true;
    }
    
    return false;
}

/** Return whether any atoms in the chain(s) identified
    by 'chainid' have been selected 
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const ChainID &chainid) const
{
    foreach (ChainIdx chainidx, chainid.map(info()))
    {
        if (this->selected(chainidx))
            return true;
    }
    
    return false;
}

/** Return whether any atoms in the segment(s) identified
    by 'segid' have been selected 
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
bool AtomSelection::selected(const SegID &segid) const
{
    foreach (SegIdx segidx, segid.map(info()))
    {
        if (this->selected(segidx))
            return true;
    }
    
    return false;
}

/** Return whether or not any of the atoms selected in 'selection'
    are also selected in this set 
    
    \throw SireError::incompatible_error
*/
bool AtomSelection::selected(const AtomSelection &selection) const
{
    this->info().assertEqualTo( selection.info() );
    
    //get rid of the easy cases...
    if (this->isEmpty() or selection.isEmpty())
        return false;
    else if (this->selectedAll() or selection.selectedAll())
        return true;
            
    //now do the hard stuff!
    if (selection.selectedAllCutGroups())
    {
        for (CGIdx i(0); i < this->info().nCutGroups(); ++i)
        {
            if (this->selectedAll(i))
                return true;
            else if (selection.selectedAll(i) and not this->selectedNone(i))
                return true;
            else if (not this->selectedNone(i))
            {
                const QSet<Index> &atoms = *(selected_atoms.find(i));
                
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    if (atoms.contains(idx))
                        return true;
                }
            }
        }
    }
    else
    {
        QList<CGIdx> cgidxs = selection.selectedCutGroups();
        
        foreach (CGIdx i, cgidxs)
        {
            if (this->selectedAll(i))
                return true;
            else if (selection.selectedAll(i) and not this->selectedNone(i))
                return true;
            else if (not this->selectedNone(i))
            {
                const QSet<Index> &atoms = *(selected_atoms.find(i));
                
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    if (atoms.contains(idx))
                        return true;
                }
            }
        }
    }
    
    return false;
}

/** Return whether all atoms have been selected */
bool AtomSelection::selectedAllAtoms() const
{
    return this->selectedAll();
}

/** Return whether all CutGroups contain at least
    one selected atom */
bool AtomSelection::selectedAllCutGroups() const
{
    return (selected_atoms.isEmpty() and nselected > 0) or
            selected_atoms.count() == info().nCutGroups();
}

/** Return whether all residues contain at least
    one selected atom */
bool AtomSelection::selectedAllResidues() const
{
    if (selected_atoms.isEmpty())
        return nselected > 0;
    else
    {
        for (ResIdx i(0); i<info().nResidues(); ++i)
        {
            if (not this->selected(i))
                return false;
        }
        
        return true;
    }
}

/** Return whether all chains contain at least
    one selected atom */
bool AtomSelection::selectedAllChains() const
{
    if (selected_atoms.isEmpty())
        return nselected > 0;
    else
    {
        for (ChainIdx i(0); i<info().nChains(); ++i)
        {
            if (not this->selected(i))
                return false;
        }
        
        return true;
    }
}

/** Return whether all segments contain at least
    one selected atom */
bool AtomSelection::selectedAllSegments() const
{
    if (selected_atoms.isEmpty())
        return nselected > 0;
    else
    {
        for (SegIdx i(0); i<info().nSegments(); ++i)
        {
            if (not this->selected(i))
                return false;
        }
        
        return true;
    }
}

/** Return whether or not no atoms have been selected */
bool AtomSelection::selectedNone() const
{
    return this->isEmpty();
}

/** Return whether the atom at index 'atomidx' has not been selected

    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(AtomIdx atomidx) const
{
    return not this->selected(atomidx);
}

/** Return whether none of the atoms in the CutGroup at
    index 'cgidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(CGIdx cgidx) const
{
    return not this->selected(cgidx);
}

/** Return whether none of the atoms in the residue at
    index 'residx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(ResIdx residx) const
{
    return not this->selected(residx);
}

/** Return whether none of the atoms in the chain at
    index 'chainidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(ChainIdx chainidx) const
{
    return not this->selected(chainidx);
}

/** Return whether none of the atoms in the segment at
    index 'segidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(SegIdx segidx) const
{
    return not this->selected(segidx);
}

/** Return whether none of the atoms identified by 'atomid'
    have been selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(const AtomID &atomid) const
{
    return not this->selected(atomid);
}

/** Return whether none of the atoms in the CutGroup(s) 
    identified by 'cgid' have been selected 
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(const CGID &cgid) const
{
    return not this->selected(cgid);
}

/** Return whether none of the atoms in the residue(s) 
    identified by 'resid' have been selected 
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(const ResID &resid) const
{
    return not this->selected(resid);
}

/** Return whether none of the atoms in the chain(s) 
    identified by 'chainid' have been selected 
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(const ChainID &chainid) const
{
    return not this->selected(chainid);
}

/** Return whether none of the atoms in the segment(s) 
    identified by 'segid' have been selected 
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedNone(const SegID &segid) const
{
    return not this->selected(segid);
}

/** Return whether none of the atoms selected in 'selection' have
    been selected in this set
    
    \throw SireError::incompatible_error
*/
bool AtomSelection::selectedNone(const AtomSelection &selection) const
{
    return not this->selected(selection);
}

/** Return whether or not all of the atoms are selected */
bool AtomSelection::selectedAll() const
{
    return nselected != 0 and selected_atoms.isEmpty();
}

/** Return whether or not the atom at index 'atomidx' is selected 

    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(AtomIdx atomidx) const
{
    return this->selected(atomidx);
}

bool AtomSelection::_pvt_selectedAll(CGIdx cgidx) const
{
    return nselected != 0 and not selected_atoms.contains(cgidx);
}

/** Return whether or not all of the atoms in the CutGroup
    at index 'cgidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(CGIdx cgidx) const
{
    return this->_pvt_selectedAll( CGIdx(cgidx.map(info().nCutGroups())) );
}

bool AtomSelection::_pvt_selectedAll(const QVector<CGAtomIdx> &atomidxs) const
{
    if (this->selectedAll())
        return true;
    else if (this->selectedNone() or nselected < atomidxs.count())
        return false;

    const CGAtomIdx *atomidxs_array = atomidxs.constData();
    int nats = atomidxs.count();

    for (int i=0; i<nats; ++i)
    {
        if (not this->_pvt_selected(atomidxs_array[i]))
            return false;
    }
    
    return true;
}

/** Return whether or not all of the atoms in the residue
    at index 'residx' have been selected 
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(ResIdx residx) const
{
    return this->_pvt_selectedAll( info().cgAtomIdxs(residx) );
}

/** Return whether or not all of the atoms in the chain
    at index 'chainidx' have been selected 
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(ChainIdx chainidx) const
{
    return this->_pvt_selectedAll( info().cgAtomIdxs(chainidx) );
}

/** Return whether or not all of the atoms in the segment
    at index 'segidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(SegIdx segidx) const
{
    return this->_pvt_selectedAll( info().cgAtomIdxs(segidx) );
}

/** Return whether or not all of the atoms matching the 
    ID 'atomid' have been selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(const AtomID &atomid) const
{
    return this->_pvt_selectedAll( info().cgAtomIdxs(atomid) );
}

/** Return whether or not all of the atoms in the CutGroups matching the 
    ID 'cgid' have been selected
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(const CGID &cgid) const
{
    foreach (CGIdx cgidx, cgid.map(this->info()))
    {
        if (not this->_pvt_selectedAll(cgidx))
            return false;
    }
    
    return true;
}

/** Return whether or not all of the atoms in the residues matching the 
    ID 'resid' have been selected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(const ResID &resid) const
{
    return this->_pvt_selectedAll( info().cgAtomIdxs(resid) );
}

/** Return whether or not all of the atoms in the chains matching the 
    ID 'atomid' have been selected
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(const ChainID &chainid) const
{
    return this->_pvt_selectedAll( info().cgAtomIdxs(chainid) );
}

/** Return whether or not all of the atoms in the segments matching the 
    ID 'atomid' have been selected
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
bool AtomSelection::selectedAll(const SegID &segid) const
{
    return this->_pvt_selectedAll( info().cgAtomIdxs(segid) );
}

/** Return the set of indicies of the atoms in the CutGroup
    at index 'cgidx' that are selected within this CutGroup
    
    \throw SireError::invalid_index
*/
QSet<Index> AtomSelection::selectedAtoms(CGIdx cgidx) const
{
    cgidx = CGIdx( cgidx.map(info().nCutGroups()) );
    
    if (this->_pvt_selectedAll(cgidx))
    {
        QSet<Index> ret;
    
        int nats = info().nAtoms(cgidx);
        
        ret.reserve(nats);
        
        for (Index i(0); i<nats; ++i)
        {
            ret.insert(i);
        }
        
        return ret;
    }
    else
        return selected_atoms[cgidx];
}

/** Return the list of indicies of CutGroups that contain at least
    one selected atom */
QList<CGIdx> AtomSelection::selectedCutGroups() const
{
    if (this->selectedAll())
    {
        QList<CGIdx> ret;
        for (CGIdx i(0); i<info().nCutGroups(); ++i)
        {
            ret.append(i);
        }
        
        return ret;
    }
    else if (this->selectedNone())
    {
        return QList<CGIdx>();
    }
    else
    {
        QList<CGIdx> cgs = selected_atoms.keys();
        qSort(cgs);
        return cgs;
    }
}

/** Return whether or not all of the atoms selected in 'selection' 
    have also been selected in this object
    
    \throw SireError::incompatible_error
*/
bool AtomSelection::selectedAll(const AtomSelection &selection) const
{
    this->info().assertEqualTo( selection.info() );
    
    //get rid of the easy cases...
    if (this->isEmpty() or selection.isEmpty())
        return false;
    else if (this->selectedAll())
        return true;
    else if (selection.selectedAll())
        return false;
    else if (selection.selectedAllCutGroups() and not this->selectedAllCutGroups())
        return false;
            
    //now do the hard stuff!
    if (selection.selectedAllCutGroups())
    {
        for (CGIdx i(0); i < this->info().nCutGroups(); ++i)
        {
            bool selected_all = this->selectedAll(i);
        
            if (selection.selectedAll(i) and not selected_all)
                return false;
            else if (selection.nSelected(i) > this->nSelected(i))
                return false;
            else if (not selected_all)
            {
                const QSet<Index> &atoms = *(selected_atoms.find(i));
            
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    if (not atoms.contains(idx))
                        return false;
                }
            }
        }
    }
    else
    {
        QList<CGIdx> cgidxs = selection.selectedCutGroups();
        
        foreach (CGIdx i, cgidxs)
        {
            if (this->selectedNone(i))
                return false;
        
            bool selected_all = this->selectedAll(i);
            
            if (selection.selectedAll(i) and not selected_all)
                return false;
            else if (selection.nSelected(i) > this->nSelected(i))
                return false;
            else
            {
                const QSet<Index> &atoms = *(selected_atoms.find(i));
                
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    if (not atoms.contains(idx))
                        return false;
                }
            }
        }
    }
    
    return true;
}

/** Return the number of atoms selected in the CutGroup at 
    index 'cgidx' */
int AtomSelection::nSelected(CGIdx cgidx) const
{
    if (this->selectedAll(cgidx))
        return info().nAtoms(cgidx);
    else
    {
        return selected_atoms.value( CGIdx(cgidx.map(info().nCutGroups())) ).count();
    }
}

/** Return whether the atom at index atomidx has been selected 

    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(AtomIdx atomidx) const
{
    if (this->_pvt_selected( AtomIdx(atomidx.map(info().nAtoms())) ))
        return 1;
    else
        return 0;
}

int AtomSelection::_pvt_nSelected(ResIdx residx) const
{
    if (this->isEmpty())
        return 0;
    else if (this->selectedAll())
        return info().nAtoms(residx);

    int nats = 0;

    foreach (const CGAtomIdx &atomidx, info().cgAtomIdxs(residx))
    {
        if (this->_pvt_selected(atomidx))
            ++nats;
    }
    
    return nats;
}

/** Return the number of atoms from the residue at index 'residx'
    that have been selected 
    
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(ResIdx residx) const
{
    return this->_pvt_nSelected( ResIdx(residx.map(info().nResidues())) );
}

/** Return the number of atoms from the chain at index 'chainidx'
    that have been selected 
    
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(ChainIdx chainidx) const
{
    chainidx = ChainIdx( chainidx.map(info().nChains()) );
    
    if (this->isEmpty())
        return 0;
    else if (this->selectedAll())
        return info().nChains();
    
    int nats = 0;
    
    foreach (ResIdx residx, info().getResiduesIn(chainidx))
    {
        nats += this->_pvt_nSelected(residx);
    }
    
    return nats;
}

/** Return the number of atoms from the segment at index 'segidx'
    that have been selected 
    
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(SegIdx segidx) const
{
    segidx = SegIdx( segidx.map(info().nSegments()) );
    
    if (this->isEmpty())
        return 0;
    else if (this->selectedAll())
        return info().nAtoms(segidx);
     
    int nats = 0;
          
    foreach (const CGAtomIdx &atomidx, info().cgAtomIdxs(segidx))
    {
        if (this->_pvt_selected(atomidx))
            ++nats;
    }
    
    return nats;
}

/** Return the number of atoms from the CutGroups identified
    by 'cgid' that have been selected 
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(const CGID &cgid) const
{
    int nats = 0;

    foreach (CGIdx cgidx, cgid.map(info()))
    {
        nats += this->nSelected(cgidx);
    }
    
    return nats;
}

/** Return the number of atoms that are identified by
    'atomid' that have been selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(const AtomID &atomid) const
{
    int nats = 0;
    
    foreach (const CGAtomIdx &atomidx, info().cgAtomIdxs(atomid))
    {
        if (this->_pvt_selected(atomidx))
            ++nats;
    }
    
    return nats;
}

/** Return the number of atoms from the residues identified
    by 'resid' that have been selected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(const ResID &resid) const
{
    int nats = 0;
    
    foreach (ResIdx residx, resid.map(info()))
    {
        nats += this->nSelected(residx);
    }
    
    return nats;
}

/** Return the number of atoms from the chain(s) identified
    by 'chainid' that have been selected
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(const ChainID &chainid) const
{
    int nats = 0;
    
    foreach (ChainIdx chainidx, chainid.map(info()))
    {
        nats += this->nSelected(chainidx);
    }
    
    return nats;
}

/** Return the number of atoms from the segment(s) 
    identified by 'segid' that have been selected
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
int AtomSelection::nSelected(const SegID &segid) const
{
    int nats = 0;
    
    foreach (SegIdx segidx, segid.map(info()))
    {
        nats += this->nSelected(segidx);
    }
    
    return nats;
}

/** Return the number of atoms from the passed selection
    that have also been selected in this selection
    
    \throw SireError::incompatible_error
*/
int AtomSelection::nSelected(const AtomSelection &selection) const
{
    info().assertEqualTo(selection.info());
    
    if (selection.isEmpty() or this->isEmpty())
        return 0;
    else if (selection.selectedAll())
        return this->nSelected();
    else if (this->selectedAll())
        return selection.nSelected();
     
    int nats = 0;
              
    if (selection.selectedAllCutGroups())
    {
        for (CGIdx i(0); i < info().nCutGroups(); ++i)
        {
            if (this->selectedAll(i))
                nats += selection.nSelected(i);
            else if (selection.selectedAll(i))
                nats += this->nSelected(i);
            else if (not this->selectedNone(i))
            {
                const QSet<Index> &atoms = *(selected_atoms.find(i));
            
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    if (atoms.contains(idx))
                        ++nats;
                }
            }
        }
    }
    else
    {
        foreach (CGIdx i, selection.selectedCutGroups())
        {
            if (this->selectedAll(i))
                nats += selection.nSelected(i);
            else if (selection.selectedAll(i))
                nats += this->nSelected(i);
            else if (not this->selectedNone(i))
            {
                const QSet<Index> &atoms = *(selected_atoms.find(i));
            
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    if (atoms.contains(idx))
                        ++nats;
                }
            }
        }
    }
    
    return nats;
}

/** Return the total number of selected atoms */
int AtomSelection::nSelectedAtoms() const
{
    return nselected;
}

/** Return the number of CutGroups that have at least
    one selected atom */
int AtomSelection::nSelectedCutGroups() const
{
    if (this->selectedAll())
        return info().nCutGroups();
    else
        return selected_atoms.count();
}

/** Return the number of residues that contain at
    least one selected atom */
int AtomSelection::nSelectedResidues() const
{
    if (this->selectedAll())
        return info().nResidues();
    else if (this->selectedNone())
        return 0;
    else
    {
        int nres = 0;
        
        for (ResIdx i(0); i<info().nResidues(); ++i)
        {
            foreach (const CGAtomIdx &atomidx, info().cgAtomIdxs(i))
            {
                if (this->_pvt_selected(atomidx))
                {
                    ++nres;
                    break;
                }
            }
        }
        
        return nres;
    }
}

/** Return the number of chains that have at least one selected atom */
int AtomSelection::nSelectedChains() const
{
    if (this->selectedAll())
        return info().nChains();
    else if (this->selectedNone())
        return 0;
    else
    {
        int nchains = 0;
        
        for (ChainIdx i(0); i<info().nChains(); ++i)
        {
            if (this->selected(i))
                ++nchains;
        }
        
        return nchains;
    }
}

/** Return the number of segments that contain at 
    least one selected atom */
int AtomSelection::nSelectedSegments() const
{
    if (this->selectedAll())
        return info().nSegments();
    else if (this->selectedNone())
        return 0;
    else
    {
        int nsegs = 0;
        
        for (SegIdx i(0); i<info().nSegments(); ++i)
        {
            if (this->selected(i))
                ++nsegs;
        }
        
        return nsegs;
    }
}

/** Return a selection that has all of the atoms selected */
AtomSelection AtomSelection::selectAll() const
{
    AtomSelection ret(*this);
    
    ret.selected_atoms.clear();
    ret.nselected = info().nAtoms();
    
    return ret;
}

/** Return a selection that has none of the atoms selected */
AtomSelection AtomSelection::deselectAll() const
{
    AtomSelection ret(*this);
    
    ret.selected_atoms.clear();
    ret.nselected = 0;
    
    return ret;
}

/** Return a selection that has none of the atoms selected */
AtomSelection AtomSelection::selectNone() const
{
    return this->deselectAll();
}

void AtomSelection::_pvt_select(const CGAtomIdx &cgatomidx)
{
    if (this->_pvt_selected(cgatomidx))
        return;
        
    if (this->selectedNone())
    {
        //we are selecting the first atom in the molecule...
        int ncg = info().nCutGroups();
        
        //create space for all of the atoms in all of the CutGroups
        selected_atoms.reserve(ncg);
        
        for (CGIdx i(0); i<ncg; ++i)
        {
            int nats = info().nAtoms(cgatomidx.cutGroup());
            
            BOOST_ASSERT(nats > 0);
        
            selected_atoms.insert(i, QSet<Index>())
                          .value().reserve( nats-1 );
        }
    }
        
    QSet<Index> &atoms = selected_atoms[cgatomidx.cutGroup()];
        
    if (atoms.count() == info().nAtoms(cgatomidx.cutGroup()) - 1)
    {
        //we've just selected the last atom in this CutGroup
        selected_atoms.remove(cgatomidx.cutGroup());
    }
    else
    {
        selected_atoms[cgatomidx.cutGroup()].insert(cgatomidx.atom());
    }
    
    ++nselected;
}

void AtomSelection::_pvt_select(const QVector<CGAtomIdx> &cgatomidxs)
{
    if (this->selectedAll())
        return;

    const CGAtomIdx *cgatomidxs_array = cgatomidxs.constData();
    int nats = cgatomidxs.count();
    
    for (int i=0; i<nats; ++i)
    {
        this->_pvt_select(cgatomidxs_array[i]);
    }
}

void AtomSelection::_pvt_select(CGIdx cgidx, const QSet<Index> &atoms)
{
    if (this->selectedAll())
        return;
        
    foreach (Index atom, atoms)
    {
        this->_pvt_select( CGAtomIdx(cgidx,atom) );
    }
}

void AtomSelection::_pvt_select(AtomIdx atomidx)
{
    this->_pvt_select( info().cgAtomIdx(atomidx) );
}

/** Return the selection that has additionally the atom at index
    'atomidx' selected */
AtomSelection AtomSelection::select(AtomIdx atomidx) const
{
    AtomSelection ret(*this);
    ret._pvt_select(atomidx);
    
    return ret;
}

void AtomSelection::_pvt_deselect(const CGAtomIdx &cgatomidx)
{
    if (not this->selected(cgatomidx))
        return;
    
    else if (nselected == 1)
    {
        //we have just removed the last selected atom
        selected_atoms.clear();
        nselected = 0;
        return;
    }
    
    if ( this->selectedAll(cgatomidx.cutGroup()) )
    {
        //we need to recreate space for this CutGroup
        int nats = info().nAtoms(cgatomidx.cutGroup());
        
        QSet<Index> atoms;
        atoms.reserve(nats-1);
        
        for (Index i(0); i<nats; ++i)
        {
            if (i != cgatomidx.atom())
                atoms.insert(i);
        }
        
        selected_atoms.insert(cgatomidx.cutGroup(), atoms);
    }
    else
    {
        selected_atoms[cgatomidx.cutGroup()].remove(cgatomidx.atom());
    }
    
    --nselected;
}

void AtomSelection::_pvt_deselect(const QVector<CGAtomIdx> &cgatomidxs)
{
    if (this->selectedNone())
        return;

    const CGAtomIdx *cgatomidxs_array = cgatomidxs.constData();
    int nats = cgatomidxs.count();
    
    for (int i=0; i<nats; ++i)
    {
        this->_pvt_deselect( cgatomidxs_array[i] );
    }
}

void AtomSelection::_pvt_deselect(AtomIdx atomidx)
{
    this->_pvt_deselect( info().cgAtomIdx(atomidx) );
}

/** Return a selection where the atom atom index 'atomidx'
    has been deselected 

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(AtomIdx atomidx) const
{
    AtomSelection ret(*this);
    
    ret._pvt_deselect( info().cgAtomIdx(atomidx) );
    
    return ret;
}

/** Return the selection in which only the atom at index
    'atomidx' has been selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(AtomIdx atomidx) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_select( info().cgAtomIdx(atomidx) );
    
    return ret;
}

void AtomSelection::_pvt_select(CGIdx cgidx)
{
    if (this->selectedAll(cgidx))
        return;
        
    QSet<Index> atoms = selected_atoms.take(cgidx);
    
    if (selected_atoms.isEmpty())
        selected_atoms.clear();
    
    nselected += info().nAtoms(cgidx) - atoms.count();
}

void AtomSelection::_pvt_deselect(CGIdx cgidx)
{
    if (this->selectedNone(cgidx))
        return;
    
    else if (this->selectedAll(cgidx))
    {
        QSet<Index> atoms;
        int nats = info().nAtoms(cgidx);
        
        atoms.reserve(nats-1);
        selected_atoms.insert(cgidx, atoms);
        
        nselected -= nats;
    }
    else
    {
        QSet<Index> &atoms = selected_atoms[cgidx];
        
        nselected -= atoms.count();
        
        atoms.clear();
    }
}

/** Return a selection that has the CutGroup at index
    'cgidx' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(CGIdx cgidx) const
{
    AtomSelection ret(*this);
    ret._pvt_select( CGIdx(cgidx.map(info().nCutGroups())) );
    
    return ret;
}

/** Return a selection that has the CutGroup at index
    'cgidx' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(CGIdx cgidx) const
{
    AtomSelection ret(*this);
    ret._pvt_deselect( CGIdx(cgidx.map(info().nCutGroups())) );
    
    return ret;
}

/** Return a selection that has only the CutGroup at index
    'cgidx' selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(CGIdx cgidx) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_select( CGIdx(cgidx.map(info().nCutGroups())) );
    
    return ret;
}

template<class IDXS>
void AtomSelection::_pvt_selectAtoms(const IDXS &atoms)
{
    foreach (const AtomIdx &atom, atoms)
    {
        this->_pvt_select(atom);
    }
}

template<class IDXS>
void AtomSelection::_pvt_deselectAtoms(const IDXS &atoms)
{
    foreach (const AtomIdx &atom, atoms)
    {
        this->_pvt_deselect(atom);
    }
}

/** Return a selection that has the residue at index
    'residx' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(ResIdx residx) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms( d->getAtomsIn(residx) );
    return ret;
}

/** Return a selection that has the residue at index
    'residx' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(ResIdx residx) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms( d->getAtomsIn(residx) );
    return ret;
}

/** Return a selection that has only the residue at index
    'residx' selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(ResIdx residx) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms( d->getAtomsIn(residx) );
    return ret;
}

/** Return a selection that has the chain at index
    'chainidx' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(ChainIdx chainidx) const
{
    AtomSelection ret(*this);
    
    foreach (const ResIdx residx, d->getResiduesIn(chainidx) )
    {
        ret._pvt_selectAtoms( d->getAtomsIn(residx) );
    }
    
    return ret;
}

/** Return a selection that has the chain at index
    'chainidx' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(ChainIdx chainidx) const
{
    AtomSelection ret(*this);
    
    foreach (const ResIdx residx, d->getResiduesIn(chainidx))
    {
        ret._pvt_deselectAtoms( d->getAtomsIn(residx) );
    }
    
    return ret;
}

/** Return a selection that has only the chain at index
    'chainidx' selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(ChainIdx chainidx) const
{
    AtomSelection ret = this->selectNone();
    
    foreach (const ResIdx &residx, d->getResiduesIn(chainidx))
    {
        ret._pvt_selectAtoms( d->getAtomsIn(residx) );
    }
    
    return ret;
}

/** Return a selection that has the segment at index
    'segidx' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(SegIdx segidx) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms( d->getAtomsIn(segidx) );
    return ret;
}

/** Return a selection that has the segment at index
    'segidx' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(SegIdx segidx) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms( d->getAtomsIn(segidx) );
    return ret;
}

/** Return a selection that has only the segment at index
    'segidx' selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(SegIdx segidx) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms( d->getAtomsIn(segidx) );
    return ret;
}

/** Deselect all of the atoms whose indicies are in 'atomidxs'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const QSet<AtomIdx> &atomidxs) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms(atomidxs);
    
    return ret;
}

/** Deselect all of the atoms whose indicies are in 'atomidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const QSet<AtomIdx> &atomidxs) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms(atomidxs);
    
    return ret;
}

/** Return a select where only the atoms whose indicies are in 
    'atomidxs' are selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const QSet<AtomIdx> &atomidxs) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms(atomidxs);
    
    return ret;
}

/** Deselect all of the atoms whose indicies are in 'atomidxs'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const QList<AtomIdx> &atomidxs) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms(atomidxs);
    
    return ret;
}

/** Deselect all of the atoms whose indicies are in 'atomidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const QList<AtomIdx> &atomidxs) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms(atomidxs);
    
    return ret;
}

/** Return a select where only the atoms whose indicies are in 
    'atomidxs' are selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const QList<AtomIdx> &atomidxs) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms(atomidxs);
    
    return ret;
}

/** Return the selection with all of the atoms in the CutGroups
    whose indicies are in 'cgidxs' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const QList<CGIdx> &cgidxs) const
{
    AtomSelection ret(*this);
    
    foreach (const CGIdx &cgidx, cgidxs)
    {
        ret._pvt_select( CGIdx(cgidx.map(d->nCutGroups())) );
    }
    
    return ret;
}

/** Return the selection with all of the atoms in the CutGroups
    whose indicies are in 'cgidxs' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const QList<CGIdx> &cgidxs) const
{
    AtomSelection ret(*this);
    
    foreach (const CGIdx &cgidx, cgidxs)
    {
        ret._pvt_deselect( CGIdx(cgidx.map(d->nCutGroups())) );
    }
    
    return ret;
}

/** Return the selection where only the atoms in the CutGroups
    whose indicies are in 'cgidxs' are selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const QList<CGIdx> &cgidxs) const
{
    AtomSelection ret = this->deselectAll();
    
    foreach (const CGIdx &cgidx, cgidxs)
    {
        ret._pvt_select( CGIdx(cgidx.map(d->nCutGroups())) );
    }
    
    return ret;
}

/** Return the selection with all of the atoms in the residues
    whose indicies are in 'residxs' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const QList<ResIdx> &residxs) const
{
    AtomSelection ret(*this);
    
    foreach (const ResIdx &residx, residxs)
    {
        ret._pvt_selectAtoms( d->getAtomsIn(residx) );
    }
    
    return ret;
}

/** Return the selection with all of the atoms in the residues
    whose indicies are in 'residxs' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const QList<ResIdx> &residxs) const
{
    AtomSelection ret(*this);
    
    foreach (const ResIdx &residx, residxs)
    {
        ret._pvt_deselectAtoms( d->getAtomsIn(residx) );
    }
    
    return ret;
}

/** Return the selection where only the atoms in the residues
    whose indicies are in 'cgidxs' are selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const QList<ResIdx> &residxs) const
{
    AtomSelection ret = this->deselectAll();
    
    foreach (const ResIdx &residx, residxs)
    {
        ret._pvt_selectAtoms( d->getAtomsIn(residx) );
    }
    
    return ret;
}

/** Return the selection with all of the atoms in the chains
    whose indicies are in 'chainidxs' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const QList<ChainIdx> &chainidxs) const
{
    AtomSelection ret(*this);
    
    foreach (const ChainIdx &chainidx, chainidxs)
    {
        foreach (const ResIdx &residx, d->getResiduesIn(chainidx))
        {
            ret._pvt_selectAtoms( d->getAtomsIn(residx) );
        }
    }
    
    return ret;
}

/** Return the selection with all of the atoms in the chains
    whose indicies are in 'chainidxs' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const QList<ChainIdx> &chainidxs) const
{
    AtomSelection ret(*this);
    
    foreach (const ChainIdx &chainidx, chainidxs)
    {
        foreach (const ResIdx &residx, d->getResiduesIn(chainidx))
        {
            ret._pvt_deselectAtoms( d->getAtomsIn(residx) );
        }
    }
    
    return ret;
}

/** Return the selection where only the atoms in the chains
    whose indicies are in 'chainidxs' are selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const QList<ChainIdx> &chainidxs) const
{
    AtomSelection ret = this->deselectAll();
    
    foreach (const ChainIdx &chainidx, chainidxs)
    {
        foreach (const ResIdx &residx, d->getResiduesIn(chainidx))
        {
            ret._pvt_selectAtoms( d->getAtomsIn(residx) );
        }
    }
    
    return ret;
}

/** Return the selection with all of the atoms in the segments
    whose indicies are in 'segidxs' additionally selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const QList<SegIdx> &segidxs) const
{
    AtomSelection ret(*this);

    foreach (const SegIdx &segidx, segidxs)
    {
        ret._pvt_selectAtoms( d->getAtomsIn(segidx) );
    }
    
    return ret;
}

/** Return the selection with all of the atoms in the segments
    whose indicies are in 'segidxs' deselected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const QList<SegIdx> &segidxs) const
{
    AtomSelection ret(*this);

    foreach (const SegIdx &segidx, segidxs)
    {
        ret._pvt_deselectAtoms( d->getAtomsIn(segidx) );
    }
    
    return ret;
}

/** Return the selection where only the atoms in the segments
    whose indicies are in 'segidxs' are selected
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const QList<SegIdx> &segidxs) const
{
    AtomSelection ret = this->deselectAll();

    foreach (const SegIdx &segidx, segidxs)
    {
        ret._pvt_selectAtoms( d->getAtomsIn(segidx) );
    }
    
    return ret;
}

/** Return a selection that has the atoms identified by
    'atomid' additionally selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const AtomID &atomid) const
{
    AtomSelection ret(*this);
    ret._pvt_select( info().cgAtomIdxs(atomid) );
    return ret;
}

/** Return a selection that has the atoms identified by
    'atomid' deselected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const AtomID &atomid) const
{
    AtomSelection ret(*this);
    ret._pvt_deselect( info().cgAtomIdxs(atomid) );
    
    return ret;
}

/** Return a selection that has only the atoms identified by
    'atomid' selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const AtomID &atomid) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_select( info().cgAtomIdxs(atomid) );
    
    return ret;
}

/** Return a selection that has the atoms in the CutGroup(s) identified by
    'cgid' additionally selected
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const CGID &cgid) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms( d->getAtomsIn(cgid) );

    return ret;
}

/** Return a selection that has the atoms in the CutGroup(s) identified by
    'cgid' deselected
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const CGID &cgid) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms( d->getAtomsIn(cgid) );
    
    return ret;
}

/** Return a selection that has only the atoms in the CutGroup(s) identified by
    'cgid' selected
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const CGID &cgid) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms( d->getAtomsIn(cgid) );
    
    return ret;
}

/** Return a selection that has the atoms in the residue(s) identified by
    'resid' additionally selected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const ResID &resid) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms( d->getAtomsIn(resid) );
    return ret;
}

/** Return a selection that has the atoms in the residue(s) identified by
    'resid' deselected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const ResID &resid) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms( d->getAtomsIn(resid) );
    
    return ret;
}

/** Return a selection that has only the atoms in the residue(s) identified by
    'resid' selected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const ResID &resid) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms( d->getAtomsIn(resid) );
    
    return ret;
}

/** Return a selection that has the atoms in the chain(s) identified by
    'chainid' additionally selected
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const ChainID &chainid) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms( d->getAtomsIn(chainid) );
    return ret;
}

/** Return a selection that has the atoms in the chain(s) identified by
    'chainid' deselected
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const ChainID &chainid) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms( d->getAtomsIn(chainid) );
    
    return ret;
}

/** Return a selection that has only the atoms in the chain(s) identified by
    'chainid' selected
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const ChainID &chainid) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms( d->getAtomsIn(chainid) );
    
    return ret;
}

/** Return a selection that has the atoms in the segment(s) identified by
    'segid' additionally selected
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::select(const SegID &segid) const
{
    AtomSelection ret(*this);
    ret._pvt_selectAtoms( d->getAtomsIn(segid) );
    return ret;
}

/** Return a selection that has the atoms in the segment(s) identified by
    'segid' deselected
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::deselect(const SegID &segid) const
{
    AtomSelection ret(*this);
    ret._pvt_deselectAtoms( d->getAtomsIn(segid) );
    
    return ret;
}

/** Return a selection that has only the atoms in the segment(s) identified by
    'segid' selected
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::selectOnly(const SegID &segid) const
{
    AtomSelection ret = this->selectNone();
    ret._pvt_selectAtoms( d->getAtomsIn(segid) );
    
    return ret;
}

void AtomSelection::_pvt_select(const AtomSelection &selection)
{
    info().assertEqualTo(selection.info());
    
    if (this->selectedAll())
        return;
    else if (selection.selectedAll())
    {
        selected_atoms.clear();
        nselected = info().nAtoms();
        return;
    }
    
    if (selection.selectedAllCutGroups())
    {
        int ncg = info().nCutGroups();
        
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selection.selectedAll(i))
                this->_pvt_select(i);
            else if (not this->selectedAll(i))
            {
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    this->_pvt_select( CGAtomIdx(i,idx) );
                }
            }
        }
    }
    else
    {
        foreach (CGIdx i, selection.selectedCutGroups())
        {
            if (selection.selectedAll(i))
                this->_pvt_select(i);
            else if (this->selectedAll(i))
            {
                foreach(Index idx, selection.selectedAtoms(i))
                {
                    this->_pvt_select( CGAtomIdx(i,idx) );
                }
            }
        }
    }
}

/** Return a selection with the atoms in 'selection' additionally   
    selected
    
    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::select(const AtomSelection &selection) const
{
    AtomSelection ret(*this);
      
    ret._pvt_select(selection);
      
    return ret;
}

/** Return a selection with all of the atoms selected in 'selection'
    deselected
    
    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::deselect(const AtomSelection &selection) const
{
    info().assertEqualTo(selection.info());
    
    if (this->selectedNone())
        return *this;
    else if (selection.selectedAll())
        return this->selectNone();
        
    AtomSelection ret(*this);
    
    if (selection.selectedAllCutGroups())
    {
        int ncg = info().nCutGroups();
        
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selection.selectedAll(i))
            {
                ret._pvt_deselect(i);
            }
            else if (not ret.selectedNone(i))
            {
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    ret._pvt_deselect( CGAtomIdx(i,idx) );
                }
            }
        }
    }
    else
    {
        foreach (CGIdx i, selection.selectedCutGroups())
        {
            if (selection.selectedAll(i))
            {
                ret._pvt_deselect(i);
            }
            else if (not ret.selectedNone(i))
            {
                foreach (Index idx, selection.selectedAtoms(i))
                {
                    ret._pvt_deselect( CGAtomIdx(i,idx) );
                }
            }
        }
    }
    
    return ret;
}

/** Return a selection where only the atoms in 'selection'
    are selected
    
    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::selectOnly(const AtomSelection &selection) const
{
    info().assertEqualTo(selection.info());
    
    AtomSelection ret(*this);
    ret.selected_atoms = selection.selected_atoms;
    ret.nselected = selection.nselected;
    
    return ret;
}

/** Return the inverse of this selection */
AtomSelection AtomSelection::invert() const
{
    if (this->selectedAll())
        return this->selectNone();
    else if (this->selectedNone())
        return this->selectAll();
        
    AtomSelection ret(*this);
    
    for (CGIdx i(0); i<info().nCutGroups(); ++i)
    {
        if (this->selectedAll(i))
            ret._pvt_deselect(i);
        else if (this->selectedNone(i))
            ret._pvt_select(i);
        else
        {
            ret._pvt_deselect(i);
            
            QSet<Index> atoms = this->selectedAtoms(i);
            int nats = info().nAtoms(i);
            
            for (Index j(0); j<nats; ++j)
            {
                if (not atoms.contains(j))
                    ret._pvt_select( CGAtomIdx(i,j) );
            }
        }
    }
    
    return ret;
}

/** Return whether or not this contains the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(AtomIdx atomidx) const
{
    return this->selected(atomidx);
}

/** Return whether or not the CutGroup at index 'cgidx' contains
    some atoms that have been selected 
    
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(CGIdx cgidx) const
{
    return this->selected(cgidx);
}

/** Return whether or not the residue at index 'residx' contains
    some atoms that have been selected 
    
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(ResIdx residx) const
{
    return this->selected(residx);
}

/** Return whether or not the chain at index 'chainidx' contains
    some atoms that have been selected 
    
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(ChainIdx chainidx) const
{
    return this->selected(chainidx);
}

/** Return whether or not the segment at index 'segidx' contains
    some atoms that have been selected 
    
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(SegIdx segidx) const
{
    return this->selected(segidx);
}

/** Return whether or not any of the atoms identified 
    by 'atomid' have been selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(const AtomID &atomid) const
{
    return this->selected(atomid);
}

/** Return whether or not any of the atoms in the CutGroup(s) identified 
    by 'cgid' have been selected
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(const CGID &cgid) const
{
    return this->selected(cgid);
}

/** Return whether or not any of the atoms in the residue(s) identified 
    by 'resid' have been selected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(const ResID &resid) const
{
    return this->selected(resid);
}

/** Return whether or not any of the atoms in the chain(s) identified 
    by 'chainid' have been selected
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(const ChainID &chainid) const
{
    return this->selected(chainid);
}

/** Return whether or not any of the atoms in the segment(s) identified 
    by 'segid' have been selected
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
bool AtomSelection::intersects(const SegID &segid) const
{
    return this->selected(segid);
}

/** Return whether any of the atoms selected in 'selection'
    are also selected in this set
    
    \throw SireError::incompatible_error
*/
bool AtomSelection::intersects(const AtomSelection &selection) const
{
    return this->selected(selection);
}

/** Return whether the atom at index 'atomidx' is selected

    \throw SireError::invalid_index
*/
bool AtomSelection::contains(AtomIdx atomidx) const
{
    return this->selected(atomidx);
}

/** Return whether all of the atoms in the CutGroup at 
    index 'cgidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(CGIdx cgidx) const
{
    return this->selectedAll(cgidx);
}

/** Return whether all of the atoms in the residue at 
    index 'residx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(ResIdx residx) const
{
    return this->selectedAll(residx);
}

/** Return whether all of the atoms in the chain at 
    index 'chainidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(ChainIdx chainidx) const
{
    return this->selectedAll(chainidx);
}

/** Return whether all of the atoms in the segments at 
    index 'segidx' have been selected
    
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(SegIdx segidx) const
{
    return this->selectedAll(segidx);
}

/** Return whether all of the atoms identified by
    'atomid' have been selected
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(const AtomID &atomid) const
{
    return this->selectedAll(atomid);
}

/** Return whether all of the atoms in the CutGroup(s) identified by
    'cgid' have been selected
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(const CGID &cgid) const
{
    return this->selectedAll(cgid);
}

/** Return whether all of the atoms in the residue(s) identified by
    'resid' have been selected
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(const ResID &resid) const
{
    return this->selectedAll(resid);
}

/** Return whether all of the atoms in the chain(s) identified by
    'chainid' have been selected
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(const ChainID &chainid) const
{
    return this->selectedAll(chainid);
}

/** Return whether all of the atoms in the segment(s) identified by
    'segid' have been selected
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
bool AtomSelection::contains(const SegID &segid) const
{
    return this->selectedAll(segid);
}

/** Return whether all of the atoms selected in 'selection' are
    selected in this set
    
    \throw SireError::incompatible_error
*/
bool AtomSelection::contains(const AtomSelection &selection) const
{
    return this->selectedAll(selection);
}

/** Return the intersection of this set with 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::intersect(const AtomSelection &selection) const
{
    info().assertEqualTo(selection.info());

    if (this->selectedNone() or selection.selectedNone())
    {
        return this->selectNone();
    }
    else if (this->selectedAll() and selection.selectedAll())
    {
        return *this;
    }
    else
    {
        AtomSelection ret(*this);
        
        if (this->selectedAllCutGroups() and selection.selectedAllCutGroups())
        {
            int ncg = info().nCutGroups();
            
            for (CGIdx i(0); i<ncg; ++i)
            {
                bool this_all = this->selectedAll(i);
                bool other_all = selection.selectedAll(i);
            
                if (this_all and other_all)
                {
                    ret._pvt_select(i);
                }
                else if (this_all)
                {
                    ret._pvt_select(i, selection.selectedAtoms(i));
                }
                else if (other_all)
                {
                    ret._pvt_select(i, this->selectedAtoms(i));
                }
                else
                {
                    QSet<Index> atoms = this->selectedAtoms(i);
                    atoms.intersect(selection.selectedAtoms(i));
                    ret._pvt_select(i, atoms);
                } 
            }
        }
        else
        {
            QSet<CGIdx> cgidxs = this->selectedCutGroups().toSet();
            cgidxs.intersect( selection.selectedCutGroups().toSet() );
            
            foreach (CGIdx i, cgidxs)
            {
                bool this_all = this->selectedAll(i);
                bool other_all = selection.selectedAll(i);
            
                if (this_all and other_all)
                {
                    ret._pvt_select(i);
                }
                else if (this_all)
                {
                    ret._pvt_select(i, selection.selectedAtoms(i));
                }
                else if (other_all)
                {
                    ret._pvt_select(i, this->selectedAtoms(i));
                }
                else
                {
                    QSet<Index> atoms = this->selectedAtoms(i);
                    atoms.intersect(selection.selectedAtoms(i));
                    ret._pvt_select(i, atoms);
                } 
            }
        }
        
        return ret;
    }
}

/** Return the intersection of this set with the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(AtomIdx atomidx) const
{
    return this->intersect( this->selectOnly(atomidx) );
}

/** Return the intersection of this set with the CutGroup at index 'cgidx'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(CGIdx cgidx) const
{
    return this->intersect( this->selectOnly(cgidx) );
}

/** Return the intersection of this set with the residue at index 'residx'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(ResIdx residx) const
{
    return this->intersect( this->selectOnly(residx) );
}

/** Return the intersection of this set with the chain at index 'chainidx'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(ChainIdx chainidx) const
{
    return this->intersect( this->selectOnly(chainidx) );
}

/** Return the intersection of this set with the segment at index 'segidx'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(SegIdx segidx) const
{
    return this->intersect( this->selectOnly(segidx) );
}

/** Return the intersection of this set with the atoms whose indicies
    are in 'atomidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(const QList<AtomIdx> &atomidxs) const
{
    return this->intersect( this->selectOnly(atomidxs) );
}

/** Return the intersection of this set with the atoms in the 
    CutGroups whose indicies are in 'cgidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(const QList<CGIdx> &cgidxs) const
{
    return this->intersect( this->selectOnly(cgidxs) );
}

/** Return the intersection of this set with the atoms in the 
    residues whose indicies are in 'residxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(const QList<ResIdx> &residxs) const
{
    return this->intersect( this->selectOnly(residxs) );
}

/** Return the intersection of this set with the atoms in the 
    chains whose indicies are in 'chainidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(const QList<ChainIdx> &chainidxs) const
{
    return this->intersect( this->selectOnly(chainidxs) );
}

/** Return the intersection of this set with the atoms in the 
    segments whose indicies are in 'segidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(const QList<SegIdx> &segidxs) const
{
    return this->intersect( this->selectOnly(segidxs) );
}

/** Return the intersection of this set with the atoms that match
    the ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::intersect(const AtomID &atomid) const
{
    return this->intersect( this->selectOnly(atomid) );
}

/** Return the intersection of this set with the atoms in the 
    CutGroup(s) that match the ID 'cgid'
    
    \throw SireMol::missing_cutgroup
*/
AtomSelection AtomSelection::intersect(const CGID &cgid) const
{
    return this->intersect( this->selectOnly(cgid) );
}

/** Return the intersection of this set with the atoms in the 
    residue(s) that match the ID 'resid'
    
    \throw SireMol::missing_residue
*/
AtomSelection AtomSelection::intersect(const ResID &resid) const
{
    return this->intersect( this->selectOnly(resid) );
}

/** Return the intersection of this set with the atoms in the 
    chain(s) that match the ID 'chainid'
    
    \throw SireMol::missing_chain
*/
AtomSelection AtomSelection::intersect(const ChainID &chainid) const
{
    return this->intersect( this->selectOnly(chainid) );
}

/** Return the intersection of this set with the atoms in the 
    segment(s) that match the ID 'segid'
    
    \throw SireMol::missing_segment
*/
AtomSelection AtomSelection::intersect(const SegID &segid) const
{
    return this->intersect( this->selectOnly(segid) );
}

/** Return the union of this set with the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(AtomIdx atomidx) const
{
    return this->select(atomidx);
}

/** Return the union of this set with the atoms in the   
    CutGroup at index 'cgidx'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(CGIdx cgidx) const
{
    return this->select(cgidx);
}

/** Return the union of this set with the atoms in the   
    residue at index 'residx'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(ResIdx residx) const
{
    return this->select(residx);
}

/** Return the union of this set with the atoms in the   
    chain at index 'chainidx'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(ChainIdx chainidx) const
{
    return this->select(chainidx);
}

/** Return the union of this set with the atoms in the   
    segment at index 'segidx'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(SegIdx segidx) const
{
    return this->select(segidx);
}

/** Return the union of this set with the atoms whose   
    indicies are in 'atomidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(const QList<AtomIdx> &atomidxs) const
{
    return this->select(atomidxs);
}

/** Return the union of this set with the atoms in the   
    CutGroups whose indicies are in 'cgidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(const QList<CGIdx> &cgidxs) const
{
    return this->select(cgidxs);
}

/** Return the union of this set with the atoms in the   
    residues whose indicies are in 'residxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(const QList<ResIdx> &residxs) const
{
    return this->select(residxs);
}

/** Return the union of this set with the atoms in the   
    chains whose indicies are in 'chainidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(const QList<ChainIdx> &chainidxs) const
{
    return this->select(chainidxs);
}

/** Return the union of this set with the atoms in the   
    segments whose indicies are in 'segidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(const QList<SegIdx> &segidxs) const
{
    return this->select(segidxs);
}

/** Return the union of this set with the atoms identified by
    'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::unite(const AtomID &atomid) const
{
    return this->select(atomid);
}

/** Return the union of this set with the atoms in the 
    CutGroups identified by 'cgid'
    
    \throw SireMol::missing_cutgroup
*/
AtomSelection AtomSelection::unite(const CGID &cgid) const
{
    return this->select(cgid);
}

/** Return the union of this set with the atoms in the 
    residues identified by 'resid'
    
    \throw SireMol::missing_residue
*/
AtomSelection AtomSelection::unite(const ResID &resid) const
{
    return this->select(resid);
}

/** Return the union of this set with the atoms in the 
    chains identified by 'chainid'
    
    \throw SireMol::missing_chain
*/
AtomSelection AtomSelection::unite(const ChainID &chainid) const
{
    return this->select(chainid);
}

/** Return the union of this set with the atoms in the 
    segments identified by 'segid'
    
    \throw SireMol::missing_segment
*/
AtomSelection AtomSelection::unite(const SegID &segid) const
{
    return this->select(segid);
}

/** Return the union of this set with the atoms selected 
    in 'selection'
    
    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::unite(const AtomSelection &selection) const
{
    return this->select(selection);
}

/** Return the union of all of the passed selections

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::unite(const QList<AtomSelection> &selections)
{
    if (selections.isEmpty())
        return AtomSelection();
    else if (selections.count() == 1)
        return selections.at(0);
    else
    {
        QList<AtomSelection>::const_iterator it = selections.constBegin();
        
        AtomSelection ret = *it;
        
        for (++it; it != selections.constEnd(); ++it)
        {
            if (ret.selectedAll())
                break;
            
            ret._pvt_select(*it);
        }
        
        return ret; 
    }
}

/** Return the selection from which the atom at index 'atomidx' 
    has been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(AtomIdx atomidx) const
{
    return this->deselect(atomidx);
}

/** Return the selection from which the atoms in the CutGroup
    at index 'cgidx' have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(CGIdx cgidx) const
{
    return this->deselect(cgidx);
}

/** Return the selection from which the atoms in the residue
    at index 'residx' have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(ResIdx residx) const
{
    return this->deselect(residx);
}

/** Return the selection from which the atoms in the chain
    at index 'chainidx' have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(ChainIdx chainidx) const
{
    return this->deselect(chainidx);
}

/** Return the selection from which the atoms in the segment
    at index 'segidx' have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(SegIdx segidx) const
{
    return this->deselect(segidx);
}

/** Return the selection from which the atoms whose indicies
    are in 'atomidxs' have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const QList<AtomIdx> &atomidxs) const
{
    return this->deselect(atomidxs);
}

/** Return the selection from which all of the atoms that
    are in the CutGroups whose indicies are in 'cgidxs'
    have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const QList<CGIdx> &cgidxs) const
{
    return this->deselect(cgidxs);
}

/** Return the selection from which all of the atoms that
    are in the residues whose indicies are in 'residxs'
    have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const QList<ResIdx> &residxs) const
{
    return this->deselect(residxs);
}

/** Return the selection from which all of the atoms that
    are in the chains whose indicies are in 'chainidxs'
    have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const QList<ChainIdx> &chainidxs) const
{
    return this->deselect(chainidxs);
}

/** Return the selection from which all of the atoms that
    are in the segments whose indicies are in 'segidxs'
    have been subtracted
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const QList<SegIdx> &segidxs) const
{
    return this->deselect(segidxs);
}

/** Return the selection from which all of the atoms that 
    match the ID 'atomid' have been subtracted
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const AtomID &atomid) const
{
    return this->deselect(atomid);
}

/** Return the selection from which all of the atoms that
    are in the CutGroup(s) that match the ID 'cgid' have
    been subtracted
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const CGID &cgid) const
{
    return this->deselect(cgid);
}

/** Return the selection from which all of the atoms that
    are in the residue(s) that match the ID 'resid' have
    been subtracted
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const ResID &resid) const
{
    return this->deselect(resid);
}

/** Return the selection from which all of the atoms that
    are in the chain(s) that match the ID 'resid' have
    been subtracted
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const ChainID &chainid) const
{
    return this->deselect(chainid);
}

/** Return the selection from which all of the atoms that
    are in the segment(s) that match the ID 'segid' have
    been subtracted
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::subtract(const SegID &segid) const
{
    return this->deselect(segid);
}

/** Return the selection from which all of the atoms selected
    in 'selection' have been subtracted
    
    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::subtract(const AtomSelection &selection) const
{
    return this->deselect(selection);
}

/** Return the selection that has been masked by the 
    atom at index 'atomidx'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(AtomIdx atomidx) const
{
    return this->intersect(atomidx);
}

/** Return the selection that has been masked by the 
    CutGroup at index 'cgidx'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(CGIdx cgidx) const
{
    return this->intersect(cgidx);
}

/** Return the selection that has been masked by the   
    residue at index 'residx' 
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(ResIdx residx) const
{
    return this->intersect(residx);
}

/** Return the selection that has been masked by the   
    chain at index 'chainidx' 
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(ChainIdx chainidx) const
{
    return this->intersect(chainidx);
}

/** Return the selection that has been masked by the   
    segment at index 'segidx' 
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(SegIdx segidx) const
{
    return this->intersect(segidx);
}

/** Return the selection that has been masked by the atoms whose
    indicies are in 'atomidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const QList<AtomIdx> &atomidxs) const
{
    return this->intersect(atomidxs);
}

/** Return the selection that has been masked by the
    CutGroups whose indicies are in 'cgidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const QList<CGIdx> &cgidxs) const
{
    return this->intersect(cgidxs);
}

/** Return the selection that has been masked by the
    residues whose indicies are in 'residxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const QList<ResIdx> &residxs) const
{
    return this->intersect(residxs);
}

/** Return the selection that has been masked by the
    chains whose indicies are in 'chainidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const QList<ChainIdx> &chainidxs) const
{
    return this->intersect(chainidxs);
}

/** Return the selection that has been masked by the
    segments whose indicies are in 'segidxs'
    
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const QList<SegIdx> &segidxs) const
{
    return this->intersect(segidxs);
}

/** Return the selection that has been masked by the 
    atoms that match the ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const AtomID &atomid) const
{
    return this->intersect(atomid);
}

/** Return the selection that has been masked by the 
    CutGroups that match the ID 'cgid'
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const CGID &cgid) const
{
    return this->intersect(cgid);
}

/** Return the selection that has been masked by the 
    residues that match the ID 'atomid'
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const ResID &resid) const
{
    return this->intersect(resid);
}

/** Return the selection that has been masked by the 
    chains that match the ID 'chainid'
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const ChainID &chainid) const
{
    return this->intersect(chainid);
}

/** Return the selection that has been masked by the 
    segments that match the ID 'atomid'
    
    \throw SireMol::missing_segments
    \throw SireError::invalid_index
*/
AtomSelection AtomSelection::mask(const SegID &segid) const
{
    return this->intersect(segid);
}

/** Return the selection that has been masked by 'selection'

    \throw SireError::incompatible_error
*/
AtomSelection AtomSelection::mask(const AtomSelection &selection) const
{
    return this->intersect(selection);
}

/** Return the list of indicies of all of the atoms that
    have been selected */
QVector<AtomIdx> AtomSelection::selectedAtoms() const
{
    if (nselected == 0)
        return QVector<AtomIdx>();

    QVector<AtomIdx> ret(nselected);
    
    AtomIdx *ret_array = ret.data();
    
    if (this->selectedAll())
    {
        for (AtomIdx i(0); i<info().nAtoms(); ++i)
        {
            ret_array[i] = i;
        }
    }
    else if (this->selectedAllCutGroups())
    {
        int count = 0;
        
        for (CGIdx i(0); i<info().nCutGroups(); ++i)
        {
            if (this->selectedAll(i))
            {
                for (Index j(0); j<info().nAtoms(i); ++i)
                {
                    ret_array[count] = info().atomIdx(CGAtomIdx(i,j));
                    ++count;
                }
            }
            else
            {
                foreach (Index j, this->selectedAtoms(i))
                {
                    ret_array[count] = info().atomIdx(CGAtomIdx(i,j));
                    ++count;
                }
            }
        }
    }
    else
    {
        int count = 0;
    
        foreach (CGIdx i, this->selectedCutGroups())
        {
            if (this->selectedAll(i))
            {
                for (Index j(0); j<info().nAtoms(i); ++i)
                {
                    ret_array[count] = info().atomIdx(CGAtomIdx(i,j));
                    ++count;
                }
            }
            else
            {
                foreach (Index j, this->selectedAtoms(i))
                {
                    ret_array[count] = info().atomIdx(CGAtomIdx(i,j));
                    ++count;
                }
            }
        }
    }
    
    qSort(ret);
    
    return ret;
}

/** Assert that this selection contains the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
void AtomSelection::assertSelected(AtomIdx atomidx) const
{
    if (not this->contains(atomidx))
        throw SireError::invalid_index( QObject::tr(
            "This selection does not contain the atom at index %1.")
                .arg(atomidx), CODELOC );
}

/** Assert that this selection contains all of the atoms identified 
    by the ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
void AtomSelection::assertSelected(const AtomID &atomid) const
{
    if (not this->contains(atomid))
        throw SireMol::missing_atom( QObject::tr(
            "This selection does not contain all of the atoms "
            "identified by the ID %1.")
                .arg(atomid.toString()), CODELOC );
}

/** Assert that this selection is compatible with the molecule info
    in 'molinfo'
    
    \throw SireError::incompatible_error
*/
void AtomSelection::assertCompatibleWith(const MoleculeInfoData &molinfo) const
{
    if (*d != molinfo)
        throw SireError::incompatible_error( QObject::tr(
            "The layout \"%1\" is incompatible with this selection, "
            "which is for the layout \"%2\".")
                .arg(molinfo.UID()).arg(d->UID()), CODELOC );
}

/** Assert that this selection is compatible with the molecule whose
    data is in 'moldata'
    
    \throw SireError::incompatible_error
*/
void AtomSelection::assertCompatibleWith(const MoleculeData &moldata) const
{
    if (*d != moldata.info())
        throw SireError::incompatible_error( QObject::tr(
            "The molecule \"%1\" (layout %2) is incompatible with this selection, "
            "which is for the layout \"%3\".")
                .arg(moldata.name()).arg(moldata.info().UID())
                .arg(d->UID()), CODELOC );
}

/** Return whether or not this selection is compatible with the molecule info
    in 'molinfo' */
bool AtomSelection::isCompatibleWith(const MoleculeInfoData &molinfo) const
{
    return molinfo == *d;
}

/** Assert that this selection is compatible with the molecule viewed
    in 'molview'
    
    \throw SireError::incompatible_error
*/
void AtomSelection::assertCompatibleWith(const MoleculeView &molview) const
{
    this->assertCompatibleWith(molview.data());
}

/** Assert that this selection is compatible with 'other'

    \throw SireError::incompatible_error
*/
void AtomSelection::assertCompatibleWith(const AtomSelection &other) const
{
    if (*d != other.info())
        throw SireError::incompatible_error( QObject::tr(
            "The selection for layout \"%1\" is incompatible with this selection, "
            "which is for the layout \"%2\".")
                .arg(other.info().UID())
                .arg(d->UID()), CODELOC );
}
