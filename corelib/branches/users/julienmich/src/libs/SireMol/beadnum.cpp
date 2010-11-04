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

#include "beadnum.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireID;
using namespace SireStream;

static const RegisterMetaType<BeadNum> r_beadnum;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const BeadNum &beadnum)
{
    writeHeader(ds, r_beadnum, 1);
    
    ds << static_cast<const SireID::Number&>(beadnum);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, BeadNum &beadnum)
{
    VersionID v = readHeader(ds, r_beadnum);
    
    if (v == 1)
    {
        ds >> static_cast<SireID::Number&>(beadnum);
    }
    else
        throw version_error( v, "1", r_beadnum, CODELOC );
        
    return ds;
}

BeadNum::BeadNum() : SireID::Number()
{}

BeadNum::BeadNum(quint32 num) : SireID::Number(num)
{}

BeadNum::BeadNum(const BeadNum &other) : SireID::Number(other)
{}

BeadNum::~BeadNum()
{}

bool BeadNum::isNull() const
{
    return Number::isNull();
}

uint BeadNum::hash() const
{
    return ::qHash( static_cast<const Number&>(*this) );
}

QString BeadNum::toString() const
{
    return QString("BeadNum(%1)").arg(_num);
}

BeadNum& BeadNum::operator=(const BeadNum &other)
{
    SireID::Number::operator=(other);
    return *this;
}

bool BeadNum::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<BeadNum>(*this, other);
}

bool BeadNum::operator==(const BeadNum &other) const
{
    return _num == other._num;
}

bool BeadNum::operator!=(const BeadNum &other) const
{
    return _num != other._num;
}

bool BeadNum::operator<(const BeadNum &other) const
{
    return _num < other._num;
}

bool BeadNum::operator<=(const BeadNum &other) const
{
    return _num <= other._num;
}

bool BeadNum::operator>(const BeadNum &other) const
{
    return _num > other._num;
}

bool BeadNum::operator>=(const BeadNum &other) const
{
    return _num >= other._num;
}

BeadNum* BeadNum::clone() const
{
    return new BeadNum(*this);
}

const char* BeadNum::typeName()
{
    return QMetaType::typeName( qMetaTypeId<BeadNum>() );
}

const char* BeadNum::what() const
{
    return BeadNum::typeName();
}

