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

/**
  * This file contains the boost::python wrapping of a CutGroup, CutGroupList and CutGroupSet
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/cutgroup.h"
#include "SireMol/atom.h"

#include "SireMaths/quaternion.h"

using namespace boost::python;

namespace SireMol
{

void  
SIREMOL_EXPORT
export_CutGroup()
{
/*    const AtomSet& (CutGroup::*wrap_atoms1)() const = &CutGroup::atoms;
    AtomSet (CutGroup::*wrap_atoms2)(AtomID strt, AtomID end) const = &CutGroup::atoms;

    const Atom& (CutGroup::*wrap_atom1)(AtomID) const = &CutGroup::atom;
    const Atom& (CutGroup::*wrap_atom2)(const AtomIndex&) const = &CutGroup::atom;

    VectorVector (CutGroup::*wrap_coordinates1)() const = &CutGroup::coordinates;
    const Vector& (CutGroup::*wrap_coordinates2)(const AtomIndex&) const = &CutGroup::coordinates;
    const Vector& (CutGroup::*wrap_coordinates3)(AtomID) const = &CutGroup::coordinates;

    void (CutGroup::*wrap_setCoordinates1)(const VectorVector&) = &CutGroup::setCoordinates;
    void (CutGroup::*wrap_setCoordinates2)(const AtomIndex&, const Vector&)
                                       = &CutGroup::setCoordinates;
    void (CutGroup::*wrap_setCoordinates3)(AtomID, const Vector&) = &CutGroup::setCoordinates;*/

    /** Provide the wrapping for the CutGroup object */
    class_<CutGroup>("CutGroup", init<>())
/*          .def(init<const AtomSet&>())
          .def(init<const CutGroup&>())
          .def("ID", &CutGroup::ID, return_value_policy<copy_const_reference>())
          .def("setID", &CutGroup::setID)
          .def("__getitem__",&readonly_indexer<CutGroup,Atom>::__getitem__)
          .def("__len__", &CutGroup::size)
          .def("__str__", &CutGroup::toString)
          .def("toString", &CutGroup::toString)
          .def("atoms", wrap_atoms1, return_value_policy<copy_const_reference>())
          .def("atoms", wrap_atoms2)
          .def("at", &CutGroup::at, return_value_policy<copy_const_reference>())
          .def("atom", wrap_atom1, return_value_policy<copy_const_reference>())
          .def("atom", wrap_atom2, return_value_policy<copy_const_reference>())
          .def("nAtoms", &CutGroup::nAtoms)
          .def("size", &CutGroup::size)
          .def("count", &CutGroup::count)
          .def("aaBox", &CutGroup::aaBox, return_value_policy<copy_const_reference>())
          .def("contains", &CutGroup::contains)
          .def("coordinates", wrap_coordinates1)
          .def("coordinates", wrap_coordinates2, return_value_policy<copy_const_reference>())
          .def("coordinates", wrap_coordinates3, return_value_policy<copy_const_reference>())
          .def("setCoordinates", wrap_setCoordinates1)
          .def("setCoordinates", wrap_setCoordinates2)
          .def("setCoordinates", wrap_setCoordinates3)*/
    ;
}

}
