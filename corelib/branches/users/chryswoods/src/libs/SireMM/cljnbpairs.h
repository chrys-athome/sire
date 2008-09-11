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

#ifndef SIREMM_CLJNBPAIRS_H
#define SIREMM_CLJNBPAIRS_H

#include "atompairs.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJNBPairs;
class CLJScaleFactor;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJNBPairs&);
QDataStream& operator>>(QDataStream&, SireMM::CLJNBPairs&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJScaleFactor&);
QDataStream& operator>>(QDataStream&, SireMM::CLJScaleFactor&);

namespace SireMM
{

/** This is the interatomic scale factor for the coulomb and
    LJ parameters for the intramolecular energy. */
class SIREMM_EXPORT CLJScaleFactor
{

friend QDataStream& ::operator<<(QDataStream&, const CLJScaleFactor&);
friend QDataStream& ::operator>>(QDataStream&, CLJScaleFactor&);

public:
    CLJScaleFactor(double scl=0)
          : cscl(scl), ljscl(scl)
    {}

    CLJScaleFactor(double scale_coul, double scale_lj)
          : cscl(scale_coul), ljscl(scale_lj)
    {}
    
    CLJScaleFactor(const CLJScaleFactor &other)
          : cscl(other.cscl), ljscl(other.ljscl)
    {}
    
    ~CLJScaleFactor()
    {}

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CLJScaleFactor>() );
    }

    const char* what() const
    {
        return CLJScaleFactor::typeName();
    }

    CLJScaleFactor& operator=(const CLJScaleFactor &other)
    {
        cscl = other.cscl;
        ljscl = other.ljscl;
        return *this;
    }

    bool operator==(const CLJScaleFactor &other) const
    {
        return cscl == other.cscl and ljscl == other.ljscl;
    }

    bool operator!=(const CLJScaleFactor &other) const
    {
        return cscl != other.cscl or ljscl != other.ljscl;
    }

    double coulomb() const
    {
        return cscl;
    }
    
    double lj() const
    {
        return ljscl;
    }
 
private:
    /** The coulomb and LJ scale factors */
    double cscl, ljscl;
};

/** This class holds all of the non-bonded scale factors that are used
    to scale the intramolecular atom-atom coulomb and Lennard-Jones
    interactions between pairs of atoms, e.g. for most MM forcefields,
    the scale factors for 1-1, 1-2 and 1-3 pairs are zero, the
    1-4 pairs are scaled by a coulomb and LJ factor (e.g. 0.5 for OPLS)
    and the 1-5 and above pairs are not scaled (i.e. the coulomb and
    LJ factors equal 1)

    @author Christopher Woods
*/
class SIREMM_EXPORT CLJNBPairs 
        : public SireBase::ConcreteProperty< CLJNBPairs, 
                                             AtomPairs<CLJScaleFactor> >
{

friend QDataStream& ::operator<<(QDataStream&, const CLJNBPairs&);
friend QDataStream& ::operator>>(QDataStream&, CLJNBPairs&);

public:
    typedef AtomPairs<CLJScaleFactor>::CGPairs CGPairs;

    CLJNBPairs();

    CLJNBPairs(const MoleculeInfoData &molinfo,
               const CLJScaleFactor &default_scale = CLJScaleFactor(1,1));

    CLJNBPairs(const CLJNBPairs &other);

    ~CLJNBPairs();

    CLJNBPairs& operator=(const CLJNBPairs &other);

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CLJNBPairs>() );
    }
    
    bool operator==(const CLJNBPairs &other) const;
    bool operator!=(const CLJNBPairs &other) const;
};

}

Q_DECLARE_METATYPE(SireMM::CLJScaleFactor)
Q_DECLARE_METATYPE(SireMM::CLJNBPairs)

Q_DECLARE_TYPEINFO( SireMM::CLJScaleFactor, Q_MOVABLE_TYPE );

SIRE_EXPOSE_CLASS( SireMM::CLJScaleFactor )
SIRE_EXPOSE_CLASS( SireMM::CLJNBPairs )
SIRE_EXPOSE_ALIAS( SireMM::AtomPairs<SireMM::CLJScaleFactor>,
                   SireMM::AtomPairs_CLJScaleFactor_ )

SIRE_END_HEADER

#endif
