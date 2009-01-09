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

#include <QMutex>

#include "qmprogram.h"
#include "latticecharges.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireBase;
using namespace SireStream;

///////
/////// Implementation of QMProgram
///////

static const RegisterMetaType<QMProgram> r_qmprog( MAGIC_ONLY,
                                                   "Squire::QMProgram" );

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const QMProgram &qmprog)
{
    writeHeader(ds, r_qmprog, 1);
    
    ds << static_cast<const Property&>(qmprog);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, QMProgram &qmprog)
{
    VersionID v = readHeader(ds, r_qmprog);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(qmprog);
    }
    else
        throw version_error(v, "1", r_qmprog, CODELOC);
        
    return ds;
}

/** Constructor */
QMProgram::QMProgram() : Property()
{}

/** Copy constructor */
QMProgram::QMProgram(const QMProgram &other) : Property(other)
{}

/** Destructor */
QMProgram::~QMProgram()
{}

/** Return the QM energy of the molecules 'molecules' surrounded by the 
    field of point charges 'lattice_charges' */
double QMProgram::calculateEnergy(const QMPotential::Molecules &molecules,
                                  const LatticeCharges &lattice_charges,
                                  int) const
{
    throw SireError::unsupported( QObject::tr(
        "This QM program (%1) does not support the use of point lattice charges.")
            .arg(this->what()), CODELOC );
}

/** Return the command file that would be used to calculate the energy
    of the molecules in 'molecules' in the field of point charges in
    'lattice_charges' */
QString QMProgram::energyCommandFile(const QMPotential::Molecules &molecules,
                                    const LatticeCharges &lattice_charges) const
{
    throw SireError::unsupported( QObject::tr(
        "This QM program (%1) does not support the use of point lattice charges.")
            .arg(this->what()), CODELOC );
}

/** Return the command file that would be used to calculate the forces
    of the molecules in 'molecules' in the field of point charges in
    'lattice_charges' (and the forces on the charges themselves) */
QString QMProgram::forceCommandFile(const QMPotential::Molecules &molecules,
                                 const LatticeCharges &lattice_charges) const
{
    throw SireError::unsupported( QObject::tr(
        "This QM program (%1) does not support the use of point lattice charges.")
            .arg(this->what()), CODELOC );
}

///////
/////// Implementation of NullQM
///////

static const RegisterMetaType<NullQM> r_nullqm;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const NullQM &nullqm)
{
    writeHeader(ds, r_nullqm, 1);
    ds << static_cast<const QMProgram&>(nullqm);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, NullQM &nullqm)
{
    VersionID v = readHeader(ds, r_nullqm);
    
    if (v == 1)
    {
        ds >> static_cast<QMProgram&>(nullqm);
    }
    else
        throw version_error(v, "1", r_nullqm, CODELOC);

    return ds;
}

/** Constructor */
NullQM::NullQM() : ConcreteProperty<NullQM,QMProgram>()
{}

static SharedPolyPointer<NullQM> shared_null;

const NullQM& QMProgram::null()
{
    if (shared_null.constData() == 0)
    {
        QMutexLocker lkr( SireBase::globalLock() );
        
        if (shared_null.constData() == 0)
            shared_null = new NullQM();
    }
    
    return *(shared_null.constData());
}

/** Copy constructor */
NullQM::NullQM(const NullQM &other)
         : ConcreteProperty<NullQM,QMProgram>(other)
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
double NullQM::calculateEnergy(const QMPotential::Molecules&, int) const
{
    return 0;
}

/** Return the QM energy of the molecules 'molecules' surrounded by the 
    field of point charges 'lattice_charges' */
double NullQM::calculateEnergy(const QMPotential::Molecules&,
                               const LatticeCharges&,
                               int) const
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


/** Return the command file that would be used to calculate the energy
    of the molecules in 'molecules' in the field of point charges in
    'lattice_charges' */
QString NullQM::energyCommandFile(const QMPotential::Molecules&,
                                  const LatticeCharges &lattice_charges) const
{
    return QString::null;
}

/** Return the command file that would be used to calculate the forces
    of the molecules in 'molecules' in the field of point charges in
    'lattice_charges' (and the forces on the charges themselves) */
QString NullQM::forceCommandFile(const QMPotential::Molecules&,
                                 const LatticeCharges&) const
{
    return QString::null;
}
