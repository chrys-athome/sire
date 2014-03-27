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

#include "atommatcher.h"
#include "atomidx.h"
#include "atomname.h"
#include "atomselection.h"
#include "atomidentifier.h"
#include "evaluator.h"
#include "moleculeinfodata.h"
#include "moleculeview.h"

#include "tostring.h"

#include "SireUnits/units.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireUnits;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implmentation of AtomMatcher
/////////

AtomMultiMatcher *null_matcher = 0;

const AtomMultiMatcher& AtomMatcher::null()
{
    if (not null_matcher)
        null_matcher = new AtomMultiMatcher();
    
    return *null_matcher;
}

static const RegisterMetaType<AtomMatcher> r_atommatcher( MAGIC_ONLY,
                                                          "SireMol::AtomMatcher" );

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomMatcher &matcher)
{
    writeHeader(ds, r_atommatcher, 1);
    
    ds << static_cast<const Property&>(matcher);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomMatcher &matcher)
{
    VersionID v = readHeader(ds, r_atommatcher);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(matcher);
    }
    else
        throw version_error(v, "1", r_atommatcher, CODELOC);
        
    return ds;
}

/** Constructor */
AtomMatcher::AtomMatcher() : Property()
{}

/** Copy constructor */
AtomMatcher::AtomMatcher(const AtomMatcher &other) : Property(other)
{}

/** Destructor */
AtomMatcher::~AtomMatcher()
{}

/** Return whether or not this matcher is null (cannot be used for matching) */
bool AtomMatcher::isNull() const
{
    return false;
}

/** Return the matcher that matches using this matcher, and then 'other' (in that order) */
AtomMultiMatcher AtomMatcher::operator+(const AtomMatcher &other) const
{
    return AtomMultiMatcher(*this, other);
}
    
/** Return the matcher that matches using this matcher, and then 'other' (in that order) */
AtomMultiMatcher AtomMatcher::add(const AtomMatcher &other) const
{
    return AtomMultiMatcher(*this, other);
}

/** Return whether or not this match changes the order of number of atoms */
bool AtomMatcher::changesOrder(const MoleculeInfoData &mol0,
                               const MoleculeInfoData &mol1) const
{
    if (mol0.nAtoms() != mol1.nAtoms())
        return true;
    
    QHash<AtomIdx,AtomIdx> map = this->match(mol0, mol1);
    
    if (map.count() != mol0.nAtoms())
        return true;
    
    for (QHash<AtomIdx,AtomIdx>::const_iterator it = map.constBegin();
         it != map.constEnd();
         ++it)
    {
        if (it.key() != it.value())
            return true;
    }
    
    return false;
}

/** Return whether or not this match changes the order or number of viewed atoms */
bool AtomMatcher::changesOrder(const MoleculeView &molview0,
                               const PropertyMap &map0,
                               const MoleculeView &molview1,
                               const PropertyMap &map1) const
{
    const int nats0 = molview0.selection().nSelectedAtoms();
    const int nats1 = molview1.selection().nSelectedAtoms();
    
    if (nats0 != nats1)
        return true;

    QHash<AtomIdx,AtomIdx> map = this->match(molview0,map0,molview1,map1);
    
    if (map.count() != nats0)
        return true;
    
    for (QHash<AtomIdx,AtomIdx>::const_iterator it = map.constBegin();
         it != map.constEnd();
         ++it)
    {
        if (it.key() != it.value())
            return true;
    }
    
    return false;
}

bool AtomMatcher::changesOrder(const MoleculeView &molview0,
                               const MoleculeView &molview1) const
{
    return this->changesOrder(molview0, PropertyMap(), molview1, PropertyMap());
}

bool AtomMatcher::changesOrder(const MoleculeView &molview0,
                               const MoleculeView &molview1,
                               const PropertyMap &map) const
{
    return this->changesOrder(molview0, map, molview1, map);
}

QHash<AtomIdx,AtomIdx> AtomMatcher::match(const MoleculeView &molview0,
                                          const MoleculeView &molview1) const
{
    return this->match(molview0, PropertyMap(), molview1, PropertyMap());
}

QHash<AtomIdx,AtomIdx> AtomMatcher::match(const MoleculeView &molview0,
                                          const MoleculeView &molview1,
                                          const PropertyMap &map) const
{
    return this->match(molview0, map, molview1, map);
}

