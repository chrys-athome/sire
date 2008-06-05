#ifndef SIREMM_ERRORS_H
#define SIREMM_ERRORS_H

#include "SireError/exception.h"

SIRE_BEGIN_HEADER

/**
This file contains the exceptions that can be thrown by the SireMM library.

@author Christopher Woods
*/

namespace SireMM
{

/** This is the base class of all SireMM errors */
class SIREMM_EXPORT siremm_error : public SireError::exception
{
public:
    siremm_error() : exception()
    {}
    
    siremm_error(QString err, QString place = QString::null) : exception(err,place)
    {}
    
    siremm_error(const siremm_error &other) : exception(other)
    {}
    
    ~siremm_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMM::siremm_error";
    }
};


/** This exception is thrown when a request is made of a non-existant bond

    @author Christopher Woods
*/
class SIREMM_EXPORT missing_bond : public siremm_error
{
public:
    missing_bond() : siremm_error()
    {}
    
    missing_bond(QString err, QString place = QString::null) 
              : siremm_error(err,place)
    {}
    
    missing_bond(const missing_bond &other) : siremm_error(other)
    {}
    
    ~missing_bond() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMM::missing_bond";
    }
};

/** This exception is thrown when a request is made of a non-existant angle

    @author Christopher Woods
*/
class SIREMM_EXPORT missing_angle : public siremm_error
{
public:
    missing_angle() : siremm_error()
    {}
    
    missing_angle(QString err, QString place = QString::null) 
              : siremm_error(err,place)
    {}
    
    missing_angle(const missing_angle &other) : siremm_error(other)
    {}
    
    ~missing_angle() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMM::missing_angle";
    }
};

/** This exception is thrown when a request is made of a non-existant dihedral

    @author Christopher Woods
*/
class SIREMM_EXPORT missing_dihedral : public siremm_error
{
public:
    missing_dihedral() : siremm_error()
    {}
    
    missing_dihedral(QString err, QString place = QString::null) 
              : siremm_error(err,place)
    {}
    
    missing_dihedral(const missing_dihedral &other) : siremm_error(other)
    {}
    
    ~missing_dihedral() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMM::missing_dihedral";
    }
};

}

Q_DECLARE_METATYPE(SireMM::missing_bond)
Q_DECLARE_METATYPE(SireMM::missing_angle)
Q_DECLARE_METATYPE(SireMM::missing_dihedral)

SIRE_END_HEADER

#endif
