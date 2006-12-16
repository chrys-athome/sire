
#include <boost/python.hpp>

#include "SireVol/coordgroup.h"

#include "SireQt/qvector.hpp"

using namespace boost::python;

using namespace SireQt;

namespace SireVol
{

void SIREVOL_EXPORT export_CoordGroup()
{
    class_<CoordGroup>("CoordGroup", init<>())
    ;

    wrap_QVector<CoordGroup>("QVector_CoordGroup_");
}

}
