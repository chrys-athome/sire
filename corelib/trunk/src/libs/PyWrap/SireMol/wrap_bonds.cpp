
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/bond.h"

#include "SireQt/qlist.hpp"
#include "SireQt/qset.hpp"
#include "SireQt/qvector.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_Bonds()
{
    wrap_QVector<Bond>("QVector_Bond_");
    wrap_QList<Bond>("QList_Bond_");
    wrap_QSet<Bond>("QSet_Bond_");
}

}

