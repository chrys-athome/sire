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

/** This class is thrown when an attempt is made to work with an invalid symbol */
class SIRECAS_EXPORT invalid_symbol : public sirecas_error
{
public:
    invalid_symbol() : sirecas_error()
    {}

    invalid_symbol(QString err, QString place = QString::null)
              : sirecas_error(err,place)
    {}

    invalid_symbol(const invalid_symbol &other) : sirecas_error(other)
    {}

    ~invalid_symbol() throw()
    {}

    const char* what() const throw()
    {
        return "SireCAS::invalid_symbol";
    }
};

}

Q_DECLARE_METATYPE(SireCAS::sirecas_error)
Q_DECLARE_METATYPE(SireCAS::unavailable_differential)
Q_DECLARE_METATYPE(SireCAS::unavailable_integral)
Q_DECLARE_METATYPE(SireCAS::unregistered_expression)

SIRE_END_HEADER

#endif
