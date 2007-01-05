
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/atomselection.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/cgatomid.h"
#include "SireMol/resnum.h"

using namespace boost::python;

namespace SireMol
{

void SIREMOL_EXPORT export_AtomSelection()
{
    class_<AtomSelection>( "AtomSelection", init<>() )
    
        .def( init<const Molecule&>() )
        .def( init<const AtomSelection&>() )
        
        .def( "isEmpty", &AtomSelection::isEmpty )
        
        .def( "nSelected", (int (AtomSelection::*)() const)
                  &AtomSelection::nSelected )

        .def( "nSelected", (int (AtomSelection::*)(CutGroupID) const)
                  &AtomSelection::nSelected )
    
        .def( "nSelected", (int (AtomSelection::*)(ResNum) const)
                  &AtomSelection::nSelected )

        .def( "selected", (bool (AtomSelection::*)(const CGAtomID&) const)
                  &AtomSelection::selected )

        .def( "selectedAll", (bool (AtomSelection::*)() const)
                  &AtomSelection::selectedAll )

        .def( "selectedAll", (bool (AtomSelection::*)(CutGroupID) const)
                  &AtomSelection::selectedAll )
    
        .def( "selectedAll", (bool (AtomSelection::*)(ResNum) const)
                  &AtomSelection::selectedAll )

        .def( "selectAll", (void (AtomSelection::*)())
                  &AtomSelection::selectAll )
    
        .def( "deselectAll", (void (AtomSelection::*)())
                  &AtomSelection::deselectAll )

        .def( "selectAll", (void (AtomSelection::*)(CutGroupID))
                  &AtomSelection::selectAll )
        
        .def( "deselectAll", (void (AtomSelection::*)(CutGroupID))
                  &AtomSelection::deselectAll )

        .def( "selectAll", (void (AtomSelection::*)(ResNum))
                  &AtomSelection::selectAll )
    
        .def( "deselectAll", (void (AtomSelection::*)(ResNum))
                  &AtomSelection::deselectAll )

        .def( "select", (void (AtomSelection::*)(const CGAtomID&))
                  &AtomSelection::select )
    
        .def( "deselect", (void (AtomSelection::*)(const CGAtomID&))
                  &AtomSelection::deselect )
    ;
}

}
