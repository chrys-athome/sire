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

#include "molviewproperty.h"
#include "moleculeinfodata.h"

#include "SireError/errors.h"

using namespace SireMol;
using namespace SireBase;

/** Constructor */
MolViewProperty::MolViewProperty() : PropertyBase()
{}

/** Copy constructor */
MolViewProperty::MolViewProperty(const MolViewProperty &other)
                : PropertyBase(other)
{}

/** Destructor */
MolViewProperty::~MolViewProperty()
{}

/** Assert that this property is compatible with the MoleculeInfoData 'info'

    \throw SireError::incompatible_error
*/
void MolViewProperty::assertCompatibleWith(const MoleculeInfoData &molinfo) const
{
    if (not this->isCompatibleWith(molinfo))
        throw SireError::incompatible_error( QObject::tr(
                "The property of type %1 is incompatible with the molecule "
                "\"%2\"")
                    .arg(this->what()).arg(molinfo.name()), CODELOC );
}
