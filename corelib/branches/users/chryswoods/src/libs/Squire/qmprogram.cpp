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

#include "qmprogram.h"
#include "latticecharges.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireBase;
using namespace SireStream;

///////
/////// Implementation of QMProg
///////

static const RegisterMetaType<QMProg> r_qmprog( MAGIC_ONLY,
                                                "Squire::QMProg" );

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const QMProg &qmprog)
{
    writeHeader(ds, r_qmprog, 1);
    
    ds << static_cast<const PropertyBase&>(qmprog);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, QMProg &qmprog)
{
    VersionID v = readHeader(ds, r_qmprog);
    
    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(qmprog);
    }
    else
        throw version_error(v, "1", r_qmprog, CODELOC);
        
    return ds;
}

/** Constructor */
QMProg::QMProg() : PropertyBase()
{}

/** Copy constructor */
QMProg::QMProg(const QMProg &other) : PropertyBase(other)
{}

/** Destructor */
QMProg::~QMProg()
{}

/** Return the QM energy of the molecules 'molecules' surrounded by the 
    field of point charges 'lattice_charges' */
double QMProg::calculateEnergy(const QMPotential::Molecules &molecules,
                               const LatticeCharges &lattice_charges) const
{
    throw SireError::unsupported( QObject::tr(
        "This QM program (%1) does not support the use of point lattice charges.")
            .arg(this->what()), CODELOC );
}

/** Return the command file that would be used to calculate the energy
    of the molecules in 'molecules' in the field of point charges in
    'lattice_charges' */
QString QMProg::energyCommandFile(const QMPotential::Molecules &molecules,
                                  const LatticeCharges &lattice_charges) const
{
    throw SireError::unsupported( QObject::tr(
        "This QM program (%1) does not support the use of point lattice charges.")
            .arg(this->what()), CODELOC );
}

/** Return the command file that would be used to calculate the forces
    of the molecules in 'molecules' in the field of point charges in
    'lattice_charges' (and the forces on the charges themselves) */
QString QMProg::forceCommandFile(const QMPotential::Molecules &molecules,
                                 const LatticeCharges &lattice_charges) const
{
    throw SireError::unsupported( QObject::tr(
        "This QM program (%1) does not support the use of point lattice charges.")
            .arg(this->what()), CODELOC );
}

///////
/////// Implementation of QMProgram
///////

static const RegisterMetaType<QMProgram> r_qmprogram;

/** Serialise a QMProgram to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const QMProgram &qmprogram)
{
    writeHeader(ds, r_qmprogram, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(qmprogram);
    
    return ds;
}

/** Deserialise a QMProgram from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, QMProgram &qmprogram)
{
    VersionID v = readHeader(ds, r_qmprogram);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(qmprogram);
    }
    else
        throw version_error(v, "1", r_qmprogram, CODELOC);
        
    return ds;
}

static QMProgram *_pvt_shared_null = 0;

const QMProgram& QMProgram::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new QMProgram( NullQM() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, empty QMProg */
QMProgram::QMProgram() : Property(QMProgram::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
QMProgram::QMProgram(const PropertyBase &property) : Property(property.asA<QMProg>())
{}

/** Construct from passed QMProg */
QMProgram::QMProgram(const QMProg &QMProgram) : Property(QMProgram)
{}

/** Copy constructor */
QMProgram::QMProgram(const QMProgram &other) : Property(other)
{}

/** Destructor */
QMProgram::~QMProgram()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
QMProgram& QMProgram::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<QMProg>());
    return *this;
}

/** Copy assignment operator from another QMProg */
QMProgram& QMProgram::operator=(const QMProg &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const QMProg* QMProgram::operator->() const
{
    return static_cast<const QMProg*>(&(d()));
}

/** Dereference this pointer */
const QMProg& QMProgram::operator*() const
{
    return static_cast<const QMProg&>(d());
}

/** Return a read-only reference to the contained object */
const QMProg& QMProgram::read() const
{
    return static_cast<const QMProg&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
QMProg& QMProgram::edit()
{
    return static_cast<QMProg&>(d());
}
    
/** Return a raw pointer to the QMProg object */
const QMProg* QMProgram::data() const
{
    return static_cast<const QMProg*>(&(d()));
}

/** Return a raw pointer to the QMProg object */
const QMProg* QMProgram::constData() const
{
    return static_cast<const QMProg*>(&(d()));
}
    
/** Return a raw pointer to the QMProg object */
QMProg* QMProgram::data()
{
    return static_cast<QMProg*>(&(d()));
}

/** Automatic casting operator */
QMProgram::operator const QMProg&() const
{
    return static_cast<const QMProg&>(d());
}

///////
/////// Implementation of NullQM
///////

static const RegisterMetaType<NullQM> r_nullqm;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const NullQM &nullqm)
{
    writeHeader(ds, r_nullqm, 1);
    ds << static_cast<const QMProg&>(nullqm);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, NullQM &nullqm)
{
    VersionID v = readHeader(ds, r_nullqm);
    
    if (v == 1)
    {
        ds >> static_cast<QMProg&>(nullqm);
    }
    else
        throw version_error(v, "1", r_nullqm, CODELOC);

    return ds;
}

/** Constructor */
NullQM::NullQM() : ConcreteProperty<NullQM,QMProg>()
{}

/** Copy constructor */
NullQM::NullQM(const NullQM &other)
         : ConcreteProperty<NullQM,QMProg>(other)
{}

/** Destructor */
NullQM::~NullQM()
{}

/** Copy assignment operator */
NullQM& NullQM::operator=(const NullQM &other)
{
    return *this;
}

/** Comparison operator */
bool NullQM::operator==(const NullQM &other) const
{
    return true;
}

/** Comparison operator */
bool NullQM::operator!=(const NullQM &other) const
{
    return false;
}

/** Calculate and return the QM energy of all of the molecules
    in 'molecules' */
double NullQM::calculateEnergy(const QMPotential::Molecules&) const
{
    return 0;
}

/** Return the command file that would be used to calculate the energy of
    the molecules in 'molecules' */
QString NullQM::energyCommandFile(const QMPotential::Molecules&) const
{
    return QString::null;
}

/** Return the command file that would be used to calculate the forces on
    the molecules in 'molecules' */
QString NullQM::forceCommandFile(const QMPotential::Molecules&) const
{
    return QString::null;
}
