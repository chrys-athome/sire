
/**
  * This file contains the boost::python wrapping of a IndexList.
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/index.h"

#include "SireQt/qlist.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_IndexList()
{
    wrap_QList<Index>("QList_Index_");
}

}
