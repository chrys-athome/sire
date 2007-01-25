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
  * This file contains the boost::python wrapping of MoleculeIO
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireIO/iobase.h"
#include "SireIO/pdb.h"

#include <QByteArray>
#include <QList>
#include <QString>
#include <QIODevice>

#include "SireMol/molecule.h"
#include "SireMol/editmol.h"

using namespace boost::python;
using namespace SireMol;

namespace SireIO
{

QList<Molecule> read1(const IOBase &iobase, const QString &file)
{
    return iobase.read(file);
}

QList<Molecule> read2(const IOBase &iobase, const QString &file,
                      const CuttingFunction &cutfunc)
{
    return iobase.read(file, cutfunc);
}

QList<Molecule> read3(const IOBase &iobase, QIODevice &iodevice)
{
    return iobase.read(iodevice);
}

QList<Molecule> read4(const IOBase &iobase, QIODevice &iodevice,
                      const CuttingFunction &cutfunc)
{
    return iobase.read(iodevice, cutfunc);
}

void
SIREIO_EXPORT
export_IOBase()
{

    class_<IOBase, boost::noncopyable>("IOBase", no_init)
              .def("read", &read1)
              .def("read", &read2)
              .def("read", &read3)
              .def("read", &read4)

              .def( "write", (void (IOBase::*)(const QList<Molecule>&,
                                               QString) const)
                              &IOBase::write )
              .def( "write", (void (IOBase::*)(const QList<Molecule>&,
                                               QIODevice&) const)
                              &IOBase::write )
              .def( "write", (void (IOBase::*)(const Molecule&,
                                               QString) const)
                              &IOBase::write )
              .def( "write", (void (IOBase::*)(const Molecule&,
                                               QIODevice&) const)
                              &IOBase::write )

              .def( "write", (void (IOBase::*)(const QList<EditMol>&,
                                               QString) const)
                              &IOBase::write )
              .def( "write", (void (IOBase::*)(const QList<EditMol>&,
                                               QIODevice&) const)
                              &IOBase::write )
              .def( "write", (void (IOBase::*)(const EditMol&,
                                               QString) const)
                              &IOBase::write )
              .def( "write", (void (IOBase::*)(const EditMol&,
                                               QIODevice&) const)
                              &IOBase::write )

    ;

    class_<PDB, bases<IOBase> >("PDB", init<>());
}

}
