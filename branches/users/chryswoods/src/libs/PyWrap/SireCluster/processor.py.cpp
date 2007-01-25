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

#include "SireCluster/processor.h"

namespace bp = boost::python;

namespace SireCluster
{

void SIRECLUSTER_EXPORT export_Processor()
{
    bp::class_< Processor, boost::noncopyable >( "Processor", bp::no_init )
        .def(
            "activate"
            , &::SireCluster::Processor::activate
            , bp::default_call_policies() )
        .def(
            "isActive"
            , &::SireCluster::Processor::isActive
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireCluster::Processor::name
            , bp::default_call_policies() )
        .def(
            "setName"
            , &::SireCluster::Processor::setName
            , ( bp::arg("newname") )
            , bp::default_call_policies() )
        .def(
            "what"
            , &::SireCluster::Processor::what
            , bp::default_call_policies() )
    ;

    bp::class_< WorkerBase, boost::noncopyable >( "WorkerBase", bp::no_init )
    ;

    bp::register_ptr_to_python< boost::shared_ptr<WorkerBase> >();
}

}
