
/**
  * This file contains the boost::python wrapping of the EditMol/Molecule conversion functions
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/convertmolecule.h"
#include "SireMol/editmol.h"
#include "SireMol/molecule.h"

using namespace boost::python;

namespace SireMol
{

void export_ConvertMolecule()
{
    def("residueCutGroups", &SireMol::residueCutGroups);
    def("moleculeCutGroups", &SireMol::moleculeCutGroups);
    
    def("rigidAndResidueCutting", &SireMol::rigidAndResidueCutting);
    def("rigidAndMoleculeCutting", &SireMol::rigidAndMoleculeCutting);
    def("flexibleAndResidueCutting", &SireMol::flexibleAndResidueCutting);
    def("flexibleAndMoleculeCutting", &SireMol::flexibleAndMoleculeCutting);
    
    def("convert", &SireMol::convert);
}

}
