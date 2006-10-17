#ifndef SIREFF_FFBASE_H
#define SIREFF_FFBASE_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/**
This class provides the base class of all forcefields. Each forcefield has two interfaces; a "worker" interface, which is a limited interface used by the internal code, and a "user" interface, which has a full interface that allows the user to fully manipulate the forcefield.

This class provides the base class of the "user" interface, and defines the user interface. This class is designed to be held via a dynamic shared pointer in the ForceField class.

@author Christopher Woods
*/
class FFBase
{
public:
    FFBase();

    ~FFBase();

};

}

SIRE_END_HEADER

#endif
