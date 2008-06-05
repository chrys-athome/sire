
/**
  * This file contains the boost::python wrapping of an EditMolList
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/editmol.h"

#include "SirePy/indexer.hpp"

using namespace boost::python;

namespace SireMol
{

void export_EditMolList()
{
    /** Wrap up an EditMolList */
    wrapList<EditMol>("EditMolList");
    wrapVector<EditMol>("EditMolVector");
}

}
