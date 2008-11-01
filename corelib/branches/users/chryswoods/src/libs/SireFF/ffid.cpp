/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "ffid.h"
#include "ffidx.h"
#include "ffname.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireID;

///////
/////// Implementation of FFID
///////

FFID::FFID() : SireID::ID()
{}

FFID::FFID(const FFID &other) : SireID::ID(other)
{}

FFID::~FFID()
{}

Specify<FFID> FFID::operator[](int i) const
{
    return Specify<FFID>(*this, i);
}

Specify<FFID> FFID::operator()(int i) const
{
    return Specify<FFID>(*this, i);
}

Specify<FFID> FFID::operator()(int i, int j) const
{
    return Specify<FFID>(*this, i, j);
}

FFFFID FFID::operator+(const FFID &other) const
{
    return FFFFID(*this, other);
}

FFFFID FFID::operator&&(const FFID &other) const
{
    return this->operator+(other);
}

IDSet<FFID> FFID::operator*(const FFID &other) const
{
    return IDSet<FFID>(*this, other);
}

IDSet<FFID> FFID::operator||(const FFID &other) const
{
    return this->operator*(other);
}

///////
/////// Implementation of FFIdx
///////

FFIdx::FFIdx() : SireID::Index_T_<FFIdx>(), FFID()
{}

FFIdx::FFIdx(qint32 idx) : SireID::Index_T_<FFIdx>(idx), FFID()
{}

FFIdx::FFIdx(const FFIdx &other) : SireID::Index_T_<FFIdx>(other), FFID(other)
{}

FFIdx::~FFIdx()
{}

FFIdx FFIdx::null()
{
    return FFIdx();
}

bool FFIdx::isNull() const
{
    return SireID::Index_T_<FFIdx>::isNull();
}

uint FFIdx::hash() const
{
    return SireID::Index_T_<FFIdx>::hash();
}

QString FFIdx::toString() const
{
    return QString("FFIdx(%1)").arg(_idx);
}

FFIdx& FFIdx::operator=(const FFIdx &other)
{
    SireID::IndexBase::operator=(other);
    FFID::operator=(other);
    return *this;
}

bool FFIdx::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<FFIdx>(*this, other);
}

///////
/////// Implementation of FFName
///////

FFName::FFName() : SireID::Name(), FFID()
{}

FFName::FFName(const QString &name) : SireID::Name(name), FFID()
{}

FFName::FFName(const FFName &other) : SireID::Name(other), FFID(other)
{}

FFName::~FFName()
{}

bool FFName::isNull() const
{
    return SireID::Name::isNull();
}

uint FFName::hash() const
{
    return qHash(_name);
}

QString FFName::toString() const
{
    return QString("FFName('%1')").arg(_name);
}

FFName& FFName::operator=(const FFName &other)
{
    SireID::Name::operator=(other);
    FFID::operator=(other);
    return *this;
}

bool FFName::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<FFName>(*this, other);
}

bool FFName::operator==(const FFName &other) const
{
    return _name == other._name;
}

bool FFName::operator!=(const FFName &other) const
{
    return _name != other._name;
}

///////
///////

template class Specify<FFID>;
template class IDSet<FFID>;

static const RegisterMetaType< Specify<FFID> > r_specify_ffid;
static const RegisterMetaType< IDSet<FFID> > r_idset_ffid;
