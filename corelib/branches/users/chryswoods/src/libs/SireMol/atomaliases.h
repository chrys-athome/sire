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

#ifndef SIREMOL_ATOMALIASES_H
#define SIREMOL_ATOMALIASES_H

#include "idmolatom.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomAliases;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomAliases&);
QDataStream& operator>>(QDataStream&, SireMol::AtomAliases&);

namespace SireMol
{

/** This class is used to provide an alias for atoms in a molecule,
    e.g. to use CA to mean CA1, or ResNum 42 is ResNum 12 etc.
    
    This is useful when aligning molecules, or when searching for
    parameters, as it allows you to have match to a molecule that is
    named using a different naming scheme (e.g. sequentially
    numbered residues versus crystallographic numbered residues).
    
    (C) Christopher Woods
*/
class SIREMOL_EXPORT AtomAliases
          : public SireBase::ConcreteProperty<AtomAliases,SireBase::PropertyBase>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomAliases&);
friend QDataStream& ::operator>>(QDataStream&, AtomAliases&);

public:
    AtomAliases();

    AtomAliases(const AtomAliases &other);
    
    ~AtomAliases();
    
    static const char* typeName()
    {
        return "SireMol::AtomAliases";
    }
    
    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;
    
    AtomAliases& operator=(const AtomAliases &other);
    
    bool operator==(const AtomAliases &other) const;
    bool operator!=(const AtomAliases &other) const;
    
    NewAtom map(const NewAtom &atom, const MolDataView &molecule) const;
    Residue map(const Residue &res, const MolDataView &residue) const;
    
    QString mapAtom(const QString &atomname) const;
    AtomIndex mapAtom(const AtomIndex &atom) const;
};
    
}

Q_DECLARE_METATYPE(SireMol::AtomAliases);

SIRE_END_HEADER

#endif
