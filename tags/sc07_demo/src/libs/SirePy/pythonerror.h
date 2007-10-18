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

#ifndef SIREPY_PYTHONERROR_H
#define SIREPY_PYTHONERROR_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SirePy
{

//predeclare 'PythonError_pvt' class, which is used internally
//by PythonError to avoid putting any python stuff into the
//header file...
class PythonError_pvt;

/**
This contains a description of a Python error. One type of error may be that the user has interupted the Python script!
 
@author Christopher Woods
*/
class SIREPY_EXPORT PythonError
{
public:
    PythonError();
    PythonError(const PythonError &err);
    ~PythonError();

    /** Copy assignment operator */
    const PythonError& operator=(const PythonError &err);
    
    /** Equivalence operator */
    bool operator==(const PythonError &err);
    bool operator!=(const PythonError &err);
        
    /** Return whether or not this is an empty error (e.g. no error) */
    bool isEmpty() const;

    /** Get and return a traceback string from this error */
    QString traceback();

    /** Return the type of this error */
    QString type();
    
    /** Return the value of this error */
    QString value();
     
    /** Debug the python error - print out all of the information and 
        backtrace */
    void debug();
       
    /** Construct and return a PythonError using the Python API */
    static PythonError fromPythonAPI();
    
protected:
    /** A string containing the detail of the error */
    QString errstring;

    /** A pointer to the PythonError_pvt class that is used
        to hold the Python specific objects (thus preventing
        the need to include python header files in this header) */
    PythonError_pvt *pvterror;
    
    /** Whether or not this is an empty error (e.g. no error at all!) */
    bool isempty;
};

}

SIRE_END_HEADER

#endif
