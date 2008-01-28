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

#include <QDataStream>
#include <boost/assert.hpp>

#include "atomselection.h"
#include "connectivity.h"
#include "moleculedata.h"
#include "moleculeinfodata.h"

#include "angleid.h"
#include "bondid.h"
#include "dihedralid.h"
#include "improperid.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireBase;

/////////
///////// Implementation of ConnectivityBase
/////////

static const RegisterMetaType<ConnectivityBase> r_conbase(MAGIC_ONLY,
                                                          "SireMol::ConnectivityBase");

/** Serialise ConnectivityBase */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const ConnectivityBase &conbase)
{
    writeHeader(ds, r_conbase, 1);

    SharedDataStream sds(ds);

    sds << conbase.connected_atoms << conbase.connected_res
        << conbase.d << static_cast<const PropertyBase&>(conbase);

    return ds;
}

/** Deserialise a MoleculeBonds */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, 
                                       ConnectivityBase &conbase)
{
    VersionID v = readHeader(ds, r_conbase);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> conbase.connected_atoms >> conbase.connected_res
            >> conbase.d
            >> static_cast<PropertyBase&>(conbase);
    }
    else
        throw version_error(v, "1", r_conbase, CODELOC);

    return ds;
}

/** Null constructor */
ConnectivityBase::ConnectivityBase()
                 : PropertyBase(), d(MoleculeInfoData::null())
{}

const MoleculeInfoData& ConnectivityBase::info() const
{
    return *d;
}

/** Construct the connectivity for molecule described by 
    the passed info object */
ConnectivityBase::ConnectivityBase(const MoleculeData &moldata)
                 : PropertyBase(), d(moldata.info())
{
    if (info().nAtoms() > 0)
    {
        connected_atoms.resize(info().nAtoms());
        connected_atoms.squeeze();
    }
    
    if (info().nResidues() > 0)
    {
        connected_res.resize(info().nResidues());
        connected_res.squeeze();
    }
}
    
/** Copy constructor */
ConnectivityBase::ConnectivityBase(const ConnectivityBase &other)
                 : PropertyBase(other),
                   connected_atoms(other.connected_atoms),
                   connected_res(other.connected_res),
                   d(other.d)
{}

/** Destructor */
ConnectivityBase::~ConnectivityBase()
{}

/** Copy assignment operator */
ConnectivityBase& ConnectivityBase::operator=(const ConnectivityBase &other)
{
    if (this != &other)
    {
        connected_atoms = other.connected_atoms;
        connected_res = other.connected_res;
        d = other.d;
    }
    
    return *this;
}

/** Comparison operator */
bool ConnectivityBase::operator==(const ConnectivityBase &other) const
{
    return (d == other.d or *d == *(other.d)) and 
           connected_atoms == other.connected_atoms;
}

/** Comparison operator */
bool ConnectivityBase::operator!=(const ConnectivityBase &other) const
{
    return (d != other.d and *d != *(other.d)) or
           connected_atoms != other.connected_atoms;
}

/** Return the total number of connections between atoms
    in this connectivity object */
int ConnectivityBase::nConnections() const
{
    int n = 0;
    
    const QSet<AtomIdx> *connected_atoms_array = connected_atoms.constData();
    int nats = connected_atoms.count();
    
    for (int i=0; i<nats; ++i)
    {
        foreach (AtomIdx j, connected_atoms_array[i])
        {
            if (i < j)
            {
                //only count connections when the i atom is less
                //than j - this avoids double counting the bond
                // i-j and j-i
                ++n;
            }
        }
    }
    
    return n;
}

/** Return the indicies of atoms connected to the atom at index 'atomidx'.
    This returns an empty set if there are no atoms connected to 
    this atom
    
    \throw SireError::invalid_index
*/
const QSet<AtomIdx>& ConnectivityBase::connectionsTo(AtomIdx atomidx) const
{
    return connected_atoms.constData()[ atomidx.map(connected_atoms.count()) ];
}

/** Return the indicies of atoms connected to the atom identified
    by 'resid' - this returns an empty set if there are no connections
    to this atom
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
const QSet<AtomIdx>& ConnectivityBase::connectionsTo(const AtomID &atomid) const
{
    return connected_atoms.constData()[ info().atomIdx(atomid) ];
}

/** Return the indicies of the residues connected to the residue at 
    index 'residx'. This returns an empty set if there are no residues
    connected to this residue
    
    \throw SireError::invalid_index
*/
const QSet<ResIdx>& ConnectivityBase::connectionsTo(ResIdx residx) const
{
    return connected_res.constData()[ residx.map(connected_res.count()) ];
}

/** Return the indicies of the residues connectd to the residue
    identified by 'resid'. This returns an empty set if there are
    no residues connected to this residue.
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
const QSet<ResIdx>& ConnectivityBase::connectionsTo(const ResID &resid) const
{
    return connected_res.constData()[ info().resIdx(resid) ];
}

/** Return the number of connections to the atom at index 'atomidx' 

    \throw SireError::index_error
*/
int ConnectivityBase::nConnections(AtomIdx atomidx) const
{
    return this->connectionsTo(atomidx).count();
}

