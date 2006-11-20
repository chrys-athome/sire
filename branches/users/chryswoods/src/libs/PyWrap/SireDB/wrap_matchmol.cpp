
/**
  * This file contains the boost::python wrapping of MatchMol
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
export_MatchMol()
{
    class_<MatchMol>("MatchMol", init<>())
        .def( self == self )
        .def( self != self )
        .def( "__and__", &MatchMol::operator&& )
        .def( "__str__", &MatchMol::toString )
        .def( "toString", &MatchMol::toString )
        .def( "isEmpty", &MatchMol::isEmpty )
        .def( "hasNameMatch", &MatchMol::hasNameMatch )
        .def( "hasGroupMatch", &MatchMol::hasGroupMatch )
        .def( "hasStateMatch", &MatchMol::hasStateMatch )
        .def( "setName", &MatchMol::setName )
        .def( "setGroup", &MatchMol::setGroup )
        .def( "setState", &MatchMol::setState )
        .def( "name", &MatchMol::name )
        .def( "group", &MatchMol::group )
        .def( "state", &MatchMol::state )
    ;

    implicitly_convertible<MatchMol,MatchMR>();
}

}
