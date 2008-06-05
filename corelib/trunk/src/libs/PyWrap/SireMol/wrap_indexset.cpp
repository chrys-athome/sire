
/**
  * This file contains the boost::python wrapping of a IndexSet
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/index.h"

#include "SireQt/qset.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_IndexSet()
{
    wrap_QSet<Index>("QSet_Index_");
}

}