/** Return the number of connections to the atom with ID 'atomid' 

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
int ConnectivityBase::nConnections(const AtomID &atomid) const
{
    return this->connectionsTo(atomid).count();
}

/** Return the number of connections to the residue at index 'residx'

    \throw SireError::invalid_index
*/
int ConnectivityBase::nConnections(ResIdx residx) const
{
    return this->connectionsTo(residx).count();
}

/** Return the number of connections to the residue identified
    by 'resid'
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
int ConnectivityBase::nConnections(const ResID &resid) const
{
    return this->connectionsTo(resid).count();
}

/** Return the number of atom connections between the residues at
    indicies 'res0' and 'res1' 
    
    \throw SireError::invalid_index
*/
int ConnectivityBase::nConnections(ResIdx res0, ResIdx res1) const
{
    if (not this->areConnected(res0, res1))
        return 0;
        
    int n = 0;
    
    QList<AtomIdx> atoms0 = info().getAtomsIn(res0);
    QList<AtomIdx> atoms1 = info().getAtomsIn(res1);
    
    foreach (AtomIdx atom0, atoms0)
    {
        const QSet<AtomIdx> &connected = this->connectionsTo(atom0);

        foreach (AtomIdx atom1, atoms1)
        {
            if (connected.contains(atom1))
                ++n;
        }
    }
    
    return n;
}

/** Return the number of atom connections between the residues
    identified by 'res0' and 'res1'
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
int ConnectivityBase::nConnections(const ResID &res0, const ResID &res1) const
{
    return this->nConnections( info().resIdx(res0), info().resIdx(res1) );
}

/** Return whether or not the atoms at indicies 'atom0' and 'atom1'
    are connected
    
    \throw SireError::invalid_index
*/
bool ConnectivityBase::areConnected(AtomIdx atom0, AtomIdx atom1) const
{
    return this->connectionsTo(atom0).contains( 
                                AtomIdx(atom1.map(connected_atoms.count())) );
}

/** Return whether or not the atoms identified by 'atom0' and 'atom1'
    are connected
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
bool ConnectivityBase::areConnected(const AtomID &atom0, const AtomID &atom1) const
{
    return this->connectionsTo(atom0).contains( info().atomIdx(atom1) );
}
    
/** Return whether or not the residues at indicies 'res0' and 'res1' 
    are connected
    
    \throw SireError::invalid_index
*/
bool ConnectivityBase::areConnected(ResIdx res0, ResIdx res1) const
{
    return this->connectionsTo(res0).contains( 
                                        ResIdx(res1.map(connected_res.count())) );
}

/** Return whether the residues identified by 'res0' and 'res1' are connected */
bool ConnectivityBase::areConnected(const ResID &res0, const ResID &res1) const
{
    return this->connectionsTo(res0).contains( info().resIdx(res1) );
}

/** Non-checking version of Connectivity::connectedTo(AtomIdx) */
const QSet<AtomIdx>& ConnectivityBase::_pvt_connectedTo(AtomIdx atom) const
{
    return connected_atoms.constData()[atom];
}

/** This is a recursive function that traces all atoms that can trace their bonding
    to 'strt', and are in the same residue as 'strt', but that don't go through 'root',
    and to add those atoms to 'group'. If any atoms are found that are in 'exclude' then
    an exception is thrown, as this indicates that the atoms found form part
    of a ring. This is an internal function that is only intended to be used
    by the splitMolecule and splitResidue functions.

    \throw SireMol::ring_error
*/
void ConnectivityBase::traceRoute(AtomIdx start, AtomIdx root,
                                  const QSet<AtomIdx> &exclude,
                                  QSet<AtomIdx> &group) const
{
    if (group.contains(start))
        //we have already processed this atom
        return;

    //add this atom to the group
    group.insert(start);

    //now see if any of its bonded atoms need to be added
    const QSet<AtomIdx> &bonded_atoms = this->_pvt_connectedTo(start);

    //loop over every bond that involves the 'start' atom
    for (QSet<AtomIdx>::const_iterator it = bonded_atoms.constBegin();
         it != bonded_atoms.constEnd();
         ++it)
    {
        //if this is the root atom then ignore it, as we don't
        //want to move backwards!
        if (*it == root)
            continue;
    
        //has this atom or residue already been selected?
        else if (group.contains(*it))
            //yes, this atom or residue is already included!
            continue;

        //is this atom already excluded?
        else if (exclude.contains(*it))
        {
            //ok, we've just found a ring!
            throw SireMol::ring_error( QObject::tr(
                "Atoms %1-%2-%3 form part of ring and cannot be "
                "unambiguously split.")
                    .arg(start.toString(), it->toString(), root.toString()),
                        CODELOC );
        }
        else
        {
            //now we can trace the atoms from the 'other' atom...
            this->traceRoute(*it, start, exclude, group);
        }
    }

    //ok - we have added all of the atoms that are connected to this atom. We
    //have finished with this atom, so we can return.
    return;
}

