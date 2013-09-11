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

#include "Siren/bytearray.h"
#include "Siren/siren.hpp"

using namespace Siren;

REGISTER_SIREN_CLASS( Siren::ByteArray )

/** Construct an empty array */
ByteArray::ByteArray() : Object()
{}

/** Construct from the passed character array */
ByteArray::ByteArray(const char *str) : Object(), d(str)
{}

/** Construct from the first 'size' bytes of the passed
    character array */
ByteArray::ByteArray(const char *data, int size)
          : Object(), d(data, size)
{}

#ifdef SIREN_QT_SUPPORT
ByteArray::ByteArray(const QByteArray &data) : Object(), d(data)
{}

ByteArray::operator QByteArray() const
{
    return d;
}
#endif

/** Copy constructor */
ByteArray::ByteArray(const ByteArray &other)
          : Object(other), d(other.d)
{}

/** Destructor */
ByteArray::~ByteArray()
{}

/** Copy assignment operator */
void ByteArray::copy_object(const ByteArray &other)
{
    d = other.d;
    super::copy_object(other);
}

/** Comparison operator */
bool ByteArray::compare_object(const ByteArray &other) const
{
    return d == other.d and super::compare_object(other);
}

/** Return the ith character in the array */
char ByteArray::at(int i) const
{
    return d.at(i);
}

/** Return the raw binary array */
const char* ByteArray::data() const
{
    return d.data();
}

/** Return the size of the array */
int ByteArray::count() const
{
    return d.count();
}

/** Return the size of the array */
int ByteArray::size() const
{
    return d.size();
}

/** Return the size of the array */
int ByteArray::length() const
{
    return d.length();
}

/** Return whether or not the byte array is empty */
bool ByteArray::isEmpty() const
{
    return d.isEmpty();
}

/** Return the byte array encoded using base64 encoding */
ByteArray ByteArray::toBase64() const
{
    return ByteArray( d.toBase64() );
}

/** Return a byte array that has been decoded from base64 encoding */
ByteArray ByteArray::fromBase64(const ByteArray &data)
{
    return ByteArray( QByteArray::fromBase64(data) );
}