/** Match atoms based only on the data in the MoleculeInfoData of the molecules. */
QHash<AtomIdx,AtomIdx> AtomMatcher::match(const MoleculeInfoData &mol0,
                                          const MoleculeInfoData &mol1) const
{
    throw SireError::unsupported( QObject::tr(
                "The AtomMatcher \"%1\" does not support matching using "
                "MoleculeInfoData objects only.")
                    .arg(this->toString()), CODELOC );

    return QHash<AtomIdx,AtomIdx>();
}

/////////
///////// Implmentation of AtomIdxMatcher
/////////

static const RegisterMetaType<AtomIdxMatcher> r_idxmatcher;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomIdxMatcher &idxmatcher)
{
    writeHeader(ds, r_idxmatcher, 1);
    ds << static_cast<const AtomMatcher&>(idxmatcher);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomIdxMatcher &idxmatcher)
{
    VersionID v = readHeader(ds, r_idxmatcher);
    
    if (v == 1)
    {
        ds >> static_cast<AtomMatcher&>(idxmatcher);
    }
    else
        throw version_error(v, "1", r_idxmatcher, CODELOC);

    return ds;
}

/** Constructor */
AtomIdxMatcher::AtomIdxMatcher() : ConcreteProperty<AtomIdxMatcher,AtomMatcher>()
{}

/** Copy constructor */
AtomIdxMatcher::AtomIdxMatcher(const AtomIdxMatcher &other)
               : ConcreteProperty<AtomIdxMatcher,AtomMatcher>(other)
{}

/** Destructor */
AtomIdxMatcher::~AtomIdxMatcher()
{}

/** Copy assignment operator */
AtomIdxMatcher& AtomIdxMatcher::operator=(const AtomIdxMatcher &other)
{
    return *this;
}

/** Comparison operator */
bool AtomIdxMatcher::operator==(const AtomIdxMatcher &other) const
{
    return true;
}

/** Comparison operator */
bool AtomIdxMatcher::operator!=(const AtomIdxMatcher &other) const
{
    return false;
}

QString AtomIdxMatcher::toString() const
{
    return QObject::tr("AtomIdxMatcher()");
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomIdxMatcher::match(const MoleculeView &mol0,
                                             const PropertyMap &map0,
                                             const MoleculeView &mol1,
                                             const PropertyMap &map1) const
{
    const AtomSelection sel0 = mol0.selection();
    const AtomSelection sel1 = mol1.selection();
    
    QHash<AtomIdx,AtomIdx> map;
    
    if (sel0.selectedAll() and sel1.selectedAll())
    {
        const int nats = qMin(sel0.nSelectedAtoms(), sel1.nSelectedAtoms());
    
        map.reserve(nats);
    
        for (int i=0; i<nats; ++i)
        {
            map.insert( AtomIdx(i), AtomIdx(i) );
        }
    }
    else
    {
        const int nats1 = mol1.data().info().nAtoms();
    
        foreach (const AtomIdx atom, sel0.selectedAtoms())
        {
            if (atom.value() < nats1)
            {
                if (sel1.selected(atom))
                    map.insert(atom, atom);
            }
        }
    }
    
    return map;
}
/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomIdxMatcher::match(const MoleculeInfoData &mol0,
                                             const MoleculeInfoData &mol1) const
{
    QHash<AtomIdx,AtomIdx> map;
    
    const int nats = qMin(mol0.nAtoms(), mol1.nAtoms());
    
    map.reserve(nats);

    for (int i=0; i<nats; ++i)
    {
        map.insert( AtomIdx(i), AtomIdx(i) );
    }
    
    return map;
}

const char* AtomIdxMatcher::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomIdxMatcher>() );
}

/////////
///////// Implementation of AtomNameMatcher
/////////

static const RegisterMetaType<AtomNameMatcher> r_namematcher;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const AtomNameMatcher &namematcher)
{
    writeHeader(ds, r_namematcher, 1);
    ds << static_cast<const AtomMatcher&>(namematcher);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomNameMatcher &namematcher)
{
    VersionID v = readHeader(ds, r_namematcher);
    
    if (v == 1)
    {
        ds >> static_cast<AtomMatcher&>(namematcher);
    }
    else
        throw version_error(v, "1", r_namematcher, CODELOC);

    return ds;
}

/** Constructor */
AtomNameMatcher::AtomNameMatcher() : ConcreteProperty<AtomNameMatcher,AtomMatcher>()
{}

/** Copy constructor */
AtomNameMatcher::AtomNameMatcher(const AtomNameMatcher &other)
               : ConcreteProperty<AtomNameMatcher,AtomMatcher>(other)
{}

