
/**
  * This file contains the boost::python wrapping of parameter IO classes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireIO/psf.h"
#include "SireIO/protoms.h"

using namespace boost::python;

namespace SireIO
{

void export_IOParam()
{
/*    class_<PSF>("PSF", init<>())
           .def("read", &PSF::read)
           .def("readParam", &PSF::readParam)
           .def("readPrm", &PSF::readPrm)
    ;

    class_<ProtoMS>("ProtoMS", init<>())
           .def("read", &ProtoMS::read)
    ;*/
}

}
