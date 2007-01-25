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

#include "SireMM/cljff.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireDB/parametertable.h"

namespace bp = boost::python;

using namespace SireFF;

namespace SireMM
{

void
SIREMM_EXPORT
export_CLJFF()
{
    bp::class_< CLJFF, bp::bases<SireFF::FFBase>, boost::noncopyable >( "CLJFF", bp::no_init )

        .def(
            "space"
            , &::SireMM::CLJFF::space
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "switchingFunction"
            , &::SireMM::CLJFF::switchingFunction
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
    ;

    bp::class_<CLJFF::Components, bp::bases<FFBase::Components> >("CLJFF_Components")

        .def( bp::init<>() )

        .def( "coulomb", &CLJFF::Components::coulomb,
                bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def( "lj", &CLJFF::Components::lj,
                bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )

        .def( "describe_coulomb", &CLJFF::Components::describe_coulomb,
                bp::default_call_policies() ).staticmethod("describe_coulomb")

        .def( "describe_lj", &CLJFF::Components::describe_lj,
                bp::default_call_policies() ).staticmethod("describe_lj")
    ;

    bp::class_<CLJFF::Parameters, bp::bases<FFBase::Parameters> >("CLJFF_Parameters")

        .def( bp::init<>() )
        .def( "coulomb", &CLJFF::Parameters::coulomb,
                            bp::return_value_policy<bp::copy_const_reference>() )
        .def( "lj", &CLJFF::Parameters::lj,
                            bp::return_value_policy<bp::copy_const_reference>() )
    ;

}

}
