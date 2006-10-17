
/**
  * This file contains the boost::python wrapping of a AtomSet.
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/atom.h"

#include "SireQt/qlist.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_AtomList()
{
    wrap_QList<Atom>("QList_Atom_");
    wrap_QList<AtomIndex>("QList_AtomIndex_");
}

}
