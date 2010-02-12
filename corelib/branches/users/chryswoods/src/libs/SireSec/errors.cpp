/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "SireSec/errors.h"

using namespace SireSec;
using namespace Siren;

//////////
////////// Implementation of key_is_private
//////////

static const RegisterObject<key_is_private> r_key_is_private;

key_is_private::key_is_private() 
               : ImplementsException<key_is_private, exception>()
{}

key_is_private::key_is_private(QString err, QString place)
               : ImplementsException<key_is_private,exception>(err, place)
{}

key_is_private::key_is_private(const key_is_private &other)
               : ImplementsException<key_is_private,exception>(other)
{}

key_is_private::~key_is_private() throw()
{}

//////////
////////// Implementation of missing_key
//////////

static const RegisterObject<missing_key> r_missing_key;

missing_key::missing_key() 
               : ImplementsException<missing_key, exception>()
{}

missing_key::missing_key(QString err, QString place)
               : ImplementsException<missing_key,exception>(err, place)
{}

missing_key::missing_key(const missing_key &other)
               : ImplementsException<missing_key,exception>(other)
{}

missing_key::~missing_key() throw()
{}

//////////
////////// Implementation of invalid_key
//////////

static const RegisterObject<invalid_key> r_invalid_key;

invalid_key::invalid_key() 
               : ImplementsException<invalid_key, exception>()
{}

invalid_key::invalid_key(QString err, QString place)
               : ImplementsException<invalid_key,exception>(err, place)
{}

invalid_key::invalid_key(const invalid_key &other)
               : ImplementsException<invalid_key,exception>(other)
{}

invalid_key::~invalid_key() throw()
{}

//////////
////////// Implementation of crypt_error
//////////

static const RegisterObject<crypt_error> r_crypt_error;

crypt_error::crypt_error() 
               : ImplementsException<crypt_error, exception>()
{}

crypt_error::crypt_error(QString err, QString place)
               : ImplementsException<crypt_error,exception>(err, place)
{}

crypt_error::crypt_error(const crypt_error &other)
               : ImplementsException<crypt_error,exception>(other)
{}

crypt_error::~crypt_error() throw()
{}
