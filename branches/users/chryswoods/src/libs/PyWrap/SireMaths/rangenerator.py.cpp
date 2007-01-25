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

#include "SireMaths/rangenerator.h"
#include "SireMaths/vector.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMaths
{

void SIREMATHS_EXPORT export_RanGenerator()
{
    class_<RanGenerator>( "RanGenerator", init<>() )
        .def( init<quint32>() )
        .def( init<const QVector<quint32>&>() )
        .def( init<const RanGenerator&>() )
        
        .def( "__rrshift__", &__rrshift__QDataStream<RanGenerator>,
                        return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<RanGenerator>,
                        return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "seed", (void (RanGenerator::*)())
                  &RanGenerator::seed )
        .def( "seed", (void (RanGenerator::*)(quint32))
                  &RanGenerator::seed )
        .def( "seed", (void (RanGenerator::*)(const QVector<quint32>&))
                  &RanGenerator::seed )
        .def( "seed", (void (RanGenerator::*)(const RanGenerator&))
                  &RanGenerator::seed )

        .def( "rand", (double (RanGenerator::*)())
                  &RanGenerator::rand )
/*        .def( "rand", &rand_1 )
        .def( "rand", &rand_2 )*/
    
        .def( "rand53", (double (RanGenerator::*)())
                  &RanGenerator::rand53 )
/*        .def( "rand53", &rand53_1 )
        .def( "rand53", &rand53_2 )*/
    
        .def( "randNorm", (double (RanGenerator::*)(double, double))
                  &RanGenerator::randNorm )
//         .def( "randNorm", &randNorm_3 )
//         .def( "randNorm", &randNorm_4 )

        .def( "vectorOnSphere", (Vector (RanGenerator::*)())
                  &RanGenerator::vectorOnSphere )
/*        .def( "vectorOnSphere", &vectorOnSphere_1 )
        .def( "vectorOnSphere", &vectorOnSphere_2 )*/
    
        .def( "randInt", (quint32 (RanGenerator::*)())
                  &RanGenerator::randInt )
/*        .def( "randInt", &randInt_1 )
        .def( "randInt", &randInt_2 )*/
    
        .def( "randInt64", (quint64 (RanGenerator::*)())
                  &RanGenerator::randInt64 )
/*        .def( "randInt64", &randInt64_1 )
        .def( "randInt64", &randInt64_2 )*/
    
        .def( "getState", (QVector<quint32> (RanGenerator::*)())
                  &RanGenerator::getState )
        .def( "setState", (void (RanGenerator::*)(const QVector<quint32>&))
                  &RanGenerator::setState )
    ;
}

}
