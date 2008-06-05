
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireDB/using_database.h"

#include "wrapAssignInternals.hpp"

#include "SireMM/assign_dihedrals.h"

using namespace boost::python;

namespace SireMM
{

void export_assign_dihedrals()
{
    wrap_assign_internals<MolDihedralInfo>("assign_internals_MolDihedralInfo_");
    
    class_<assign_dihedrals, bases< assign_internals<MolDihedralInfo> > >
                          ("assign_dihedrals", init<>())
    
        .def( init< const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, 
                    optional<const DihedralGeneratorBase&> >() )
        
        .def( init< const QSet<Dihedral>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const DihedralGeneratorBase&> >() )
                    
        .def( "typeName", &assign_dihedrals::typeName ).staticmethod("typeName")
    ;
}

}
