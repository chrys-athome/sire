/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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

void export_CLJFF();
void export_CombiningRules();
void export_InterCLJFF();
void export_SwitchingFunction();
void export_Tip4PFF();
void export_AtomicCharges();
void export_AtomicLJs();

void SIREMM_EXPORT export_SireMM()
{
    export_CLJParameter();
    //export_MMDB();
    //export_MMDB2();
    export_ChargeTable();
    export_AtomicCharges();
    export_AtomicLJs();
    export_LJTable();
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

    export_CLJFF();
    export_CombiningRules();
    export_InterCLJFF();
    export_SwitchingFunction();
    export_Tip4PFF();
}

}
