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

/**
  * This file contains the boost::python wrapping of MatchRes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/matchdata.h"
#include "SireDB/matchatom.h"
#include "SireDB/matchres.h"
#include "SireDB/matchmol.h"
#include "SireDB/matchatomtype.h"
#include "SireDB/matchatomtypedata.h"
#include "SireDB/matchmr.h"
#include "SireDB/matchmrdata.h"
#include "SireDB/matchmra.h"
#include "SireDB/matchmradata.h"

using namespace boost::python;

namespace SireDB
{

void  
SIREDB_EXPORT
export_MatchRes()
{
    QString (MatchRes::*wrap_MatchRes_toString1)() const = &MatchRes::toString;
    QString (MatchRes::*wrap_MatchRes_toString2)(uint atm) const = &MatchRes::toString;
    
    class_<MatchRes>("MatchRes", init<>())
        .def( self == self )
        .def( self != self )
        .def( "__and__", &MatchRes::operator&& )
        .def( "__str__", wrap_MatchRes_toString1 )
        .def( "toString", wrap_MatchRes_toString1 )
        .def( "toString", wrap_MatchRes_toString2 )
        .def( "isEmpty", &MatchRes::isEmpty )
        .def( "hasNumberMatch", &MatchRes::hasNumberMatch )
        .def( "hasNameMatch", &MatchRes::hasNameMatch )
        .def( "hasResResBonds", &MatchRes::hasResResBonds )
        .def( "hasNoResResBonds", &MatchRes::hasNoResResBonds )
        .def( "hasAliasMatch", &MatchRes::hasAliasMatch )
        .def( "setName", &MatchRes::setName )
        .def( "setNumber", &MatchRes::setNumber )
        .def( "setAlias", &MatchRes::setAlias )
        .def( "addResResBond", &MatchRes::addResResBond )
        .def( "addNoResResBond", &MatchRes::addNoResResBond )
        .def( "name", &MatchRes::name )
        .def( "number", &MatchRes::number )
        .def( "alias", &MatchRes::alias )
        .def( "resResBonds", &MatchRes::resResBonds )
        .def( "noResResBonds", &MatchRes::noResResBonds )
    ;
    
    implicitly_convertible<MatchRes,MatchMR>();
}

}