/** This is a recursive function that traces all atoms that can trace their bonding
    to 'strt', and are in the same residue as 'strt', but that don't go through 'root',
    and to add those atoms to 'group'. If any atoms are found that are in 'exclude' then
    an exception is thrown, as this indicates that the atoms found form part
    of a ring. This is an internal function that is only intended to be used
    by the splitMolecule and splitResidue functions.

    This only searches atoms that are selected in 'selected_atoms'

    \throw SireMol::ring_error
*/
void ConnectivityBase::traceRoute(const AtomSelection &selected_atoms,
                                  AtomIdx start, AtomIdx root,
                                  const QSet<AtomIdx> &exclude,
                                  QSet<AtomIdx> &group) const
{
    if (group.contains(start) or not selected_atoms.selected(start))
        //we have already processed this atom or it
        //is not one of the selected atoms
        return;

    //add this atom to the group
    group.insert(start);

    //now see if any of its bonded atoms need to be added
    const QSet<AtomIdx> &bonded_atoms = this->_pvt_connectedTo(start);

    //loop over every bond that involves the 'start' atom
    for (QSet<AtomIdx>::const_iterator it = bonded_atoms.constBegin();
         it != bonded_atoms.constEnd();
         ++it)
    {
        //if this is the root atom then ignore it, as we don't
        //want to move backwards!
        if (*it == root)
            continue;
    
        //has this atom or residue already been selected?
        else if (group.contains(*it))
            //yes, this atom or residue is already included!
            continue;

        //is this atom already excluded?
        else if (exclude.contains(*it))
        {
            //ok, we've just found a ring!
            throw SireMol::ring_error( QObject::tr(
                "Atoms %1-%2-%3 form part of ring and cannot be "
                "unambiguously split.")
                    .arg(start.toString(), it->toString(), root.toString()),
                        CODELOC );
        }
        else
        {
            //now we can trace the atoms from the 'other' atom...
            this->traceRoute(selected_atoms, *it, start, exclude, group);
        }
    }

    //ok - we have added all of the atoms that are connected to this atom. We
    //have finished with this atom, so we can return.
    return;
}

/** Return the two AtomSelection objects corresponding to the atoms
    selected in 'group0' and 'group1' */
tuple<AtomSelection,AtomSelection>
ConnectivityBase::selectGroups(const QSet<AtomIdx> &group0,
                               const QSet<AtomIdx> &group1) const
{
    AtomSelection grp0(*d);
    AtomSelection grp1(*d);

    tuple<AtomSelection,AtomSelection> groups( grp0.selectOnly(group0),
                                               grp1.selectOnly(group1) );
    
    return groups;
}

