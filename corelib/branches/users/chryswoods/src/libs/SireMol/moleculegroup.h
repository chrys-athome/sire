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

#ifndef SIREMOL_MOLECULEGROUP_H
#define SIREMOL_MOLECULEGROUP_H

#include "molgroup.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeGroup&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeGroup&);

namespace SireMol
{

/** This is the polymorphic pointer holder for the 
    MolGroup hierarchy of classes (molecule groups).
    
    Molecule groups are groupings of molecules that
    provide full indexing, versioning and identification.
    Molecule groups are used by the forcefield classes
    to hold the molecules, and are used by the System
    classes as well. They provide the consistent interface
    for the indexing, searching and manipulation
    of sets of molecules.

    Like all Property polymorphic pointer holder classes,
    this class holds the polymorphic MolGroup object as 
    an implicitly shared pointer. You can access the 
    const functions of this object by dereferencing this
    pointer, or by using the MoleculeGroup::read() function, e.g.;
    
    int nats = moleculegroup->nAtoms();
    nats = moleculegroup.read().nAtoms();
    
    You must use the MoleculeGroup::edit() function to
    access the non-const member functions, e.g.;
    
    moleculegroup.edit().update(mols);
    
    Because an implicitly shared pointer is held, this
    class can be copied and passed around quickly. A copy
    is only made when the object being pointed to is
    edited via the .edit() function.

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroup : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroup&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroup&);

public:
    MoleculeGroup();
    MoleculeGroup(const PropertyBase &property);
    MoleculeGroup(const MolGroup &molgroup);

    MoleculeGroup(const MoleculeGroup &other);
    
    ~MoleculeGroup();
    
    virtual MoleculeGroup& operator=(const PropertyBase &property);
    virtual MoleculeGroup& operator=(const MolGroup &other);

    const MolGroup* operator->() const;
    const MolGroup& operator*() const;
    
    const MolGroup& read() const;
    MolGroup& edit();
    
    const MolGroup* data() const;
    const MolGroup* constData() const;
    
    MolGroup* data();
    
    operator const MolGroup&() const;

    static const MoleculeGroup& shared_null();
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeGroup);

SIRE_EXPOSE_PROPERTY( SireMol::MoleculeGroup, SireMol::MolGroup )

SIRE_END_HEADER

#endif
