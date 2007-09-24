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

#ifndef SIREMOL_ATOMIDCOMBO_H
#define SIREMOL_ATOMIDCOMBO_H

#include "atomid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class represents a combination of ID types that
    are all combined so that we can try to identify some
    atoms.
    
    @author Christopher Woods
*/
class AtomIDCombo : public AtomID
{
public:
    AtomIDCombo();
    
    AtomIDCombo(const AtomIDCombo &other);
    
    ~AtomIDCombo();
    
    static const char* typeName()
    {
        return "SireMol::AtomIDCombo";
    }
    
    const char* what() const
    {
        return AtomIDCombo::typeName();
    }
    
    void add(const SegID &segid) const;
    void add(const ChainID &chainid) const;
    void add(const CGID &cgid) const;
    void add(const ResID &resid) const;
    void add(const AtomID &atomid) const;
    
    QString toString() const;
    
    QList<AtomIdx> map(const MoleculeInfo &molinfo) const;

private:
    /** The collection of Segment IDs that are part of this combo */
    QList<SegmentIdentifier> segids;
    
    /** The collection of Chain IDs that are part of this combo */
    QList<ChainIdentifier> chainids;
    
    /** The collection of CutGroup IDs that are part of this combo */
    QList<CGIdentifier> cgids;
    
    /** The collection of Residue IDs that are part of this combo */
    QList<ResIdentifier> resids;

    /** The collection of Atom IDs that are part of this combo */ 
    QList<AtomIdentifier> atomids;
};

AtomIDCombo operator+(const AtomIDCombo &combo, const AtomIDCombo &combo);
AtomIDCombo operator+(const MolPartID &part, const AtomIDCombo &combo);
AtomIDCombo operator+(const AtomIDCombo &combo, const MolPartID &part);
AtomIDCombo operator+(const MolPartID &part0, const MolPartID &part1);

class ResAtomID : public AtomID
{
public:
    ResAtomID();
    
    ResAtomID(const ResAtomID &other) const;
    
    ~ResAtomID();
    
    static const char* typeName()
    {
        return "SireMol::ResAtomID";
    }
    
    const char* what() const
    {
        return ResAtomID::typeName();
    }
    
    ResAtomID* clone() const
    {
        return new ResAtomID(*this);
    }
    
    QString toString() const
    {
        return QString("%1 and %2").arg(resid->toString(), atomid->toString());
    }
    
    QList<AtomIdx> map(const MoleculeInfo &molinfo) const;
    
private:
    boost::shared_ptr<ResID> resid;
    boost::shared_ptr<AtomID> atomid;
};

}

SIRE_END_HEADER

#endif
