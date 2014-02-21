/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "interff.h"
#include "cljshiftfunction.h"
#include "cljcalculator.h"

#include "SireBase/booleanproperty.h"
#include "SireBase/lengthproperty.h"

#include "SireError/errors.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QElapsedTimer>
#include <QDebug>

using namespace SireMM;
using namespace SireMol;
using namespace SireFF;
using namespace SireBase;
using namespace SireStream;

static RegisterMetaType<InterFF> r_interff;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const InterFF &interff)
{
    writeHeader(ds, r_interff, 1);
    
    SharedDataStream sds(ds);
    sds << static_cast<const G1FF&>(interff);
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, InterFF &interff)
{
    VersionID v = readHeader(ds, r_interff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> static_cast<G1FF&>(interff);
        
        interff._pvt_updateName();
    }
    else
        throw version_error(v, "1", r_interff, CODELOC);
    
    return ds;
}

/** Constructor */
InterFF::InterFF() : ConcreteProperty<InterFF,G1FF>()
{
    this->_pvt_updateName();
    this->setCLJFunction( CLJShiftFunction::defaultShiftFunction() );
}

/** Construct, specifying the name of the forcefield */
InterFF::InterFF(const QString &name) : ConcreteProperty<InterFF, G1FF>()
{
    G1FF::setName(name);
    this->setCLJFunction( CLJShiftFunction::defaultShiftFunction() );
}

/** Copy constructor */
InterFF::InterFF(const InterFF &other)
        : ConcreteProperty<InterFF,G1FF>(other),
          cljatoms(other.cljatoms), changed_atoms(other.changed_atoms),
          changed_mols(other.changed_mols), maps_for_mol(other.maps_for_mol),
          cljfunc(other.cljfunc), cljboxes(other.cljboxes), fixed_atoms(other.fixed_atoms),
          cljcomps(other.cljcomps), props(other.props)
{}

/** Destructor */
InterFF::~InterFF()
{}

/** Function used to set the CLJFunction used to calculate the energy */
void InterFF::setCLJFunction(const CLJFunction &func)
{
    if (not cljfunc.read().equals(func))
    {
        fixed_atoms.setCLJFunction(func);
        cljfunc = func;
        rebuildProps();
        this->mustNowRecalculateFromScratch();
    }
}

/** Return the function used to calculate the energy */
const CLJFunction& InterFF::cljFunction() const
{
    return cljfunc.read();
}

/** Internal function called when the name of the forcefield changes */
void InterFF::_pvt_updateName()
{
    cljcomps = CLJComponent( this->name() );
    G1FF::_pvt_updateName();
}

const char* InterFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<InterFF>() );
}

const char* InterFF::what() const
{
    return InterFF::typeName();
}

