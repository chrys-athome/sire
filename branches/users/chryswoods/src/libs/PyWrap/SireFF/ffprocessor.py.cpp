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

#include "SireFF/ffprocessor.h"
#include "SireFF/ffthreadprocessor.h"
#include "SireFF/forcefield.h"
#include "SireFF/ffcalculator.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void SIREFF_EXPORT export_FFProcessor()
{
    bp::class_< FFProcessorBase, bp::bases<SireCluster::Processor>,
                boost::noncopyable >( "FFProcessorBase", bp::no_init )
        .def(
            "forcefield"
            , &::SireFF::FFProcessorBase::forcefield
            , bp::default_call_policies() )
        .def(
            "setForceField"
            , &::SireFF::FFProcessorBase::setForceField
            , ( bp::arg("forcefield") )
            , bp::default_call_policies() )
    ;

    bp::class_< FFProcessor, bp::bases< SireFF::FFProcessorBase > >( "FFProcessor" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireFF::ForceField const & >(( bp::arg("ffield") )) )
        .def( bp::init< SireFF::FFProcessor const & >(( bp::arg("other") )) )
        .def(
            "typeName"
            , &::SireFF::FFProcessor::typeName
            , bp::default_call_policies() )
        .staticmethod( "typeName" )
    ;

    bp::class_< FFThreadProcessor, bp::bases< SireFF::FFProcessorBase > >( "FFThreadProcessor" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireFF::ForceField const & >(( bp::arg("ffield") )) )
        .def( bp::init< SireFF::FFThreadProcessor const & >(( bp::arg("other") )) )
        .def(
            "typeName"
            , &::SireFF::FFThreadProcessor::typeName
            , bp::default_call_policies() )
        .staticmethod( "typeName" )
    ;

}

}
