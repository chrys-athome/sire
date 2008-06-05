#ifndef SIRECAS_ERRORS_H
#define SIRECAS_ERRORS_H

#include "SireError/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This is the base class of all SireCAS errors */
class SIRECAS_EXPORT sirecas_error : public SireError::exception
{
public:
    sirecas_error() : exception()
    {}
    
    sirecas_error(QString err, QString place = QString::null) 
                  : exception(err,place)
    {}
    
    sirecas_error(const sirecas_error &other) : exception(other)
    {}
    
    ~sirecas_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireCAS::sirecas_error";
    }
};

/** This class is thrown when an unavailable differential is requested */
class unavailable_differential : public sirecas_error
{
public:
    unavailable_differential() : sirecas_error()
    {}
    
    unavailable_differential(QString err, QString place = QString::null) 
              : sirecas_error(err,place)
    {}
    
    unavailable_differential(const unavailable_differential &other) : sirecas_error(other)
    {}
    
    ~unavailable_differential() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireCAS::unavailable_differential";
    }
};

/** This class is thrown when an unavailable integral is requested */
class unavailable_integral : public sirecas_error
{
public:
    unavailable_integral() : sirecas_error()
    {}
    
    unavailable_integral(QString err, QString place = QString::null) 
              : sirecas_error(err,place)
    {}
    
    unavailable_integral(const unavailable_integral &other) : sirecas_error(other)
    {}
    
    ~unavailable_integral() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireCAS::unavailable_integral";
    }
};

/** This class is thrown when an attempt is made to work with an unregistered expression */
class SIRECAS_EXPORT unregistered_expression : public sirecas_error
{
public:
    unregistered_expression() : sirecas_error()
    {}
    
    unregistered_expression(QString err, QString place = QString::null) 
              : sirecas_error(err,place)
    {}
    
    unregistered_expression(const unregistered_expression &other) : sirecas_error(other)
    {}
    
    ~unregistered_expression() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireCAS::unregistered_expression";
    }
};

}

Q_DECLARE_METATYPE(SireCAS::sirecas_error)
Q_DECLARE_METATYPE(SireCAS::unavailable_differential)
Q_DECLARE_METATYPE(SireCAS::unavailable_integral)
Q_DECLARE_METATYPE(SireCAS::unregistered_expression)

SIRE_END_HEADER

#endif
