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

#include "molid.h"
#include "molidx.h"
#include "molnum.h"
#include "molname.h"

#include "molatomid.h"
#include "molmolid.h"

#include "atomidx.h"

#include "specifymol.h"

#include "molecules.h"
#include "moleculegroup.h"
#include "moleculegroups.h"

#include "mgidx.h"

#include "mover.hpp"

#include "SireBase/incremint.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

///////
/////// Implementation of MolID
///////

/** Constructor */
MolID::MolID() : SireID::ID()
{}

/** Copy constructor */
MolID::MolID(const MolID &other) : SireID::ID(other)
{}

/** Destructor */
MolID::~MolID()
{}

/** Specify the ith molecule that matches this ID */
SpecifyMol MolID::operator[](int i) const
{
    return SpecifyMol(*this, i);
}

/** Specify the ith molecule that matches this ID */
SpecifyMol MolID::operator()(int i) const
{
    return SpecifyMol(*this, i);
}

/** Specify the ith to jth molecules that match this ID */
SpecifyMol MolID::operator()(int i, int j) const
{
    return SpecifyMol(*this, i, j);
}

/** Combine this ID with another molecule ID */
MolMolID MolID::operator+(const MolID &other) const
{
    return MolMolID(*this, other);
}

/** Syntactic sugar for operator+ */
MolMolID MolID::operator&&(const MolID &other) const
{
    return this->operator+(other);
}

/** Search for matching atoms in the matching molecules */
MolAtomID MolID::operator+(const AtomID &other) const
{
    return MolAtomID(*this, other);
}

/** Syntactic sugar for operator+ */
MolAtomID MolID::operator&&(const AtomID &other) const
{
    return this->operator+(other);
}

/** Search for matching molecules using this ID, or other */
IDSet<MolID> MolID::operator*(const MolID &other) const
{
    return IDSet<MolID>(*this, other);
}

/** Syntactic sugar for operator* */
IDSet<MolID> MolID::operator||(const MolID &other) const
{
    return this->operator*(other);
}

void MolID::processMatches(QList<MolNum> &matches, const Molecules &mols) const
{
    if (matches.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "There were no molecules that matched the ID \"%1\".")
                .arg(this->toString()), CODELOC );
}

///////
/////// Implementation of MolIdx
///////

static const RegisterMetaType<MolIdx> r_molidx;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MolIdx &molidx)
{
    writeHeader(ds, r_molidx, 1);
    
    ds << static_cast<const SireID::Index_T_<MolIdx>&>(molidx);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MolIdx &molidx)
{
    VersionID v = readHeader(ds, r_molidx);
    
    if (v == 1)
    {
        ds >> static_cast<SireID::Index_T_<MolIdx>&>(molidx);
    }
    else
        throw version_error( v, "1", r_molidx, CODELOC );
        
    return ds;
}

MolIdx::MolIdx() : SireID::Index_T_<MolIdx>(), MolID()
{}

MolIdx::MolIdx(qint32 idx) : SireID::Index_T_<MolIdx>(idx), MolID()
{}

MolIdx::MolIdx(const MolIdx &other) : SireID::Index_T_<MolIdx>(other), MolID(other)
{}

MolIdx::~MolIdx()
{}
  
MolIdx MolIdx::null()
{
    return MolIdx();
}

bool MolIdx::isNull() const
{
    return SireID::Index_T_<MolIdx>::isNull();
}

uint MolIdx::hash() const
{
    return SireID::Index_T_<MolIdx>::hash();
}

QString MolIdx::toString() const
{
    return QString("MolIdx(%1)").arg(_idx);
}

MolIdx& MolIdx::operator=(const MolIdx &other)
{
    SireID::IndexBase::operator=(other);
    MolID::operator=(other);
    return *this;
}

bool MolIdx::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<MolIdx>(*this, other);
}

QList<MolNum> MolIdx::map(const Molecules &molecules) const
{
    int i = SireID::Index(*this).map( molecules.count() );

    QList<MolNum> molnums;
    
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        if (i == 0)
        {
            molnums.append(it.key());
            break;
        }
    }
    
    BOOST_ASSERT( not molnums.isEmpty() );
    
    return molnums;
}

QList<MolNum> MolIdx::map(const MoleculeGroup &molgroup) const
{
    return molgroup.map(*this);
}

QList<MolNum> MolIdx::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

///////
/////// Implementation of MolNum
///////

