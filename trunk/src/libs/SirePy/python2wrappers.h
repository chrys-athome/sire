#ifndef SIREPY_PYTHON2WRAPPERS_H
#define SIREPY_PYTHON2WRAPPERS_H

// Collection of Python2 wrappers for C++ objects

#include <boost/python.hpp>
using namespace boost::python;

//include the wrappers for the SirePy module
//#include "wrap_SirePy.h"

//now include the wrappers for each of the other modules - note that the SirePy
//library will need to be linked with each of the module's libraries listed below
//#include "SireStream/wrap_SireStream.h"
//#include "SireMaths/wrap_SireMaths.h"
//#include "SireCAS/wrap_SireCAS.h"
//#include "SireBase/wrap_SireBase.h"
//#include "SireDB/wrap_SireDB.h"
//#include "SireMM/wrap_SireMM.h"
//#include "SireError/wrap_SireError.h"
//#include "SireTest/wrap_SireTest.h"
//#include "SireMol/wrap_SireMol.h"
//#include "SireVol/wrap_SireVol.h"
//#include "SireIO/wrap_SireIO.h"
//#include "SireFF/wrap_SireFF.h"

/*
void init_python_modules()
{ 
    //initialise each python module. The 'BOOST_PYTHON_MODULE' macro
    //has created an initmodule() function for each 'module' that is created.
    //As these are statically linked, these functions need to be called.
    init_SireError();
    init_SireStream();
    init_SireMaths();
    init_SireCAS();
    init_SireBase();
    init_SireMol();
    init_SireVol();
    init_SireDB();
    init_SireMM();
    init_SireFF();
    init_SireIO();
    init_SireTest();
    init_SirePy();
}
*/

#endif
