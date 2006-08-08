
/**
  * This file contains the boost::python wrapping of a AtomNumVector and ResNumVector.
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/atom.h"

#include "SireQt/qvector.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_AtomNumVector()
{
    wrap_QVector<AtomNum>("QVector_AtomNum_");
    wrap_QVector<ResNum>("QVector_ResNum_");
}

}