/** Copy assignment operator */
InterFF& InterFF::operator=(const InterFF &other)
{
    if (this != &other)
    {
        cljatoms = other.cljatoms;
        changed_atoms = other.changed_atoms;
        changed_mols = other.changed_mols;
        maps_for_mol = other.maps_for_mol;
        cljfunc = other.cljfunc;
        cljboxes = other.cljboxes;
        fixed_atoms = other.fixed_atoms;
        cljcomps = other.cljcomps;
        props = other.props;
        G1FF::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool InterFF::operator==(const InterFF &other) const
{
    return (this == &other) or
           (G1FF::operator==(other) and fixed_atoms == other.fixed_atoms);
}

/** Comparison operator */
bool InterFF::operator!=(const InterFF &other) const
{
    return not operator==(other);
}

InterFF* InterFF::clone() const
{
    return new InterFF(*this);
}

/** Return the energy components of this forcefield */
const CLJComponent& InterFF::components() const
{
    return cljcomps;
}

/** Internal function used to rebuild the properties object that 
    stores all of the properties of this forcefield */
void InterFF::rebuildProps()
{
    props = cljfunc.read().properties();
    props.setProperty("cljFunction", cljfunc);
    props.setProperty("useGrid", BooleanProperty(fixed_atoms.usesGrid()));
    props.setProperty("gridBuffer", LengthProperty(fixed_atoms.gridBuffer()));
    props.setProperty("gridSpacing", LengthProperty(fixed_atoms.gridSpacing()));
}

/** Set the forcefield property called 'name' to the value 'property' */
bool InterFF::setProperty(const QString &name, const Property &property)
{
    if (name == "cljFunction")
    {
        if (not cljfunc.read().equals(property))
        {
            this->setCLJFunction(property.asA<CLJFunction>());
            return true;
        }
        else
            return false;
    }
    else if (name == "useGrid")
    {
        bool use_grid = property.asA<BooleanProperty>().value();
        
        if (use_grid != fixed_atoms.usesGrid())
        {
            fixed_atoms.setUseGrid(use_grid);
            props.setProperty("useGrid", BooleanProperty(use_grid));
            return true;
        }
        else
            return false;
    }
    else if (name == "gridBuffer")
    {
        Length buffer = property.asA<LengthProperty>().value();
        
        if (buffer != fixed_atoms.gridBuffer())
        {
            fixed_atoms.setGridBuffer(buffer);
            props.setProperty("gridBuffer", property);
            return true;
        }
        else
            return false;
    }
    else if (name == "gridSpacing")
    {
        Length spacing = property.asA<LengthProperty>().value();
        
        if (spacing != fixed_atoms.gridSpacing())
        {
            fixed_atoms.setGridSpacing(spacing);
            props.setProperty("gridSpacing", property);
            return true;
        }
        else
            return false;
    }
    else
    {
        PropertyPtr old_prop = cljfunc.read().property(name);
    
        if (not property.equals(old_prop.read()))
        {
            //need to set the property
            this->setCLJFunction( cljfunc.read().setProperty(name, property) );
            return true;
        }
        else
            return false;
    }
}

/** Return the value of the forcefield property with name 'name' */
const Property& InterFF::property(const QString &name) const
{
    return props.property(name);
}

/** Return whether or not this forcefield contains the property 'property' */
bool InterFF::containsProperty(const QString &name) const
{
    return props.hasProperty(name);
}

/** Return all of the properties of this function */
const Properties& InterFF::properties() const
{
    return props;
}

/** Add the passed atoms as fixed atoms to the forcefield */
void InterFF::addFixedAtoms(const CLJAtoms &atoms)
{
    fixed_atoms.addFixedAtoms(atoms);
    this->mustNowRecalculateFromScratch();
}

/** Add the passed atoms as fixed atoms to the forcefield */
void InterFF::addFixedAtoms(const MoleculeView &molecule, const PropertyMap &map)
{
    this->addFixedAtoms( CLJAtoms(molecule,map) );
}

/** Add the passed molecules as fixed atoms to the forcefield */
void InterFF::addFixedAtoms(const Molecules &molecules, const PropertyMap &map)
{
    this->addFixedAtoms( CLJAtoms(molecules,map) );
}

/** Set the fixed atoms equal to 'atoms' */
void InterFF::setFixedAtoms(const CLJAtoms &atoms)
{
    fixed_atoms.setFixedAtoms(atoms);
    this->mustNowRecalculateFromScratch();
}

/** Set the fixed atoms equal to 'molecule' */
void InterFF::setFixedAtoms(const MoleculeView &molecule, const PropertyMap &map)
{
    this->setFixedAtoms( CLJAtoms(molecule,map) );
}

/** Set the fixed atoms equal to 'molecules' */
void InterFF::setFixedAtoms(const Molecules &molecules, const PropertyMap &map)
{
    this->setFixedAtoms( CLJAtoms(molecules,map) );
}

/** Set whether or not a grid is used to optimise energy calculations with the fixed atoms */
void InterFF::setUseGrid(bool on)
{
    if (fixed_atoms.usesGrid() != on)
    {
        fixed_atoms.setUseGrid(on);
        this->mustNowRecalculateFromScratch();
        props.setProperty("useGrid", BooleanProperty(on));
    }
}

/** Turn on the use of the grid */
void InterFF::enableGrid()
{
    this->setUseGrid(true);
}

/** Turn off use of the grid */
void InterFF::disableGrid()
{
    this->setUseGrid(false);
}

/** Return whether or not the grid is used */
bool InterFF::usesGrid() const
{
    return fixed_atoms.usesGrid();
}

/** Set the buffer used when using a grid. This is the distance
    added around the maximum extent of the atoms when working out the
    dimension of the grid */
void InterFF::setGridBuffer(Length buffer)
{
    if (fixed_atoms.gridBuffer() != buffer)
    {
        fixed_atoms.setGridBuffer(buffer);
        props.setProperty("gridBuffer", LengthProperty(buffer));
        
        if (usesGrid())
            this->mustNowRecalculateFromScratch();
    }
}

/** Return the buffer used when working out the dimension of the grid */
Length InterFF::gridBuffer() const
{
    return fixed_atoms.gridBuffer();
}

/** Set the spacing between grid points */
void InterFF::setGridSpacing(Length spacing)
{
    if (fixed_atoms.gridSpacing() != spacing)
    {
        fixed_atoms.setGridSpacing(spacing);
        props.setProperty("gridSpacing", LengthProperty(spacing));
        
        if (usesGrid())
            this->mustNowRecalculateFromScratch();
    }
}

/** Return spacing between grid points */
Length InterFF::gridSpacing() const
{
    return fixed_atoms.gridSpacing();
}

/** Return the grid used to calculate the energy with fixed atoms. This will
    only be set after the first energy calculation that uses the grid */
GridInfo InterFF::grid() const
{
    return fixed_atoms.grid();
}

/** Internal function used to rebox atoms */
void InterFF::reboxAtoms()
{
    //rebox everything
    QElapsedTimer t;
    t.start();
    
    cljboxes = CLJBoxes();
    for (QHash<MolNum,QPair<QVector<CLJBoxIndex>,CLJAtoms> >::iterator it = cljatoms.begin();
         it != cljatoms.end();
         ++it)
    {
        it.value().first = cljboxes.add(it.value().second);
    }
    
    qint64 ns = t.nsecsElapsed();
    
    qDebug() << "Reboxing took" << (0.000001*ns) << "ms";
}

/** Internal function used to rebox changed atoms */
void InterFF::reboxChangedAtoms()
{
    QElapsedTimer t;
    t.start();

    foreach (const MolNum &molnum, changed_mols)
    {
        QHash<MolNum,QPair<QVector<CLJBoxIndex>,CLJAtoms> >::iterator it = cljatoms.find(molnum);
        
        if (it == cljatoms.end())
            throw SireError::program_bug( QObject::tr(
                    "How can the changed molecule %1 be missing from cljatoms?")
                        .arg(molnum.value()), CODELOC );
        
        it.value().first = cljboxes.add(it.value().second);
    }
    
    changed_atoms = CLJBoxes();
    changed_mols.clear();
    
    qint64 ns = t.nsecsElapsed();
    
    qDebug() << "Reboxing the molecules took" << (0.000001*ns) << "ms";
}

/** Internal function used to regrid the atoms */
void InterFF::regridAtoms()
{
    if (usesGrid() and not fixed_atoms.isEmpty())
    {
        fixed_atoms.setGridDimensions( cljboxes.atoms() );
    }
}

/** Signal that this forcefield must now be recalculated from scratch */
void InterFF::mustNowRecalculateFromScratch()
{
    if (not cljatoms.isEmpty())
    {
        changed_atoms = CLJBoxes();
        changed_mols.clear();
        cljatoms.clear();
        
        if (fixed_atoms.usesGrid())
        {
            //turn the grid off, then on again - this clears it
            fixed_atoms.disableGrid();
            fixed_atoms.enableGrid();
        }
    }
    
    FF::setDirty();
}

/** Re-extract all of the atoms - this also reboxes them all */
void InterFF::reextractAtoms()
{
    QElapsedTimer t;
    t.start();

    mustNowRecalculateFromScratch();
    
    foreach (const MolNum &molnum, this->molNums())
    {
        CLJAtoms mol( this->operator[](molnum), maps_for_mol.value(molnum,PropertyMap()) );
        
        QVector<CLJBoxIndex> idxs = cljboxes.add(mol);
        
        cljatoms.insert(molnum, QPair<QVector<CLJBoxIndex>,CLJAtoms>(idxs,mol));
    }
    
    qint64 ns = t.nsecsElapsed();
    
    qDebug() << "Re-extracting took" << (0.000001*ns) << "ms";
}

/** Recalculate the energy of this forcefield */
void InterFF::recalculateEnergy()
{
    if (cljboxes.isEmpty())
    {
        //everything needs to be recalculated from scratch
        if (cljatoms.isEmpty())
        {
            //extract all of the atoms
            reextractAtoms();
            
            //if there are no atoms, then nothing to be done
            if (cljatoms.isEmpty())
            {
                cljcomps.setEnergy(*this, CLJEnergy(0.0));
                FF::setClean();
                return;
            }
        }
        else
            this->reboxAtoms();
        
        //calculate the energy from scratch
        CLJCalculator calc;
        tuple<double,double> nrgs = calc.calculate(*cljfunc, cljboxes);

        if (not fixed_atoms.isEmpty())
        {
            this->regridAtoms();
            tuple<double,double> grid_nrgs = fixed_atoms.calculate(cljboxes);
            nrgs.get<0>() += grid_nrgs.get<0>();
            nrgs.get<1>() += grid_nrgs.get<1>();
        }
        
        cljcomps.setEnergy(*this, CLJEnergy(nrgs.get<0>(), nrgs.get<1>()));
        FF::setClean();
    }
    else if (not changed_atoms.isEmpty())
    {
        //calculate the change in energy using the molecules in changed_atoms
        CLJCalculator calc;
        tuple<double,double> delta_nrgs = calc.calculate(*cljfunc, changed_atoms, cljboxes);
        
        if (not fixed_atoms.isEmpty())
        {
            tuple<double,double> grid_deltas = fixed_atoms.calculate(changed_atoms);
            
            delta_nrgs.get<0>() += grid_deltas.get<0>();
            delta_nrgs.get<1>() += grid_deltas.get<1>();
        }
        
        //now rebox the changed molecules
        this->reboxChangedAtoms();
        
        cljcomps.changeEnergy(*this, CLJEnergy(delta_nrgs.get<0>(), delta_nrgs.get<1>()));
        FF::setClean();
    }
    else
    {
        //strange - the forcefield thinks that it is dirty...
        //Recalculate the energy from scratch to be sure we are ok
        this->mustNowRecalculateFromScratch();
        this->recalculateEnergy();
        return;
    }
}

/** Function called to add a molecule to this forcefield */
void InterFF::_pvt_added(const SireMol::PartialMolecule &mol, const SireBase::PropertyMap &map)
{
    //be lazy for the moment - recalculate everything!
    mustNowRecalculateFromScratch();
    
    //store the map used to get properties
    if (map.isDefault())
    {
        if (maps_for_mol.contains(mol.number()))
            maps_for_mol.remove(mol.number());
    }
    else
    {
        maps_for_mol.insert(mol.number(),map);
    }
}

/** Function called to remove a molecule from this forcefield */
void InterFF::_pvt_removed(const SireMol::PartialMolecule &mol)
{
    //be lazy for the moment - recalculate everything!
    mustNowRecalculateFromScratch();
    
    if (maps_for_mol.contains(mol.number()))
    {
        if (not this->contains(mol.number()))
        {
            maps_for_mol.remove(mol.number());
        }
    }
}

/** Function called to indicate that a molecule in this forcefield has changed */
void InterFF::_pvt_changed(const SireMol::Molecule &molecule)
{
    MolNum molnum = molecule.number();

    if (changed_mols.contains(molnum))
    {
        //we are trying to change the same molecule twice in a row. This is too
        //complicated to sort out, so we will have to do everything from scratch
        mustNowRecalculateFromScratch();
        return;
    }

    QHash<MolNum,QPair<QVector<CLJBoxIndex>,CLJAtoms > >::iterator it = cljatoms.find(molnum);
    
    if (it == cljatoms.end())
    {
        //this molecule doesn't exist? - see if recalculating from scratch will be ok
        mustNowRecalculateFromScratch();
        return;
    }

    //create the new atoms
    CLJAtoms new_atoms(this->operator[](molecule.number()),
                       maps_for_mol.value(molecule.number(), PropertyMap()));
    
    //remove the old atoms from the grid
    cljboxes.remove( it.value().first );
    
    //get the negative of the old atoms
    CLJAtoms old_atoms = it.value().second.negate();
    
    //box up and save the new atoms
    it.value().first = cljboxes.add(new_atoms);
    it.value().second = new_atoms;
    
    if (changed_atoms.isEmpty())
    {
        changed_atoms = CLJBoxes(old_atoms, new_atoms);
    }
    else
    {
        changed_atoms.add(old_atoms);
        changed_atoms.add(new_atoms);
    }
    
    changed_mols.append(molnum);
}

/** Function called to indicate that a list of molecules in this forcefield have changed */
void InterFF::_pvt_changed(const QList<SireMol::Molecule> &molecules)
{
    //be lazy for the moment - recalculate everything!
    mustNowRecalculateFromScratch();
}

/** Function called to indicate that all molecules in this forcefield have been removed */
void InterFF::_pvt_removedAll()
{
    cljatoms.clear();
    cljboxes = CLJBoxes();
    changed_atoms = CLJBoxes();
    maps_for_mol.clear();

    mustNowRecalculateFromScratch();
}

/** Function called to query whether or not a change in source properties would
    change the properties needed by this forcefield for the molecule with number 'molnum' */
bool InterFF::_pvt_wouldChangeProperties(SireMol::MolNum molnum,
                                         const SireBase::PropertyMap &map) const
{
    if (map.isDefault())
    {
        return maps_for_mol.contains(molnum);
    }
    else
    {
        return not maps_for_mol.contains(molnum);
    }
}
