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

#include "qhash_sirecas.h"

#include "values.h"
#include "symbol.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

static const RegisterMetaType<Values> r_values;

/** Serialise to a binary data stream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Values &values)
{
    writeHeader(ds, r_values, 1) << values.vals;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Values &values)
{
    VersionID v = readHeader(ds, r_values);

    if (v == 1)
    {
        ds >> values.vals;
    }
    else
        throw version_error(v, "1", r_values, CODELOC);

    return ds;
}

/** Construct an empty set of values */
Values::Values()
{}

/** Construct from a list of values */
Values::Values(const QList<SymbolValue> &values)
{
    for (QList<SymbolValue>::const_iterator it = values.begin();
         it != values.end();
         ++it)
    {
        add(*it);
    }
}

/** Copy constructor */
Values::Values(const Values &other) : vals(other.vals)
{}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0)
{
    add(val0);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1)
{
    add(val0);
    add(val1);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2)
{
    add(val0);
    add(val1);
    add(val2);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6, const SymbolValue &val7)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
    add(val7);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
    add(val7);
    add(val8);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8,
                 const SymbolValue &val9)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
    add(val7);
    add(val8);
    add(val9);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1)
{
    add(val0,val1);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2)
{
    add(val0,val1,val2);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3)
{
    add(val0,val1,val2,val3);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4)
{
    add(val0,val1,val2,val3,val4);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5)
{
    add(val0,val1,val2,val3,val4,val5);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6)
{
    add(val0,val1,val2,val3,val4,val5,val6);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6, const SymbolValue &val7)
{
    add(val0,val1,val2,val3,val4,val5,val6,val7);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8)
{
    add(val0,val1,val2,val3,val4,val5,val6,val7,val8);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8,
               const SymbolValue &val9)
{
    add(val0,val1,val2,val3,val4,val5,val6,val7,val8,val9);
}

/** Destructor */
Values::~Values()
{}

/** Return the value of the Symbol with ID 'id', or 0.0 if there is no such symbol */
double Values::value(const Symbol &sym) const
{
    return vals.value(sym.ID(),0.0);
}

/** Add the contents of 'other' to this set - this overwrites any
    existing values that are also in 'other' */
Values& Values::operator+=(const Values &other)
{
    if (other.vals.isEmpty())
        return *this;
    else if (vals.isEmpty())
    {
        vals = other.vals;
        return *this;
    }
    else
    {
        vals.reserve( vals.count() + other.vals.count() );
    
        for (QHash<SymbolID,double>::const_iterator it = other.vals.begin();
             it != other.vals.end();
             ++it)
        {
            vals.insert( it.key(), it.value() );
        }
        
        return *this;
    }
}
