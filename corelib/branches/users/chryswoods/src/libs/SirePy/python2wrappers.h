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
