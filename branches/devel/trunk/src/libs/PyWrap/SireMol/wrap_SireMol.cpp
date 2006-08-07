
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireMol.h"
#endif

namespace SireMol
{
void export_AABox();
void export_Atom();
void export_AtomSet();
void export_AtomVector();
void export_AtomList();
void export_AtomNumSet();
void export_AtomNumVector();
void export_AtomNumList();
void export_ConvertMolecule();
void export_CutGroup();
void export_CutGroupSet();
void export_IndexList();
void export_IndexSet();
void export_IndexVector();
void export_IDTypes();
void export_Internals();
void export_Bonds();
void export_Angles();
void export_Dihedrals();
void export_Impropers();
void export_EditMol();
void export_EditMolList();
void export_EditRes();
void export_Molecule();
void export_MoleculeBonds();
void export_MoleculeCGInfo();
void export_MoleculeSet();
void export_MoleculeSignature();
void export_Residue();
void export_ResidueBonds();
void export_ResidueIDSet();
void export_ResidueSet();

void SIREMOL_EXPORT export_SireMol()
{
    export_AABox();
    export_Atom();
    export_AtomList();
    export_AtomSet();
    export_AtomVector();
    export_AtomNumSet();
    export_AtomNumVector();
    export_AtomNumList();
    export_ConvertMolecule();
    export_CutGroup();
    export_CutGroupSet();
    export_EditMol();
    export_EditMolList();
    export_EditRes();
    export_IndexList();
    export_IndexSet();
    export_IndexVector();
    export_IDTypes();
    export_Internals();
    export_Bonds();
    export_Angles();
    export_Dihedrals();
    export_Impropers();
    export_Molecule();
    export_MoleculeBonds();
    export_MoleculeCGInfo();
    export_MoleculeSet();
    export_MoleculeSignature();
    export_Residue();
    export_ResidueBonds();
    export_ResidueIDSet();
    export_ResidueSet();
}

}
