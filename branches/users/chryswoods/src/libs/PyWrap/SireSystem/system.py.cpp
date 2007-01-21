
#include <boost/python.hpp>

#include "SireSystem/system.h"
#include "SireSystem/move.h"
#include "SireSystem/moves.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"

#include "SireFF/ffexpression.h"
#include "SireFF/forcefields.h"

#include "SireCAS/function.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireQt;
using namespace SireMol;
using namespace SireFF;
using namespace SireCAS;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_System()
{
    class_<System>( "System", init<>() )
        .def( init<const QString&>() )
        .def( init<const System&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__rrshift__", &__rrshift__QDataStream<System>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<System>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
    ;
}

}
