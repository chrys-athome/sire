
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireDB/using_database.h"

#include "wrapAssignInternals.hpp"
#include "assign_bonds.h"

using namespace boost::python;

namespace SireMM
{

void export_assign_bonds()
{
    wrap_assign_internals<MolBondInfo>("assign_internals_MolBondInfo_");
    
    class_<assign_bonds, bases< assign_internals<MolBondInfo> > >("assign_bonds", init<>())
    
        .def( init< const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, 
                    optional<const BondGeneratorBase&> >() )
        
        .def( init< const QSet<Bond>&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    const using_database&, const using_database&,
                    optional<const BondGeneratorBase&> >() )
                    
        .def( "typeName", &assign_bonds::typeName ).staticmethod("typeName")
    ;
}

}
