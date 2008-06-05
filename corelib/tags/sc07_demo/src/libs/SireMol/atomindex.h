/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOL_ATOMINDEX_H
#define SIREMOL_ATOMINDEX_H

#include <QDataStream>
//need to include QHash and QString or else qHash(SireMol::AtomIndex) will call itself, as
//QString implicitly converts to AtomIndex)
#include <QString>
#include <QHash>

#include <boost/tuple/tuple.hpp>

#include "resnum.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomIndex;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::AtomIndex&);
QDataStream& operator>>(QDataStream&, SireMol::AtomIndex&);

uint qHash(const SireMol::AtomIndex&);

namespace SireMol
{

using boost::tuple;

/**
An AtomIndex is a small class that holds the atom name / residue number combination that is used to uniquely index an atom within a molecule.

@author Christopher Woods
*/
class SIREMOL_EXPORT AtomIndex
{

friend QDataStream& ::operator<<(QDataStream&, const AtomIndex&);
friend QDataStream& ::operator>>(QDataStream&, AtomIndex&);

public:
    AtomIndex(const QString &nm = QString::null, ResNum rnum = ResNum(1)) 
        : nme(nm), resnum(rnum)
    {}
    AtomIndex(const AtomIndex &other) : nme(other.nme), resnum(other.resnum)
    {}
    
    AtomIndex(const tuple<QString,ResNum> &tuple);
    
    ~AtomIndex()
    {}
    
    bool operator==(const AtomIndex &other) const
    {
        return resnum == other.resnum and nme == other.nme;
    }
    
    bool operator!=(const AtomIndex &other) const
    {
        return resnum != other.resnum or nme != other.nme;
    }

    bool operator<=(const AtomIndex &other) const
    {
        return ( resnum < other.resnum ) or
               ( resnum == other.resnum and nme <= other.nme);
    }
    
    bool operator<(const AtomIndex &other) const
    {
        return ( resnum < other.resnum ) or
               ( resnum == other.resnum and nme < other.nme );
    }
    
    bool operator>(const AtomIndex &other) const
    {
        return ( resnum > other.resnum ) or
               ( resnum == other.resnum and nme > other.nme );
    }
    
    bool operator>=(const AtomIndex &other) const
    {
        return ( resnum > other.resnum ) or
               ( resnum == other.resnum and nme >= other.nme );
    }
    
    QString toString() const;
    
    QString name() const
    {
        return nme;
    }
    
    ResNum resNum() const
    {
        return resnum;
    }
    
protected:
    QString nme;
    ResNum resnum;
};

/** Return a reference to the lowest atom */
inline const AtomIndex& lowestAtom(const AtomIndex &atm0, const AtomIndex &atm1)
{
    if (atm0 > atm1)
        return atm1;
    else
        return atm0;
}

inline const AtomIndex& highestAtom(const AtomIndex &atm0, const AtomIndex &atm1)
{
    if (atm0 > atm1)
        return atm0;
    else
        return atm1;
}

}

/** Hash an AtomIndex */
inline uint qHash(const SireMol::AtomIndex &atm)
{
    return ( qHash(atm.name()) << 16 ) | ( qHash(atm.resNum()) | 0x0000FFFF );
}

Q_DECLARE_METATYPE(SireMol::AtomIndex)

SIRE_END_HEADER

#endif
