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

#include "fastintercljff.h"

#include "SireMol/molecule.h"
#include "SireMol/viewsofmol.h"
#include "SireMol/partialmolecule.h"

#include "SireBase/propertymap.h"
#include "SireBase/properties.h"

#include "SireError/errors.h"
#include "SireBase/errors.h"

#include "tostring.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireMM;
using namespace SireFF;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<FastInterCLJFF> r_fastintercljff;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const FastInterCLJFF &fastcljff)
{
    writeHeader(ds, r_fastintercljff, 1);
    
    SharedDataStream sds(ds);
    
    sds << fastcljff.props << fastcljff.molprops
        << static_cast<const G1FF&>(fastcljff);
    
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       FastInterCLJFF &fastcljff)
{
    VersionID v = readHeader(ds, r_fastintercljff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> fastcljff.props >> fastcljff.molprops
            >> static_cast<G1FF&>(fastcljff);
        
        fastcljff.rebuildAll();
    }
    else
        throw version_error(v, "1", r_fastintercljff, CODELOC);
        
    return ds;
}

/** Rebuild the properties object from the current properties */
void FastInterCLJFF::rebuildProperties()
{
    props.clear();
    props.setProperty("patching", ptchs.patching());
    props.setProperty("space", ptchs.space());
    props.setProperty("switchingFunction", switchfunc);
    props.setProperty("shiftElectrostatics", VariantProperty(use_electrostatic_shifting));
    props.setProperty("combiningRules", 
                        VariantProperty(LJParameterDB::toString(combining_rules)));
}

/** Construct an un-named, empty FastInterCLJFF */
FastInterCLJFF::FastInterCLJFF() 
               : ConcreteProperty<FastInterCLJFF,G1FF>(), FF3D(),
                 switchfunc( SwitchingFunction::null() ),
                 combining_rules( LJParameterDB::interpret("arithmetic") ),
                 need_update_ljpairs(true), use_electrostatic_shifting(false),
                 recalc_from_scratch(true)
{
    this->_pvt_updateName();
    this->rebuildProperties();
}

/** Construct a named, empty FastInterCLJFF */
FastInterCLJFF::FastInterCLJFF(const QString &name)
               : ConcreteProperty<FastInterCLJFF,G1FF>(), FF3D(),
                 switchfunc( SwitchingFunction::null() ),
                 combining_rules( LJParameterDB::interpret("arithmetic") ),
                 need_update_ljpairs(true), use_electrostatic_shifting(false),
                 recalc_from_scratch(true)
{
    FF::setName(name);
    this->_pvt_updateName();
    this->rebuildProperties();
}

/** Copy constructor */
FastInterCLJFF::FastInterCLJFF(const FastInterCLJFF &other)
               : ConcreteProperty<FastInterCLJFF,G1FF>(other), FF3D(other),
                 ffcomponents(other.ffcomponents), 
                 ljpairs(other.ljpairs), props(other.props),
                 molprops(other.molprops), mol_to_beadid(other.mol_to_beadid),
                 switchfunc(other.switchfunc),
                 combining_rules(other.combining_rules), ptchs(other.ptchs),
                 need_update_ljpairs(other.need_update_ljpairs),
                 use_electrostatic_shifting(other.use_electrostatic_shifting),
                 recalc_from_scratch(other.recalc_from_scratch)
{}

/** Destructor */
FastInterCLJFF::~FastInterCLJFF()
{}

const char* FastInterCLJFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<FastInterCLJFF>() );
}

/** Copy assignment operator */
FastInterCLJFF& FastInterCLJFF::operator=(const FastInterCLJFF &other)
{
    if (this != &other)
    {
        G1FF::operator=(other);
        ffcomponents = other.ffcomponents;
        ljpairs = other.ljpairs;
        props = other.props;
        molprops = other.molprops;
        mol_to_beadid = other.mol_to_beadid;
        switchfunc = other.switchfunc;
        combining_rules = other.combining_rules;
        ptchs = other.ptchs;
        need_update_ljpairs = other.need_update_ljpairs;
        use_electrostatic_shifting = other.use_electrostatic_shifting;
        recalc_from_scratch = other.recalc_from_scratch;
    }
    
    return *this;
}

/** Comparison operator */
bool FastInterCLJFF::operator==(const FastInterCLJFF &other) const
{
    return G1FF::operator==(other) and
           props == other.props;
}

/** Comparison operator */
bool FastInterCLJFF::operator!=(const FastInterCLJFF &other) const
{
    return not FastInterCLJFF::operator==(other);
}

FastInterCLJFF* FastInterCLJFF::clone() const
{
    return new FastInterCLJFF(*this);
}

const CLJComponent& FastInterCLJFF::_pvt_components() const
{
    return ffcomponents;
}

/** Call to rebuild the entire internal state from the current
    molecules in this forcefield */
