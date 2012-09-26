/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#ifndef SIRESIM_SIMPARAMS_H
#define SIRESIM_SIMPARAMS_H

#include "SireBase/property.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace SireSim
{
class Sim;
class SimParams;
}

QDataStream& operator<<(QDataStream&, const SireSim::Sim&);
QDataStream& operator>>(QDataStream&, SireSim::Sim&);

QDataStream& operator<<(QDataStream&, const SireSim::SimParams&);
QDataStream& operator>>(QDataStream&, SireSim::SimParams&);

namespace SireSim
{

/** This is the base class of the helper classes that are
    used to run the template simulations available in Sire.
    These provide a simple, high-level interface that allow
    users to quickly and easily use Sire to run common types
    of simulation
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT Sim : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Sim&);
friend QDataStream& ::operator>>(QDataStream&, Sim&);

public:
    Sim();
    Sim(const Sim &other);
    
    virtual ~Sim();

    static const char* typeName();
    
    virtual Sim* clone() const=0;
};

/** This is the base class of the helper classes that are
    used to store user parameters for a simulation. These are used
    to allow the user to configure a simulation that is about to 
    run. Normally, you should provide a SimParams helper class for
    each simulation class that you create.
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT SimParams : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const SimParams&);
friend QDataStream& ::operator>>(QDataStream&, SimParams&);

public:
    SimParams();
    SimParams(const SimParams &other);
    
    virtual ~SimParams();
    
    static const char* typeName();
    
    virtual SimParams* clone() const=0;
};

}

SIRE_EXPOSE_CLASS( SireSim::Sim )
SIRE_EXPOSE_CLASS( SireSim::SimParams )

#endif
