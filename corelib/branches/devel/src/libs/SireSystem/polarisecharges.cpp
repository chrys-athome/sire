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

#include "polarisecharges.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireStream;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const PolariseCharges &polarisecharges)
{
    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, 
                                          PolariseCharges &polarisecharges)
{
    return ds;
}

/** Null constructor */
PolariseCharges::PolariseCharges() 
                : ConcreteProperty<PolariseCharges,ChargeConstraint>()
{}


PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const PropertyMap &map);
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                const MoleculeGroup &chargegroup,
                const PropertyMap &map = PropertyMap());

PolariseCharges::PolariseCharges(const PolariseCharges &other);

PolariseCharges::~PolariseCharges();

PolariseCharges& PolariseCharges::operator=(const PolariseCharges &other);

bool PolariseCharges::operator==(const PolariseCharges &other) const;
bool PolariseCharges::operator!=(const PolariseCharges &other) const;

const char* PolariseCharges::typeName() const;

PolariseCharges* PolariseCharges::clone() const;

QString PolariseCharges::toString() const;

void PolariseCharges::setSystem(const System &system);

bool PolariseCharges::mayChange(const Delta &delta, quint32 last_subversion) const;

bool PolariseCharges::fullApply(Delta &delta);
bool PolariseCharges::deltaApply(Delta &delta, quint32 last_subversion);
