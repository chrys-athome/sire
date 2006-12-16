
/**
  * This file contains the boost::python wrapping of a CutGroup, CutGroupList and CutGroupSet
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/cutgroup.h"
#include "SireMol/cutgroupid.h"

#include "SireQt/qhash.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMol
{

void  
SIREMOL_EXPORT
export_CutGroupSet()
{
    /** Wrap up a CutGroupSet */
    wrap_QHash<CutGroupID,CutGroup>("QHash_CutGroupID_CutGroup_");
}

}
