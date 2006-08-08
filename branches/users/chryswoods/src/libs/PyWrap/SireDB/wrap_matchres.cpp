
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

void export_MatchRes()
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
