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

#include "atomicproperties.h"

#include "cgatomid.h"

#include "SireError/errors.h"

using namespace SireMol;

/** Constructor */
AtomicProperties::AtomicProperties() : PropertyBase()
{}

/** Copy constructor */
AtomicProperties::AtomicProperties(const AtomicProperties &other)
                 : PropertyBase(other)
{}

/** Destructor */
AtomicProperties::~AtomicProperties()
{}

/** Throw an error caused by a missing CutGroup

    \throw SireError::invalid_index
*/
void AtomicProperties::throwMissingCutGroup(CutGroupID cgid, int ncg) const
{
    throw SireError::invalid_index( QObject::tr(
          "There is no CutGroup with ID == %1 in the AtomicCharges "
          "(Max ID == %2)")
              .arg(cgid).arg(ncg-1), CODELOC );
}

/** Throw an error caused by a missing atom

    \throw SireError::invalid_index
*/
void AtomicProperties::throwMissingAtom(const CGAtomID &cgatomid, int natms) const
{
    throw SireError::invalid_index( QObject::tr(
          "There is no Atom with ID == %1 in the CutGroup with "
          "ID == %2 in the AtomicCharges (Max ID == %2)")
              .arg(cgatomid.atomID()).arg(cgatomid.cutGroupID()).arg(natms-1),
                      CODELOC );
}
