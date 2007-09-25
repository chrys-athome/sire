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

#include "residentifier.h"
#include "moleculeinfodata.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireID;

static const RegisterMetaType<ResIdentifier> r_resid;

///////
/////// Implementation of ResName
///////

QList<ResIdx> ResName::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

///////
/////// Implementation of ResNum
///////

QList<ResIdx> ResNum::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

///////
/////// Implementation of ResIdentifier
///////

/** Serialise to a binary datastream */
QDataStream SIREID_EXPORT &operator<<(QDataStream &ds, const ResIdentifier &resid)
{
}

/** Deserialise from a binary datastream */
QDataStream SIREID_EXPORT &operator>>(QDataStream &ds, ResIdentifier &resid)
{
    return resid.load(ds, r_resid);
}

/** Return the hash of this ID */
uint SIREID_EXPORT qHash(const ResIdentifier &resid)
{
}

/** Null constructor */
ResIdentifier::ResIdentifier() : ResID()
{}
    
/** Allow implicit conversion from a ResName */
ResIdentifier::ResIdentifier(const ResName &name)
              : ResID(), _name(name)
{}

/** Allow implicit conversion from a ResNum */
ResIdentifier::ResIdentifier(const ResNum &num)
              : ResID(), _num(num)
{}

/** Allow implicit conversion from a ResIdx */
ResIdentifier::ResIdentifier(const ResIdx &idx)
              : ResID(), _idx(idx)
{}

  
ResIdentifier::ResIdentifier(const ResName &name, const ResNum &num);
  
ResIdentifier::ResIdentifier(const ResID &id0)
{
    id0.update(*this);
}

ResIdentifier::ResIdentifier(const ResID &id0, const ResID &id1)
{
    id0.update(*this);
    id1.update(*this);
}

ResIdentifier::ResIdentifier(const ResID &id0, const ResID &id1, const ResID &id2)
{
    id0.update(*this);
    id1.update(*this);
    id2.update(*this);
}
    
ResIdentifier::~ResIdentifier()
{}

void ResIdentifier::setName(const ResName &name)
{
    _name = name;
}

void ResIdentifier::setNumber(const ResNum &num)
{
    _num = num;
}

void ResIdentifier::setIndex(const ResIdx &idx)
{
    _idx = idx;
}

void ResName::update(ResIdentifier &resid) const
{
    resid.setName(*this);
}

void ResNum::update(ResIdentifier &resid) const
{
    resid.setNumber(*this);
}

void ResIdx::update(ResIdentifier &resid) const
{
    resid.setIndex(*this);
}

void ResIdentifier::update(ResIdentifier &resid) const
{
    if (this != &resid)
    {
        if (not _name.isNull())
            resid.setName(_name);
            
        if (not _num.isNull())
            resid.setNumber(_num);
            
        if (not _idx.isNull())
            resid.setIndex(_idx);
    }
}

/** Map from this residue identifier to the list of indicies of residues
    that match this ID in the molecule described by 'molinfo'. This
    raises an exception if there are no residues that match this ID
    in this molecule.
    
    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
QList<ResIdx> ResIdentifier::map(const MoleculeInfo &molinfo) const
{
    this->assertNotNull();
    
    QList<ResIdx> residxs;
    
    if (_name.isNull() and _num.isNull())
        //only the index is set
        return molinfo.map(_idx);
    else if (_name.isNull())
        //only the number is set
        residxs = molinfo.map(_num);
    else if (_num.isNull())
        //only the name is set
        residxs = molinfo.map(_name);
    else
    {
        //both are set - combine them together
        residxs = MoleculeInfoData::intersection( molinfo.map(_name),
                                                  molinfo.map(_num) );
    
        if (residxs.isEmpty())
            throw SireMol::missing_residue( QObject:tr(
                "There is no residue \"%1\" (%2) in the molecule called \"%3\".")
                    .arg(_name).arg(_num).arg(molinfo.name()), CODELOC );
    }
    
    if (not _idx.isNull())
    {
        QList<T> single_res;
        
        single_res.append( residxs.at( _idx.map(residxs.count()) ) );
        
        return single_res;
    }
    else
        return residxs;
}
