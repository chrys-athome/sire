
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/angle.h"

#include "SireQt/qvector.hpp"
#include "SireQt/qlist.hpp"
#include "SireQt/qset.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void export_Angles()
{
    wrap_QVector<Angle>("QVector_Angle_");
    wrap_QList<Angle>("QList_Angle_");
    wrap_QSet<Angle>("QSet_Angle_");
}

}

