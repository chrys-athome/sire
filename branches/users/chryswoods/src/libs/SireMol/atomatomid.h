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

#ifndef SIREMOL_ATOMATOMID_H
#define SIREMOL_ATOMATOMID_H

#include "atomidentifier.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class holds a pair of AtomIDs, which are used together
    to identify atom(s)
    
    @author Christopher Woods
*/
class AtomAtomID : public AtomID
{
public:
    AtomAtomID();
    
    AtomAtomID(const AtomID &id0, const AtomID &id1);
    
    AtomAtomID(const AtomAtomID &other);
    
    ~AtomAtomID();
    
    static const char* typeName()
    {
        return "SireMol::AtomAtomID";
    }
    
    const char* what() const
    {
        return AtomAtomID::typeName();
    }

    AtomAtomID* clone() const
    {
        return new AtomAtomID(*this);
    }

    QString toString() const;

    QList<AtomIdx> map(const MoleculeInfoData &molinfo) const;

private:
    /** The pair of IDs that are combined */
    AtomIdentifier atomid0, atomid1;
};

AtomAtomID operator+(const AtomID &id0, const AtomID &id1);

}

SIRE_END_HEADER

#endif