/** Split this molecule into two parts about the atoms
    atom0 and atom1. For example;
    
    For example;

          C1--C2--C3--C4--C5--C6--C7--C8

    Splitting C3 and C4 would result in two groups, {C1,C2,C3} and {C4,C5,C6,C7,C8}

          C1\
          C2-C4--C5
          C3/

    Splitting C4 and C5 would result in two groups, {C1,C2,C3,C4} and {C5}

    However splitting C1 and C5 would add a bond between C1 and C5. This would mean
    than C1-C4-C5 would form a ring, so an exception would be thrown.

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection> 
ConnectivityBase::split(AtomIdx atom0, AtomIdx atom1) const
{
    QSet<AtomIdx> group0, group1;

    //map the atoms
    int nats = d->nAtoms();
    atom0 = atom0.map(nats);
    atom1 = atom1.map(nats);

    if (atom0 == atom1)
        throw SireMol::ring_error( QObject::tr(
            "You cannot split a molecule into two parts using the same atom! (%1).")
                .arg(atom0), CODELOC );

    //make sure that there is sufficient space for the
    //selections - this prevents mallocs while tracing
    //the bonds
    group0.reserve(nats);
    group1.reserve(nats);
    
    //add the two atoms to their respective groups
    group0.insert(atom0);
    group1.insert(atom1);
    
    //add the atoms bonded to atom0 to group0
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom0))
    {
        if (not bonded_atom == atom1)
        {
            this->traceRoute(bonded_atom, atom0, group1, group0);
        }
    }
    
    //now add the atoms bonded to atom1 to group1
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom1))
    {
        if (not bonded_atom == atom0)
        {
            if (group0.contains(bonded_atom))
                throw SireMol::ring_error( QObject::tr(
                    "Atoms %1-%2-%3 form part of ring and cannot be "
                    "unambiguously split.")
                        .arg(atom0.toString(), bonded_atom.toString(), 
                             atom1.toString()), CODELOC );

            this->traceRoute(bonded_atom, atom1, group0, group1);
        }
    }
    
    return this->selectGroups(group0, group1);
}

/** Split the molecule into two parts about the bond between atom0 and atom1.

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection> 
ConnectivityBase::split(const AtomID &atom0, const AtomID &atom1) const
{
    return this->split( d->atomIdx(atom0), d->atomIdx(atom1) );
}

/** Split the molecule into two parts about the bond 'bond'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection> 
ConnectivityBase::split(const BondID &bond) const
{
    return this->split( bond.atom0(), bond.atom1() );
}

/** Split the selected atoms of this molecule into two parts about the atoms
    atom0 and atom1. For example;
    
    For example;

          C1--C2--C3--C4--C5--C6--C7--C8

    Splitting C3 and C4 would result in two groups, {C1,C2,C3} and {C4,C5,C6,C7,C8}

          C1\
          C2-C4--C5
          C3/

    Splitting C4 and C5 would result in two groups, {C1,C2,C3,C4} and {C5}

    However splitting C1 and C5 would add a bond between C1 and C5. This would mean
    than C1-C4-C5 would form a ring, so an exception would be thrown.

    Note that both atom0 and atom1 *must* be selected as part of 'selected_atoms'
    or else a missing_atom exception will be thrown.

    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(AtomIdx atom0, AtomIdx atom1, 
                        const AtomSelection &selected_atoms) const
{
    selected_atoms.assertCompatibleWith(*d);
    
    if (selected_atoms.selectedAll())
        return this->split(atom0, atom1);
 
    selected_atoms.assertSelected(atom0);
    selected_atoms.assertSelected(atom1);

    QSet<AtomIdx> group0, group1;

    //make sure that there is sufficient space for the
    //selections - this prevents mallocs while tracing
    //the bonds
    group0.reserve(selected_atoms.nSelected());
    group1.reserve(selected_atoms.nSelected());
    
    //map the atoms
    atom0 = atom0.map(d->nAtoms());
    atom1 = atom1.map(d->nAtoms());

    if (atom0 == atom1)
        throw SireMol::ring_error( QObject::tr(
            "You cannot split a molecule into two parts using the same atom! (%1).")
                .arg(atom0), CODELOC );

    //add the two atoms to their respective groups
    group0.insert(atom0);
    group1.insert(atom1);
    
    //add the atoms bonded to atom0 to group0
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom0))
    {
        if ( (not bonded_atom == atom1) and 
             selected_atoms.selected(bonded_atom) )
        {
            this->traceRoute(selected_atoms, bonded_atom, 
                             atom0, group1, group0);
        }
    }
    
    //now add the atoms bonded to atom1 to group1
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom1))
    {
        if ( (not bonded_atom == atom0) and
             selected_atoms.selected(bonded_atom) )
        {
            if (group0.contains(bonded_atom))
                throw SireMol::ring_error( QObject::tr(
                    "Atoms %1-%2-%3 form part of ring and cannot be "
                    "unambiguously split.")
                        .arg(atom0.toString(), bonded_atom.toString(), 
                             atom1.toString()), CODELOC );
                    
            this->traceRoute(selected_atoms, bonded_atom,
                             atom1, group0, group1);
        }
    }
    
    return this->selectGroups(group0, group1);    
}

/** Split the selected atoms of this molecule about the atoms 
    'atom0' and 'atom1'

    \throw SireMol::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   

tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const AtomID &atom0, const AtomID &atom1,
                        const AtomSelection &selected_atoms) const
{
    return this->split( d->atomIdx(atom0), d->atomIdx(atom1),
                        selected_atoms );
}

/** Split the selected atoms of this molecule into two parts
    about the bond 'bond'

    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const BondID &bond, const AtomSelection &selected_atoms) const
{
    return this->split( bond.atom0(), bond.atom1(), selected_atoms );
}

/** Split this molecule into three parts about the atoms
    'atom0', 'atom1' and 'atom2'.
    
    An exception will be thrown if it is not possible to split the molecule
    unambiguously in two, as the angle is part of a ring.

    For example;

      C1   C3--C8
        \ /
        C2
       /  \
     C4   C5--C6-C7

    Splitting C5,C2,C3 would return {C5,C6,C7} in one group, and {C3,C8} 
    in the other group.

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection> 
ConnectivityBase::split(AtomIdx atom0, AtomIdx atom1, AtomIdx atom2) const
{
    QSet<AtomIdx> group0, group1;

    //map the atoms
    int nats = d->nAtoms();
    atom0 = atom0.map(nats);
    atom1 = atom1.map(nats);
    atom2 = atom2.map(nats);

    if (atom0 == atom1 or atom0 == atom2 or atom1 == atom2)
        throw SireMol::ring_error( QObject::tr(
            "You cannot split a molecule into two parts using the same atoms! "
            "(%1, %2, %3).")
                .arg(atom0).arg(atom1).arg(atom2), CODELOC );

    //make sure that there is sufficient space for the
    //selections - this prevents mallocs while tracing
    //the bonds
    group0.reserve(nats);
    group1.reserve(nats);
    
    //add the end atoms to their respective groups
    group0.insert(atom0);
    group1.insert(atom2);
    
    //add the atoms bonded to atom0 to group0
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom0))
    {
        if (not bonded_atom == atom1)
        {
            this->traceRoute(bonded_atom, atom0, group1, group0);
        }
    }
    
    //now add the atoms bonded to atom1 to group1
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom2))
    {
        if (not bonded_atom == atom1)
        {
            if (group0.contains(bonded_atom))
                throw SireMol::ring_error( QObject::tr(
                    "Atoms %1-%2-%3-%4 form part of ring and cannot be "
                    "unambiguously split.")
                        .arg(atom0.toString(), bonded_atom.toString(), 
                             atom1.toString(), atom2.toString()), CODELOC );

            this->traceRoute(bonded_atom, atom2, group0, group1);
        }
    }
    
    return this->selectGroups(group0, group1);
}

/** Split the molecule into two parts based on the three supplied atoms

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection> 
ConnectivityBase::split(const AtomID &atom0, const AtomID &atom1,
                        const AtomID &atom2) const
{
    return this->split( d->atomIdx(atom0), d->atomIdx(atom1),
                        d->atomIdx(atom2) );
}

/** Split the molecule into two parts based on the supplied angle

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection> 
ConnectivityBase::split(const AngleID &angle) const
{
    return this->split( angle.atom0(), angle.atom1(), angle.atom2() );
}

/** Split the selected atoms of this molecule into three parts about the atoms
    'atom0', 'atom1' and 'atom2'.
    
    Note that all three atoms must be contained in the selection or else
    a missing_atom exception will be thrown
    
    An exception will be thrown if it is not possible to split the molecule
    unambiguously in two, as the angle is part of a ring.

    For example;

      C1   C3--C8
        \ /
        C2
       /  \
     C4   C5--C6-C7

    Splitting C5,C2,C3 would return {C5,C6,C7} in one group, and {C3,C8} in the other group.

    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/   
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(AtomIdx atom0, AtomIdx atom1, AtomIdx atom2,
                        const AtomSelection &selected_atoms) const
{
    selected_atoms.assertCompatibleWith(*d);
    
    if (selected_atoms.selectedAll())
        return this->split(atom0, atom1, atom2);
 
    selected_atoms.assertSelected(atom0);
    selected_atoms.assertSelected(atom1);
    selected_atoms.assertSelected(atom2);

    QSet<AtomIdx> group0, group1;

    //make sure that there is sufficient space for the
    //selections - this prevents mallocs while tracing
    //the bonds
    group0.reserve(selected_atoms.nSelected());
    group1.reserve(selected_atoms.nSelected());
    
    //map the atoms
    atom0 = atom0.map(d->nAtoms());
    atom1 = atom1.map(d->nAtoms());
    atom2 = atom2.map(d->nAtoms());

    if (atom0 == atom1 or atom0 == atom2 or atom1 == atom2)
        throw SireMol::ring_error( QObject::tr(
            "You cannot split a molecule into two parts using the same atoms! "
            "(%1, %2, %3).")
                .arg(atom0).arg(atom1).arg(atom2), CODELOC );

    //add the two end atoms to their respective groups
    group0.insert(atom0);
    group1.insert(atom2);
    
    //add the atoms bonded to atom0 to group0
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom0))
    {
        if ( (not bonded_atom == atom1) and 
             selected_atoms.selected(bonded_atom) )
        {
            this->traceRoute(selected_atoms, bonded_atom, 
                             atom0, group1, group0);
        }
    }
    
    //now add the atoms bonded to atom1 to group1
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom2))
    {
        if ( (not bonded_atom == atom1) and
             selected_atoms.selected(bonded_atom) )
        {
            if (group0.contains(bonded_atom))
                throw SireMol::ring_error( QObject::tr(
                    "Atoms %1-%2-%3-%4 form part of ring and cannot be "
                    "unambiguously split.")
                        .arg(atom0.toString(), bonded_atom.toString(), 
                             atom1.toString(), atom2.toString()), CODELOC );
                    
            this->traceRoute(selected_atoms, bonded_atom,
                             atom2, group0, group1);
        }
    }
    
    return this->selectGroups(group0, group1);
}

/** Split the selected atoms of the molecule into two groups around the 
    three supplied atoms

    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const AtomID &atom0, const AtomID &atom1, const AtomID &atom2,
                        const AtomSelection &selected_atoms) const
{
    return this->split( d->atomIdx(atom0), d->atomIdx(atom1),
                        d->atomIdx(atom2), selected_atoms );
}
      
/** Split the selected atoms 'selected_atoms' of this molecule  
    into two parts based on the angle identified in 
    'angle'. This splits the molecule about atom0() and atom2()
    of the angle, ignoring atom atom1().
    
    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const AngleID &angle, 
                        const AtomSelection &selected_atoms) const
{
    return this->split( angle.atom0(), angle.atom1(),
                        angle.atom2(), selected_atoms );
}

/** Split this molecule into two parts based on the passed atoms. 
    This splits the molecule between atom0 and atom3, ignoring 
    atom1 and atom2.

    An exception will be thrown if it is not possible to split the molecule
    unambiguously in two, as the dihedral is part of a ring.

    C1   C4--C5--C6
      \ /
      C2    C8--C9
     /  \  /
    C3   C7
           \
            C10--C11

    Splitting C4,C2,C7,C10 will return {C4,C5,C6} and {C10,C11}. 
    If this molecule had been split by just Bond(C2,C7) using the above 
    function, then the first returned group would
    be {C1,C2,C3,C4,C5,C6}, while the second group would be {C7,C8,C9,C10,C11}.

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(AtomIdx atom0, AtomIdx atom1, 
                        AtomIdx atom2, AtomIdx atom3) const
{
    QSet<AtomIdx> group0, group1;

    //map the atoms
    int nats = d->nAtoms();
    atom0 = atom0.map(nats);
    atom1 = atom1.map(nats);
    atom2 = atom2.map(nats);
    atom3 = atom3.map(nats);

    if (atom0 == atom1 or atom0 == atom2 or atom0 == atom3 or
        atom1 == atom2 or atom1 == atom3 or
        atom2 == atom3)
        throw SireMol::ring_error( QObject::tr(
            "You cannot split a molecule into two parts using the same atoms! "
            "(%1, %2, %3, %4).")
                .arg(atom0).arg(atom1)
                .arg(atom2).arg(atom3), CODELOC );

    //make sure that there is sufficient space for the
    //selections - this prevents mallocs while tracing
    //the bonds
    group0.reserve(nats);
    group1.reserve(nats);
    
    //add the end atoms to their respective groups
    group0.insert(atom0);
    group1.insert(atom3);
    
    //add the atoms bonded to atom0 to group0
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom0))
    {
        if (not bonded_atom == atom1)
        {
            this->traceRoute(bonded_atom, atom0, group1, group0);
        }
    }
    
    //now add the atoms bonded to atom1 to group1
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom3))
    {
        if (not bonded_atom == atom2)
        {
            if (group0.contains(bonded_atom))
                throw SireMol::ring_error( QObject::tr(
                    "Atoms %1-%2-%3-%4-%5 form part of ring and cannot be "
                    "unambiguously split.")
                        .arg(atom0.toString(), bonded_atom.toString(), 
                             atom1.toString(), atom2.toString(),
                             atom3.toString()), CODELOC );

            this->traceRoute(bonded_atom, atom3, group0, group1);
        }
    }
    
    return this->selectGroups(group0, group1);
}

/** Split this molecule into two parts based on the passed atoms. 
    This splits the molecule between atom0 and atom3, ignoring 
    atom1 and atom2.

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const AtomID &atom0, const AtomID &atom1, 
                    const AtomID &atom2, const AtomID &atom3) const
{
    return this->split( d->atomIdx(atom0), d->atomIdx(atom1),
                        d->atomIdx(atom2), d->atomIdx(atom3) );
}
      
/** Split this molecule into two parts based on the dihedral identified in 
    'dihedral'. This splits the molecule about atom0() and atom3()
    of the dihedral, ignoring atoms atom1() and atom2().
    
    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const DihedralID &dihedral) const
{
    return this->split( dihedral.atom0(), dihedral.atom1(),
                        dihedral.atom2(), dihedral.atom3() );
}

/** Split the selected atoms of this molecule into two parts 
    based on the passed atoms. 
    
    This splits the molecule between atom0 and atom3, ignoring 
    atom1 and atom2.

    All four atoms must be selected in 'selected_atoms' or else
    a missing_atom exception will be thrown

    An exception will be thrown if it is not possible to split the molecule
    unambiguously in two, as the dihedral is part of a ring.

    C1   C4--C5--C6
      \ /
      C2    C8--C9
     /  \  /
    C3   C7
           \
            C10--C11

    Splitting C4,C2,C7,C10 will return {C4,C5,C6} and {C10,C11}. 
    If this molecule had been split by just Bond(C2,C7) using the above 
    function, then the first returned group would
    be {C1,C2,C3,C4,C5,C6}, while the second group would be {C7,C8,C9,C10,C11}.

    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(AtomIdx atom0, AtomIdx atom1, 
                        AtomIdx atom2, AtomIdx atom3,
                        const AtomSelection &selected_atoms) const
{
    selected_atoms.assertCompatibleWith(*d);
    
    if (selected_atoms.selectedAll())
        return this->split(atom0, atom1, atom2, atom3);
 
    selected_atoms.assertSelected(atom0);
    selected_atoms.assertSelected(atom1);
    selected_atoms.assertSelected(atom2);
    selected_atoms.assertSelected(atom3);

    QSet<AtomIdx> group0, group1;

    //make sure that there is sufficient space for the
    //selections - this prevents mallocs while tracing
    //the bonds
    group0.reserve(selected_atoms.nSelected());
    group1.reserve(selected_atoms.nSelected());
    
    //map the atoms
    atom0 = atom0.map(d->nAtoms());
    atom1 = atom1.map(d->nAtoms());
    atom2 = atom2.map(d->nAtoms());
    atom3 = atom3.map(d->nAtoms());

    if (atom0 == atom1 or atom0 == atom2 or atom0 == atom3 or
        atom1 == atom2 or atom1 == atom3 or
        atom2 == atom3)
        throw SireMol::ring_error( QObject::tr(
            "You cannot split a molecule into two parts using the same atoms! "
            "(%1, %2, %3, %4).")
                .arg(atom0).arg(atom1)
                .arg(atom2).arg(atom3), CODELOC );

    //add the two end atoms to their respective groups
    group0.insert(atom0);
    group1.insert(atom3);
    
    //add the atoms bonded to atom0 to group0
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom0))
    {
        if ( (not bonded_atom == atom1) and 
             selected_atoms.selected(bonded_atom) )
        {
            this->traceRoute(selected_atoms, bonded_atom, 
                             atom0, group1, group0);
        }
    }
    
    //now add the atoms bonded to atom1 to group1
    foreach (const AtomIdx &bonded_atom, this->_pvt_connectedTo(atom3))
    {
        if ( (not bonded_atom == atom2) and
             selected_atoms.selected(bonded_atom) )
        {
            if (group0.contains(bonded_atom))
                throw SireMol::ring_error( QObject::tr(
                    "Atoms %1-%2-%3-%4-%5 form part of ring and cannot be "
                    "unambiguously split.")
                        .arg(atom0.toString(), bonded_atom.toString(), 
                             atom1.toString(), atom2.toString(),
                             atom3.toString()), CODELOC );
                    
            this->traceRoute(selected_atoms, bonded_atom,
                             atom3, group0, group1);
        }
    }
    
    return this->selectGroups(group0, group1);
}
      
/** Split the selected atoms 'selected_atoms' of this molecule
    into two parts based on the passed atoms. This splits
    the molecule between atom0 and atom3, ignoring atom1 and 
    atom2.
    
    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const AtomID &atom0, const AtomID &atom1,
                    const AtomID &atom2, const AtomID &atom3,
                    const AtomSelection &selected_atoms) const
{
    return this->split( d->atomIdx(atom0), d->atomIdx(atom1),
                        d->atomIdx(atom2), d->atomIdx(atom3),
                        selected_atoms );
}
      
/** Split the selected atoms 'selected_atoms' of this molecule  
    into two parts based on the dihedral identified in 
    'dihedral'. This splits the molecule about atom0() and atom3()
    of the dihedral, ignoring atoms atom1() and atom2().
    
    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const DihedralID &dihedral,
                    const AtomSelection &selected_atoms) const
{
    return this->split( dihedral.atom0(), dihedral.atom1(),
                        dihedral.atom2(), dihedral.atom3(),
                        selected_atoms );
}

/** Split this molecule into two parts based on the improper angle
    identified by 'improper'. This splits the molecule about
    bond between atom0() and atom1() of the improper
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const ImproperID &improper) const
{
    return this->split( improper.atom0(), improper.atom1() );
}

/** Split the selected atoms in 'selected_atoms' in this molecule
    into two parts based on the improper angle
    identified by 'improper'. This splits the molecule about
    bond between atom0() and atom1() of the improper
    
    \throw SireError::incompatible_error
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMol::ring_error
*/      
tuple<AtomSelection,AtomSelection>
ConnectivityBase::split(const ImproperID &improper,
                    const AtomSelection &selected_atoms) const
{
    return this->split( improper.atom0(), improper.atom1(),
                        selected_atoms );
}

