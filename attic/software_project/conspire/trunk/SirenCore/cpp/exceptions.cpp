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
using namespace Siren::Qt;

///////////
/////////// Implementation of file_error
///////////

REGISTER_SIREN_CLASS( Siren::file_error )

file_error::file_error() : Exception()
{}

file_error::file_error(const String &error, CODELOC_ARGS)
             : Exception(error, CODELOC_PASS_ARGS)
{}

file_error::file_error(const File &f, const String &error, CODELOC_ARGS)
             : Exception(error, CODELOC_PASS_ARGS)
{}

file_error::file_error(const file_error &other) : Exception(other)
{}

file_error::~file_error() throw()
{}

void file_error::throwSelf() const
{
    throw *this;
}

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

///////////
/////////// Implementation of unavailable_class
///////////

REGISTER_SIREN_CLASS( Siren::unavailable_class )

unavailable_class::unavailable_class() : Exception()
{}

unavailable_class::unavailable_class(const String &error, CODELOC_ARGS)
                : Exception(error, CODELOC_PASS_ARGS)
{}

unavailable_class::unavailable_class(const unavailable_class &other) : Exception(other)
{}

unavailable_class::~unavailable_class() throw()
{}

void unavailable_class::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of program_bug
///////////

REGISTER_SIREN_CLASS( Siren::program_bug )

program_bug::program_bug() : Exception()
{}

program_bug::program_bug(const String &error, CODELOC_ARGS)
                : Exception(error, CODELOC_PASS_ARGS)
{}

program_bug::program_bug(const program_bug &other) : Exception(other)
{}

program_bug::~program_bug() throw()
{}

void program_bug::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of standard_exception
///////////

REGISTER_SIREN_CLASS( Siren::standard_exception )

standard_exception::standard_exception() : Exception()
{}

standard_exception::standard_exception(const std::exception &e, CODELOC_ARGS)
                   : Exception( Siren::tr("Caught standard exception %1")
                                    .arg(e.what()), CODELOC_PASS_ARGS)
{}

standard_exception::standard_exception(const String &error, 
                                       const std::exception &e, CODELOC_ARGS)
                   : Exception( Siren::tr("%1. Caught standard exception %2")
                                    .arg(error, e.what()), CODELOC_PASS_ARGS)
{}

standard_exception::standard_exception(const standard_exception &other) 
                   : Exception(other)
{}

standard_exception::~standard_exception() throw()
{}

void standard_exception::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of unknown_exception
///////////

REGISTER_SIREN_CLASS( Siren::unknown_exception )

unknown_exception::unknown_exception() : Exception()
{}

unknown_exception::unknown_exception(CODELOC_ARGS)
                  : Exception( Siren::tr("Caught an unknown exception"),
                               CODELOC_PASS_ARGS )
{}

unknown_exception::unknown_exception(const String &error, CODELOC_ARGS)
                : Exception(Siren::tr("%1. Caught an unknown exception")
                                .arg(error), CODELOC_PASS_ARGS)
{}

unknown_exception::unknown_exception(const unknown_exception &other) : Exception(other)
{}

unknown_exception::~unknown_exception() throw()
{}

void unknown_exception::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of invalid_arg
///////////

REGISTER_SIREN_CLASS( Siren::invalid_arg )

invalid_arg::invalid_arg() : Exception()
{}

invalid_arg::invalid_arg(const String &error, CODELOC_ARGS)
                : Exception(error, CODELOC_PASS_ARGS)
{}

invalid_arg::invalid_arg(const invalid_arg &other) : Exception(other)
{}

invalid_arg::~invalid_arg() throw()
{}

void invalid_arg::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of interupted_thread
///////////

REGISTER_SIREN_CLASS( Siren::interupted_thread )

interupted_thread::interupted_thread() : Exception()
{}

interupted_thread::interupted_thread(const String &error, CODELOC_ARGS)
                  : Exception(error, CODELOC_PASS_ARGS)
{}

interupted_thread::interupted_thread(const interupted_thread &other) : Exception(other)
{}

interupted_thread::~interupted_thread() throw()
{}

void interupted_thread::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of system_error
///////////

REGISTER_SIREN_CLASS( Siren::system_error )

system_error::system_error() : Exception()
{}

system_error::system_error(const String &error, CODELOC_ARGS)
             : Exception(error, CODELOC_PASS_ARGS)
{}

system_error::system_error(const system_error &other) : Exception(other)
{}

system_error::~system_error() throw()
{}

void system_error::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of invalid_state
///////////

REGISTER_SIREN_CLASS( Siren::invalid_state )

invalid_state::invalid_state() : Exception()
{}

invalid_state::invalid_state(const String &error, CODELOC_ARGS)
              : Exception(error, CODELOC_PASS_ARGS)
{}

invalid_state::invalid_state(const invalid_state &other) : Exception(other)
{}

invalid_state::~invalid_state() throw()
{}

void invalid_state::throwSelf() const
{
    throw *this;
}

///////////
/////////// Implementation of unavailable_resource
///////////

REGISTER_SIREN_CLASS( Siren::unavailable_resource )

unavailable_resource::unavailable_resource() : Exception()
{}

unavailable_resource::unavailable_resource(const String &error, CODELOC_ARGS)
                     : Exception(error, CODELOC_PASS_ARGS)
{}

unavailable_resource::unavailable_resource(const unavailable_resource &other) 
                     : Exception(other)
{}

unavailable_resource::~unavailable_resource() throw()
{}

void unavailable_resource::throwSelf() const
{
    throw *this;
}
