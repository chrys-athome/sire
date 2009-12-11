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

#include "Siren/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This exception is thrown when an unavailable differential is requested */
class SIRECAS_EXPORT unavailable_differential
        : public Siren::ImplementsException<unavailable_differential,Siren::exception>
{
public:
    unavailable_differential();

    unavailable_differential(QString err, QString place = QString::null);

    unavailable_differential(const unavailable_differential &other);

    ~unavailable_differential() throw();
};

/** This class is thrown when an unavailable integral is requested */
class SIRECAS_EXPORT unavailable_integral
        : public Siren::ImplementsException<unavailable_integral,Siren::exception>
{
public:
    unavailable_integral();

    unavailable_integral(QString err, QString place = QString::null);

    unavailable_integral(const unavailable_integral &other);

    ~unavailable_integral() throw();
};

/** This class is thrown when an attempt is made to work with an unregistered expression */
class SIRECAS_EXPORT unregistered_expression
        : public Siren::ImplementsException<unregistered_expression,Siren::exception>
{
public:
    unregistered_expression();

    unregistered_expression(QString err, QString place = QString::null);

    unregistered_expression(const unregistered_expression &other);

    ~unregistered_expression() throw();
};

/** This class is thrown when an attempt is made to work with an invalid symbol */
class SIRECAS_EXPORT invalid_symbol
        : public Siren::ImplementsException<invalid_symbol,Siren::exception>
{
public:
    invalid_symbol();

    invalid_symbol(QString err, QString place = QString::null);

    invalid_symbol(const invalid_symbol &other);

    ~invalid_symbol() throw();
};

/** This class is thrown when an expression is missing a required symbol */
class SIRECAS_EXPORT missing_symbol
        : public Siren::ImplementsException<missing_symbol,Siren::exception>
{
public:
    missing_symbol();

    missing_symbol(QString err, QString place = QString::null);

    missing_symbol(const missing_symbol &other);

    ~missing_symbol() throw();
};

/** This class is thrown when an attempt is made to rearrange
    an equation in a way that is not possible */
class SIRECAS_EXPORT rearrangement_error
        : public Siren::ImplementsException<rearrangement_error,Siren::exception>
{
public:
    rearrangement_error();

    rearrangement_error(QString err, QString place = QString::null);

    rearrangement_error(const rearrangement_error &other);

    ~rearrangement_error() throw();
};

}

Q_DECLARE_METATYPE(SireCAS::unavailable_differential)
Q_DECLARE_METATYPE(SireCAS::unavailable_integral)
Q_DECLARE_METATYPE(SireCAS::unregistered_expression)
Q_DECLARE_METATYPE(SireCAS::invalid_symbol)
Q_DECLARE_METATYPE(SireCAS::missing_symbol)
Q_DECLARE_METATYPE(SireCAS::rearrangement_error)

SIRE_END_HEADER

#endif
