
/**
  * This file contains the boost::python wrapping of a AtomNumSet and ResNumSet.
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

void export_AtomNumSet()
{
    wrap_QSet<AtomNum>("QSet_AtomNum_");
    wrap_QSet<ResNum>("QSet_ResNum_");
}

}
