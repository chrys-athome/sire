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

#include "weightfunction.h"

#include "atomselection.h"
#include "moleculedata.h"
#include "moleculeview.h"
#include "partialmolecule.h"
#include "evaluator.h"

#include "mover.hpp"
#include "editor.hpp"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of WeightFunc
//////////

static const RegisterMetaType<WeightFunc> r_weightfunc(MAGIC_ONLY,
                                                       "SireMol::WeightFunc");

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds,
                                    const SireMol::WeightFunc &weightfunc)
{
    writeHeader(ds, r_weightfunc, 1);

    ds << static_cast<const PropertyBase&>(weightfunc);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    WeightFunc &weightfunc)
{
    VersionID v = readHeader(ds, r_weightfunc);

    if (v == 1)
    {
        ds >> static_cast<WeightFunc&>(weightfunc);
    }
    else
        throw version_error(v, "1", r_weightfunc, CODELOC);

    return ds;
}

/** Constructor */
WeightFunc::WeightFunc() : PropertyBase()
{}

/** Copy constructor */
WeightFunc::WeightFunc(const WeightFunc &other)
               : PropertyBase(other)
{}

/** Destructror */
WeightFunc::~WeightFunc()
{}

//////////
////////// Implementation of WeightFunction
//////////

static const RegisterMetaType<WeightFunction> r_weightfunction;

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds,
                                    const WeightFunction &weightfunc)
{
    writeHeader(ds, r_weightfunction, 1);

    SharedDataStream sds(ds);
    sds << static_cast<const Property&>(weightfunc);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds, WeightFunction &weightfunc)
{
    VersionID v = readHeader(ds, r_weightfunction);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(weightfunc);
    }
    else
        throw version_error(v, "1", r_weightfunction, CODELOC);

    return ds;
}

static WeightFunction *_pvt_shared_null = 0;

const WeightFunction& WeightFunction::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new WeightFunction( RelFromNumber() );

    return *_pvt_shared_null;
}

/** Default constructor - this is a RelByNumber weight function */
WeightFunction::WeightFunction() : Property( WeightFunction::shared_null() )
{}

/** Create from the passed WeightFunc */
WeightFunction::WeightFunction(const WeightFunc &weightfunc)
               : Property(weightfunc)
{}

/** Create from the passed property

    \throw SireError::invalid_cast
*/
WeightFunction::WeightFunction(const PropertyBase &property)
               : Property( property.asA<WeightFunc>() )
{}

/** Copy constructor */
WeightFunction::WeightFunction(const WeightFunction &other)
               : Property(other)
{}

/** Destructor */
WeightFunction::~WeightFunction()
{}

/** Copy assignment from a WeightFunc */
WeightFunction& WeightFunction::operator=(const WeightFunc &weightfunc)
{
    Property::operator=(weightfunc);
    return *this;
}

/** Copy assignment operator */
WeightFunction& WeightFunction::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<WeightFunc>());
    return *this;
}

const WeightFunc* WeightFunction::operator->() const
{
    return static_cast<const WeightFunc*>( &(d()) );
}

const WeightFunc& WeightFunction::operator*() const
{
    return static_cast<const WeightFunc&>( d() );
}

const WeightFunc& WeightFunction::read() const
{
    return static_cast<const WeightFunc&>( d() );
}

WeightFunc& WeightFunction::edit()
{
    return static_cast<WeightFunc&>( d() );
}

const WeightFunc* WeightFunction::data() const
{
    return static_cast<const WeightFunc*>( &(d()) );
}

const WeightFunc* WeightFunction::constData() const
{
    return static_cast<const WeightFunc*>( &(d()) );
}

WeightFunc* WeightFunction::data()
{
    return static_cast<WeightFunc*>( &(d()) );
}

WeightFunction::operator const WeightFunc&() const
{
    return static_cast<const WeightFunc&>( d() );
}

/** Return the relative weight of view0 and view1 using
    the supplied PropertyMap to find any required properties
    from both views

    \throw SireBase::missing_property
*/
double WeightFunction::operator()(const MoleculeView &view0,
                                  const MoleculeView &view1,
                                  const PropertyMap &map) const
{
    return read()(view0, view1, map);
}

