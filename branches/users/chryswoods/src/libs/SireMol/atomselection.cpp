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

using namespace SireMol;

/** Null constructor */
AtomSelection::AtomSelection() : nselected(0)
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
{
    this->operator=(molecule.selectedAtoms());
}

/** Construct a selection of all of the atoms in the 
    molecule whose data is in 'moldata' */
AtomSelection::AtomSelection(const MoleculeData &moldata)
              : d(moldata.info()), nselected(moldata.info().nAtoms())
{}              

/** Copy constructor */
AtomSelection::AtomSelection(const AtomSelection &other)
              : selected_atoms(other.selected_atoms),
                d(other.d), nselected(other.nselected)
{}

/** Destructor */
AtomSelection::~AtomSelection()
{}

/** Copy assignment operator */
AtomSelection& AtomSelection::operator=(const AtomSelection &other)
{
    selected_atoms = other.selected_atoms;
    d = other.d;
    nselected = other.nselected;
    
    return *this;
}

/** Comparison operator */
bool AtomSelection::operator==(const AtomSelection &other) const
{
    return nselected == other.nselected and 
           (d == other.d or *d == *(other.d)) and
           selected_atoms == other.selected_atoms;
}

/** Comparison operator */
bool AtomSelection::operator!=(const AtomSelection &other) const
{
    return nselected != other.nselected or
           (d != other.d and *d != *(other.d)) or
           selected_atoms != other.selected_atoms;
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
    this->info().assertSameFingerprint( selection.info() );
    
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
        QSet<CGIdx> cgidxs = selection.selectedCutGroups();
        
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

/** Return whether or not all of the atoms selected in 'selection' 
    have also been selected in this object
    
    \throw SireError::incompatible_error
*/
bool AtomSelection::selectedAll(const AtomSelection &selection) const
{
    this->info().assertSameFingerprint( selection.info() );
    
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
        QSet<CGIdx> cgidxs = selection.selectedCutGroups();
        
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
    info().assertSameFingerprint(selection.info());
    
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
/*
AtomSelection AtomSelection::selectAll() const;
AtomSelection AtomSelection::deselectAll() const;
AtomSelection AtomSelection::selectNone() const;

AtomSelection AtomSelection::select(AtomIdx atomidx) const;
AtomSelection AtomSelection::deselect(AtomIdx atomidx) const;
AtomSelection AtomSelection::selectOnly(AtomIdx atomidx) const;

AtomSelection AtomSelection::select(CGIdx cgidx) const;
AtomSelection AtomSelection::deselect(CGIdx cgidx) const;
AtomSelection AtomSelection::selectOnly(CGIdx cgidx) const;

AtomSelection AtomSelection::select(ResIdx residx) const;
AtomSelection AtomSelection::deselect(ResIdx residx) const;
AtomSelection AtomSelection::selectOnly(ResIdx residx) const;

AtomSelection AtomSelection::select(ChainIdx chainidx) const;
AtomSelection AtomSelection::deselect(ChainIdx chainidx) const;
AtomSelection AtomSelection::selectOnly(ChainIdx chainidx) const;

AtomSelection AtomSelection::select(SegIdx segidx) const;
AtomSelection AtomSelection::deselect(SegIdx segidx) const;
AtomSelection AtomSelection::selectOnly(SegIdx segidx) const;

AtomSelection AtomSelection::select(const AtomID &atomid) const;
AtomSelection AtomSelection::deselect(const AtomID &atomid) const;
AtomSelection AtomSelection::selectOnly(const AtomID &atomid) const;

AtomSelection AtomSelection::select(const CGID &cgid) const;
AtomSelection AtomSelection::deselect(const CGID &cgid) const;
AtomSelection AtomSelection::selectOnly(const CGID &cgid) const;

AtomSelection AtomSelection::select(const ResID &resid) const;
AtomSelection AtomSelection::deselect(const ResID &resid) const;
AtomSelection AtomSelection::selectOnly(const ResID &resid) const;

AtomSelection AtomSelection::select(const ChainID &chainid) const;
AtomSelection AtomSelection::deselect(const ChainID &chainid) const;
AtomSelection AtomSelection::selectOnly(const ChainID &chainid) const;

AtomSelection AtomSelection::select(const SegID &segid) const;
AtomSelection AtomSelection::deselect(const SegID &segid) const;
AtomSelection AtomSelection::selectOnly(const SegID &segid) const;

AtomSelection AtomSelection::select(const AtomSelection &selection) const;
AtomSelection AtomSelection::deselect(const AtomSelection &selection) const;
AtomSelection AtomSelection::selectOnly(const AtomSelection &selection) const;

AtomSelection AtomSelection::invert() const;

bool AtomSelection::intersects(AtomIdx atomidx) const;
bool AtomSelection::intersects(CGIdx cgidx) const;
bool AtomSelection::intersects(ResIdx residx) const;
bool AtomSelection::intersects(ChainIdx chainidx) const;
bool AtomSelection::intersects(SegIdx segidx) const;

bool AtomSelection::intersects(const AtomID &atomid) const;
bool AtomSelection::intersects(const CGID &cgid) const;
bool AtomSelection::intersects(const ResID &resid) const;
bool AtomSelection::intersects(const ChainID &chainid) const;
bool AtomSelection::intersects(const SegID &segid) const;

bool AtomSelection::intersects(const AtomSelection &selection) const;

bool AtomSelection::contains(AtomIdx atomidx) const;
bool AtomSelection::contains(CGIdx cgidx) const;
bool AtomSelection::contains(ResIdx residx) const;
bool AtomSelection::contains(ChainIdx chainidx) const;
bool AtomSelection::contains(SegIdx segidx) const;

bool AtomSelection::contains(const AtomID &atomid) const;
bool AtomSelection::contains(const CGID &cgid) const;
bool AtomSelection::contains(const ResID &resid) const;
bool AtomSelection::contains(const ChainID &chainid) const;
bool AtomSelection::contains(const SegID &segid) const;

bool AtomSelection::contains(const AtomSelection &selection) const;

AtomSelection AtomSelection::intersect(AtomIdx atomidx) const;
AtomSelection AtomSelection::intersect(CGIdx cgidx) const;
AtomSelection AtomSelection::intersect(ResIdx residx) const;
AtomSelection AtomSelection::intersect(ChainIdx chainidx) const;
AtomSelection AtomSelection::intersect(SegIdx segidx) const;

AtomSelection AtomSelection::intersect(const QSet<AtomIdx> &atomidx) const;
AtomSelection AtomSelection::intersect(const QSet<CGIdx> &cgidx) const;
AtomSelection AtomSelection::intersect(const QSet<ResIdx> &residx) const;
AtomSelection AtomSelection::intersect(const QSet<ChainIdx> &chainidx) const;
AtomSelection AtomSelection::intersect(const QSet<SegIdx> &segidx) const;

AtomSelection AtomSelection::intersect(const AtomID &atomid) const;
AtomSelection AtomSelection::intersect(const CGID &cgid) const;
AtomSelection AtomSelection::intersect(const ResID &resid) const;
AtomSelection AtomSelection::intersect(const ChainID &chainid) const;
AtomSelection AtomSelection::intersect(const SegID &segid) const;

AtomSelection AtomSelection::intersect(const AtomSelection &selection) const;

AtomSelection AtomSelection::unite(AtomIdx atomidx) const;
AtomSelection AtomSelection::unite(CGIdx cgidx) const;
AtomSelection AtomSelection::unite(ResIdx residx) const;
AtomSelection AtomSelection::unite(ChainIdx chainidx) const;
AtomSelection AtomSelection::unite(SegIdx segidx) const;

AtomSelection AtomSelection::unite(const QSet<AtomIdx> &atomidx) const;
AtomSelection AtomSelection::unite(const QSet<CGIdx> &cgidx) const;
AtomSelection AtomSelection::unite(const QSet<ResIdx> &residx) const;
AtomSelection AtomSelection::unite(const QSet<ChainIdx> &chainidx) const;
AtomSelection AtomSelection::unite(const QSet<SegIdx> &segidx) const;

AtomSelection AtomSelection::unite(const AtomID &atomid) const;
AtomSelection AtomSelection::unite(const CGID &cgid) const;
AtomSelection AtomSelection::unite(const ResID &resid) const;
AtomSelection AtomSelection::unite(const ChainID &chainid) const;
AtomSelection AtomSelection::unite(const SegID &segid) const;

AtomSelection AtomSelection::unite(const AtomSelection &selection) const;

AtomSelection AtomSelection::subtract(AtomIdx atomidx) const;
AtomSelection AtomSelection::subtract(CGIdx cgidx) const;
AtomSelection AtomSelection::subtract(ResIdx residx) const;
AtomSelection AtomSelection::subtract(ChainIdx chainidx) const;
AtomSelection AtomSelection::subtract(SegIdx segidx) const;

AtomSelection AtomSelection::subtract(const QSet<AtomIdx> &atomidx) const;
AtomSelection AtomSelection::subtract(const QSet<CGIdx> &cgidx) const;
AtomSelection AtomSelection::subtract(const QSet<ResIdx> &residx) const;
AtomSelection AtomSelection::subtract(const QSet<ChainIdx> &chainidx) const;
AtomSelection AtomSelection::subtract(const QSet<SegIdx> &segidx) const;

AtomSelection AtomSelection::subtract(const AtomID &atomid) const;
AtomSelection AtomSelection::subtract(const CGID &cgid) const;
AtomSelection AtomSelection::subtract(const ResID &resid) const;
AtomSelection AtomSelection::subtract(const ChainID &chainid) const;
AtomSelection AtomSelection::subtract(const SegID &segid) const;

AtomSelection AtomSelection::subtract(const AtomSelection &selection) const;

AtomSelection AtomSelection::mask(AtomIdx atomidx) const;
AtomSelection AtomSelection::mask(CGIdx cgidx) const;
AtomSelection AtomSelection::mask(ResIdx residx) const;
AtomSelection AtomSelection::mask(ChainIdx chainidx) const;
AtomSelection AtomSelection::mask(SegIdx segidx) const;

AtomSelection AtomSelection::mask(const QSet<AtomIdx> &atomidx) const;
AtomSelection AtomSelection::mask(const QSet<CGIdx> &cgidx) const;
AtomSelection AtomSelection::mask(const QSet<ResIdx> &residx) const;
AtomSelection AtomSelection::mask(const QSet<ChainIdx> &chainidx) const;
AtomSelection AtomSelection::mask(const QSet<SegIdx> &segidx) const;

AtomSelection AtomSelection::mask(const AtomID &atomid) const;
AtomSelection AtomSelection::mask(const CGID &cgid) const;
AtomSelection AtomSelection::mask(const ResID &resid) const;
AtomSelection AtomSelection::mask(const ChainID &chainid) const;
AtomSelection AtomSelection::mask(const SegID &segid) const;

AtomSelection AtomSelection::mask(const AtomSelection &selection) const;

QSet<AtomIdx> AtomSelection::selectedAtoms() const;

QSet<Index> AtomSelection::selectedAtoms(CGIdx cgid) const;

QSet<CGAtomIdx> AtomSelection::selectedAtoms(ResIdx residx) const;
QSet<CGAtomIdx> AtomSelection::selectedAtoms(ChainIdx chainidx) const;
QSet<CGAtomIdx> AtomSelection::selectedAtoms(SegIdx segidx) const;

QSet<CGIdx> AtomSelection::selectedCutGroups() const;
QSet<ResIdx> AtomSelection::selectedResidues() const;
QSet<ChainIdx> AtomSelection::selectedChains() const;
QSet<SegIdx> AtomSelection::selectedSegments() const;

void AtomSelection::assertCompatibleWith(const MoleculeData &moldata) const;
void AtomSelection::assertCompatibleWith(const MoleculeView &molview) const;
void AtomSelection::assertCompatibleWith(const AtomSelection &other) const;
*/