/** Destructor */
AtomNameMatcher::~AtomNameMatcher()
{}

/** Copy assignment operator */
AtomNameMatcher& AtomNameMatcher::operator=(const AtomNameMatcher &other)
{
    return *this;
}

/** Comparison operator */
bool AtomNameMatcher::operator==(const AtomNameMatcher &other) const
{
    return true;
}

/** Comparison operator */
bool AtomNameMatcher::operator!=(const AtomNameMatcher &other) const
{
    return false;
}

QString AtomNameMatcher::toString() const
{
    return QObject::tr("AtomNameMatcher()");
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomNameMatcher::match(const MoleculeView &mol0,
                                              const PropertyMap &map0,
                                              const MoleculeView &mol1,
                                              const PropertyMap &map1) const
{
    QHash<AtomIdx,AtomIdx> map;

    const AtomSelection sel0 = mol0.selection();
    const AtomSelection sel1 = mol1.selection();
    
    if (sel0.selectedAll() and sel1.selectedAll())
    {
        for (int i=0; i<mol0.data().info().nAtoms(); ++i)
        {
            const AtomIdx idx0(i);
        
            const AtomName name = mol0.data().info().name(idx0);
            
            try
            {
                AtomIdx idx1 = mol1.data().info().atomIdx(name);
                map.insert( idx0, idx1 );
            }
            catch(...)
            {}
        }
    }
    else
    {
        foreach (const AtomIdx idx0, sel0.selectedAtoms())
        {
            const AtomName name = mol0.data().info().name(idx0);
            
            try
            {
                AtomIdx idx1 = mol1.data().info().atomIdx(name);
                
                if (sel1.selected(idx1))
                    map.insert(idx0, idx1);
            }
            catch(...)
            {}
        }
    }

    return map;
}
/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomNameMatcher::match(const MoleculeInfoData &mol0,
                                              const MoleculeInfoData &mol1) const
{
    QHash<AtomIdx,AtomIdx> map;

    for (int i=0; i<mol0.nAtoms(); ++i)
    {
        const AtomIdx idx0(i);
    
        const AtomName name = mol0.name(idx0);
        
        try
        {
            AtomIdx idx1 = mol1.atomIdx(name);
            map.insert( idx0, idx1 );
        }
        catch(...)
        {}
    }

    return map;
}

const char* AtomNameMatcher::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomNameMatcher>() );
}

/////////
///////// Implmentation of AtomResultMatcher
/////////

static const RegisterMetaType<AtomResultMatcher> r_resmatcher;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomResultMatcher &resmatcher)
{
    writeHeader(ds, r_resmatcher, 1);
    SharedDataStream sds(ds);
    sds << resmatcher.m << static_cast<const AtomMatcher&>(resmatcher);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomResultMatcher &resmatcher)
{
    VersionID v = readHeader(ds, r_resmatcher);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> resmatcher.m >> static_cast<AtomMatcher&>(resmatcher);
    }
    else
        throw version_error(v, "1", r_resmatcher, CODELOC);

    return ds;
}

/** Constructor */
AtomResultMatcher::AtomResultMatcher() : ConcreteProperty<AtomResultMatcher,AtomMatcher>()
{}

/** Constructor */
AtomResultMatcher::AtomResultMatcher(const QHash<AtomIdx,AtomIdx> &results, bool invert)
                  : ConcreteProperty<AtomResultMatcher,AtomMatcher>(), m(results)
{
    if (invert and not results.isEmpty())
    {
        //invert the map (this allows reverse lookups)
        m.clear();
        m.reserve(results.count());
        
        for (QHash<AtomIdx,AtomIdx>::const_iterator it = results.constBegin();
             it != results.constEnd();
             ++it)
        {
            m.insert( it.value(), it.key() );
        }
    }
}

/** Copy constructor */
AtomResultMatcher::AtomResultMatcher(const AtomResultMatcher &other)
                  : ConcreteProperty<AtomResultMatcher,AtomMatcher>(other),
                    m(other.m)
{}

/** Destructor */
AtomResultMatcher::~AtomResultMatcher()
{}

/** Copy assignment operator */
AtomResultMatcher& AtomResultMatcher::operator=(const AtomResultMatcher &other)
{
    m = other.m;
    return *this;
}

/** Comparison operator */
bool AtomResultMatcher::operator==(const AtomResultMatcher &other) const
{
    return m == other.m;
}

/** Comparison operator */
bool AtomResultMatcher::operator!=(const AtomResultMatcher &other) const
{
    return not operator==(other);
}

