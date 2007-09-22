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

#ifndef SIREMOL_RESIDENTIFIER_H
#define SIREMOL_RESIDENTIFIER_H

#include "resid.h"
#include "resnum.h"

namespace SireMol
{

/** This class is used as a means of storing all of the ID
    for a residue that can be contained in a combination of
    any or all of the other residue ID types (ResName, 
    ResNum and ResIdx)
    
    @author Christopher Woods
*/
class ResIdentifier : public ResID
{
public:
    ResIdentifier();
    
    ResIdentifier(const ResName &name);
    ResIdentifier(const ResNum &num);
    ResIdentifier(const ResIdx &idx);
    
    ResIdentifier(const ResName &name, const ResNum &num);
    
    ResIdentifier(const ResID &id0);
    ResIdentifier(const ResID &id0, const ResID &id1);
    ResIdentifier(const ResID &id0, const ResID &id1, const ResID &id2);

    ResIdentifier(const ResIdentifier &other);
    
    ~ResIdentifier();
    
    void setName(const ResName &name);
    void setNumber(const ResNum &num);
    void setIndex(const ResIdx &idx);
    
    const ResName& name() const;
    const ResNum& number() const;
    const ResIdx& index() const;

    QList<ResIdx> map(const MoleculeInfo &molinfo) const;
    
private:
    void assignFrom(const ResID &resid);

    /** The name of the residue */
    ResName _name;
    
    /** The number of the residue */
    ResNum _number;
    
    /** The index of this residue, using the above criteria */
    ResIdx _idx;
};

}

QDataStream& operator<<(QDataStream&, const SireMol::ResIdentifier&);
QDataStream& operator>>(QDataStream&, SireMol::ResIdentifier&);

XMLStream& operator<<(XMLStream&, const SireMol::ResIdentifier&);
XMLStream& operator>>(XMLStream&, SireMol::ResIdentifier&);

uint qHash(const SireMol::ResIdentifier &resid);

Q_DECLARE_METATYPE(SireMol::ResIdentifier);

#endif