static const RegisterMetaType<MolNum> r_molnum;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MolNum &molnum)
{
    writeHeader(ds, r_molnum, 1);
    
    ds << static_cast<const SireID::Number&>(molnum);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MolNum &molnum)
{
    VersionID v = readHeader(ds, r_molnum);
    
    if (v == 1)
    {
        ds >> static_cast<SireID::Number&>(molnum);
    }
    else
        throw version_error( v, "1", r_molnum, CODELOC );
        
    return ds;
}

MolNum::MolNum() : SireID::Number(), MolID()
{}

MolNum::MolNum(quint32 num) : SireID::Number(num), MolID()
{}

MolNum::MolNum(const MolNum &other) : SireID::Number(other), MolID(other)
{}

MolNum::~MolNum()
{}

static Incremint last_num(0);

MolNum MolNum::getUniqueNumber()
{
    return MolNum( last_num.increment() );
}

bool MolNum::isNull() const
{
    return SireID::Number::isNull();
}

uint MolNum::hash() const
{
    return ::qHash( static_cast<const SireID::Number&>(*this) );
}

QString MolNum::toString() const
{
    return QString("MolNum(%1)").arg(_num);
}

MolNum& MolNum::operator=(const MolNum &other)
{
    SireID::Number::operator=(other);
    MolID::operator=(other);
    return *this;
}

bool MolNum::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<MolNum>(*this, other);
}

bool MolNum::operator==(const MolNum &other) const
{
    return _num == other._num;
}

bool MolNum::operator!=(const MolNum &other) const
{
    return _num != other._num;
}

QList<MolNum> MolNum::map(const Molecules &molecules) const
{
    if (not molecules.contains(*this))
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with number %1 in the set of molecules.")
                .arg(this->toString()), CODELOC );
                
    QList<MolNum> molnums;
    molnums.append(*this);
    
    return molnums;
}

QList<MolNum> MolNum::map(const MoleculeGroup &molgroup) const
{
    return molgroup.map(*this);
}

QList<MolNum> MolNum::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

///////
/////// Implementation of MolName
///////

static const RegisterMetaType<MolName> r_molname;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MolName &molname)
{
    writeHeader(ds, r_molname, 1);
    
    ds << static_cast<const SireID::Name&>(molname);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MolName &molname)
{
    VersionID v = readHeader(ds, r_molname);
    
    if (v == 1)
    {
        ds >> static_cast<SireID::Name&>(molname);
    }
    else
        throw version_error( v, "1", r_molname, CODELOC );
        
    return ds;
}

MolName::MolName() : SireID::Name(), MolID()
{}

MolName::MolName(const QString &name) : SireID::Name(name), MolID()
{}

MolName::MolName(const MolName &other) : SireID::Name(other), MolID(other)
{}

MolName::~MolName()
{}

bool MolName::isNull() const
{
    return SireID::Name::isNull();
}

uint MolName::hash() const
{
    return qHash(_name);
}

QString MolName::toString() const
{
    return QString("MolName('%1')").arg(_name);
}

MolName& MolName::operator=(const MolName &other)
{
    SireID::Name::operator=(other);
    MolID::operator=(other);
    return *this;
}

bool MolName::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<MolName>(*this, other);
}

bool MolName::operator==(const MolName &other) const
{
    return _name == other._name;
}

bool MolName::operator!=(const MolName &other) const
{
    return _name != other._name;
}

QList<MolNum> MolName::map(const Molecules &molecules) const
{
    QList<MolNum> molnums;
    
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        if (it.value().name() == *this)
            molnums.append( it.key() );
    }
    
    if (molnums.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with name \"%1\" in the set of molecules.")
                .arg(_name), CODELOC );
                
    return molnums;
}

QList<MolNum> MolName::map(const MoleculeGroup &molgroup) const
{
    return molgroup.map(*this);
}

QList<MolNum> MolName::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

//////
////// Implementation of IDSet<MolID>
//////

/** Null constructor */
IDSet<MolID>::IDSet() : MolID()
{}

/** Add the passed ID to the list */
void IDSet<MolID>::add(const MolID &id)
{
    if (id.isNull())
        return;

    else if (id.isA<MolIdentifier>())
    {
        this->add(id.asA<MolIdentifier>().base());
    }
    else if (id.isA< IDSet<MolID> >())
        ids += id.asA< IDSet<MolID> >().ids;
    else
        ids.insert( MolIdentifier(id) );
}

/** Construct from the passed ID */
IDSet<MolID>::IDSet(const MolID &id) : MolID()
{
    this->add(id);
}

/** Construct from the passed IDs */
IDSet<MolID>::IDSet(const MolID &id0, const MolID &id1) : MolID()
{
    this->add(id0);
    this->add(id1);
}