bool AtomResultMatcher::isNull() const
{
    return m.isEmpty();
}

QString AtomResultMatcher::toString() const
{
    if (isNull())
        return QObject::tr("AtomResultMatcher::null");
    else
        return QObject::tr("AtomResultMatcher( %1 )").arg(Sire::toString(m));
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomResultMatcher::match(const MoleculeView &mol0,
                                                const PropertyMap &map0,
                                                const MoleculeView &mol1,
                                                const PropertyMap &map1) const
{
    const AtomSelection sel0 = mol0.selection();
    const AtomSelection sel1 = mol1.selection();
    
    QHash<AtomIdx,AtomIdx> map;
    
    const int nats0 = mol0.data().info().nAtoms();
    const int nats1 = mol1.data().info().nAtoms();
    
    for (QHash<AtomIdx,AtomIdx>::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        if (it.key().value() >= 0 and it.key().value() < nats0)
        {
            if (it.value().value() >= 0 and it.value().value() < nats1)
            {
                if (sel0.selected(it.key()) and sel1.selected(it.value()))
                    map.insert(it.key(), it.value());
            }
        }
    }
    
    return map;
}
/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomResultMatcher::match(const MoleculeInfoData &mol0,
                                                const MoleculeInfoData &mol1) const
{
    QHash<AtomIdx,AtomIdx> map;
    
    const int nats0 = mol0.nAtoms();
    const int nats1 = mol1.nAtoms();
    
    for (QHash<AtomIdx,AtomIdx>::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        if (it.key().value() >= 0 and it.key().value() < nats0)
        {
            if (it.value().value() >= 0 and it.value().value() < nats1)
            {
                map.insert(it.key(), it.value());
            }
        }
    }
    
    return map;
}

const char* AtomResultMatcher::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomResultMatcher>() );
}

/////////
///////// Implmentation of AtomMatchInverter
/////////

static const RegisterMetaType<AtomMatchInverter> r_inverter;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomMatchInverter &inverter)
{
    writeHeader(ds, r_inverter, 1);
    SharedDataStream sds(ds);
    sds << inverter.m << static_cast<const AtomMatcher&>(inverter);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomMatchInverter &inverter)
{
    VersionID v = readHeader(ds, r_inverter);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> inverter.m >> static_cast<AtomMatcher&>(inverter);
    }
    else
        throw version_error(v, "1", r_inverter, CODELOC);

    return ds;
}

/** Constructor */
AtomMatchInverter::AtomMatchInverter() : ConcreteProperty<AtomMatchInverter,AtomMatcher>()
{}

/** Constructor */
AtomMatchInverter::AtomMatchInverter(const AtomMatcher &matcher)
                  : ConcreteProperty<AtomMatchInverter,AtomMatcher>()
{
    if (not matcher.isNull())
        m = matcher;
}

/** Copy constructor */
AtomMatchInverter::AtomMatchInverter(const AtomMatchInverter &other)
                  : ConcreteProperty<AtomMatchInverter,AtomMatcher>(other),
                    m(other.m)
{}

/** Destructor */
AtomMatchInverter::~AtomMatchInverter()
{}

/** Copy assignment operator */
AtomMatchInverter& AtomMatchInverter::operator=(const AtomMatchInverter &other)
{
    m = other.m;
    return *this;
}

/** Comparison operator */
bool AtomMatchInverter::operator==(const AtomMatchInverter &other) const
{
    return m == other.m;
}

/** Comparison operator */
bool AtomMatchInverter::operator!=(const AtomMatchInverter &other) const
{
    return not operator==(other);
}

bool AtomMatchInverter::isNull() const
{
    return m.constData() == 0 or m.read().isNull();
}

