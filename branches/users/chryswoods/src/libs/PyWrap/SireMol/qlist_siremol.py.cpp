
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireQt/qlist.hpp"

#include "SireMol/molecule.h"
#include "SireMol/editmol.h"

using namespace SireQt;

namespace SireMol
{

void
SIREMOL_EXPORT
export_QList_Molecule()
{
    wrap_QList<Molecule>("QList_Molecule_");
    wrap_QList<EditMol>("QList_EditMol_");
}

}
