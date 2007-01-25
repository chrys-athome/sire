/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
