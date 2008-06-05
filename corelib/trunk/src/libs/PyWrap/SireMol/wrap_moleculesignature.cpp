
/**
  * This file contains the boost::python wrapping of a MoleculeSignature
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/moleculesignature.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireMol
{

void export_MoleculeSignature()
{
    class_<MoleculeSignature>("MoleculeSignature", init<>())
                .def(self == self)
                .def(self != self)
                .def( "__str__", &__str__<MoleculeSignature> )
                .def( "toString", &MoleculeSignature::toString )
    ;
}

}
