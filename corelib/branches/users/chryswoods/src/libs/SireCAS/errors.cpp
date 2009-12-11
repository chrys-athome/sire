/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "SireCAS/errors.h"

using namespace SireCAS;
using namespace Siren;

//////////
////////// Implementation of unavailable_differential
//////////

static const RegisterObject<unavailable_differential> r_unavailable_differential;

unavailable_differential::unavailable_differential() 
                 : ImplementsException<unavailable_differential, exception>()
{}

unavailable_differential::unavailable_differential(QString err, QString place)
                 : ImplementsException<unavailable_differential,exception>(err, place)
{}

unavailable_differential::unavailable_differential(const unavailable_differential &other)
                 : ImplementsException<unavailable_differential,exception>(other)
{}

unavailable_differential::~unavailable_differential() throw()
{}

//////////
////////// Implementation of unavailable_integral
//////////

static const RegisterObject<unavailable_integral> r_unavailable_integral;

unavailable_integral::unavailable_integral() 
             : ImplementsException<unavailable_integral, exception>()
{}

unavailable_integral::unavailable_integral(QString err, QString place)
                 : ImplementsException<unavailable_integral,exception>(err, place)
{}

unavailable_integral::unavailable_integral(const unavailable_integral &other)
                 : ImplementsException<unavailable_integral,exception>(other)
{}

unavailable_integral::~unavailable_integral() throw()
{}

//////////
////////// Implementation of unregistered_expression
//////////

static const RegisterObject<unregistered_expression> r_unregistered_expression;

unregistered_expression::unregistered_expression() 
                   : ImplementsException<unregistered_expression, exception>()
{}

unregistered_expression::unregistered_expression(QString err, QString place)
                 : ImplementsException<unregistered_expression,exception>(err, place)
{}

unregistered_expression::unregistered_expression(const unregistered_expression &other)
                 : ImplementsException<unregistered_expression,exception>(other)
{}

unregistered_expression::~unregistered_expression() throw()
{}

//////////
////////// Implementation of invalid_symbol
//////////

static const RegisterObject<invalid_symbol> r_invalid_symbol;

invalid_symbol::invalid_symbol() : ImplementsException<invalid_symbol, exception>()
{}

invalid_symbol::invalid_symbol(QString err, QString place)
                 : ImplementsException<invalid_symbol,exception>(err, place)
{}

invalid_symbol::invalid_symbol(const invalid_symbol &other)
                 : ImplementsException<invalid_symbol,exception>(other)
{}

invalid_symbol::~invalid_symbol() throw()
{}

//////////
////////// Implementation of missing_symbol
//////////

static const RegisterObject<missing_symbol> r_missing_symbol;

missing_symbol::missing_symbol() : ImplementsException<missing_symbol, exception>()
{}

missing_symbol::missing_symbol(QString err, QString place)
                 : ImplementsException<missing_symbol,exception>(err, place)
{}

missing_symbol::missing_symbol(const missing_symbol &other)
                 : ImplementsException<missing_symbol,exception>(other)
{}

missing_symbol::~missing_symbol() throw()
{}

//////////
////////// Implementation of rearrangement_error
//////////

static const RegisterObject<rearrangement_error> r_rearrangement_error;

rearrangement_error::rearrangement_error() 
                 : ImplementsException<rearrangement_error, exception>()
{}

rearrangement_error::rearrangement_error(QString err, QString place)
                 : ImplementsException<rearrangement_error,exception>(err, place)
{}

rearrangement_error::rearrangement_error(const rearrangement_error &other)
                 : ImplementsException<rearrangement_error,exception>(other)
{}

rearrangement_error::~rearrangement_error() throw()
{}