/** Construct from the passed list of IDs */
IDSet<MolID>::IDSet(const QList<MolIdentifier> &new_ids) : MolID()
{
    for (QList<MolIdentifier>::const_iterator it = new_ids.constBegin();
         it != new_ids.constEnd();
         ++it)
    {
        this->add(it->base());
    }
}

/** Copy constructor */
IDSet<MolID>::IDSet(const IDSet &other) : MolID(other), ids(other.ids)
{}

/** Destructor */
IDSet<MolID>::~IDSet()
{}

/** Is this selection null? */
bool IDSet<MolID>::isNull() const
{
    return ids.isEmpty();
}

/** Return a hash of this identifier */
uint IDSet<MolID>::hash() const
{
    uint h = 0;
    
    for (QSet<MolIdentifier>::const_iterator it = ids.constBegin();
         it != ids.constEnd();
         ++it)
    {
        h += it->hash();
    }
    
    return h;
}
            
/** Return a string representatio of this ID */
QString IDSet<MolID>::toString() const
{
    if (ids.isEmpty())
        return QObject::tr("null");
    else
    {
        QStringList idstrings;
        
        for (QSet<MolIdentifier>::const_iterator it = ids.constBegin();
             it != ids.constEnd();
             ++it)
        {
            idstrings.append( it->toString() );
        }
    
        return idstrings.join( QObject::tr(" and ") );
    }
}

/** Return all of the IDs in this set */
const QSet<MolIdentifier>& IDSet<MolID>::IDs() const
{
    return ids;
}

/** Copy assignment operator */
IDSet<MolID>& IDSet<MolID>::operator=(const IDSet<MolID> &other)
{
    ids = other.ids;
    return *this;
}

/** Copy assignment operator */
IDSet<MolID>& IDSet<MolID>::operator=(const MolID &other)
{
    ids.clear();
    this->add(other);
    
    return *this;
}

/** Comparison operator */
bool IDSet<MolID>::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare< IDSet<MolID> >(*this, other);
}

/** Comparison operator */
bool IDSet<MolID>::operator==(const IDSet<MolID> &other) const
{
    return ids == other.ids;
}

/** Comparison operator */
bool IDSet<MolID>::operator!=(const IDSet<MolID> &other) const
{
    return ids != other.ids;
}

/** Comparison operator */
bool IDSet<MolID>::operator==(const MolID &other) const
{
    return this->operator==( IDSet<MolID>(other) );
}

/** Comparison operator */
bool IDSet<MolID>::operator!=(const MolID &other) const
{
    return this->operator!=( IDSet<MolID>(other) );
}

QList<MolNum> IDSet<MolID>::process(QList<MolNum> molnums) const
{
    
    QSet<MolNum> set;
    set.reserve(molnums.count());
    
    QMutableListIterator<MolNum> it(molnums);
    
    while (it.hasNext())
    {
        it.next();
    
        if (set.contains(it.value()))
            it.remove();
        else
            set.insert(it.value());
    }

    if (molnums.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "There are no molecules that match the ID \"%1\".")
                .arg(this->toString()), CODELOC );

    return molnums;
}

/** Map this ID to the list of indicies that match this ID

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
QList<MolNum> IDSet<MolID>::map(const Molecules &mols) const
{
    if (ids.isEmpty())
        return MolIdentifier().map(mols);
        
    QList<MolNum> molnums;
        
    for (QSet<MolIdentifier>::const_iterator it = ids.constBegin();
         it != ids.constEnd();
         ++it)
    {
        try
        {
            molnums += it->map(mols);
        }
        catch(...)
        {
            //no match
        }
    }

    return this->process(molnums);
}

/** Map this ID to the list of indicies that match this ID

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
QList<MolNum> IDSet<MolID>::map(const MoleculeGroup &molgroup) const
{
    if (ids.isEmpty())
        return MolIdentifier().map(molgroup);
        
    QList<MolNum> molnums;
        
    for (QSet<MolIdentifier>::const_iterator it = ids.constBegin();
         it != ids.constEnd();
         ++it)
    {
        try
        {
            molnums += it->map(molgroup);
        }
        catch(...)
        {
            //no match
        }
    }

    return this->process(molnums);
}

/** Map this ID to the list of indicies that match this ID

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
QList<MolNum> IDSet<MolID>::map(const MolGroupsBase &molgroups) const
{
    if (ids.isEmpty())
        return MolIdentifier().map(molgroups);
        
    QList<MolNum> molnums;
        
    for (QSet<MolIdentifier>::const_iterator it = ids.constBegin();
         it != ids.constEnd();
         ++it)
    {
        try
        {
            molnums += it->map(molgroups);
        }
        catch(...)
        {
            //no match
        }
    }

    return this->process(molnums);
}
