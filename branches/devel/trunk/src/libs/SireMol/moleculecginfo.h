#ifndef SIREMOL_MOLECULECGINFO_H
#define SIREMOL_MOLECULECGINFO_H

#include "moleculeinfo.hpp"
#include "residueinfo.hpp"
#include "idtypes.h"

namespace SireMol
{

typedef MoleculeInfo<CGAtomID> MoleculeCGInfo;
typedef ResidueInfo<CGAtomID> ResidueCGInfo;

}

#endif
