
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireDB/using_database.h"

#include "wrapAssignInternals.hpp"

#include "SireMM/assign_angles.h"

using namespace boost::python;

namespace SireMM
{

void  
SIREMM_EXPORT
export_assign_angles()
{
    wrap_assign_internals<MolAngleInfo>("assign_internals_MolAngleInfo_");
    
    class_<assign_angles, bases< assign_internals<MolAngleInfo> > >
                                ("assign_angles", init<>())
    
        .def( init< const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, 
                    optional<const AngleGeneratorBase&> >() )
        
        .def( init< const QSet<Angle>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const AngleGeneratorBase&> >() )
                    
        .def( "typeName", &assign_angles::typeName ).staticmethod("typeName")
    ;
}

}
