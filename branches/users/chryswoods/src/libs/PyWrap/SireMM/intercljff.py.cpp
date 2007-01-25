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

#include "SireMM/intercljff.h"
#include "SireMM/chargetable.h"
#include "SireMM/ljtable.h"

#include "SireMM/switchingfunction.h"
#include "SireMM/combiningrules.h"

#include "SireVol/space.h"

#include "SireMol/molecule.h"

using namespace boost::python;
using namespace SireVol;
using namespace SireMol;

namespace SireMM
{

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(intercljff_add_overloads, add, 1, 2);

void SIREMM_EXPORT export_InterCLJFF()
{
    class_< InterCLJFF, bases< SireMM::CLJFF > >( "InterCLJFF" )
        .def( init<>() )
        .def( init<const Space&, const SwitchingFunction&>() )
        .def( "typeName", &InterCLJFF::typeName )

        .def( "add", &InterCLJFF::add, intercljff_add_overloads() )

        .staticmethod( "typeName" )
    ;
}

}

