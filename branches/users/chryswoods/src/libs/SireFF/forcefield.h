#ifndef SIREFF_FORCEFIELD_H
#define SIREFF_FORCEFIELD_H

#include "SireBase/dynamicsharedptr.hpp"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceField;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceField&);
QDataStream& operator>>(QDataStream&, SireFF::ForceField&);

namespace SireFF
{

class FFBase;

/**
This class is the "user" interface to all forcefield classes. This holds an implicitly 
shared copy of the forcefield class.

The smaller, "worker" interface to all forcefield classes is provided by "FField".

@author Christopher Woods
*/
class SIREFF_EXPORT ForceField
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    ForceField();
    ForceField(const ForceField &other);
    ForceField(const FFBase &ffbase);
    
    ~ForceField();

    ForceField& operator=(const ForceField &other);
    ForceField& operator=(const FFBase &ffbase);

private:
    /** Dynamic shared pointer to the FFBase "user" interface */
    SireBase::DynamicSharedPtr<FFBase> d;
};

}

SIRE_END_HEADER

#endif
