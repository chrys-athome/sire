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
#include "SireFF/forcefield.h"
#include "SireFF/ffcalculator.h"
#include "SireFF/ffworker.h"
#include "SireFF/ffthreadworker.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void SIREFF_EXPORT export_FFWorker()
{
    bp::class_< FFWorkerBase, boost::noncopyable >( "FFWorkerBase", bp::no_init )
        .def(
            "assertContains"
            , &::SireFF::FFWorkerBase::assertContains
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "energies"
            , &::SireFF::FFWorkerBase::energies
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFWorkerBase::* )(  ) )( &::SireFF::FFWorkerBase::energy )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFWorkerBase::* )( ::SireFF::FFComponent const & ) )( &::SireFF::FFWorkerBase::energy )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "forcefield"
            , bp::pure_virtual( &::SireFF::FFWorkerBase::forcefield )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireFF::FFWorkerBase::* )( ::SireMol::Molecule const & ) )( &::SireFF::FFWorkerBase::change )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "change"
            , (void ( ::SireFF::FFWorkerBase::* )( ::SireMol::Residue const & ) )( &::SireFF::FFWorkerBase::change )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "recalculateEnergy"
            , &::SireFF::FFWorkerBase::recalculateEnergy
            , bp::default_call_policies() )
        .def(
            "setForceField"
            , &::SireFF::FFWorkerBase::setForceField
            , ( bp::arg("forcefield") )
            , bp::default_call_policies() )
        .def(
            "waitUntilReady"
            , &::SireFF::FFWorkerBase::waitUntilReady
            , bp::default_call_policies() )
    ;

    bp::register_ptr_to_python< boost::shared_ptr<SireFF::FFWorkerBase> >();

    bp::class_< FFWorker, bp::bases< SireFF::FFWorkerBase, SireCluster::WorkerBase >,
                boost::noncopyable >( "FFWorker", bp::no_init )
    ;

    bp::register_ptr_to_python< boost::shared_ptr<SireFF::FFWorker> >();

    bp::class_< FFThreadWorker, bp::bases< SireFF::FFWorkerBase, SireCluster::ThreadWorker >,
                boost::noncopyable >( "FFThreadWorker", bp::no_init );
    ;
}

}
