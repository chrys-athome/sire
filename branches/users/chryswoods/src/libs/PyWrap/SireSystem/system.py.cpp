
#include <boost/python.hpp>

#include "SireSystem/system.h"
#include "SireSystem/moleculegroup.h"
#include "SireSystem/ffexpression.h"
#include "SireSystem/move.h"
#include "SireSystem/moves.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

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
    class_<System, bases<SystemData> >( "System", init<>() )
        .def( init<const QString&>() )
        .def( init<const System&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__rrshift__", &__rrshift__QDataStream<System>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<System>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        
        .def( "add", (void (System::*)(const ForceField&))
                    &System::add )
        
        .def( "remove", (void (System::*)(const ForceField&))
                    &System::remove )

        .def( "add", (void (System::*)(const FFExpression&))
                    &System::add )
    
        .def( "remove", (void (System::*)(const FFExpression&))
                    &System::remove )
        
        .def( "remove", (void (System::*)(const Function&))
                    &System::remove )

        .def( "add", (void (System::*)(const MoleculeGroup&))
                    &System::add )
        
        .def( "remove", (void (System::*)(const MoleculeGroup&))
                    &System::remove )

        .def( "change", (void (System::*)(const Molecule&))
                    &System::change )
        .def( "change", (void (System::*)(const Residue&))
                    &System::change )
        .def( "change", (void (System::*)(const NewAtom&))
                    &System::change )

        .def( "remove", (void (System::*)(const Molecule&))
                    &System::remove )

        .def( "forceFields", (const QHash<ForceFieldID,ForceField>& (System::*)() const)
                    &System::forceFields,
                    return_value_policy<copy_const_reference>() )

        .def( "run", (void (System::*)(const Move&, quint32))
                    &System::run )
    
        .def( "run", (void (System::*)(const Moves&))
                    &System::run )
    
        .def( "run", (void (System::*)(const Moves&, quint32))
                    &System::run )
    ;
}

}