/////////
///////// Implementation of Connectivity
/////////

/** Reduce the memory usage of this object to a minimum */
void Connectivity::squeeze()
{
    connected_atoms.squeeze();
    
    QSet<AtomIdx> *connected_atoms_array = connected_atoms.data();
    int nats = connected_atoms.count();
    
    for (int i=0; i<nats; ++i)
    {
        connected_atoms_array[i].squeeze();
    }
    
    connected_res.squeeze();
    
    QSet<ResIdx> *connected_res_array = connected_res.data();
    int nres = connected_res.count();
    
    for (int i=0; i<nres; ++i)
    {
        connected_res_array[i].squeeze();
    }
}

/** Null constructor */
Connectivity::Connectivity() 
             : ConcreteProperty<Connectivity,ConnectivityBase>()
{}

/** Construct the connectivity for the molecule whose data
    is in 'moldata' */
Connectivity::Connectivity(const MoleculeData &moldata)
             : ConcreteProperty<Connectivity,ConnectivityBase>(moldata)
{}

/** Construct the connectivity from the passed editor */
Connectivity::Connectivity(const ConnectivityEditor &editor)
             : ConcreteProperty<Connectivity,ConnectivityBase>(editor)
{
    this->squeeze();
}

/** Copy constructor */
Connectivity::Connectivity(const Connectivity &other)
             : ConcreteProperty<Connectivity,ConnectivityBase>(other)
{}