QString AtomMatchInverter::toString() const
{
    if (isNull())
        return QObject::tr("AtomMatchInverter::null");
    else
        return QObject::tr("AtomMatchInverter{ %1 }").arg(m.read().toString());
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomMatchInverter::match(const MoleculeView &mol0,
                                                const PropertyMap &map0,
                                                const MoleculeView &mol1,
                                                const PropertyMap &map1) const
{
    if (isNull())
        return QHash<AtomIdx,AtomIdx>();
    
    //apply the match backwards, and then invert the result
    QHash<AtomIdx,AtomIdx> map = m.read().match(mol1,map1,mol0,map0);
    
    //invert the match
    if (not map.isEmpty())
    {
        QHash<AtomIdx,AtomIdx> invmap;
        invmap.reserve(map.count());
        
        for (QHash<AtomIdx,AtomIdx>::const_iterator it = map.constBegin();
             it != map.constEnd();
             ++it)
        {
            invmap.insert( it.value(), it.key() );
        }
        
        return invmap;
    }
    else
        return map;
}
/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomMatchInverter::match(const MoleculeInfoData &mol0,
                                                const MoleculeInfoData &mol1) const
{
    if (isNull())
        return QHash<AtomIdx,AtomIdx>();
    
    //apply the match backwards, and then invert the result
    QHash<AtomIdx,AtomIdx> map = m.read().match(mol1,mol0);
    
    //invert the match
    if (not map.isEmpty())
    {
        QHash<AtomIdx,AtomIdx> invmap;
        invmap.reserve(map.count());
        
        for (QHash<AtomIdx,AtomIdx>::const_iterator it = map.constBegin();
             it != map.constEnd();
             ++it)
        {
            invmap.insert( it.value(), it.key() );
        }
        
        return invmap;
    }
    else
        return map;
}

const char* AtomMatchInverter::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomMatchInverter>() );
}

/////////
///////// Implementation of AtomMCSMatcher
/////////

static const RegisterMetaType<AtomMCSMatcher> r_mcsmatcher;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const AtomMCSMatcher &mcsmatcher)
{
    writeHeader(ds, r_mcsmatcher, 1);
    ds << static_cast<const AtomMatcher&>(mcsmatcher)
       << double( mcsmatcher.timeout.to(second) );
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomMCSMatcher &mcsmatcher)
{
    VersionID v = readHeader(ds, r_mcsmatcher);
    
    if (v == 1)
    {
        double timeout;
        ds >> static_cast<AtomMatcher&>(mcsmatcher) >> timeout;
        
        mcsmatcher.timeout = timeout*second;
    }
    else
        throw version_error(v, "1", r_namematcher, CODELOC);

    return ds;
}

/** Constructor */
AtomMCSMatcher::AtomMCSMatcher()
               : ConcreteProperty<AtomMCSMatcher,AtomMatcher>(), timeout(5*second)
{}

/** Construct specifying the timeout for the MCS match */
AtomMCSMatcher::AtomMCSMatcher(const SireUnits::Dimension::Time &t)
               : ConcreteProperty<AtomMCSMatcher,AtomMatcher>(), timeout(t)
{}

/** Copy constructor */
AtomMCSMatcher::AtomMCSMatcher(const AtomMCSMatcher &other)
               : ConcreteProperty<AtomMCSMatcher,AtomMatcher>(other), timeout(other.timeout)
{}

/** Destructor */
AtomMCSMatcher::~AtomMCSMatcher()
{}

/** Copy assignment operator */
AtomMCSMatcher& AtomMCSMatcher::operator=(const AtomMCSMatcher &other)
{
    timeout = other.timeout;
    return *this;
}

/** Comparison operator */
bool AtomMCSMatcher::operator==(const AtomMCSMatcher &other) const
{
    return timeout == other.timeout;
}

/** Comparison operator */
bool AtomMCSMatcher::operator!=(const AtomMCSMatcher &other) const
{
    return false;
}

QString AtomMCSMatcher::toString() const
{
    return QObject::tr("AtomMCSMatcher()");
}

const char* AtomMCSMatcher::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomMCSMatcher>() );
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomMCSMatcher::match(const MoleculeView &mol0,
                                             const PropertyMap &map0,
                                             const MoleculeView &mol1,
                                             const PropertyMap &map1) const
{
    return Evaluator(mol0).findMCS(mol1, timeout, map0, map1);
}

/////////
///////// Implementation of AtomIDMatcher
/////////

static const RegisterMetaType<AtomIDMatcher> r_idmatcher;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const AtomIDMatcher &idmatcher)
{
    writeHeader(ds, r_idmatcher, 1);
    
    SharedDataStream sds(ds);
    sds << idmatcher.m << static_cast<const AtomMatcher&>(idmatcher);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomIDMatcher &idmatcher)
{
    VersionID v = readHeader(ds, r_idmatcher);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> idmatcher.m >> static_cast<AtomMatcher&>(idmatcher);
    }
    else
        throw version_error(v, "1", r_idmatcher, CODELOC);

    return ds;
}

/** Constructor */
AtomIDMatcher::AtomIDMatcher() : ConcreteProperty<AtomIDMatcher,AtomMatcher>()
{}

