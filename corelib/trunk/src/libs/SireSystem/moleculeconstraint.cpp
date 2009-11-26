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

#include "moleculeconstraint.h"
#include "system.h"

#include "SireMol/molecules.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <boost/shared_ptr.hpp>

#include <QDebug>

using namespace SireSystem;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<MoleculeConstraint> r_molconstraint( MAGIC_ONLY, 
                                                       MoleculeConstraint::typeName() );

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const MoleculeConstraint &molconstraint)
{
    writeHeader(ds, r_molconstraint, 1);
    
    ds << molconstraint.sysuid
       << static_cast<const Constraint&>(molconstraint);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, 
                                          MoleculeConstraint &molconstraint)
{
    VersionID v = readHeader(ds, r_molconstraint);
    
    if (v == 1)
    {
        ds >> molconstraint.sysuid
           >> static_cast<Constraint&>(molconstraint);
           
        molconstraint.sysversion = Version();
    }
    else
        throw version_error( v, "1", r_molconstraint, CODELOC );

    return ds;
}

/** Constructor */
MoleculeConstraint::MoleculeConstraint() : Constraint()
{}

/** Copy constructor */
MoleculeConstraint::MoleculeConstraint(const MoleculeConstraint &other)
                   : Constraint(other),
                     sysuid(other.sysuid), sysversion(other.sysversion)
{}

/** Destructor */
MoleculeConstraint::~MoleculeConstraint()
{}

const char* MoleculeConstraint::typeName()
{
    return "SireSystem::MoleculeConstraint";
}

/** Copy assignment operator */
MoleculeConstraint& MoleculeConstraint::operator=(const MoleculeConstraint &other)
{
    if (this != &other)
    {
        sysuid = other.sysuid;
        sysversion = other.sysversion;
        Constraint::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool MoleculeConstraint::operator==(const MoleculeConstraint &other) const
{
    return sysuid == other.sysuid and sysversion == other.sysversion and
           Constraint::operator==(other);
}

/** Comparison operator */
bool MoleculeConstraint::operator!=(const MoleculeConstraint &other) const
{
    return not this->operator==(other);
}

/** Internal function called to record the UID and version of the 
    system from which this constraint has been updated */
void MoleculeConstraint::updatedFrom(const System &system)
{
    sysuid = system.UID();
    sysversion = system.version();
}

/** Apply this constraint to the system 'system' - this returns whether
    or not this changes the system */
bool MoleculeConstraint::apply(System &system) const
{
    boost::shared_ptr<MoleculeConstraint> c( this->clone() );
    
    Molecules changed_mols = c->update(system);
    
    if (not changed_mols.isEmpty())
    {
        system.update(changed_mols);
        return true;
    }
    else
        return false;
}

/** Return whether or not this constraint is satisfied in the 
    passed system */
bool MoleculeConstraint::isSatisfied(const System &system) const
{
    if (sysuid == system.UID() and sysversion == system.version())
        return true;
    else
    {
        boost::shared_ptr<MoleculeConstraint> c( this->clone() );
    
        Molecules changed_mols = c->update(system);
        
        return changed_mols.isEmpty();
    }
}

/** Return the UID of the system from which this constraint was
    last updated */
const QUuid& MoleculeConstraint::sysUID() const
{
    return sysuid;
}

/** Return the version of the system from which this constraint
    was last updated */
const Version& MoleculeConstraint::sysVersion() const
{
    return sysversion;
}
