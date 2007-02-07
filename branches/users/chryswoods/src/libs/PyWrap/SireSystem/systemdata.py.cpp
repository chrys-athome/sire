/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include <boost/python.hpp>

#include "SireMol/wrapID.hpp"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"

#include "SireSystem/systemdata.h"

#include "SireQt/qdatastream.hpp"

using namespace SireMol;
using namespace SireBase;
using namespace SireQt;

using namespace boost::python;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_SystemData()
{
    wrap_ID<SystemID>("SystemID");

    class_<SystemData>( "SystemData", init<>() )
        .def( init<const QString&>() )

        .def( self == self )
        .def( self != self )

        .def( "__rrshift__", &__rrshift__QDataStream<SystemData>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<SystemData>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )

        .def( "name", (const QString& (SystemData::*)() const)
                    &SystemData::name, return_value_policy<copy_const_reference>() )

        .def( "ID", (SystemID (SystemData::*)() const)
                    &SystemData::ID )
        .def( "version", (const Version& (SystemData::*)() const)
                    &SystemData::version, return_value_policy<copy_const_reference>() )

        .def( "add", (void (SystemData::*)(const MoleculeGroup&))
                    &SystemData::add )

        .def( "contains", (bool (SystemData::*)(const Molecule&) const)
                    &SystemData::contains )
        .def( "contains", (bool (SystemData::*)(MoleculeID) const)
                    &SystemData::contains )

        .def( "remove", (void (SystemData::*)(MoleculeGroupID))
                    &SystemData::remove )
        .def( "remove", (void (SystemData::*)(const MoleculeGroup&))
                    &SystemData::remove )

        .def( "applyConstraints", (QHash<MoleculeID,Molecule> (SystemData::*)(const Molecule&))
                    &SystemData::applyConstraints )

        .def( "group", (const MoleculeGroup& (SystemData::*)(MoleculeGroupID) const)
                    &SystemData::group, return_value_policy<copy_const_reference>() )
        .def( "groups", (const MoleculeGroups& (SystemData::*)() const)
                    &SystemData::groups, return_value_policy<copy_const_reference>() )

        .def( "change", (QHash<MoleculeID,Molecule> (SystemData::*)(const Molecule&))
                    &SystemData::change )
        .def( "change", (QHash<MoleculeID,Molecule> (SystemData::*)(const Residue&))
                    &SystemData::change )
        .def( "change", (QHash<MoleculeID,Molecule> (SystemData::*)(const NewAtom&))
                    &SystemData::change )

        .def( "remove", (void (SystemData::*)(const Molecule&))
                    &SystemData::remove )

        .def( "incrementMinorVersion", (void (SystemData::*)())
                    &SystemData::incrementMinorVersion )
        .def( "incrementMajorVersion", (void (SystemData::*)())
                    &SystemData::incrementMajorVersion )
    ;
}

}
