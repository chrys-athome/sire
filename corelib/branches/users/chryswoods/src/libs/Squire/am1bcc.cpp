/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "am1bcc.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<AM1BCC> r_am1bcc;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const AM1BCC &am1bcc)
{
    writeHeader(ds, r_am1bcc, 1);
    
    SharedDataStream sds(ds);
    
    sds << am1bcc.mopac
        << static_cast<const QMChargeCalculator&>(am1bcc);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, AM1BCC &am1bcc)
{
    VersionID v = readHeader(ds, r_am1bcc);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> am1bcc.mopac
            >> static_cast<QMChargeCalculator&>(am1bcc);
    }
    else
        throw version_error(v, "1", r_am1bcc, CODELOC);
        
    return ds;
}

/** Constructor */
AM1BCC::AM1BCC() : ConcreteProperty<AM1BCC,QMChargeCalculator>()
{}

/** Copy constructor */
AM1BCC::AM1BCC(const AM1BCC &other)
       : ConcreteProperty<AM1BCC,QMChargeCalculator>(other),
         mopac(other.mopac)
{}

/** Destructor */
AM1BCC::~AM1BCC()
{}

const char* AM1BCC::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AM1BCC>() );
}

/** Comparison operator */
AM1BCC& AM1BCC::operator=(const AM1BCC &other)
{
    if (this != &other)
    {
        mopac = other.mopac;
    
        QMChargeCalculator::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool AM1BCC::operator==(const AM1BCC &other) const
{
    return this == &other or
           (mopac == other.mopac and QMChargeCalculator::operator==(other));
}

/** Comparison operator */
bool AM1BCC::operator!=(const AM1BCC &other) const
{
    return not AM1BCC::operator==(other);
}

/** This calculates and returns the AM1BCC charges for the atoms in 
    molecule 'molecule'. This only calculates the charges for the selected
    atoms in the molecule. Either default (0) charges, or the original
    charges are use for atoms that are not selected */
AtomCharges AM1BCC::operator()(const PartialMolecule &molecule,
                               const PropertyMap &map) const
{
    return AtomCharges( molecule.data().info() );
}

/** This returns whether or not the charges will change when going
    from 'oldmol' to 'newmol' - note that this assumes that the 
    charges in 'oldmol' are already AM1BCC charges! If they are
    not, then this will give the wrong answer! */
bool AM1BCC::mayChangeCharges(const PartialMolecule &oldmol,
                              const PartialMolecule &newmol,
                              const PropertyMap &map) const
{
    return true;
}
