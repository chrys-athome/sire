
#include <boost/python.hpp>

#include "SireCAS/function.h"
#include "SireCAS/expression.h"

#include "SireFF/forcefieldid.h"

#include "SireSystem/ffexpression.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireCAS;
using namespace SireFF;
using namespace SireQt;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_FFExpression()
{
    class_<FFExpression>( "FFExpression", init<>() )
          .def( init<const QString&, const Expression&>() )
          .def( init<const Function&>() )
          .def( init<FFExpression&>() )
          
          .def( self == self )
          .def( self != self )
          
          .def( "__rrshift__", &__rrshift__QDataStream<FFExpression>,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )
          .def( "__rlshift__", &__rlshift__QDataStream<FFExpression>,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )
          
          .def( "forceFieldIDs", (const QSet<ForceFieldID>& (FFExpression::*)() const)
                    &FFExpression::forceFieldIDs, 
                    return_value_policy<copy_const_reference>() )
    
          .def( "dependencies", (const QSet<Function>& (FFExpression::*)() const)
                    &FFExpression::dependencies,
                    return_value_policy<copy_const_reference>() )

          .def( "function", (const Function& (FFExpression::*)() const)
                    &FFExpression::function,
                    return_value_policy<copy_const_reference>() )

          .def( "expression", (const Expression& (FFExpression::*)() const)
                    &FFExpression::expression,
                    return_value_policy<copy_const_reference>() )

          .def( "evaluate", (double (FFExpression::*)(const Values&) const)
                    &FFExpression::evaluate )
    ;
}

}
