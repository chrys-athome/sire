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
  * This file contains the boost::python wrapping of MoleculeBonds
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/moleculebonds.h"
#include "SireMol/bond.h"

using namespace boost::python;

namespace SireMol
{

void  
SIREMOL_EXPORT
export_MoleculeBonds()
{
    void (MoleculeBonds::*wrap_add1)(const AtomIndex&, const AtomIndex&)
                                      = &MoleculeBonds::add;
    void (MoleculeBonds::*wrap_add2)(ResNum, const QString&, const QString&)
                                      = &MoleculeBonds::add;
    void (MoleculeBonds::*wrap_add3)(const Bond&) = &MoleculeBonds::add;

    void (MoleculeBonds::*wrap_remove1)(const AtomIndex&, const AtomIndex&)
                                      = &MoleculeBonds::remove;
    void (MoleculeBonds::*wrap_remove2)(ResNum, const QString&, const QString&)
                                      = &MoleculeBonds::remove;
    void (MoleculeBonds::*wrap_remove3)(const Bond&)
                                      = &MoleculeBonds::remove;

    void (MoleculeBonds::*wrap_remove4)(const AtomIndex&) = &MoleculeBonds::remove;
    void (MoleculeBonds::*wrap_remove5)(ResNum) = &MoleculeBonds::remove;

    QList<Bond> (MoleculeBonds::*wrap_bonds1)() const = &MoleculeBonds::bonds;
    QList<Bond> (MoleculeBonds::*wrap_bonds2)(const AtomIndex&) const = &MoleculeBonds::bonds;
    QList<Bond> (MoleculeBonds::*wrap_bonds3)(ResNum) const = &MoleculeBonds::bonds;

    bool (MoleculeBonds::*wrap_contains1)(ResNum) const = &MoleculeBonds::contains;
    bool (MoleculeBonds::*wrap_contains2)(const AtomIndex&) const = &MoleculeBonds::contains;
    bool (MoleculeBonds::*wrap_contains3)(const Bond&) const = &MoleculeBonds::contains;

    bool (MoleculeBonds::*wrap_bonded1)(ResNum, ResNum) const = &MoleculeBonds::bonded;
    bool (MoleculeBonds::*wrap_bonded2)(const AtomIndex&, const AtomIndex&) const
                                     = &MoleculeBonds::bonded;


    class_<MoleculeBonds>("MoleculeBonds", init<>())
      .def(init<const MoleculeBonds&>())
      .def(self == self)
      .def(self != self)
      .def("__str__", &MoleculeBonds::toString)
      .def("add", wrap_add1)
      .def("add", wrap_add2)
      .def("add", wrap_add3)
      .def("isEmpty", &MoleculeBonds::isEmpty)
      .def("remove", wrap_remove1)
      .def("remove", wrap_remove2)
      .def("remove", wrap_remove3)
      .def("remove", wrap_remove4)
      .def("remove", wrap_remove5)
      .def("clear", &MoleculeBonds::clear)
      .def("finalise", &MoleculeBonds::finalise)
      .def("residue", &MoleculeBonds::residue)
      .def("bondedResidues", &MoleculeBonds::bondedResidues)
      .def("bonds", wrap_bonds1)
      .def("bonds", wrap_bonds2)
      .def("bonds", wrap_bonds3)
      .def("nBonds", &MoleculeBonds::nBonds)
      .def("contains", wrap_contains1)
      .def("contains", wrap_contains2)
      .def("contains", wrap_contains3)
      .def("bonded", wrap_bonded1)
      .def("bonded", wrap_bonded2)
      .def("resNumsBondedTo", &MoleculeBonds::resNumsBondedTo)
      .def("resNums", &MoleculeBonds::resNums)
    ;

}

}
