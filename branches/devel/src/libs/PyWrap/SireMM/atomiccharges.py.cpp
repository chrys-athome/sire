
#include <boost/python.hpp>

#include "SireMM/atomiccharges.h"

#include "SireQt/qvector.hpp"

using namespace boost::python;

using namespace SireQt;
using namespace SireMol;

namespace SireMM
{

void SIREMM_EXPORT export_AtomicCharges()
{
    //Atomic charges is based on QVector< QVector<ChargeParameter> >
    // Wrap up these first....

    //wrap up QVector<ChargeParameter>
    wrap_QVector<ChargeParameter>( "QVector_ChargeParameter_" );

    //now QVector< QVector<ChargeParameter> >
    wrap_QVector< QVector<ChargeParameter> >( "QVector_QVector_ChargeParameter_" );

    //now AtomicCharges
    class_<AtomicCharges,
           bases< PropertyBase, QVector< QVector<ChargeParameter> > >
          >( "AtomicCharges", init<>() )


        .def( init<const QVector< QVector<ChargeParameter> >&>() )
        .def( init<const QVector<ChargeParameter>&>() )
        .def( init<const Property&>() )
        .def( init<const AtomicCharges&>() )
    ;
}

}
