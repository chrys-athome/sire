
/**
  * This file contains the boost::python wrapping of MD5Sum
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireBase/md5sum.h"

using namespace boost::python;

namespace SireBase
{

void export_MD5Sum()
{
    /** Wrapping up the 'MD5Sum' class */
    class_<MD5Sum>("MD5Sum",init<const QString&>())
    .def(init<const char*,int>())
    .def(init<>())
    .def(self == self)
    .def(self != self)
    .def("toString",&MD5Sum::toString)
    ;
}

}
