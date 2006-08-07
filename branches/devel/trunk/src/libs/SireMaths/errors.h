#ifndef SIREMATHS_ERRORS_H
#define SIREMATHS_ERRORS_H

#include "SireError/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** This is the base class of all SireMaths errors */
class SIREMATHS_EXPORT siremaths_error : public SireError::exception
{
public:
    siremaths_error() : exception()
    {}
    
    siremaths_error(QString err, QString place = QString::null) 
                  : exception(err,place)
    {}
    
    siremaths_error(const siremaths_error &other) : exception(other)
    {}
    
    ~siremaths_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMaths::siremaths_error";
    }
};

/** This class represents a general maths error */
class SIREMATHS_EXPORT math_error : public siremaths_error
{
public:
    math_error() : siremaths_error()
    {}
    
    math_error(QString err, QString place = QString::null) 
              : siremaths_error(err,place)
    {}
    
    math_error(const math_error &other) : siremaths_error(other)
    {}
    
    ~math_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMaths::math_error";
    }
};

/** This class represents a domain error */
class SIREMATHS_EXPORT domain_error : public siremaths_error
{
public:
    domain_error() : siremaths_error()
    {}
    
    domain_error(QString err, QString place = QString::null) 
              : siremaths_error(err,place)
    {}
    
    domain_error(const domain_error &other) : siremaths_error(other)
    {}
    
    ~domain_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireMaths::domain_error";
    }
};

}

Q_DECLARE_METATYPE(SireMaths::siremaths_error)
Q_DECLARE_METATYPE(SireMaths::math_error)
Q_DECLARE_METATYPE(SireMaths::domain_error)

SIRE_END_HEADER

#endif
