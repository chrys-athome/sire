
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/improper.h"

#include "SireQt/qvector.hpp"
#include "SireQt/qlist.hpp"
#include "SireQt/qset.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_Impropers()
{
    wrap_QVector<Improper>("QVector_Improper_");
    wrap_QList<Improper>("QList_Improper_");
    wrap_QSet<Improper>("QSet_Improper_");
}

}

