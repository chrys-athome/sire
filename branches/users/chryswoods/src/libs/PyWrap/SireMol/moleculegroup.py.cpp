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

#include "SireSystem/moleculegroup.h"

#include "SireMol/molecule.h"

#include "SireMol/wrapID.hpp"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireQt;
using namespace SireMol;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_MoleculeGroup()
{
    wrap_ID<MoleculeGroupID>("MoleculeGroupID");

    class_<MoleculeGroup>( "MoleculeGroup", init<>() )
        .def( init<const QString&>() )
        .def( init<const QString&, const QVector<Molecule>&>() )
        .def( init<const MoleculeGroup&>() )
    
        .def( self == self )
        .def( self != self )
        
        .def( "__rrshift__", &__rrshift__QDataStream<MoleculeGroup>,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<MoleculeGroup>,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )

        .def( "at", (const Molecule& (MoleculeGroup::*)(MoleculeID) const)
                  &MoleculeGroup::at,
                  return_value_policy<copy_const_reference>() )

        .def( "molecule", (const Molecule& (MoleculeGroup::*)(MoleculeID) const)
                  &MoleculeGroup::molecule,
                  return_value_policy<copy_const_reference>() )
        
        .def( "at", (const Molecule& (MoleculeGroup::*)(const Molecule&) const)
                  &MoleculeGroup::at,
                  return_value_policy<copy_const_reference>() )
        
        .def( "molecule", (const Molecule& (MoleculeGroup::*)(const Molecule&) const)
                  &MoleculeGroup::molecule,
                  return_value_policy<copy_const_reference>() )

        .def( "name", (const QString& (MoleculeGroup::*)() const)
                  &MoleculeGroup::name,
                  return_value_policy<copy_const_reference>() )

        .def( "ID", (MoleculeGroupID (MoleculeGroup::*)() const)
                  &MoleculeGroup::ID )
    
        .def( "version", (const Version& (MoleculeGroup::*)() const)
                  &MoleculeGroup::version,
                  return_value_policy<copy_const_reference>() )

        .def( "molecules", (const QVector<Molecule>& (MoleculeGroup::*)() const)
                  &MoleculeGroup::molecules,
                  return_value_policy<copy_const_reference>() )

        .def( "count", (int (MoleculeGroup::*)() const)
                  &MoleculeGroup::count )

        .def( "add", (void (MoleculeGroup::*)(const Molecule&))
                  &MoleculeGroup::add )
        .def( "change", (void (MoleculeGroup::*)(const Molecule&))
                  &MoleculeGroup::change )
        .def( "remove", (void (MoleculeGroup::*)(const Molecule&))
                  &MoleculeGroup::remove )

        .def( "add", (void (MoleculeGroup::*)(const QVector<Molecule>&))
                  &MoleculeGroup::add )
        .def( "change", (void (MoleculeGroup::*)(const QVector<Molecule>&))
                  &MoleculeGroup::change )
        .def( "remove", (void (MoleculeGroup::*)(const QVector<Molecule>&))
                  &MoleculeGroup::remove )

        .def( "rename", (void (MoleculeGroup::*)(const QString&))
                  &MoleculeGroup::rename )

        .def( "contains", (bool (MoleculeGroup::*)(const Molecule&) const)
                  &MoleculeGroup::contains )
        .def( "contains", (bool (MoleculeGroup::*)(MoleculeID) const)
                  &MoleculeGroup::contains )
    ;
}

}