void FastInterCLJFF::rebuildAll()
{
    recalc_from_scratch = true;
    need_update_ljpairs = true;
    
    switchfunc = props.property("switchingFunction").asA<SwitchingFunction>();
    ptchs = Patches( props.property("patching").asA<Patching>() );
    combining_rules = LJParameterDB::interpret( props.property("combiningRules")
                                        .asA<VariantProperty>().convertTo<QString>() );
    use_electrostatic_shifting = props.property("shiftElectrostatics")
                                      .asA<VariantProperty>().convertTo<bool>();
    
    //now rebuild all of the bead groups from the list of molecules
    const Molecules &mols = this->operator[](MGIdx(0)).molecules();
    
    QHash<MolNum,PropertyMap> old_molprops = molprops;
    
    molprops = QHash<MolNum,PropertyMap>();
    
    for (Molecules::const_iterator it = mols.constBegin();
         it != mols.constEnd();
         ++it)
    {
        this->_pvt_added( *it, molprops.value(it.key(), PropertyMap()) );
    }

    //the energy almost certainly will need recalculating
    FF::setDirty();
}

/** Recalculate the energy - this will either increment the energy
    or will recaculate it from scratch */
void FastInterCLJFF::recalculateEnergy()
{
    if (need_update_ljpairs)
    {
        //rebuild the LJ pair parameter database
        
        recalc_from_scratch = true;
    }

    if (recalc_from_scratch)
    {
    }
    else
    {
    }
    
    this->components().setEnergy(*this, CLJEnergy(0));
}

void FastInterCLJFF::_pvt_updateName()
{
    ffcomponents = CLJComponent(this->name());
    G1FF::_pvt_updateName();
}

/** Called when the passed molecule view has been added to the forcefield */
void FastInterCLJFF::_pvt_added(const PartialMolecule &mol, const PropertyMap &map)
{
    if (mol_to_beadid.contains(mol.number()))
    {
        //remove the current version of the molecule
        ptchs.remove( mol_to_beadid.value(mol.number()) );
        mol_to_beadid.remove(mol.number());
        #warning I AM HERE
        //beadmols.remove(mol.number());
    }
    
    //break the molecule into beads
    #warning AND HERE
    
    //Beads beads = mol.splitIntoBeads(map);

    //beads.property<Vector>() returns AtomCoords
    //beads.property<Charge>() returns AtomCharges

    //bead.property<Vector>() returns correct CoordGroup
    //bead.property<Charge>() returns correct AtomCharges

    return;
}
                
/** Called when the passed molecule view has been removed from the forcefield */
void FastInterCLJFF::_pvt_removed(const PartialMolecule &mol)
{
    return;
}

/** Called when the passed molecule view has been changed */
void FastInterCLJFF::_pvt_changed(const Molecule &mol)
{
    return;
}

/** Called when the passed list of molecules has been changed */
void FastInterCLJFF::_pvt_changed(const QList<Molecule> &mols)
{
    return;
}

/** Called when all molecules have been removed */
void FastInterCLJFF::_pvt_removedAll()
{
    return;
}

/** Called to see if the properties for molecule 'molnum' would change
    if the passed property map was used to get properties */
bool FastInterCLJFF::_pvt_wouldChangeProperties(MolNum molnum, 
                                                const PropertyMap &map) const
{
    return false;
}

/** Return the symbols for the energy components of this forcefield */
const CLJComponent& FastInterCLJFF::components() const
{
    return ffcomponents;
}

/** Set the space used by this forcefield - this returns whether or
    not this changed the space */
bool FastInterCLJFF::setSpace(const Space &space)
{
    if (not ptchs.space().equals(space))
    {
        ptchs.repatch(space);
        props.setProperty("space", space);        
        this->mustNowRecalculateFromScratch();
        return true;
    }
    else
        return false;
}

/** Set the patching scheme (together with enclosed space) to use
    to break the molecules into patches */
bool FastInterCLJFF::setPatching(const Patching &patching)
{
    if (not ptchs.patching().equals(patching))
    {
        FastInterCLJFF new_cljff(*this);
        
        new_cljff.ptchs = Patches(patching);
        new_cljff.props.setProperty("space", patching.space());
        new_cljff.rebuildAll();
        new_cljff.mustNowRecalculateFromScratch();
        FastInterCLJFF::operator=(new_cljff);
        return true;
    }
    else
        return false;
}

/** Set the switching function used to scale the interactions between
    CutGroups to zero at the cutoff */
bool FastInterCLJFF::setSwitchingFunction(const SwitchingFunction &new_switchfunc)
{
    if ( not switchfunc->equals(new_switchfunc) )
    {
        switchfunc = new_switchfunc;
        props.setProperty( "switchingFunction", new_switchfunc );
        this->mustNowRecalculateFromScratch();
        return true;
    }
    else
        return false;
}

/** Set whether or not to shift the electrostatics between CutGroups so that
    the group-group net electrostatic interaction energy between CutGroups
    is zero at the cutoff */