/** Construct to match atom names */
AtomIDMatcher::AtomIDMatcher(const QList< QPair<QString,QString> > &names)
              : ConcreteProperty<AtomIDMatcher,AtomMatcher>()
{
    for (int i=0; i<names.count(); ++i)
    {
        const QPair<QString,QString> &name = names[i];

        if (not (name.first.isEmpty() or name.second.isEmpty()))
        {
            m.append( QPair<AtomIdentifier,AtomIdentifier>( AtomName(name.first),
                                                            AtomName(name.second) ) );
        }
    }
}

/** Construct to match atom indexes */
AtomIDMatcher::AtomIDMatcher(const QList< QPair<int,int> > &idxs)
              : ConcreteProperty<AtomIDMatcher,AtomMatcher>()
{
    for (int i=0; i<idxs.count(); ++i)
    {
        const QPair<int,int> &idx = idxs[i];
        
        m.append( QPair<AtomIdentifier,AtomIdentifier>( AtomIdx(idx.first),
                                                        AtomIdx(idx.second) ) );
    }
}

/** Construct to match specified AtomIdentifiers */
AtomIDMatcher::AtomIDMatcher(const QList< QPair<AtomIdentifier,AtomIdentifier> > &ids)
              : ConcreteProperty<AtomIDMatcher,AtomMatcher>()
{
    for (int i=0; i<ids.count(); ++i)
    {
        const QPair<AtomIdentifier,AtomIdentifier> id = ids[i];
        
        if (not (id.first.isNull() or id.second.isNull()))
            m.append(id);
    }
}

/** Construct to match atom names */
AtomIDMatcher::AtomIDMatcher(const QList< boost::tuple<QString,QString> > &names)
              : ConcreteProperty<AtomIDMatcher,AtomMatcher>()
{
    for (int i=0; i<names.count(); ++i)
    {
        const boost::tuple<QString,QString> &name = names[i];

        if (not (name.get<0>().isEmpty() or name.get<1>().isEmpty()))
        {
            m.append( QPair<AtomIdentifier,AtomIdentifier>( AtomName(name.get<0>()),
                                                            AtomName(name.get<1>()) ) );
        }
    }
}

/** Construct to match atom indexes */
AtomIDMatcher::AtomIDMatcher(const QList< boost::tuple<int,int> > &idxs)
              : ConcreteProperty<AtomIDMatcher,AtomMatcher>()
{
    for (int i=0; i<idxs.count(); ++i)
    {
        const boost::tuple<int,int> &idx = idxs[i];
        
        m.append( QPair<AtomIdentifier,AtomIdentifier>( AtomIdx(idx.get<0>()),
                                                        AtomIdx(idx.get<1>()) ) );
    }
}

/** Construct to match specified AtomIdentifiers */
AtomIDMatcher::AtomIDMatcher(const QList< boost::tuple<AtomIdentifier,AtomIdentifier> > &ids)
              : ConcreteProperty<AtomIDMatcher,AtomMatcher>()
{
    for (int i=0; i<ids.count(); ++i)
    {
        const boost::tuple<AtomIdentifier,AtomIdentifier> id = ids[i];
        
        if (not (id.get<0>().isNull() or id.get<1>().isNull()))
            m.append( QPair<AtomIdentifier,AtomIdentifier>(id.get<0>(),id.get<1>()) );
    }
}

/** Copy constructor */
AtomIDMatcher::AtomIDMatcher(const AtomIDMatcher &other)
              : ConcreteProperty<AtomIDMatcher,AtomMatcher>(other),
                m(other.m)
{}

/** Destructor */
AtomIDMatcher::~AtomIDMatcher()
{}

/** Copy assignment operator */
AtomIDMatcher& AtomIDMatcher::operator=(const AtomIDMatcher &other)
{
    m = other.m;
    return *this;
}

/** Comparison operator */
bool AtomIDMatcher::operator==(const AtomIDMatcher &other) const
{
    return m == other.m;
}

/** Comparison operator */
bool AtomIDMatcher::operator!=(const AtomIDMatcher &other) const
{
    return not operator==(*this);
}

/** Return whether or not this matcher is null (cannot be used for matching) */
bool AtomIDMatcher::isNull() const
{
    return m.isEmpty();
}