/** Destructor */
Connectivity::~Connectivity()
{}

/** Copy assignment from another Connectivity object */
Connectivity& Connectivity::operator=(const Connectivity &other)
{
    ConnectivityBase::operator=(other);
    return *this;
}

/** Copy assignment from a ConnectivityEditor */
Connectivity& Connectivity::operator=(const ConnectivityEditor &editor)
{
    ConnectivityBase::operator=(editor);
    this->squeeze();
    return *this;
}

/** Comparison operator */
bool Connectivity::operator==(const Connectivity &other) const
{
    return ConnectivityBase::operator==(other);
}

/** Comparison operator */
bool Connectivity::operator!=(const Connectivity &other) const
{
    return ConnectivityBase::operator!=(other);
}

/** Return an editor that can edit a copy of this connectivity */
ConnectivityEditor Connectivity::edit() const
{
    return ConnectivityEditor(*this);
}

/////////
///////// Implementation of ConnectivityEditor
/////////

/** Null constructor */
ConnectivityEditor::ConnectivityEditor()
                   : ConcreteProperty<ConnectivityEditor,ConnectivityBase>()
{}

/** Construct an editor to edit a copy of the passed 
    Connectivity object */
ConnectivityEditor::ConnectivityEditor(const Connectivity &connectivity)
                   : ConcreteProperty<ConnectivityEditor,ConnectivityBase>(connectivity)
{}

