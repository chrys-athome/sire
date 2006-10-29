#ifndef SIREFF_ERRORS_H
#define SIREFF_ERRORS_H

#include "SireError/exception.h"

SIRE_BEGIN_HEADER

/**
This file contains the exceptions that can be thrown by the SireFF library.

@author Christopher Woods
*/

namespace SireFF
{

/** This is the base class of all SireFF errors */
class SIREFF_EXPORT sireff_error : public SireError::exception
{
public:
    sireff_error() : exception()
    {}
    
    sireff_error(QString err, QString place = QString::null) : exception(err,place)
    {}
    
    sireff_error(const sireff_error &other) : exception(other)
    {}
    
    ~sireff_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMol::sireff_error";
    }
};


/** This exception is thrown when a request is made of a non-existant 
    forcefield component

    @author Christopher Woods
*/
class SIREFF_EXPORT missing_component : public sireff_error
{
public:
    missing_component() : sireff_error()
    {}
    
    missing_component(QString err, QString place = QString::null) 
              : sireff_error(err,place)
    {}
    
    missing_component(const missing_component &other) : sireff_error(other)
    {}
    
    ~missing_component() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireFF::missing_component";
    }
};

}

Q_DECLARE_METATYPE(SireFF::sireff_error)
Q_DECLARE_METATYPE(SireFF::missing_component)

SIRE_END_HEADER

#endif