/** Return the relative weight of group0 and group1 in the molecule
    whose data is in 'moldata' using the passed PropertyMap to
    find any required properties

    \throw SireError::incompatible_error
    \throw SireBase::missing_property
*/
double WeightFunction::operator()(const MoleculeData &moldata,
                                  const AtomSelection &group0,
                                  const AtomSelection &group1,
                                  const PropertyMap &map) const
{
    return read()(moldata, group0, group1, map);
}

/** Return the relative weight of view0 and view1 using
    map0 to find any required properties from view0, and
    map1 to find any required properties from view1

    \throw SireBase::missing_property
*/
double WeightFunction::operator()(const MoleculeView &view0,
                                  const PropertyMap &map0,
                                  const MoleculeView &view1,
                                  const PropertyMap &map1) const
{
    return read()(view0, map0, view1, map1);
}

//////////
////////// Implementation of AbsFromNumber
//////////

static const RegisterMetaType<AbsFromNumber> r_absfromnum;

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds,
                                    const AbsFromNumber &absfromnum)
{
    writeHeader(ds, r_absfromnum, 1);

    ds << static_cast<const PropertyBase&>(absfromnum);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    AbsFromNumber &absfromnum)
{
    VersionID v = readHeader(ds, r_absfromnum);

    if (v == 1)
    {
        ds >> static_cast<AbsFromNumber&>(absfromnum);
    }
    else
        throw version_error(v, "1", r_absfromnum, CODELOC);

    return ds;
}

AbsFromNumber::AbsFromNumber()
              : ConcreteProperty<AbsFromNumber,WeightFunc>()
{}

AbsFromNumber::AbsFromNumber(const AbsFromNumber &other)
              : ConcreteProperty<AbsFromNumber,WeightFunc>(other)
{}

AbsFromNumber::~AbsFromNumber()
{}

inline double AbsFromNumber::weight(int nats0, int nats1) const
{
    if (nats0 > nats1)
        return 1;
    else if (nats0 < nats1)
        return 0;
    else
        return 0.5;
}

double AbsFromNumber::operator()(const MoleculeData &moldata,
                                 const AtomSelection &group0,
                                 const AtomSelection &group1,
                                 const PropertyMap&) const
{
    group0.assertCompatibleWith(moldata);
    group1.assertCompatibleWith(moldata);

    return weight(group0.nSelected(), group1.nSelected());
}

double AbsFromNumber::operator()(const MoleculeView &view0,
                                 const PropertyMap&,
                                 const MoleculeView &view1,
                                 const PropertyMap&) const
{
    return weight(view0.selection().nSelected(),
                  view1.selection().nSelected());
}

//////////
////////// Implementation of RelFromNumber
//////////

static const RegisterMetaType<RelFromNumber> r_relfromnum;

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds,
                                    const RelFromNumber &relfromnum)
{
    writeHeader(ds, r_relfromnum, 1);

    ds << static_cast<const PropertyBase&>(relfromnum);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    RelFromNumber &relfromnum)
{
    VersionID v = readHeader(ds, r_relfromnum);

    if (v == 1)
    {
        ds >> static_cast<RelFromNumber&>(relfromnum);
    }
    else
        throw version_error(v, "1", r_relfromnum, CODELOC);

    return ds;
}

RelFromNumber::RelFromNumber()
              : ConcreteProperty<RelFromNumber,WeightFunc>()
{}

RelFromNumber::RelFromNumber(const RelFromNumber &other)
              : ConcreteProperty<RelFromNumber,WeightFunc>(other)
{}

RelFromNumber::~RelFromNumber()
{}

inline double RelFromNumber::weight(int nats0, int nats1) const
{
    double total = nats0 + nats1;

    if (total > 0)
        return nats1 / total;
    else
        return 0.5;
}

