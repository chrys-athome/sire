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

#include "connectivity.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireBase;

/////////
///////// Implementation of ConnectivityBase
/////////

static const RegisterMetaType<ConnectivityBase> r_conbase;

/** Serialise ConnectivityBase */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const ConnectivityBase &conbase)
{
    writeHeader(ds, r_conbase, 1);

    SharedDataStream sds(ds);

    sds << conbase.connected_atoms << conbase.connected_residues
        << conbase.molinfo << static_cast<const PropertyBase&>(conbase);

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
        
        sds >> conbase.connected_atoms >> conbase.connected_residues
            >> conbase.molinfo
            >> static_cast<PropertyBase&>(conbase);
    }
    else
        throw version_error(v, "1", r_conbase, CODELOC);

    return ds;
}

/** Null constructor */
ConnectivityBase::ConnectivityBase() : PropertyBase()
{}

/** Construct the connectivity for molecule described by 
    the passed info object */
ConnectivityBase::ConnectivityBase(const MoleculeInfo &info)
                 : PropertyBase(), molinfo(info)
{
    if (molinfo.nAtoms() > 0)
    {
        connected_atoms.resize(molinfo.nAtoms());
        connected_atoms.squeeze();
    }
    
    if (molinfo.nResidues() > 0)
    {
        connected_res.resize(molinfo.nResidues());
        connected_res.squeeze();
    }
}
    
/** Copy constructor */
ConnectivityBase::ConnectivityBase(const ConnectivityBase &other)
                 : PropertyBase(other),
                   connected_atoms(other.connected_atoms),
                   connected_res(other.connected_res),
                   molinfo(other.molinfo)
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
        molinfo = other.molinfo;
    }
    
    return *this;
}

/** Comparison operator */
bool ConnectivityBase::operator==(const ConnectivityBase &other) const
{
    return molinfo == other.molinfo and 
           connected_atoms == other.connected_atoms;
}

/** Comparison operator */
bool ConnectivityBase::operator!=(const ConnectivityBase &other) const
{
    return molinfo != other.molinfo or
           connected_atoms != other.connected_atoms;
}

/** Return the total number of connections between atoms
    in this connectivity object */
int ConnectivityBase::nConnections() const
{
    int n = 0;
    
    QSet<AtomIdx> *connected_atoms_array = connected_atoms.constData();
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
    return connected_atoms.constData()[ molinfo.atomIdx(atomid) ];
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
    return connected_res.constData()[ molinfo.resIdx(resid) ];
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
    
    QList<AtomIdx> atoms0 = molinfo.getAtomsIn(res0);
    QList<AtomIdx> atoms1 = molinfo.getAtomsIn(res1);
    
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
    return this->nConnections( molinfo.resIdx(res0), molinfo.resIdx(res1) );
}

/** Return whether or not the atoms at indicies 'atom0' and 'atom1'
    are connected
    
    \throw SireError::invalid_index
*/
bool ConnectivityBase::areConnected(AtomIdx atom0, AtomIdx atom1) const
{
    return this->connectionsTo(atom0).contains( atom1.map(connected_atoms.count()) );
}

/** Return whether or not the atoms identified by 'atom0' and 'atom1'
    are connected
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
bool ConnectivityBase::areConnected(const AtomID &atom0, const AtomID &atom1) const
{
    return this->connectionsTo(atom0).contains( molinfo.atomIdx(atom1) );
}
    
/** Return whether or not the residues at indicies 'res0' and 'res1' 
    are connected
    
    \throw SireError::invalid_index
*/
bool ConnectivityBase::areConnected(ResIdx res0, ResIdx res1) const
{
    return this->connectionsTo(res0).contains( res1.map(connected_res.count()) );
}

/** Return whether the residues identified by 'res0' and 'res1' are connected */
bool ConnectivityBase::areConnected(const ResID &res0, const ResID &res1) const
{
    return this->connectionsTo(res0).contains( molinfo.resIdx(res1) );
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

/** Construct the connectivity for the molecule described by the 
    passed MoleculeInfo */
Connectivity::Connectivity(const MoleculeInfo &molinfo)
             : ConnectivityBase(molinfo)
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
    AtomIdx atomidx0 = atom0.map(connected_atoms.count());
    AtomIdx atomidx1 = atom1.map(connected_atoms.count());
    
    if (atomidx0 == atomidx1)
        return;
    
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
    AtomIdx atomidx0 = map.atomIdx(atom0);
    AtomIdx atomidx1 = map.atomIdx(atom1);
    
    if (atomidx0 == atomidx1)
        return;
        
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
        AtomIdx atomidx0 = atom0.map( connected_atoms.count() );
        AtomIdx atomidx1 = atom1.map( connected_atoms.count() );
        
        connected_atoms[atomidx0].remove(atomidx1);
        connected_atoms[atomidx1].remove(atomidx0);
        
        //now check to see if the residues are still connected
        ResIdx residx0 = molinfo.parentResidue(atomidx0);
        ResIdx residx1 = molinfo.parentResidue(atomidx1);

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
    return this->disconnect( molinfo.atomIdx(atom0), molinfo.atomIdx(atom1) );
}

/** Remove all of the connections to the atom at index 'atomidx' 

    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnectAll(AtomIdx atomidx)
{
    QSet<AtomIdx> connected = this->connectedTo(atomidx);
    
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
    return this->disconnectAll(molinfo.atomIdx(atomid));
}

/** Remove all of the connections that involve any of the atoms
    in the residue at index 'residx'
    
    \throw SireError::invalid_index
*/
ConnectivityEditor& ConnectivityEditor::disconnectAll(ResIdx residx)
{
    QSet<AtomIdx> atomidxs = molinfo.getAtomsIn(residx);
    
    foreach (AtomIdx atomidx, atomidxs)
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
    return this->disconnectAll( molinfo.resIdx(resid) );
}
