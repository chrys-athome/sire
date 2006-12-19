
#include <boost/python.hpp>

#include "SireMM/atomicljs.h"

#include "SireQt/qvector.hpp"

using namespace boost::python;

using namespace SireQt;
using namespace SireMol;

namespace SireMM
{

void SIREMM_EXPORT export_AtomicLJs()
{
    //Atomic ljs is based on QVector< QVector<LJParameter> >
    // Wrap up these first....

    //wrap up QVector<LJParameter>
    wrap_QVector<LJParameter>( "QVector_LJParameter_" );

    //now QVector< QVector<LJParameter> >
    wrap_QVector< QVector<LJParameter> >( "QVector_QVector_LJParameter_" );

    //now AtomicCharges
    class_<AtomicLJs,
           bases< PropertyBase, QVector< QVector<LJParameter> > >
          >( "AtomicLJs", init<>() )


        .def( init<const QVector< QVector<LJParameter> >&>() )
        .def( init<const QVector<LJParameter>&>() )
        .def( init<const AtomicLJs&>() )
    ;
}

}
