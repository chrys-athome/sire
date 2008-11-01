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

#ifndef SIREFF_FFID_H
#define SIREFF_FFID_H

#include <QList>

#include "SireID/id.h"

#include "SireID/idset.hpp"
#include "SireID/specify.hpp"

SIRE_BEGIN_HEADER

namespace SireFF
{

using SireID::IDSet;
using SireID::Specify;

class FFIdx;
class FFIdentifier;
class FFName;
class FFFFID;

class ForceFields;

/** The base class of all ForceField identifiers

    @author Christopher Woods
*/
class SIREFF_EXPORT FFID : public SireID::ID
{
public:
    typedef FFIdx Index;
    typedef FFIdentifier Identifier;
    typedef ForceFields SearchObject;

    FFID();
    FFID(const FFID &other);

    virtual ~FFID();
    
    static const char* typeName()
    {
        return "SireFF::FFID";
    }
    
    virtual FFID* clone() const=0;
    
    Specify<FFID> operator[](int i) const;
    Specify<FFID> operator()(int i) const;
    Specify<FFID> operator()(int i, int j) const;
    
    FFFFID operator+(const FFID &other) const;
    FFFFID operator&&(const FFID &other) const;
    
    IDSet<FFID> operator*(const FFID &other) const;
    IDSet<FFID> operator||(const FFID &other) const;
    
    virtual QList<FFIdx> map(const ForceFields &ffields) const=0;
};

}

Q_DECLARE_METATYPE( SireID::Specify<SireFF::FFID> )
Q_DECLARE_METATYPE( SireID::IDSet<SireFF::FFID> )

SIRE_EXPOSE_CLASS( SireFF::FFID )
SIRE_EXPOSE_ALIAS( SireID::IDSet<SireFF::FFID>, SireFF::IDSet_FFID_ )
SIRE_EXPOSE_ALIAS( SireID::Specify<SireFF::FFID>, SireFF::Specify_FFID_ )

#ifdef SIRE_INSTANTIATE_TEMPLATES
template class SireID::Specify<SireFF::FFID>;
template class SireID::IDSet<SireFF::FFID>;
#endif

SIRE_END_HEADER

#endif