QString AtomIDMatcher::toString() const
{
    if (isNull())
        return QObject::tr("AtomIDMatcher::null");

    QStringList matches;
    
    for (QList< QPair<AtomIdentifier,AtomIdentifier> >::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        matches.append( QObject::tr(" %1 <=> %2").arg(it->first.toString(),
                                                      it->second.toString()) );
    }

    return QObject::tr("AtomIDMatcher( %1 )").arg( matches.join("\n") );
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomIDMatcher::match(const MoleculeView &mol0,
                                            const PropertyMap &map0,
                                            const MoleculeView &mol1,
                                            const PropertyMap &map1) const
{
    QHash<AtomIdx,AtomIdx> map;

    const AtomSelection sel0 = mol0.selection();
    const AtomSelection sel1 = mol1.selection();

    QSet<AtomIdx> found_1;
    
    const int nats0 = sel0.nSelectedAtoms();
    const int nats1 = sel1.nSelectedAtoms();
    
    for (QList< QPair<AtomIdentifier,AtomIdentifier> >::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        //find the atom in mol0
        try
        {
            AtomIdx idx0 = mol0.data().info().atomIdx(it->first);
            
            if (sel0.selected(idx0) and not map.contains(idx0))
            {
                try
                {
                    AtomIdx idx1 = mol1.data().info().atomIdx(it->second);
                    
                    if (sel1.selected(idx1) and not found_1.contains(idx1))
                    {
                        map.insert(idx0, idx1);
                        found_1.insert(idx1);
                    }
                }
                catch(...)
                {}
            }
        }
        catch(...)
        {}
        
        if (map.count() >= nats0 or found_1.count() >= nats1)
            break;
    }
        
    return map;
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomIDMatcher::match(const MoleculeInfoData &mol0,
                                            const MoleculeInfoData &mol1) const
{
    QHash<AtomIdx,AtomIdx> map;


    QSet<AtomIdx> found_1;
    
    const int nats0 = mol0.nAtoms();
    const int nats1 = mol1.nAtoms();
    
    for (QList< QPair<AtomIdentifier,AtomIdentifier> >::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        //find the atom in mol0
        try
        {
            AtomIdx idx0 = mol0.atomIdx(it->first);
            
            if (not map.contains(idx0))
            {
                try
                {
                    AtomIdx idx1 = mol1.atomIdx(it->second);
                    
                    if (not found_1.contains(idx1))
                    {
                        map.insert(idx0, idx1);
                        found_1.insert(idx1);
                    }
                }
                catch(...)
                {}
            }
        }
        catch(...)
        {}
        
        if (map.count() >= nats0 or found_1.count() >= nats1)
            break;
    }
        
    return map;
}

const char* AtomIDMatcher::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomIDMatcher>() );
}

/////////
///////// Implementation of AtomMultiMatcher
/////////

static const RegisterMetaType<AtomMultiMatcher> r_multimatcher;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const AtomMultiMatcher &multimatcher)
{
    writeHeader(ds, r_multimatcher, 1);
    
    SharedDataStream sds(ds);
    sds << multimatcher.m << static_cast<const AtomMatcher&>(multimatcher);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomMultiMatcher &multimatcher)
{
    VersionID v = readHeader(ds, r_multimatcher);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> multimatcher.m >> static_cast<AtomMatcher&>(multimatcher);
    }
    else
        throw version_error(v, "1", r_multimatcher, CODELOC);

    return ds;
}

/** Constructor */
AtomMultiMatcher::AtomMultiMatcher() : ConcreteProperty<AtomMultiMatcher,AtomMatcher>()
{}

/** Construct from a single match */
AtomMultiMatcher::AtomMultiMatcher(const AtomMatcher &matcher)
                 : ConcreteProperty<AtomMultiMatcher,AtomMatcher>()
{
    if (matcher.isA<AtomMultiMatcher>())
    {
        this->operator=(matcher.asA<AtomMultiMatcher>());
    }
    else
    {
        m.append( matcher );
    }
}

/** Construct from a pair of matches */
AtomMultiMatcher::AtomMultiMatcher(const AtomMatcher &m0, const AtomMatcher &m1)
                 : ConcreteProperty<AtomMultiMatcher,AtomMatcher>()
{
    this->operator=( AtomMultiMatcher(m0) );
    
    if (m1.isA<AtomMultiMatcher>())
    {
        m += m1.asA<AtomMultiMatcher>().m;
    }
    else
    {
        m += m1;
    }
}

/** Copy constructor */
AtomMultiMatcher::AtomMultiMatcher(const AtomMultiMatcher &other)
                 : ConcreteProperty<AtomMultiMatcher,AtomMatcher>(other),
                   m(other.m)
{}

/** Destructor */
AtomMultiMatcher::~AtomMultiMatcher()
{}