double RelFromNumber::operator()(const MoleculeData &moldata,
                                 const AtomSelection &group0,
                                 const AtomSelection &group1,
                                 const PropertyMap&) const
{
    group0.assertCompatibleWith(moldata);
    group1.assertCompatibleWith(moldata);

    return weight(group0.nSelected(), group1.nSelected());
}

double RelFromNumber::operator()(const MoleculeView &view0,
                                 const PropertyMap&,
                                 const MoleculeView &view1,
                                 const PropertyMap&) const
{
    return weight(view0.selection().nSelected(),
                  view1.selection().nSelected());
}

//////////
////////// Implementation of AbsFromMass
//////////

static const RegisterMetaType<AbsFromMass> r_absfrommass;

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds,
                                    const AbsFromMass &absfrommass)
{
    writeHeader(ds, r_absfrommass, 1);

    ds << static_cast<const PropertyBase&>(absfrommass);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    AbsFromMass &absfrommass)
{
    VersionID v = readHeader(ds, r_absfrommass);

    if (v == 1)
    {
        ds >> static_cast<AbsFromMass&>(absfrommass);
    }
    else
        throw version_error(v, "1", r_absfrommass, CODELOC);

    return ds;
}

AbsFromMass::AbsFromMass()
              : ConcreteProperty<AbsFromMass,WeightFunc>()
{}

AbsFromMass::AbsFromMass(const AbsFromMass &other)
              : ConcreteProperty<AbsFromMass,WeightFunc>(other)
{}

AbsFromMass::~AbsFromMass()
{}

inline double AbsFromMass::weight(double mass0, double mass1) const
{
    if (mass0 > mass1)
        return 1;
    else if (mass0 < mass1)
        return 0;
    else
        return 0.5;
}

double AbsFromMass::operator()(const MoleculeData &moldata,
                               const AtomSelection &group0,
                               const AtomSelection &group1,
                               const PropertyMap &map) const
{
    return weight( PartialMolecule(moldata,group0).evaluate().mass(map),
                   PartialMolecule(moldata,group1).evaluate().mass(map) );
}

double AbsFromMass::operator()(const MoleculeView &view0,
                               const PropertyMap &map0,
                               const MoleculeView &view1,
                               const PropertyMap &map1) const
{
    return weight( PartialMolecule(view0).evaluate().mass(map0),
                   PartialMolecule(view1).evaluate().mass(map1) );
}

//////////
////////// Implementation of RelFromMass
//////////

static const RegisterMetaType<RelFromMass> r_relfrommass;

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds,
                                    const RelFromMass &relfrommass)
{
    writeHeader(ds, r_relfrommass, 1);

    ds << static_cast<const PropertyBase&>(relfrommass);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    RelFromMass &relfrommass)
{
    VersionID v = readHeader(ds, r_relfrommass);

    if (v == 1)
    {
        ds >> static_cast<RelFromMass&>(relfrommass);
    }
    else
        throw version_error(v, "1", r_relfrommass, CODELOC);

    return ds;
}

RelFromMass::RelFromMass()
              : ConcreteProperty<RelFromMass,WeightFunc>()
{}

RelFromMass::RelFromMass(const RelFromMass &other)
              : ConcreteProperty<RelFromMass,WeightFunc>(other)
{}

RelFromMass::~RelFromMass()
{}

inline double RelFromMass::weight(double mass0, double mass1) const
{
    double total = mass0 + mass1;

    if (total > 0)
        return mass1 / total;
    else
        return 0.5;
}

double RelFromMass::operator()(const MoleculeData &moldata,
                               const AtomSelection &group0,
                               const AtomSelection &group1,
                               const PropertyMap &map) const
{
    return weight( PartialMolecule(moldata,group0).evaluate().mass(map),
                   PartialMolecule(moldata,group1).evaluate().mass(map) );
}

double RelFromMass::operator()(const MoleculeView &view0,
                               const PropertyMap &map0,
                               const MoleculeView &view1,
                               const PropertyMap &map1) const
{
    return weight( PartialMolecule(view0).evaluate().mass(map0),
                   PartialMolecule(view1).evaluate().mass(map1) );
}
