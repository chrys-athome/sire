
/**
  * This file contains the boost::python wrapping of the Match/MatchData classes
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

void export_MatchAtom();
void export_MatchAtomType();
void export_MatchRes();
void export_MatchMol();
void export_MatchMR();
void export_MatchMRA();

void  
SIREDB_EXPORT
export_Match()
{
    class_<MatchData, boost::noncopyable>("MatchData", no_init)
        .def( "__str__", &MatchData::toString )
        .def( "toString", &MatchData::toString )
        .def( "isEmpty", &MatchData::isEmpty )
        .def( "nMatches", &MatchData::nMatches )
        .def( "addTo", &MatchData::addTo )
    
        //wrap up "isA<T>()" in python
    ;

    //need to allow MatchData to be implicitly converted to 
    //the AssertMatch classes
    implicitly_convertible< MatchData, AssertMatch<1> >();
    implicitly_convertible< MatchData, AssertMatch<2> >();
    implicitly_convertible< MatchData, AssertMatch<3> >();
    implicitly_convertible< MatchData, AssertMatch<4> >();

    //export the match classes
    export_MatchRes();
    export_MatchMol();
    export_MatchMR();
    export_MatchAtom();
    export_MatchMRA();
    export_MatchAtomType();
}

}
