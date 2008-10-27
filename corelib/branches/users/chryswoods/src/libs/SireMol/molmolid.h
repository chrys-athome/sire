/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREMOL_MOLMOLID_H
#define SIREMOL_MOLMOLID_H

#include "molidentifier.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class represents a combination of Molecule IDs

    @author Christopher Woods
*/
class SIREMOL_EXPORT MolMolID : public MolID
{
public:
    MolMolID();
    MolMolID(const MolID &mol0, const MolID &mol1);
    MolMolID(const boost::tuple<MolIdentifier,MolIdentifier> &molids);
    
    MolMolID(const MolMolID &other);
    
    ~MolMolID();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MolMolID>() );
    }
    
    const char* what() const
    {
        return MolMolID::typeName();
    }
    
    MolMolID* clone() const
    {
        return new MolMolID(*this);
    }
    
    MolMolID& operator=(const MolMolID &other);
    
    bool operator==(const MolMolID &other) const;
    bool operator!=(const MolMolID &other) const;
    
    bool operator==(const SireID::ID &other) const;
    bool operator!=(const SireID::ID &other) const;
    
    uint hash() const;
    
    QString toString() const;
    
    bool isNull() const;
    
    QList<MolNum> map(const Molecules &molecules) const;
    QList<MolNum> map(const MoleculeGroup &molgroup) const;
    QList<MolNum> map(const MolGroupsBase &molgroups) const;

private:
    /** The two Molecule identifiers */
    MolIdentifier molid0, molid1;
};

}

Q_DECLARE_METATYPE( SireMol::MolMolID )

SIRE_EXPOSE_CLASS( SireMol::MolMolID )

SIRE_END_HEADER

#endif
