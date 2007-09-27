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

#include "atomsinmol.h"

using namespace SireMol;

QList<AtomIdx> AtomsInMol::getIndicies() const
{
    if (selected_atoms.selectedAll())
        return d->info().getAtoms();
    else
    {
        QSet<CGIdx> cgidxs = selected_atoms.selectedCutGroups();
        
        QList<AtomIdx> indicies;
        
        foreach (CGIdx cgidx, cgidxs)
        {
            if (selected_atoms.selectedAll(cgidx))
                indicies.append( d->info().getAtoms(cgidx) );
            else
            {
                QSet<CGAtomIdx> atms = selected_atoms.selectedAtoms(cgidx);
                
                for (QSet<CGAtomIdx>::const_iterator it = atms.constBegin();
                     it != atms.constEnd();
                     ++it)
                {
                    indicies.append( d->info().atomIdx(*it) );
                }
            }
        }
        
        qSort(indicies);
        return indicies;
    }
}

AtomSelection AtomsInMol::getSelectedAtoms() const
{
    AtomSelection selection(d->info());
    selection.select(atomidxs);
    
    return selection;
}

/** Null constructor */
AtomsInMol::AtomsInMol() : MoleculeView()
{}

/** Construct a view of all of the atoms in 'view' */
AtomsInMol::AtomsInMol(const MoleculeView &view)
           : MoleculeView(view)
{
    selected_atoms = view.selectedAtoms();
    atomidxs = getIndices();
}

/** Construct a view of the atoms that match 'atomid' in the 
    passed molecule */
AtomsInMol::AtomsInMol(const MoleculeView &view, const AtomID &atomid)
           : MoleculeView(view)
{
    atomidxs = atomid.map(d->info());
    selected_atoms = getSelectedAtoms();
}

/** Construct a view of all of the atoms that are in the residues
    that match 'resid' */
AtomsInMol::AtomsInMol(const MoleculeView &view, const ResID &resid)
           : MoleculeView(view)
{
    atomidxs = d->info().getAtoms(resid);
    selected_atoms = getSelectedAtoms();
}

/** Construct a view of all of the atoms that are in the chains that
    match 'chainid' */
AtomsInMol::AtomsInMol(const MoleculeView &view, const ChainID &chainid)
           : MoleculeView(view)
{
    atomidxs = d->info().getAtoms(chainid);
    selected_atoms = getSelectedAtoms();
}

/** Construct a view of all of the atoms that are in the CutGroups
    that match 'cgid' */
AtomsInMol::AtomsInMol(const MoleculeView &view, const CGID &cgid)
           : MoleculeView(view)
{
    atomidxs = d->info().getAtoms(cgid);
    selected_atoms = getSelectedAtoms();
}

/** Construct a view of all of the atoms that are in the segments
    that match 'segid' */
AtomsInMol::AtomsInMol(const MoleculeView &view, const SegID &segid)
           : MoleculeView(view)
{
    atomidxs = d->info().getAtoms(segid);
    selected_atoms = getSelectedAtoms();
}

/** Copy constructor */
AtomsInMol::AtomsInMol(const AtomsInMol &other)
           : MoleculeView(other), atomidxs(other.atomidxs),
             selected_atoms(other.selected_atoms)
{}

/** Destructor */
AtomsInMol::~AtomsInMol()
{}

/** Copy assignment operator */
AtomsInMol& AtomsInMol::operator=(const AtomsInMol &other)
{
    if (this != &other)
    {
        MoleculeView::operator=(other);
        atomidxs = other.atomidxs;
        selected_atoms = other.selected_atoms;
    }
    
    return *this;
}

/** Copy assignment from a MoleculeView - this returns a view
    of all of the atoms from 'other' */
AtomsInMol& AtomsInMol::operator=(const MoleculeView &other)
{
    if (other.isA<AtomsInMol>())
        return this->operator=(other.asA<AtomsInMol>());
    
    MoleculeView::operator=(other);
    selected_atoms = other.selectedAtoms();
    atomidxs = getIndicies();
}

/** Return the selection of atoms that are in this view */
AtomSelection AtomsInMol::selectedAtoms() const
{
    return selected_atoms;
}

/** Update this view to a new version of the molecule */
void AtomsInMol::update(const MoleculeView &other)
{
    other.assertSameMolecule(*this);
    d = other.d;
}

AtomSelection AtomsInMol::_pvt_getSelection(int strt, int end) const
{
    strt = Index(strt).map(atomidxs.count());
    end = Index(end).map(atomidxs.count());
    
    if (strt > end)
        qSwap(strt,end);
    
    if (end - strt == atomidxs.count())
        return selected_atoms;
                
    AtomSelection selection(d->info());
    
    for (int i=strt; i<=end; ++i)
    {
        selection.select( atomidxs[i] );
    }
    
    return selection;
}

/** Return the ith atom in this view */
Atom AtomsInMol::operator[](int i) const
{
    return Atom( *this, _pvt_getSelection(i,i) );
}

