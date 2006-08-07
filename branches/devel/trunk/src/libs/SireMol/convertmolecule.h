#ifndef SIREMOL_CONVERTMOLECULE_H
#define SIREMOL_CONVERTMOLECULE_H

#include "editmolset.h"
#include "moleculeset.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

CutGroupVector residueCutGroups(const EditMol &editmol);
CutGroupVector moleculeCutGroups(const EditMol &editmol);

MoleculeSet rigidAndResidueCutting(const EditMolList &mols);
MoleculeSet flexibleAndResidueCutting(const EditMolList &mols);

MoleculeSet rigidAndMoleculeCutting(const EditMolList &mols);
MoleculeSet flexibleAndMoleculeCutting(const EditMolList &mols);

EditMolList convert(const MoleculeSet &mols);

}

SIRE_END_HEADER

#endif
