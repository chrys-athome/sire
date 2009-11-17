/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "stream.h"

////////
//////// Implementation of Schema
////////

Schema::Schema(Stream *s, const QString &name, int v)
       : stream(s), type_name(name), version(v)
{
    BOOST_ASSERT( s );
    
    s->incref(type_name);
}

Schema::~Schema()
{
    if (s)
        s->decref(type_name);
}

////////
//////// Implementation of Stream
////////

void Stream::start(const QString &type_name)
{
    refcounts.append( 0 );
}

void Stream::incref(const QString&)
{
    refcounts[ refcounts.count() - 1 ] += 1;
}

void Stream::decref(const QString &type_name)
{
    int last = refcounts().count() - 1;

    refcounts[last] -= 1;
    
    if (refcounts[last] == 0)
    {
        this->end(type_name);
        refcounts.resize(last); 
    }
}
