
/**
  * This file contains the boost::python wrapping of a AtomSet.
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/atom.h"

#include "SireQt/qvector.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_AtomVector()
{
    SireQt::wrap_QVector<Atom>("QVector_Atom_");
    SireQt::wrap_QVector<AtomIndex>("QVector_AtomIndex_");
}

}
