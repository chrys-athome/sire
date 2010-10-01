/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREMM_FASTINTERCLJFF_H
#define SIREMM_FASTINTERCLJFF_H

#include "cljcomponent.h"
#include "switchingfunction.h"
#include "ljpotential.h"

#include "SireFF/g1ff.h"
#include "SireFF/ff3d.h"

#include "SireVol/space.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class FastInterCLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::FastInterCLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::FastInterCLJFF&);

namespace SireFF
{
class ForceTable;
class FieldTable;
class PotentialTable;
class Probe;
}

namespace SireMM
{

using SireBase::Properties;
using SireBase::PropertyMap;

using SireMol::Molecule;
using SireMol::MoleculeData;
using SireMol::PartialMolecule;
using SireMol::ViewsOfMol;
using SireMol::Molecules;

using SireFF::ForceTable;
using SireFF::FieldTable;
using SireFF::PotentialTable;
using SireFF::Probe;

using SireVol::Space;

using SireCAS::Symbol;
using SireCAS::Symbols;
using SireCAS::Values;

/** This is a new version of InterCLJFF that I am writing as 
    a template for the redesign of the MM forcefield classes.
    
    Hopefully this class will be significantly faster than InterCLJFF
    
    @author Christopher Woods
*/
class SIREMM_EXPORT FastInterCLJFF 
            : public SireBase::ConcreteProperty<FastInterCLJFF,SireFF::G1FF>,
              public SireFF::FF3D
{

friend QDataStream& ::operator<<(QDataStream&, const FastInterCLJFF&);
friend QDataStream& ::operator>>(QDataStream&, FastInterCLJFF&);

public:
    FastInterCLJFF();
    FastInterCLJFF(const QString &name);
    
    FastInterCLJFF(const FastInterCLJFF &other);
    
    ~FastInterCLJFF();
    
    static const char* typeName();
    
    FastInterCLJFF& operator=(const FastInterCLJFF &other);
    
    bool operator==(const FastInterCLJFF &other) const;
    bool operator!=(const FastInterCLJFF &other) const;
    
    FastInterCLJFF* clone() const;
    
    const CLJComponent& components() const;
    
    const Space& space() const;
    const SwitchingFunction& switchingFunction() const;
    bool shiftElectrostatics() const;
    const QString& combiningRules() const;
    
    bool setSpace(const Space &space);
    bool setSwitchingFunction(const SwitchingFunction &new_switchfunc);
    bool setShiftElectrostatics(bool switchelectro);
    bool setCombiningRules(const QString &combiningrules);
    
    bool setProperty(const QString &name, const Property &property);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;
    
    void mustNowRecalculateFromScratch();    

    void force(ForceTable &forcetable, double scale_force=1);
    
    void force(ForceTable &forcetable, const Symbol &symbol,
               double scale_force=1);
               
    void field(FieldTable &fieldtable, double scale_field=1);
    
    void field(FieldTable &fieldtable, const Symbol &component,
               double scale_field=1);
               
    void potential(PotentialTable &potentialtable, double scale_potential=1);
    
    void potential(PotentialTable &potentialtable, const Symbol &component,
                   double scale_potential=1);

    void field(FieldTable &fieldtable, const Probe &probe, double scale_field=1);
    
    void field(FieldTable &fieldtable, const Symbol &component,
               const Probe &probe, double scale_field=1);
               
    void potential(PotentialTable &potentialtable, const Probe &probe,
                   double scale_potential=1);
    
    void potential(PotentialTable &potentialtable, const Symbol &component,
                   const Probe &probe, double scale_potential=1);

protected:

    ////
    //// Virtual functions from SireFF::FF
    ////

    const CLJComponent& _pvt_components() const;
    
    void recalculateEnergy();
    
    void _pvt_updateName();
    
    ////
    //// Virtual functions from SireFF::G1FF
    ////

    void _pvt_added(const SireMol::PartialMolecule &mol, 
                    const SireBase::PropertyMap&);
                    
    void _pvt_removed(const SireMol::PartialMolecule &mol);
    
    void _pvt_changed(const SireMol::Molecule &mol);
    
    void _pvt_changed(const QList<SireMol::Molecule> &mols);
    
    void _pvt_removedAll();
    
    bool _pvt_wouldChangeProperties(SireMol::MolNum molnum, 
                                    const SireBase::PropertyMap &map) const;

private:
    void rebuildProperties();
    void rebuildAll();

    /** The components of the energy */
    CLJComponent ffcomponents;

    /** All possible LJ parameter pair combinations, arranged
        in a symmetric 2D array */
    LJPairMatrix ljpairs;

    /** All of the properties of this forcefield */
    Properties props;

    /** The space in which the restraints are evaluated */
    SireVol::SpacePtr spce;

    /** The switching function used to implement a non-bonded cutoff */
    SwitchFuncPtr switchfunc;

    /** The combining rules to use to get mixed LJ parameters */
    LJParameterDB::CombiningRules combining_rules;

    /** Whether or not the LJ pair matrix needs to be rebuilt */
    bool need_update_ljpairs;
    
    /** Whether or not electrostatic potential shifting is used
        (this shifts the entire electrostatic potential so that it
        is zero at the cutoff distance) */
    bool use_electrostatic_shifting;
    
    /** Whether or not to recalculate everything from scratch */
    bool recalc_from_scratch;
};

}

Q_DECLARE_METATYPE( SireMM::FastInterCLJFF )

SIRE_EXPOSE_CLASS( SireMM::FastInterCLJFF )

SIRE_END_HEADER

#endif
