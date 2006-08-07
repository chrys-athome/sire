#ifndef SIREFF_FORCEFIELD_H
#define SIREFF_FORCEFIELD_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceField;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceField&);
QDataStream& operator>>(QDataStream&, SireFF::ForceField&);

namespace SireFF
{

/**
An ForceField is the virtual base class of all forcefields. A forcefield is not handled directly by the code - rather it is handled via an FFGroup, which represents a group of forcefields, with the FFGroup holding the  forcefields via FFWrappers  (a templated class that handles all of the energy bookkeeping).
 
@author Christopher Woods
*/
class SIREFF_EXPORT ForceField
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    ForceField();
    virtual ~ForceField();

    /** Return the class name of the forcefield */
    virtual const char* what() const=0;
};

}

SIRE_END_HEADER

#endif