/** Copy constructor */
ConnectivityEditor::ConnectivityEditor(const ConnectivityEditor &other)
                   : ConcreteProperty<ConnectivityEditor,ConnectivityBase>(other)
{}

/** Destructor */
ConnectivityEditor::~ConnectivityEditor()
{}

/** Copy assignment operator */
ConnectivityEditor& ConnectivityEditor::operator=(const ConnectivityBase &other)
{
    ConnectivityBase::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool ConnectivityEditor::operator==(const ConnectivityEditor &other) const
{
    return ConnectivityBase::operator==(other);
}

/** Comparison operator */
bool ConnectivityEditor::operator!=(const ConnectivityEditor &other) const
{
    return ConnectivityBase::operator!=(other);
}

/** Record the connection between the atoms at indicies 'atom0' 
    and 'atom1'
    
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::connect(AtomIdx atom0, AtomIdx atom1)
{
    AtomIdx atomidx0 = AtomIdx( atom0.map(connected_atoms.count()) );
    AtomIdx atomidx1 = AtomIdx( atom1.map(connected_atoms.count()) );
    
    if (atomidx0 == atomidx1)
        return *this;
    
    QSet<AtomIdx> *connected_atoms_array = connected_atoms.data();
    
    connected_atoms_array[atomidx0].insert(atomidx1);
    connected_atoms_array[atomidx1].insert(atomidx0);
    
    return *this;
}

/** Record a connection between the atom identified by 'atom0' and 
    the atom identified by 'atom1'
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::connect(const AtomID &atom0,
                                                const AtomID &atom1)
{
    AtomIdx atomidx0 = info().atomIdx(atom0);
    AtomIdx atomidx1 = info().atomIdx(atom1);
    
    if (atomidx0 == atomidx1)
        return *this;
        
    QSet<AtomIdx> *connected_atoms_array = connected_atoms.data();
    
    connected_atoms_array[atomidx0].insert(atomidx1);
    connected_atoms_array[atomidx1].insert(atomidx0);
    
    return *this;
}

/** Remove the connection between the atoms at indicies 'atom0'
    and 'atom1' - this does nothing if there isn't already a connection
    
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnect(AtomIdx atom0, AtomIdx atom1)
{
    if (this->areConnected(atom0, atom1))
    {
        AtomIdx atomidx0 = AtomIdx( atom0.map( connected_atoms.count() ) );
        AtomIdx atomidx1 = AtomIdx( atom1.map( connected_atoms.count() ) );
        
        connected_atoms[atomidx0].remove(atomidx1);
        connected_atoms[atomidx1].remove(atomidx0);
        
        //now check to see if the residues are still connected
        ResIdx residx0 = info().parentResidue(atomidx0);
        ResIdx residx1 = info().parentResidue(atomidx1);

        if (this->nConnections(residx0, residx1) == 0)
        {
            connected_res[residx0].remove(residx1);
            connected_res[residx1].remove(residx0);
        }
    }

    return *this;
}

/** Disconnect the atoms that are identified by 'atom0' and 'atom1' - 
    this does nothing if there isn't a connection between these atoms
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnect(const AtomID &atom0, 
                                                   const AtomID &atom1)
{
    return this->disconnect( info().atomIdx(atom0), info().atomIdx(atom1) );
}

/** Remove all of the connections to the atom at index 'atomidx' 

    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnectAll(AtomIdx atomidx)
{
    QSet<AtomIdx> connected = this->connectionsTo(atomidx);
    
    foreach (AtomIdx atom1, connected)
    {
        this->disconnect(atomidx, atom1);
    }
    
    return *this;
}

/** Remove all of the connections to the atom identified by 'atomid' 

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnectAll(const AtomID &atomid)
{
    return this->disconnectAll(info().atomIdx(atomid));
}

/** Remove all of the connections that involve any of the atoms
    in the residue at index 'residx'
    
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnectAll(ResIdx residx)
{
    foreach (AtomIdx atomidx, info().getAtomsIn(residx))
    {
        this->disconnectAll(atomidx);
    }
    
    return *this;
}

/** Remove all of the connections that involve any of the atoms
    in the residue identified by 'resid'
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnectAll(const ResID &resid)
{
    return this->disconnectAll( info().resIdx(resid) );
}
