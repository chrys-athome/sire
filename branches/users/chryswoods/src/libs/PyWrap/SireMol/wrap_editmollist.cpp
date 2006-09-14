
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

#include "SireQt/qlist.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_EditMolList()
{
    /** Wrap up an EditMolList */
    wrap_QList<EditMol>("QList_EditMol_");
    wrap_QVector<EditMol>("QVector_EditMol_");
}

}
