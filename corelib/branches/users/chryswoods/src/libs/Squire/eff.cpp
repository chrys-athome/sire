/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "eff.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireFF;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<EFF> r_eff;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const EFF &eff)
{
    writeHeader(ds, r_eff, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const G1FF&>(eff);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, EFF &eff)
{
    VersionID v = readHeader(ds, r_eff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> static_cast<G1FF&>(eff);
    }
    else
        throw version_error(v, "1", r_eff, CODELOC);
        
    return ds;
}

/** Constructor */
EFF::EFF() : ConcreteProperty<EFF,G1FF>(), FF3D()
{}

/** Construct, passing the name for the forcefield */
EFF::EFF(const QString &name) : ConcreteProperty<EFF,G1FF>(), FF3D()
{
    FF::setName(name);
}

/** Copy constructor */
EFF::EFF(const EFF &other) : ConcreteProperty<EFF,G1FF>(other), FF3D(other)
{}

/** Destructor */
EFF::~EFF()
{}

/** Copy assignment operator */
EFF& EFF::operator=(const EFF &other)
{
    if (this != &other)
    {
        G1FF::operator=(other);
        FF3D::operator=(other);
        
    }
    
    return *this;
}

/** Comparison operator */
bool EFF::operator==(const EFF &other) const
{
    return G1FF::operator==(other);
}

/** Comparison operator */
bool EFF::operator!=(const EFF &other) const
{
    return G1FF::operator!=(other);
}

/** Recalculate the energy of the current state of this forcefield. This
    will recalculate the energy using the quickest possible route, e.g.
    if will only recalculate the energies of molecules that have changed
    since the last evaluation */
void EFF::recalculateEnergy()
{
    //first calculate the nuclear-nuclear energy (this is a constant)
    
    //loop over nuclei...

    //need to minimise the energy of the electrons for the new
    //configuration of the atoms...
    
    //loop until minimised
    {

        //now calculate the energy...
        double e_ke = 0;
        double e_ne = 0;
        double e_ee = 0;
        double e_pauli_parallel = 0;
        double e_pauli_antiparallel = 0;
    
        //loop over electrons...
        for (int i=0; i<nelectrons; ++i)
        {
            //get the coordinates and size of this electron
            const Vector &coords_and_size = all_electrons[i];
        
            //kinetic energy = (3/2) 1 / s^2
            e_ke += 1.5 / SireMaths::pow_2(coords_and_size.w());
            
            
            
    }

}

/** Restore this forcefield back to the version in 'ffield' */
void EFF::_pvt_restore(const ForceField &ffield)
{
    if (not ffield->isA<EFF>())
    {
        throw SireError::program_bug( QObject::tr(
            "There is a bug in the program as a %1 forcefield is being "
            "updated with a %2 forcefield!")
                .arg(this->what()).arg(ffield->what()), CODELOC );
    }

    this->operator=( ffield.asA<EFF>() );
}

/** Record the fact that the molecule 'mol' has been added to this
    forcefield and use the supplied property map to find the
    properties of this molecule that will be used by this forcefield
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void EFF::_pvt_added(const PartialMolecule &mol, const PropertyMap &map)
{
}

/** Record the fact that the molecule 'mol' has been removed from this forcefield */
void EFF::_pvt_removed(const PartialMolecule &mol)
{
}

/** Record that fact that the molecule 'molecule' has been changed

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void EFF::_pvt_changed(const SireMol::Molecule &molecule)
{
}

/** Record that the provided list of molecules have changed 

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void EFF::_pvt_changed(const QList<SireMol::Molecule> &molecules)
{
}

/** Record that all of the molecules have been removed */
void EFF::_pvt_removedAll()
{
}
    
/** Return whether or not the supplied property map contains different
    properties for the molecule with number 'molnum' */       
bool EFF::_pvt_wouldChangeProperties(MolNum molnum, 
                                const PropertyMap &map) const
{
    return false;
}

/** Function used to perform the work of changing the name of this 
    forcefield - this renames the component symbols and the molecule group */
void EFF::_pvt_updateName()
{
    G1FF::_pvt_updateName();
}