/** Copy assignment operator */
AtomMultiMatcher& AtomMultiMatcher::operator=(const AtomMultiMatcher &other)
{
    m = other.m;
    return *this;
}

/** Comparison operator */
bool AtomMultiMatcher::operator==(const AtomMultiMatcher &other) const
{
    return m == other.m;
}

/** Comparison operator */
bool AtomMultiMatcher::operator!=(const AtomMultiMatcher &other) const
{
    return not operator==(*this);
}

/** Return whether or not this matcher is null (cannot be used for matching) */
bool AtomMultiMatcher::isNull() const
{
    return m.isEmpty();
}

QString AtomMultiMatcher::toString() const
{
    if (isNull())
        return QObject::tr("AtomMultiMatcher::null");

    QStringList matches;
    
    for (QList<AtomMatcherPtr>::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        matches.append( QObject::tr(" %1").arg(it->read().toString()) );
    }

    return QObject::tr("AtomMultiMatcher{ %1 }").arg( matches.join(" +\n") );
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomMultiMatcher::match(const MoleculeView &mol0,
                                               const PropertyMap &map0,
                                               const MoleculeView &mol1,
                                               const PropertyMap &map1) const
{
    if (m.isEmpty())
        return QHash<AtomIdx,AtomIdx>();
    else if (m.count() == 1)
        return m.at(0).read().match(mol0,mol1);

    QHash<AtomIdx,AtomIdx> map;

    QSet<AtomIdx> found_1;
    const int nats0 = mol0.selection().nSelected();
    const int nats1 = mol1.selection().nSelected();
    
    for (QList<AtomMatcherPtr>::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        QHash<AtomIdx,AtomIdx> lmap = it->read().match(mol0, map0, mol1, map1);
        
        for (QHash<AtomIdx,AtomIdx>::const_iterator it2 = lmap.constBegin();
             it2 != lmap.constEnd();
             ++it2)
        {
            if (not (map.contains(it2.key()) or found_1.contains(it2.value())))
            {
                map.insert( it2.key(), it2.value() );
                found_1.insert( it2.value() );
            }
        }
        
        if (map.count() == nats0)
            break;
        
        if (found_1.count() == nats1)
            break;
        
        if (map.count() > nats0)
            throw SireError::program_bug( QObject::tr(
                    "Should not have excess matched atoms???"), CODELOC );
        
        if (found_1.count() > nats1)
            throw SireError::program_bug( QObject::tr(
                    "Should not have excess matched atoms???"), CODELOC );
    }
        
    return map;
}

/** Match the atoms in 'mol1' to the atoms in 'mol0' - this
    returns the AtomIdxs of the atoms in 'mol1' that are in
    'mol0', indexed by the AtomIdx of the atom in 'mol0'.
    
     This skips atoms in 'mol1' that are not in 'mol0'
*/
QHash<AtomIdx,AtomIdx> AtomMultiMatcher::match(const MoleculeInfoData &mol0,
                                               const MoleculeInfoData &mol1) const
{
    if (m.isEmpty())
        return QHash<AtomIdx,AtomIdx>();
    else if (m.count() == 1)
        return m.at(0).read().match(mol0,mol1);

    QHash<AtomIdx,AtomIdx> map;

    QSet<AtomIdx> found_1;
    const int nats0 = mol0.nAtoms();
    const int nats1 = mol1.nAtoms();
    
    for (QList<AtomMatcherPtr>::const_iterator it = m.constBegin();
         it != m.constEnd();
         ++it)
    {
        QHash<AtomIdx,AtomIdx> lmap = it->read().match(mol0, mol1);
        
        for (QHash<AtomIdx,AtomIdx>::const_iterator it2 = lmap.constBegin();
             it2 != lmap.constEnd();
             ++it2)
        {
            if (not (map.contains(it2.key()) or found_1.contains(it2.value())))
            {
                map.insert( it2.key(), it2.value() );
                found_1.insert( it2.value() );
            }
        }
        
        if (map.count() == nats0)
            break;
        
        if (found_1.count() == nats1)
            break;
        
        if (map.count() > nats0)
            throw SireError::program_bug( QObject::tr(
                    "Should not have excess matched atoms???"), CODELOC );
        
        if (found_1.count() > nats1)
            throw SireError::program_bug( QObject::tr(
                    "Should not have excess matched atoms???"), CODELOC );
    }
        
    return map;
}

const char* AtomMultiMatcher::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomMultiMatcher>() );
}
