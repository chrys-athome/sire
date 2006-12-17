
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#include "wrap_SireMol.h"

namespace SireMol
{
void export_Atom();
void export_AtomSet();
void export_AtomVector();
void export_AtomList();
void export_AtomNumSet();
void export_AtomNumVector();
void export_AtomNumList();
void export_CutGroup();
void export_CutGroupSet();
void export_IndexList();
void export_IndexSet();
void export_IndexVector();
void export_IDTypes();
void export_IDMolAtom();
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
void export_MoleculeInfo();
void export_Residue();
void export_ResidueBonds();
void export_ResidueInfo();

void export_QList_Molecule();

void SIREMOL_EXPORT export_SireMol()
{
    export_Atom();
    export_AtomList();
    export_AtomSet();
    export_AtomVector();
    export_AtomNumSet();
    export_AtomNumVector();
    export_AtomNumList();
    export_CutGroup();
    export_CutGroupSet();
    export_EditMol();
    export_EditRes();
    export_IDMolAtom();
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
    export_MoleculeInfo();
    export_Residue();
    export_ResidueBonds();
    export_ResidueInfo();
    
    export_QList_Molecule();
}

}
