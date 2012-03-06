/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "Conspire/exceptions.h"
#include "Conspire/conspire.hpp"

using namespace Conspire;
using namespace Conspire::Qt;

///////////
/////////// Implementation of parse_error
///////////

REGISTER_SIREN_CLASS( Conspire::parse_error )

parse_error::parse_error() : Exception()
{}

parse_error::parse_error(const String &error, CODELOC_ARGS)
             : Exception(error, CODELOC_PASS_ARGS)
{}

parse_error::parse_error(const parse_error &other) : Exception(other)
{}

parse_error::~parse_error() throw()
{}

void parse_error::throwSelf() const
{
    throw *this;
}
