#ifndef SIRETEST_RUNTESTS_H
#define SIRETEST_RUNTESTS_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This is a simple class that I use to run my tests.
 
@author Christopher Woods
*/
class SIRETEST_EXPORT RunTests
{
public:
    RunTests();
    ~RunTests();

    void run();
    
};

}

SIRE_END_HEADER

#endif
