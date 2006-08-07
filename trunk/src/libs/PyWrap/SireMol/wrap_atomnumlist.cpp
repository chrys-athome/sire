
/**
  * This file contains the boost::python wrapping of a AtomNumList and ResNumList.
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

void export_AtomNumList()
{
    wrap_QList<AtomNum>("QList_AtomNum_");
    wrap_QList<ResNum>("QList_ResNum_");
}

}
