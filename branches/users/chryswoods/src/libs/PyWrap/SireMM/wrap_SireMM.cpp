
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireMM.h"
#endif

namespace SireMM
{
void export_CLJParameter();
void export_MMDB();
void export_MMDB2();
void export_ChargeTable();
void export_LJTable();
void export_BondTable();
void export_AngleTable();
void export_DihedralTable();
void export_MolAngleInfo();
void export_MolBondInfo();
void export_MolDihedralInfo();
void export_ResAngleInfo();
void export_ResBondInfo();
void export_ResDihedralInfo();
void export_BondGenerator();
void export_AngleGenerator();
void export_DihedralGenerator();
void export_assign_bonds();
void export_assign_angles();
void export_assign_dihedrals();

void SIREMM_EXPORT export_SireMM()
{
    export_CLJParameter();
    //export_MMDB();
    //export_MMDB2();
    //export_ChargeTable();
    //export_LJTable();
    export_BondTable();
    export_AngleTable();
    export_DihedralTable();
    export_MolAngleInfo();
    export_MolBondInfo();
    export_MolDihedralInfo();
    export_ResAngleInfo();
    export_ResBondInfo();
    export_ResDihedralInfo();
    export_BondGenerator();
    export_AngleGenerator();
    export_DihedralGenerator();
    export_assign_bonds();
    export_assign_angles();
    export_assign_dihedrals();
}

}
