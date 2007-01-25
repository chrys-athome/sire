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
  * This file contains the boost::python wrapping of ResidueBonds
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include <QSet>

#include "SireMol/residuebonds.h"
#include "SireMol/bond.h"

using namespace boost::python;

namespace SireMol
{

void  
SIREMOL_EXPORT
export_ResidueBonds()
{

    QList<Bond> (ResidueBonds::*wrap_bonds1)() const = &ResidueBonds::bonds;
    QList<Bond> (ResidueBonds::*wrap_bonds2)(ResNum) const = &ResidueBonds::bonds;

    QList<Bond> (ResidueBonds::*wrap_interBonds1)(const QString&) const
                      = &ResidueBonds::interBonds;
    QList<Bond> (ResidueBonds::*wrap_interBonds2)() const
                      = &ResidueBonds::interBonds;

    QList<Bond> (ResidueBonds::*wrap_intraBonds1)(const QString&) const = &ResidueBonds::intraBonds;
    QList<Bond> (ResidueBonds::*wrap_intraBonds2)() const = &ResidueBonds::intraBonds;

    bool (ResidueBonds::*wrap_contains1)(const QString&) const = &ResidueBonds::contains;
    bool (ResidueBonds::*wrap_contains2)(const Bond&) const = &ResidueBonds::contains;


    class_<ResidueBonds>("ResidueBonds", init<>())
        .def(init<ResNum>())
        .def(init<const ResidueBonds&>())
        .def(self == self)
        .def(self != self)
        .def("__str__", &ResidueBonds::toString)
        .def("isEmpty", &ResidueBonds::isEmpty)
        .def("resNum", &ResidueBonds::resNum)
        .def("bondedResidues", &ResidueBonds::bondedResidues)
        .def("residuesBondedTo", &ResidueBonds::residuesBondedTo)
        .def("bondedTo", &ResidueBonds::bondedTo)
        .def("bonded", &ResidueBonds::bonded)
        .def("interBonds", wrap_interBonds1)
        .def("interBonds", wrap_interBonds2)
        .def("intraBonds", wrap_intraBonds1)
        .def("intraBonds", wrap_intraBonds2)
        .def("asymmetricBonds", &ResidueBonds::asymmetricBonds)
        .def("bonds", wrap_bonds1)
        .def("bonds", wrap_bonds2)
        .def("nBonds", &ResidueBonds::nBonds)
        .def("nAsymmetricBonds", &ResidueBonds::nAsymmetricBonds)
        .def("anchors", &ResidueBonds::anchors)
        .def("interBondedAtoms", &ResidueBonds::interBondedAtoms)
        .def("contains", wrap_contains1)
        .def("contains", wrap_contains2)
    ;

}

}
