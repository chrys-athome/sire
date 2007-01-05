
#include <boost/python.hpp>

#include "SireMol/idmolatom.h"
#include "SireMol/cgatomid.h"
#include "SireMol/cgnumatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/residatomid.h"
#include "SireMol/atomindex.h"

using namespace boost::python;

namespace SireMol
{

void SIREMOL_EXPORT export_IDMolAtom()
{
    class_<IDMolAtom>( "IDMolAtom", no_init )
    ;

    implicitly_convertible<CGAtomID, IDMolAtom>();
    implicitly_convertible<CGNumAtomID, IDMolAtom>();
    implicitly_convertible<ResNumAtomID, IDMolAtom>();
    implicitly_convertible<ResIDAtomID, IDMolAtom>();
    implicitly_convertible<AtomIndex, IDMolAtom>();
    implicitly_convertible<AtomID, IDMolAtom>();

}

}
