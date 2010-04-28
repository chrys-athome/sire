/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREFF_PROBE_H
#define SIREFF_PROBE_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class Probe;
class NullProbe;
}

QDataStream& operator<<(QDataStream&, const SireFF::Probe&);
QDataStream& operator>>(QDataStream&, SireFF::Probe&);

QDataStream& operator<<(QDataStream&, const SireFF::NullProbe&);
QDataStream& operator>>(QDataStream&, SireFF::NullProbe&);

namespace SireFF
{

/** This is the base class of all Probes - a Probe is an object
    that is used to probe the potential or field at a point in 
    space caused by a ForceField. For example, a Probe could
    be a point unit charge, or a united atom methane point etc.

    @author Christopher Woods
*/
class SIREFF_EXPORT Probe : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Probe&);
friend QDataStream& ::operator>>(QDataStream&, Probe&);

public:
    Probe();
    Probe(const Probe &other);
    
    ~Probe();
    
    static const char* typeName();
    
    virtual Probe* clone() const=0;
    
    static const Probe& null();

protected:
    Probe& operator=(const Probe &other);
    
    bool operator==(const Probe &other) const;
    bool operator!=(const Probe &other) const;
};

/** This is a null Probe */
class NullProbe : public SireBase::ConcreteProperty<NullProbe,Probe>
{

friend QDataStream& ::operator<<(QDataStream&, const NullProbe&);
friend QDataStream& ::operator>>(QDataStream&, NullProbe&);

public:
    NullProbe();
    NullProbe(const NullProbe &other);
    
    ~NullProbe();
    
    NullProbe& operator=(const NullProbe &other);
    
    bool operator==(const NullProbe &other) const;
    bool operator!=(const NullProbe &other) const;
    
    static const char* typeName();
};

typedef SireBase::PropPtr<Probe> ProbePtr;

}

Q_DECLARE_METATYPE( SireFF::NullProbe )

SIRE_EXPOSE_CLASS( SireFF::Probe )
SIRE_EXPOSE_CLASS( SireFF::NullProbe )

SIRE_EXPOSE_PROPERTY( SireFF::PropePtr, SireFF::Probe )

SIRE_END_HEADER

#endif
