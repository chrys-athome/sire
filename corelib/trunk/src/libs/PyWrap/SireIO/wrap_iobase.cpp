
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
#include <QString>
#include <QIODevice>

#include "SireMol/editmol.h"

using namespace boost::python;
using namespace SireMol;

namespace SireIO
{

void export_IOBase()
{
    /** Create some function pointers for overloaded functions */
    EditMolList (IOBase::*read_QStr)(QString) const = &IOBase::read;
    EditMolList (IOBase::*read_QIO)(QIODevice&) const = &IOBase::read;
    
    void (IOBase::*write_Edit_QStr)(const EditMolList&, QString) const = &IOBase::write;
    void (IOBase::*write_Edit_QIO)(const EditMolList&, QIODevice&) const = &IOBase::write;
    void (IOBase::*write_EditMol_QStr)(const EditMol&, QString) const = &IOBase::write;
    void (IOBase::*write_EditMol_QIO)(const EditMol&, QIODevice&) const = &IOBase::write;
    
                          
    class_<IOBase, boost::noncopyable>("IOBase", no_init)
              .def("read", read_QStr)
              .def("read", read_QIO)
              .def("write", write_Edit_QStr)
              .def("write", write_Edit_QIO)
              .def("write", write_EditMol_QStr)
              .def("write", write_EditMol_QIO)
    ;
    
    class_<PDB, bases<IOBase> >("PDB", init<>());
    
    class_<MCT, bases<IOBase> >("MCT", init<>());
    
    class_<Mol2, bases<IOBase> >("Mol2", init<>());
  
}

}