bool FastInterCLJFF::setShiftElectrostatics(bool switchelectro)
{
    if (switchelectro != use_electrostatic_shifting)
    {
        use_electrostatic_shifting = switchelectro;
        props.setProperty( "shiftElectrostatics", VariantProperty(switchelectro) );
        this->mustNowRecalculateFromScratch();
        
        return true;
    }
    else
        return false;
}

/** Set the combining rules to use to obtain mixed LJ parameters */
bool FastInterCLJFF::setCombiningRules(const QString &combiningrules)
{
    LJParameterDB::CombiningRules new_rules = LJParameterDB::interpret(combiningrules);
    
    if (new_rules != combining_rules)
    {
        combining_rules = new_rules;
        need_update_ljpairs = true;
        props.setProperty( "combiningRules", VariantProperty(combiningrules) );
        this->mustNowRecalculateFromScratch();

        return true;
    }
    else
        return false;
}

/** Set the property 'name' to the value 'value'. Returns whether or not
    this changes this forcefield.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool FastInterCLJFF::setProperty(const QString &name, const Property &value)
{
    if (name == QLatin1String("space"))
    {
        return this->setSpace( value.asA<Space>() );
    }
    else if (name == QLatin1String("patching"))
    {
        return this->setPatching( value.asA<Patching>() );
    }
    else if (name == QLatin1String("switchingFunction"))
    {
        return this->setSwitchingFunction( value.asA<SwitchingFunction>() );
    }
    else if (name == QLatin1String("shiftElectrostatics"))
    {
        return this->setShiftElectrostatics( value.asA<VariantProperty>()
                                                     .convertTo<bool>() );
    }
    else if (name == QLatin1String("combiningRules"))
    {
        return this->setCombiningRules( value.asA<VariantProperty>()
                                                     .convertTo<QString>() );
    }
    else
        throw SireBase::missing_property( QObject::tr(
            "The CLJ potentials do not have a property called \"%1\" that "
            "can be changed. Available properties are [ %2 ].")
                .arg(name, QStringList(props.propertyKeys()).join(", ")), CODELOC );
                
    return false;
}

/** Return the 3D space in which this potential is evaluated */
const Space& FastInterCLJFF::space() const
{
    return ptchs.space();
}

/** Return the patching function used to divide space into patches */
const Patching& FastInterCLJFF::patching() const
{
    return ptchs.patching();
}

/** Return the switching function used to scale the group-group
    interactions to zero at the cutoff */
const SwitchingFunction& FastInterCLJFF::switchingFunction() const
{
    return switchfunc.read();
}

/** Return whether or not the net group-group electrostatic interaction
    energy is shifted so that it is zero at the cutoff */
bool FastInterCLJFF::shiftElectrostatics() const
{
    return use_electrostatic_shifting;
}

/** Return the string identifying the combining rules used to 
    obtain the mixed LJ parameters */
const QString& FastInterCLJFF::combiningRules() const
{
    return LJParameterDB::toString(combining_rules);
}

/** Return the property 'name' from this forcefield

    \throw SireBase::missing_property
*/
const Property& FastInterCLJFF::property(const QString &name) const
{
    if (props.hasProperty(name))
    {
        return props.property(name);
    }
    else
    {
        throw SireBase::missing_property( QObject::tr(
                "There is no property called \"%1\" in the forcefield %2. "
                "Available properties are [ %3 ].")
                    .arg(name, this->toString())
                    .arg( Sire::toString(props.propertyKeys()) ), CODELOC );
    }
}

/** Return whether or not this forcefield contains a property called 'name' */
bool FastInterCLJFF::containsProperty(const QString &name) const
{
    return props.hasProperty(name);
}

/** Return all of the properties of this forcefield */
const Properties& FastInterCLJFF::properties() const
{
    return props;
}

/** Set that this forcefield must recalculate the energy from scratch */
void FastInterCLJFF::mustNowRecalculateFromScratch()
{
    recalc_from_scratch = true;
}

void FastInterCLJFF::force(ForceTable &forcetable, double scale_force)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support force calculations!"), CODELOC );
}

void FastInterCLJFF::force(ForceTable &forcetable, const Symbol &symbol,
                           double scale_force)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support force calculations!"), CODELOC );
}
           
void FastInterCLJFF::field(FieldTable &fieldtable, double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::field(FieldTable &fieldtable, const Symbol &component,
                           double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::potential(PotentialTable &potentialtable, double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}

void FastInterCLJFF::potential(PotentialTable &potentialtable, const Symbol &component,
                               double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}

void FastInterCLJFF::field(FieldTable &fieldtable, const Probe &probe, double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::field(FieldTable &fieldtable, const Symbol &component,
                           const Probe &probe, double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::potential(PotentialTable &potentialtable, const Probe &probe,
                               double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}
                               
void FastInterCLJFF::potential(PotentialTable &potentialtable, const Symbol &component,
                               const Probe &probe, double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}
