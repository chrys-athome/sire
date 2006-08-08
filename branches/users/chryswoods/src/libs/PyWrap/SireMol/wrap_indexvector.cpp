
/**
  * This file contains the boost::python wrapping of a IndexVector
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/index.h"

#include "SireQt/qvector.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_IndexVector()
{
    wrap_QVector<Index>("QVector_Index_");
}

}
