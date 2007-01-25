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

#include "SireFF/ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void
SIREFF_EXPORT
export_FFBase()
{
    bp::class_< FFBase, boost::noncopyable >( "FFBase", bp::no_init )
        .def(
            "components"
            , &::SireFF::FFBase::components
            , bp::return_internal_reference<>() )
        .def(
            "energies"
            , (Values ( ::SireFF::FFBase::* )( ) )( &::SireFF::FFBase::energies )
            , bp::default_call_policies() )
        .def(
            "energies"
            , (Values ( ::SireFF::FFBase::* )( const ::QSet< ::SireFF::FFComponent>& ) )( &::SireFF::FFBase::energies )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFBase::* )(  ) )( &::SireFF::FFBase::energy )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFBase::* )( ::SireCAS::Function const & ) )( &::SireFF::FFBase::energy )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "ID"
            , &::SireFF::FFBase::ID
            , bp::default_call_policies() )
        .def(
            "version"
            , &::SireFF::FFBase::version
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "change"
            , (bool ( ::SireFF::FFBase::* )( ::SireMol::Molecule const & ) )(&::SireFF::FFBase::change)
            , ( bp::arg("mol") )
            , bp::default_call_policies() )
        .def(
            "change"
            , (bool ( ::SireFF::FFBase::* )( ::SireMol::Residue const & ) )(&::SireFF::FFBase::change)
            , ( bp::arg("res") )
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireFF::FFBase::name
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "parameters"
            , &::SireFF::FFBase::parameters
            , bp::return_internal_reference<>() )
        .def(
            "setName"
            , &::SireFF::FFBase::setName
            , ( bp::arg("name") )
            , bp::default_call_policies() )
        .def(
            "what"
            , bp::pure_virtual( &::SireFF::FFBase::what )
            , bp::default_call_policies() )
    ;

    bp::class_<FFBase::Components, boost::noncopyable>( "FFBase_Components", bp::no_init )

        .def( "total", &FFBase::Components::total,
                       bp::return_value_policy< bp::copy_const_reference,
                       bp::default_call_policies >() )

        .def( "describe_total", &FFBase::Components::describe_total,
                       bp::default_call_policies() ).staticmethod("describe_total")
    ;

    bp::class_<FFBase::Parameters, boost::noncopyable>( "FFBase_Parameters", bp::no_init )
    ;
}

}