/** Return the ith atom in this view */
Atom AtomsInMol::operator()(int i) const
{
    return this->operator[](i);
}

/** Return the range of atoms in this view from i to j */
AtomsInMol AtomsInMol::operator()(int i, int j) const
{
    return AtomsInMol(*this, _pvt_getSelection(i,j));
}

/** Return the ith atom in this view */
Atom AtomsInMol::at(int i) const
{
    return this->operator[](i);
}

/** Return the range of atoms in this view from i to j */
AtomsInMol AtomsInMol::at(int i, int j) const
{
    return this->operator()(i,j);
}

/** Return a Mover that moves all of the atoms in this view */
Mover<AtomsInMol> AtomsInMol::move() const
{
    return Mover<AtomsInMol>(*this, selected_atoms);
}

/** Return a Mover that moves only the ith atom in this view */
Mover<AtomsInMol> AtomsInMol::move(int i) const
{
    return Mover<AtomsInMol>(*this, _pvt_getSelection(i,i));
}

/** Return a Mover that moves the range of atoms from i to j 
    from this view
*/
Mover<AtomsInMol> AtomsInMol::move(int i, int j) const
{
    return Mover<AtomsInMol>(*this, _pvt_getSelection(i,j));
}

/** Return an Evaluator that uses all of the atoms in this view */
Evaluator<AtomsInMol> AtomsInMol::evaluate() const
{
    return Evaluator<AtomsInMol>(*this, selected_atoms);
}

/** Return an Evaluator that evaluates using only the ith atom
    in this view */
Evaluator<AtomsInMol> AtomsInMol::evaluate(int i) const
{
    return Evaluator<AtomsInMol>(*this, _pvt_getSelection(i,i));
}

/** Return an Evaluator that evaluates using the range of atoms
    from i to j from this view */
Evaluator<AtomsInMol> AtomsInMol::evaluate(int i, int j) const
{
    return Evaluator<AtomsInMol>(*this, _pvt_getSelection(i,j));
}

/** Return an Editor that can edit all of the atoms in this view */
Editor<AtomsInMol> AtomsInMol::edit() const
{
    return Editor<AtomsInMol>(*this, selected_atoms);
}

/** Return an Editor that can edit only the ith atom from this view */
Editor<AtomsInMol> AtomsInMol::edit(int i) const
{
    return Editor<AtomsInMol>(*this, _pvt_getSelection(i,i));
}

/** Return an Editor that can edit the atoms from i to j form this view */
Editor<AtomsInMol> AtomsInMol::edit(int i, int j) const
{
    return Editor<AtomsInMol>(*this, _pvt_getSelection(i,j));
}

/** Return a selector that can play with the selection of atoms
    from this view */
Selector<AtomsInMol> AtomsInMol::selection() const
{
    return Selector<AtomsInMol>(*this, selected_atoms);
}

QList<AtomIdx> AtomsInMol::_pvt_select(const AtomID &atomid) const
{
    QList<AtomIdx> combined = MoleculeInfoData::combine(
                                    atomidxs, atomid.map(d->info()) );
                                    
    if (combined.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "None of the atoms in this view of atoms with indicies %1 "
            "in the molecule \"%2\" match %3.")
                .arg(Sire::toString(atomidxs), d->info().name(),
                     atomid.toString()), CODELOC );

    return combined;
}

/** Select the atom that is in this view *and* matches the ID 'atomid' */
Atom AtomsInMol::select(const AtomID &atomid) const
{
    QList<AtomIdx> combined = this->_pvt_select(atomid);
     
    if (combined.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "There is more than one atom in this view of atoms with "
            "indicies %1 in the molecule \"%2\" that match %3.")
                .arg(Sire::toString(atomidxs), d->info().name(),
                     atomid.toString()), CODELOC );
                     
    return Atom(*this, combined.first());
}

/** Select all of the atoms are in this view *and* match the ID 'atomid' */
AtomsInMol AtomsInMol::selectAll(const AtomID &atomid) const
{
    return AtomsInMol(*this, this->_pvt_select(atomid));
}

/** Return the atom that is in this view *and* matches the ID 'atomid */
Atom AtomsInMol::atom(const AtomID &atomid) const
{
    return this->select(atomid);
}

/** Return the atoms that are in this view *and* match the ID 'atomid' */
AtomsInMol AtomsInMol::atoms(const AtomID &atomid) const
{
    return this->selectAll(atomid);
}

/** Return the property associated with the key 'key'. If this is a 
    molecular property then it will be masked to only return the 
    parts relevant for the atoms in this view
    
    \throw SireBase::missing_property
*/
const Property& AtomsInMol::property(const QString &key) const
{
    Property prop = d->property(key);
    
    if (prop.isA<MolecularProperty>())
        return prop.mask(selected_atoms);
    else
        return prop;
}

/** Return the metadata associated with the property at key 'key'

    \throw SireBase::missing_property
*/
const Properties& AtomsInMol::metadata(const QString &key) const
{
    return d->metadata(key);
}
