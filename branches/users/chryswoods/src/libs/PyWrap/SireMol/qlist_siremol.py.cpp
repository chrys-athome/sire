
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireQt/qlist.hpp"

#include "SireMol/molecule.h"

using namespace SireQt;

namespace SireMol
{

void export_QList_Molecule()
{
    wrap_QList<Molecule>("QList_Molecule_");
}

}
