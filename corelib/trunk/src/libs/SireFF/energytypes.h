#ifndef SIREFF_ENERGYTYPES_H
#define SIREFF_ENERGYTYPES_H

#include "energy.h"
#include "energy.hpp"
#include "energypack.hpp"
#include "energymaths.hpp"

//macro to register new energy types - this registers and name and increments
//the number in one line, though you could do it yourself if you want to type more...

#define ADD_ENERGY(id, typnam, nam) \
   typedef EnergyT<id> typnam; \
   \
   template<> \
   inline QString getName(const EnergyT<id>&) \
   { \
      return nam; \
   }
   
//end of macro

SIRE_BEGIN_HEADER

namespace SireFF
{

namespace EnergyType
{
/** Enum of all of the different types of energy component available to this program. If
    you want to add a new component, then you must add it to this list and ensure that it is
    given a unique ID number. You should also call the 'ADD_ENERGY' function so that the 
    code can return the name of the component as well */
enum EnergyType { Coulomb = 1,       ///< The coulomb (electrostatic energy)
                  LJ = 2,            ///< The Lennard Jones energy
                  Bond = 3,          ///< The bond energy
                  Angle = 4,         ///< The angle energy
                  Dihedral = 5,      ///< The dihedral energy
                  HBond = 6,         ///< The energy of HBonds 
                  IntraCoulomb = 7,  ///< Intramolecular coulomb energy
                  IntraLJ = 8,       ///< Intramolecular LJ energy
                  QM = 9             ///< The quantum mechanics energy
                };
}

ADD_ENERGY(EnergyType::Coulomb, CoulombEnergy, "Coulomb")
ADD_ENERGY(EnergyType::LJ, LJEnergy, "LJ")
ADD_ENERGY(EnergyType::Bond, BondEnergy, "Bond")
ADD_ENERGY(EnergyType::Angle, AngleEnergy, "Angle")
ADD_ENERGY(EnergyType::Dihedral, DihedralEnergy, "Dihedral")
ADD_ENERGY(EnergyType::HBond, HBondEnergy, "HBond")
ADD_ENERGY(EnergyType::IntraCoulomb, IntraCoulombEnergy, "Intra-Coulomb")
ADD_ENERGY(EnergyType::IntraLJ, IntraLJEnergy, "Intra-LJ")
ADD_ENERGY(EnergyType::QM, QMEnergy, "QM")

/** Define an EnergyPack that contains all major components */
typedef EnergyPack<EnergyType::Coulomb,EnergyType::LJ,EnergyType::Bond,
                   EnergyType::Angle,EnergyType::Dihedral,EnergyType::HBond,
                   EnergyType::IntraCoulomb,EnergyType::IntraLJ,EnergyType::QM> EnergyPackAll;

/** Define an EnergyPack to hold CLJ energies */
typedef EnergyPack<EnergyType::Coulomb, EnergyType::LJ> CLJEnergy;

}

SIRE_END_HEADER

#endif
