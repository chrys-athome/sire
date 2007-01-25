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

#include <QObject>

#include "atominfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AtomInfo> r_atominfo;

/** Serialise an AtomInfo to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomInfo &atominfo)
{
    writeHeader(ds, r_atominfo, 1) << atominfo.number() << atominfo.index()
                                   << atominfo.element();

    return ds;
}

/** Deserialise an AtomInfo from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomInfo &atominfo)
{
    VersionID v = readHeader(ds, r_atominfo);

    if (v == 1)
    {
        ds >> atominfo.atmnum >> atominfo.index() >> atominfo.element();
    }
    else
        throw version_error(v, "1", r_atominfo, CODELOC);

    return ds;
}

/** Construct a null AtomInfo */
AtomInfo::AtomInfo() : AtomIndex(), Element(), atmnum(0)
{}

/** Copy constructor */
AtomInfo::AtomInfo(const AtomInfo &other)
     : AtomIndex(other), Element(other), atmnum(other.atmnum)
{}

/** Construct a new AtomInfo, specifying its details... */
AtomInfo::AtomInfo(const AtomIndex &idx, const Element &element)
     : AtomIndex(idx), Element(element), atmnum(0)
{}

/** Construct a new AtomInfo, specifying its details... */
AtomInfo::AtomInfo(const AtomIndex &atm) : AtomIndex(atm), Element(atm.name()), atmnum(0)
{}

/** Construct a copy of the AtomInfo, using a new residue number 'resnum' */
AtomInfo::AtomInfo(const AtomInfo &other, ResNum resnum)
     : AtomIndex(other.name(),resnum), Element(other), atmnum(other.atmnum)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
AtomInfo::AtomInfo(AtomNum num, const AtomIndex &atomindex, const Element &element)
     : AtomIndex(atomindex), Element(element), atmnum(num)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
AtomInfo::AtomInfo(AtomNum atm, const AtomInfo &other)
     : AtomIndex(other), Element(other), atmnum(atm)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
AtomInfo::AtomInfo(AtomNum atm, const AtomInfo &other, ResNum resnum)
     : AtomIndex(other.name(),resnum), Element(other), atmnum(atm)
{}

/** Convienience constructors - these are used to allow string based creation of
    AtomInfos. This is particularly helpful when scripting the creation of AtomInfos.

    Create an AtomInfo called 'name' in residue 0, with element guessed from 'name' and
    coordinates (0,0,0) and atom number 0.
*/
AtomInfo::AtomInfo(const QString &name) : AtomIndex(name), Element(name), atmnum(0)
{}

/** Convienience constructors - these are used to allow string based creation of
    AtomInfos. This is particularly helpful when scripting the creation of AtomInfos.

    Create an AtomInfo called 'name' in residue 0, with element guessed from 'elem' and
    coordinates (0,0,0) and atom number 0.
*/
AtomInfo::AtomInfo(const QString &name, const QString &elem)
     : AtomIndex(name), Element(elem), atmnum(0)
{}

/** Convienience constructors - these are used to allow string based creation of
    AtomInfos. This is particularly helpful when scripting the creation of AtomInfos.

    Create an AtomInfo called 'name' in residue resnum, with element guessed from 'name' and
    coordinates (0,0,0) and atom number 0.
*/
AtomInfo::AtomInfo(const QString &name, ResNum resnum)
     : AtomIndex(name,resnum), Element(name), atmnum(0)
{}

/** Convienience constructors - these are used to allow string based creation of
    AtomInfos. This is particularly helpful when scripting the creation of AtomInfos.

    Create an AtomInfo called 'name' in residue resnum, with element guessed from 'elem' and
    coordinates (0,0,0) and atom number 0.
*/
AtomInfo::AtomInfo(const QString &name, ResNum resnum, const QString &elem)
     : AtomIndex(name,resnum), Element(elem), atmnum(0)
{}

/** Convienience constructors - this allows easy changing of the name of an AtomInfo */
AtomInfo::AtomInfo(const QString &name, const AtomInfo &other)
     : AtomIndex(name,other.resNum()), Element(other), atmnum(other.atmnum)
{}

/** Convienience constructors - these are used to allow string based creation of
    AtomInfos. This is particularly helpful when scripting the creation of AtomInfos.

    Create an AtomInfo called 'name' in residue 0, with element guessed from 'name' and
    coordinates (0,0,0) and atom number num.
*/
AtomInfo::AtomInfo(AtomNum num, const QString &name)
         : AtomIndex(name), Element(name), atmnum(num)
{}

/** Convienience constructors - these are used to allow string based creation of
    AtomInfos. This is particularly helpful when scripting the creation of AtomInfos.

    Create an AtomInfo called 'name' in residue 0, with element guessed from 'elem' and
    coordinates (0,0,0) and atom number num.
*/
AtomInfo::AtomInfo(AtomNum num, const QString &name, const QString &elem)
         : AtomIndex(name), Element(elem), atmnum(num)
{}

/** Destructor */
AtomInfo::~AtomInfo()
{}

/** Return a string representation of the AtomInfo */
QString AtomInfo::toString() const
{
    return QObject::tr("[%1|%2|%3]")
             .arg(this->name(), this->resNum().toString(), this->symbol());
}
