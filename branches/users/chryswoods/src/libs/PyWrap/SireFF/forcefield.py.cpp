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

#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireFF/forcefield.h"
#include "SireFF/ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void SIREFF_EXPORT export_ForceField()
{
    bp::class_< SireFF::ForceField >( "ForceField" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireFF::FFBase const & >(( bp::arg("ffbase") )) )
        .def(
            "components"
            , &::SireFF::ForceField::components
            , bp::return_internal_reference<>() )
        .def(
            "energies"
            , (Values ( ::SireFF::ForceField::* )( ) )( &::SireFF::ForceField::energies )
            , bp::default_call_policies() )
        .def(
            "energies"
            , (Values ( ::SireFF::ForceField::* )( const ::QSet< ::SireFF::FFComponent>& ) )( &::SireFF::ForceField::energies )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::ForceField::* )(  ) )( &::SireFF::ForceField::energy )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::ForceField::* )( ::SireFF::FFComponent const & ) )( &::SireFF::ForceField::energy )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "parameters"
            , &::SireFF::ForceField::parameters
            , bp::return_internal_reference<>() )
        .def(
            "isClean"
            , &::SireFF::ForceField::isClean
            , bp::default_call_policies() )
        .def(
            "isDirty"
            , &::SireFF::ForceField::isDirty
            , bp::default_call_policies() )
        .def(
            "change"
            , (bool ( ::SireFF::ForceField::* )( ::SireMol::Molecule const & ) )( &::SireFF::ForceField::change )
            , ( bp::arg("mol") )
            , bp::default_call_policies() )
        .def(
            "change"
            , (bool ( ::SireFF::ForceField::* )( ::SireMol::Residue const & ) )( &::SireFF::ForceField::change )
            , ( bp::arg("res") )
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireFF::ForceField::name
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "setName"
            , &::SireFF::ForceField::setName
            , ( bp::arg("name") )
            , bp::default_call_policies() )
        .def( 
             "ID", &::SireFF::ForceField::ID )
        .def(
            "what"
            , &::SireFF::ForceField::what
            , bp::default_call_policies() )
    ;

    bp::implicitly_convertible< SireFF::FFBase const &, SireFF::ForceField >();
    //bp::register_ptr_to_python< SireFF::ForceField >();

}

}
