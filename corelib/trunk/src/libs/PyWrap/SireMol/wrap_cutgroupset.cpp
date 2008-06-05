
/**
  * This file contains the boost::python wrapping of a CutGroup, CutGroupList and CutGroupSet
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/cutgroup.h"
#include "SireMol/atom.h"
#include "SireMol/aabox.h"

#include "SireMaths/quaternion.h"

#include "SirePy/indexer.hpp"
#include "SireBase/indexset.h"
#include "../SireBase/wrap_set.h"

using namespace boost::python;

namespace SireMol
{

void export_CutGroupSet()
{
    /** Wrap up a CutGroupSet */
    SireBase::wrapSet<MolCutGroupID, CutGroup>("CutGroupSet");
}

}
