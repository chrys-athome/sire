
/**
  * This file contains the boost::python wrapping of a Molecule
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"
#include "SireMol/atomidgroup.h"
#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"
#include "SireMol/splitmolecule.h"
#include "SireMol/weightfunction.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/angle.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SirePy/indexer.hpp"
#include "../SireBase/wrap_set.h"

using namespace boost::python;

namespace SireMol
{

void export_MoleculeSet()
{
    SireBase::wrapSet<MoleculeID, Molecule>("MoleculeSet");
}

}
