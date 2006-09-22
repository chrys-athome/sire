
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

#include "SireQt/qset.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_AtomSet()
{
    wrap_QSet<Atom>("QSet_Atom_");
    wrap_QSet<AtomIndex>("QSet_AtomIndex_");
}

}
