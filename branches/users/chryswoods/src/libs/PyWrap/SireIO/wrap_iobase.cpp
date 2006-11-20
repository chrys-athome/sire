
/**
  * This file contains the boost::python wrapping of MoleculeIO
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireIO/iobase.h"
#include "SireIO/mct.h"
#include "SireIO/pdb.h"
#include "SireIO/mol2.h"

#include <QByteArray>
#include <QList>
#include <QString>
#include <QIODevice>

#include "SireMol/molecule.h"

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
    ;

    class_<PDB, bases<IOBase> >("PDB", init<>());
}

}
