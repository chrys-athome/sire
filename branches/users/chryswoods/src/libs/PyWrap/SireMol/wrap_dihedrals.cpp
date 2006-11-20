
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/dihedral.h"

#include "SireQt/qvector.hpp"
#include "SireQt/qlist.hpp"
#include "SireQt/qset.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void  
SIREMOL_EXPORT
export_Dihedrals()
{
    wrap_QVector<Dihedral>("QVector_Dihedral_");
    wrap_QList<Dihedral>("QList_Dihedral_");
    wrap_QSet<Dihedral>("QSet_Dihedral_");
}

}

