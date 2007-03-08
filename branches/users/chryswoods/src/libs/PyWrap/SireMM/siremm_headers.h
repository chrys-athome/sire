#ifndef PYPLUSPLUS_SIREMM_HEADERS_H
#define PYPLUSPLUS_SIREMM_HEADERS_H

//need to increase the arity of boost::python
//to allow it to cope with assign_bonds, assign_angles
//and assign_dihedrals - do it here, so that
//this definition is consistent across the entire module
#define BOOST_PYTHON_MAX_ARITY 15

#include "SireMol/qhash_siremol.h"
#include "SireFF/qhash_sireff.h"
#include "SireCAS/qhash_sirecas.h"
#include "SireMM/qhash_siremm.h"

#include "SireMM/angledb.h"
#include "SireMM/anglegenerator.h"
#include "SireMM/angletable.h"
#include "SireMM/assign_angles.h"
#include "SireMM/assign_bonds.h"
#include "SireMM/assign_dihedrals.h"
#include "SireMM/atomiccharges.h"
#include "SireMM/atomicljs.h"
#include "SireMM/bonddb.h"
#include "SireMM/bondgenerator.h"
#include "SireMM/bondtable.h"
#include "SireMM/chargedb.h"
#include "SireMM/chargeparameter.h"
#include "SireMM/chargetable.h"
#include "SireMM/cljff.h"
#include "SireMM/cljpair.h"
#include "SireMM/cljparameter.h"
#include "SireMM/combiningrules.h"
#include "SireMM/dihedraldb.h"
#include "SireMM/dihedralgenerator.h"
#include "SireMM/dihedraltable.h"
#include "SireMM/intercljff.h"
#include "SireMM/interljff.h"
#include "SireMM/intergroupcljff.h"
#include "SireMM/internaldatatypes.h"
#include "SireMM/ljdb.h"
#include "SireMM/ljff.h"
#include "SireMM/ljpair.h"
#include "SireMM/ljparameter.h"
#include "SireMM/ljtable.h"
#include "SireMM/molangleinfo.h"
#include "SireMM/molbondinfo.h"
#include "SireMM/moldihedralinfo.h"
#include "SireMM/resangleinfo.h"
#include "SireMM/resbondinfo.h"
#include "SireMM/resdihedralinfo.h"
#include "SireMM/switchingfunction.h"
#include "SireMM/tip4pff.h"
#include "SireMM/ureybradleydb.h"


#endif
