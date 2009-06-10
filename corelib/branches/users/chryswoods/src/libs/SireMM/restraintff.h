/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREMM_RESTRAINTFF_H
#define SIREMM_RESTRAINTFF_H

#include "SireBase/propertymap.h"

#include "SireFF/g1ff.h"
#include "SireFF/ff3d.h"
#include "SireFF/forcetable.h"

#include "SireFF/detail/ffmolecules3d.h"

namespace SireMM
{
class RestraintFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::RestraintFF&);
QDataStream& operator>>(QDataStream&, SireMM::RestraintFF&);

namespace SireVol
{
class CoordGroup;
}

namespace SireMM
{

using SireFF::ForceField;
using SireFF::G1FF;
using SireFF::FF3D;
using SireFF::ForceTable;
using SireFF::MolForceTable;
using SireFF::FFComponent;

using SireMol::MolNum;
using SireMol::PartialMolecule;
using SireMol::MoleculeGroup;

using SireVol::CoordGroup;

using SireBase::Property;
using SireBase::Properties;
using SireBase::PropertyMap;
using SireBase::PropertyName;

/** This class provides the default parameter names used to 
    find properties to parameterise molecules using
    the RestraintPotential */
class SIREMM_EXPORT RestraintParameterNames
{
public:
    RestraintParameterNames()
    {}
    
    ~RestraintParameterNames()
    {}
};

/** This class contains the default parameter names for the 
    RestraintPotential when it is used in a 3D forcefield */
class SIREMM_EXPORT RestraintParameterNames3D
         : public RestraintParameterNames, 
           public SireFF::detail::Coords3DParameterName
{
public:
    RestraintParameterNames3D()
           : RestraintParameterNames(), 
             SireFF::detail::Coords3DParameterName()
    {}
    
    ~RestraintParameterNames3D()
    {}
};

/** This potential provides a set of restraints that can be applied
    to molecules (or parts of molecules)
    
    @author Christopher Woods
*/
class SIREMM_EXPORT RestraintPotential
{
public:
    typedef RestraintEnergy Energy;
    typedef Energy::Components Components;
    
    typedef RestraintParameters3D Parameters;
    typedef RestraintParameterNames3D ParameterNames;
    
    typedef SireFF::detail::FFMolecule<RestraintPotential> Molecule;
    typedef SireFF::detail::FFMolecules<RestraintPotential> Molecules;
    typedef Molecules::ChangedMolecule ChangedMolecule;
    
    ~RestraintPotential();

    static const RestraintSymbols& symbols();
    
    static const ParameterNames& parameters();
    
    RestraintPotential::Parameters
    getParameters(const PartialMolecule &molecule, const PropertyMap &map) const;
    
    RestraintPotential::Parameters
    updateParameters(const RestraintPotential::Parameters &old_parameters,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &old_map,
                     const PropertyMap &new_map) const;
                     
    RestraintPotential::Molecule
    parameterise(const PartialMolecule &molecule, const PropertyMap &map) const;
    
    RestraintPotential::Molecules
    parameterise(const MoleculeGroup &molecules, const PropertyMap &map) const;

protected:
    RestraintPotential();
    RestraintPotential(const RestraintPotential &other);
    
    RestraintPotential& operator=(const RestraintPotential &other);
    
    bool operator==(const RestraintPotential &other) const;
    bool operator!=(const RestraintPotential &other) const;
    
    void calculateEnergy(const RestraintPotential::Molecule &molecule,
                         RestraintPotential::Energy &energy,
                         double scale_energy=1) const;

    void calculateForce(const RestraintPotential::Molecule &molecule,
                        MolForceTable &forces,
                        double scale_force=1) const;

private:
    static ParameterNames param_names;
    static RestraintSymbols restraint_symbols;
};

/** This is a forcefield that calculates energies and forces
    caused by restraints that have been added to the molecule(s)
    
    @author Christopher Woods
*/
class SIREMM_EXPORT RestraintFF
         : public SireBase::ConcreteProperty<RestraintFF,G1FF>,
           public FF3D,
           protected RestraintPotential
{

friend QDataStream& ::operator<<(QDataStream&, const RestraintFF&);
friend QDataStream& ::operator>>(QDataStream&, RestraintFF&);

public:
    typedef RestraintPotential::Components Components;
    typedef RestraintPotential::ParameterNames ParameterNames;
    
    RestraintFF();
    RestraintFF(const QString &name);
    
    RestraintFF(const RestraintFF &other);
    
    ~RestraintFF();
    
    RestraintFF& operator=(const RestraintFF &other);
    
    bool operator==(const RestraintFF &other) const;
    bool operator!=(const RestraintFF &other) const;
    
    RestraintFF* clone() const
    {
        return new RestraintFF(*this);
    }
    
    static const char* typeName();
    
    const RestraintSymbols& symbols() const;
    const ParameterNames& parameters() const;
    const Components& components() const;
    
    bool setProperty(const QString &name, const Property &property);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;
    
    void force(ForceTable &forcetable, double scale_force=1);
    
    void force(ForceTable &forcetable, const Symbol &symbol,
               double scale_force=1);

    void mustNowRecalculateFromScratch();    

protected:
    const FFComponent& _pvt_components() const;

    void recalculateEnergy();

    void _pvt_added(const PartialMolecule &mol, const PropertyMap &map);

    void _pvt_removed(const PartialMolecule &mol);

    void _pvt_changed(const SireMol::Molecule &molecule);
    void _pvt_changed(const QList<SireMol::Molecule> &molecules);
    
    void _pvt_removedAll();
        
    bool _pvt_wouldChangeProperties(MolNum molnum, 
                                    const PropertyMap &map) const;

    void _pvt_updateName();

private:
    typedef RestraintPotential::Molecule Molecule;
    typedef RestraintPotential::Molecules Molecules;
    typedef RestraintPotential::ChangedMolecule ChangedMolecule;
    
    bool recordingChanges() const;
    void recordChange(const ChangedMolecule &change);
    
    /** All of the molecules currently in this forcefield */
    Molecules mols;

    /** The list of molecules that have changed since the last evaluation.
        While ffmols only contains the newest version of the molecule,
        this list contains both the newest version, and the version of the
        molecule at the last energy evaluation. */
    QHash<MolNum,ChangedMolecule> changed_mols;

    /** The energy components available for this forcefield */
    Components ffcomponents;
    
    /** The properties of this forcefield */
    Properties props;
};


#ifndef SIRE_SKIP_INLINE_FUNCTIONS

////////
//////// Inline functions of RestraintFF
////////

/** Return all of the symbols used in the internal energy functions */
inline const RestraintSymbols& RestraintFF::symbols() const
{
    return RestraintPotential::symbols();
}

/** Return the names of all of the properties used to store the 
    parameters for this potential */
inline const RestraintFF::ParameterNames& RestraintFF::parameters() const
{
    return RestraintPotential::parameters();
}

/** Return all of the symbols representing the components
    of this forcefield */
inline const RestraintFF::Components& RestraintFF::components() const
{
    return ffcomponents;
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE( SireMM::RestraintFF )

SIRE_EXPOSE_CLASS( SireMM::RestraintParameterNames )
SIRE_EXPOSE_CLASS( SireMM::RestraintParameterNames3D )
SIRE_EXPOSE_CLASS( SireMM::RestraintFF )

#endif
