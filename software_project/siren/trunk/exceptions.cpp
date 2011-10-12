/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/exceptions.h"

#include "Siren/siren.hpp"

using namespace Siren;

///////////
/////////// Implementation of invalid_cast
///////////

REGISTER_SIREN_CLASS( Siren::invalid_cast )

invalid_cast::invalid_cast() : Exception()
{}

invalid_cast::invalid_cast(const String &error, CODELOC_ARGS)
             : Exception(error, CODELOC_PASS_ARGS)
{}

invalid_cast::invalid_cast(const invalid_cast &other) : Exception(other)
{}

invalid_cast::~invalid_cast() throw()
{}

void invalid_cast::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of invalid_index
///////////

REGISTER_SIREN_CLASS( Siren::invalid_index )

invalid_index::invalid_index() : Exception()
{}

invalid_index::invalid_index(const String &error, CODELOC_ARGS)
              : Exception(error, CODELOC_PASS_ARGS)
{}

invalid_index::invalid_index(const invalid_index &other) : Exception(other)
{}

invalid_index::~invalid_index() throw()
{}

void invalid_index::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of invalid_key
///////////

REGISTER_SIREN_CLASS( Siren::invalid_key )

invalid_key::invalid_key() : Exception()
{}

invalid_key::invalid_key(const String &error, CODELOC_ARGS)
             : Exception(error, CODELOC_PASS_ARGS)
{}

invalid_key::invalid_key(const invalid_key &other) : Exception(other)
{}

invalid_key::~invalid_key() throw()
{}

void invalid_key::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of numeric_overflow
///////////

REGISTER_SIREN_CLASS( Siren::numeric_overflow )

numeric_overflow::numeric_overflow() : Exception()
{}

numeric_overflow::numeric_overflow(const String &error, CODELOC_ARGS)
                 : Exception(error, CODELOC_PASS_ARGS)
{}

numeric_overflow::numeric_overflow(const numeric_overflow &other) : Exception(other)
{}

numeric_overflow::~numeric_overflow() throw()
{}

void numeric_overflow::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of incomplete_code
///////////

REGISTER_SIREN_CLASS( Siren::incomplete_code )

incomplete_code::incomplete_code() : Exception()
{}

incomplete_code::incomplete_code(const String &error, CODELOC_ARGS)
                : Exception(error, CODELOC_PASS_ARGS)
{}

incomplete_code::incomplete_code(const incomplete_code &other) : Exception(other)
{}

incomplete_code::~incomplete_code() throw()
{}

void incomplete_code::throwSelf() const
{
    throw *this;
}
