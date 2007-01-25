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

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "wrapInternalGenerator.hpp"

#include "SireMM/bondgenerator.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMM
{

void  
SIREMM_EXPORT
export_BondGenerator()
{
    wrap_InternalGenerator<MolBondInfo>("InternalGenerator_MolBondInfo_");
    wrap_UsePassedInternals<MolBondInfo>("UsePassedInternals_MolBondInfo_");
    
    class_<BondGenerator, bases<BondGeneratorBase> >("BondGenerator", init<>())
        
        .def( "typeName", &BondGenerator::typeName )
        
        .def( "__rlshift__", &__rlshift__QDataStream<BondGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<BondGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
    
    class_<UsePassedBonds, bases<UsePassedBondsBase> >("UsePassedBonds", init<>())
    
        .def( init<const QSet<Bond>&>() )
        
        .def( "typeName", &UsePassedBonds::typeName )
    
        .def( "__rlshift__", &__rlshift__QDataStream<UsePassedBonds>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<UsePassedBonds>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
}

}
