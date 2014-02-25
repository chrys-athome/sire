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

namespace SireMM
{
    namespace detail
    {
        class InterFFData : public QSharedData
        {
        public:
            InterFFData() : QSharedData()
            {}
            
            InterFFData(const InterFFData &other)
                 : maps_for_mol(other.maps_for_mol),
                   cljfunc(other.cljfunc),
                   fixed_atoms(other.fixed_atoms),
                   cljcomps(other.cljcomps),
                   props(other.props)
            {}
            
            ~InterFFData()
            {}
            
            /** The property map used to add each molecule */
            QHash<SireMol::MolNum,PropertyMap> maps_for_mol;

            /** The function used to calculate energies */
            CLJFunctionPtr cljfunc;
            
            /** All of the fixed atoms */
            CLJGrid fixed_atoms;

            /** The energy components available for this forcefield */
            CLJComponent cljcomps;
            
            /** All of the properties in this forcefield */
            Properties props;
        };
    }
}

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
    d = new detail::InterFFData();
    this->_pvt_updateName();
    this->setCLJFunction( CLJShiftFunction::defaultShiftFunction() );
}

/** Construct, specifying the name of the forcefield */
InterFF::InterFF(const QString &name) : ConcreteProperty<InterFF, G1FF>()
{
    d = new detail::InterFFData();
    G1FF::setName(name);
    this->setCLJFunction( CLJShiftFunction::defaultShiftFunction() );
}

/** Copy constructor */
InterFF::InterFF(const InterFF &other)
        : ConcreteProperty<InterFF,G1FF>(other),
          atom_locs(other.atom_locs), changed_atoms(other.changed_atoms),
          changed_mols(other.changed_mols), cljboxes(other.cljboxes), d(other.d)
{}

/** Destructor */
InterFF::~InterFF()
{}

/** Function used to set the CLJFunction used to calculate the energy */
void InterFF::setCLJFunction(const CLJFunction &func)
{
    if (not d.constData()->cljfunc.read().equals(func))
    {
        d->fixed_atoms.setCLJFunction(func);
        d->cljfunc = func;
        rebuildProps();
        qDebug() << this->properties().toString();
        this->mustNowRecalculateFromScratch();
    }
}

/** Return the function used to calculate the energy */
const CLJFunction& InterFF::cljFunction() const
{
    return d.constData()->cljfunc.read();
}

/** Internal function called when the name of the forcefield changes */
void InterFF::_pvt_updateName()
{
    d->cljcomps = CLJComponent( this->name() );
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
    //qDebug() << "COPY ASSIGNMENT";

    if (this != &other)
    {
        atom_locs = other.atom_locs;
        changed_atoms = other.changed_atoms;
        changed_mols = other.changed_mols;
        cljboxes = other.cljboxes;
        d = other.d;
        G1FF::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool InterFF::operator==(const InterFF &other) const
{
    return (this == &other) or
           (G1FF::operator==(other) and d->fixed_atoms == other.d->fixed_atoms);
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
    return d->cljcomps;
}

/** Internal function used to rebuild the properties object that 
    stores all of the properties of this forcefield */
void InterFF::rebuildProps()
{
    d->props = d->cljfunc.read().properties();
    d->props.setProperty("cljFunction", d->cljfunc);
    d->props.setProperty("useGrid", BooleanProperty(d->fixed_atoms.usesGrid()));
    d->props.setProperty("gridBuffer", LengthProperty(d->fixed_atoms.gridBuffer()));
    d->props.setProperty("gridSpacing", LengthProperty(d->fixed_atoms.gridSpacing()));
}

/** Set the forcefield property called 'name' to the value 'property' */
bool InterFF::setProperty(const QString &name, const Property &property)
{
    if (name == "cljFunction")
    {
        if (not d.constData()->cljfunc.read().equals(property))
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
        
        if (use_grid != d.constData()->fixed_atoms.usesGrid())
        {
            d->fixed_atoms.setUseGrid(use_grid);
            d->props.setProperty("useGrid", BooleanProperty(use_grid));
            return true;
        }
        else
            return false;
    }
    else if (name == "gridBuffer")
    {
        Length buffer = property.asA<LengthProperty>().value();
        
        if (buffer != d.constData()->fixed_atoms.gridBuffer())
        {
            d->fixed_atoms.setGridBuffer(buffer);
            d->props.setProperty("gridBuffer", property);
            return true;
        }
        else
            return false;
    }
    else if (name == "gridSpacing")
    {
        Length spacing = property.asA<LengthProperty>().value();
        
        if (spacing != d.constData()->fixed_atoms.gridSpacing())
        {
            d->fixed_atoms.setGridSpacing(spacing);
            d->props.setProperty("gridSpacing", property);
            return true;
        }
        else
            return false;
    }
    else
    {
        PropertyPtr old_prop = d.constData()->cljfunc.read().property(name);
    
        if (not property.equals(old_prop.read()))
        {
            //need to set the property
            this->setCLJFunction( d.constData()->cljfunc.read().setProperty(name, property) );
            return true;
        }
        else
            return false;
    }
}

/** Return the value of the forcefield property with name 'name' */
const Property& InterFF::property(const QString &name) const
{
    return d->props.property(name);
}

/** Return whether or not this forcefield contains the property 'property' */
bool InterFF::containsProperty(const QString &name) const
{
    return d->props.hasProperty(name);
}

/** Return all of the properties of this function */
const Properties& InterFF::properties() const
{
    return d->props;
}

/** Add the passed atoms as fixed atoms to the forcefield */
void InterFF::addFixedAtoms(const CLJAtoms &atoms)
{
    d->fixed_atoms.addFixedAtoms(atoms);
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
    d->fixed_atoms.setFixedAtoms(atoms);
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
    if (d.constData()->fixed_atoms.usesGrid() != on)
    {
        d->fixed_atoms.setUseGrid(on);
        this->mustNowRecalculateFromScratch();
        d->props.setProperty("useGrid", BooleanProperty(on));
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
    return d->fixed_atoms.usesGrid();
}

/** Set the buffer used when using a grid. This is the distance
    added around the maximum extent of the atoms when working out the
    dimension of the grid */
void InterFF::setGridBuffer(Length buffer)
{
    if (d.constData()->fixed_atoms.gridBuffer() != buffer)
    {
        d->fixed_atoms.setGridBuffer(buffer);
        d->props.setProperty("gridBuffer", LengthProperty(buffer));
        
        if (usesGrid())
            this->mustNowRecalculateFromScratch();
    }
}

/** Return the buffer used when working out the dimension of the grid */
Length InterFF::gridBuffer() const
{
    return d->fixed_atoms.gridBuffer();
}

/** Set the spacing between grid points */
void InterFF::setGridSpacing(Length spacing)
{
    if (d.constData()->fixed_atoms.gridSpacing() != spacing)
    {
        d->fixed_atoms.setGridSpacing(spacing);
        d->props.setProperty("gridSpacing", LengthProperty(spacing));
        
        if (usesGrid())
            this->mustNowRecalculateFromScratch();
    }
}

/** Return spacing between grid points */
Length InterFF::gridSpacing() const
{
    return d->fixed_atoms.gridSpacing();
}

/** Return the grid used to calculate the energy with fixed atoms. This will
    only be set after the first energy calculation that uses the grid */
GridInfo InterFF::grid() const
{
    return d->fixed_atoms.grid();
}

/** Internal function used to rebox changed atoms */
void InterFF::reboxChangedAtoms()
{
    for (QHash<MolNum,CLJAtoms>::const_iterator it = changed_mols.constBegin();
         it != changed_mols.constEnd();
         ++it)
    {
        ChunkedHash< MolNum,QVector<CLJBoxIndex> >::iterator it2 = atom_locs.find(it.key());
        
        if (it2 == atom_locs.end())
            throw SireError::program_bug( QObject::tr(
                    "How can the changed molecule %1 be missing from cljatoms?")
                        .arg(it.key()), CODELOC );
        
        it2.value() = cljboxes.add(it.value());
    }
    
    changed_atoms = CLJBoxes();
    changed_mols.clear();
}

/** Internal function used to regrid the atoms */
void InterFF::regridAtoms()
{
    if (usesGrid() and not d.constData()->fixed_atoms.isEmpty())
    {
        d->fixed_atoms.setGridDimensions( cljboxes.atoms() );
    }
}

/** Signal that this forcefield must now be recalculated from scratch */
void InterFF::mustNowRecalculateFromScratch()
{
    if (not changed_mols.isEmpty())
    {
        reboxChangedAtoms();
    }
    
    this->setDirty();
}

/** Signal to completely do everything from scratch */
void InterFF::mustNowReallyRecalculateFromScratch()
{
    atom_locs.clear();
    changed_atoms = CLJBoxes();
    changed_mols.clear();
    mustNowRecalculateFromScratch();
}

/** Re-extract all of the atoms - this also reboxes them all */
void InterFF::reextractAtoms()
{
    QElapsedTimer t;
    t.start();

    mustNowRecalculateFromScratch();
    
    //we know that we are only a single molecule group
    const Molecules &mols = this->group( MGIdx(0) ).molecules();
    
    atom_locs.clear();
    atom_locs.reserve(mols.nMolecules());
    cljboxes = CLJBoxes();
    
    for (Molecules::const_iterator it = mols.constBegin();
         it != mols.constEnd();
         ++it)
    {
        CLJAtoms mol( it.value(), d.constData()->maps_for_mol.value(it.key(),PropertyMap()) );
        
        QVector<CLJBoxIndex> idxs = cljboxes.add(mol);
        
        atom_locs.insert(it.key(), idxs);
    }
    
    qint64 ns = t.nsecsElapsed();
    
    qDebug() << "Re-extracting took" << (0.000001*ns) << "ms";
}

/** Recalculate the energy of this forcefield */
void InterFF::recalculateEnergy()
{
    if (atom_locs.isEmpty())
    {
        //everything needs to be recalculated from scratch
        //extract all of the atoms
        reextractAtoms();
            
        //if there are no atoms, then nothing to be done
        if (atom_locs.isEmpty())
        {
            d.constData()->cljcomps.setEnergy(*this, CLJEnergy(0.0));
            this->setClean();
            return;
        }
        
        //calculate the energy from scratch
        QElapsedTimer t;
        t.start();
        CLJCalculator calc;
        tuple<double,double> nrgs = calc.calculate(*(d.constData()->cljfunc), cljboxes);
        //tuple<double,double> nrgs = d.constData()->cljfunc->calculate(cljboxes);
        qint64 ns = t.nsecsElapsed();
        
        qDebug() << "Calculating total energy took" << (0.000001*ns) << "ms";

        if (not d.constData()->fixed_atoms.isEmpty())
        {
            this->regridAtoms();
            tuple<double,double> grid_nrgs = d.constData()->fixed_atoms.calculate(cljboxes);
            nrgs.get<0>() += grid_nrgs.get<0>();
            nrgs.get<1>() += grid_nrgs.get<1>();
        }
        
        d.constData()->cljcomps.setEnergy(*this, CLJEnergy(nrgs.get<0>(), nrgs.get<1>()));
        this->setClean();
    }
    else if (not changed_mols.isEmpty())
    {
        //calculate the change in energy using the molecules in changed_atoms
        //CLJCalculator calc;
        //tuple<double,double> delta_nrgs = calc.calculate(*(d.constData()->cljfunc),
        //                                                 changed_atoms, cljboxes);
        
        tuple<double,double> delta_nrgs = d.constData()->cljfunc->calculate(changed_atoms,
                                                                            cljboxes);
        
        if (not d.constData()->fixed_atoms.isEmpty())
        {
            tuple<double,double> grid_deltas = d.constData()->fixed_atoms.calculate(changed_atoms);
            
            delta_nrgs.get<0>() += grid_deltas.get<0>();
            delta_nrgs.get<1>() += grid_deltas.get<1>();
        }
        
        //now rebox the changed molecules
        this->reboxChangedAtoms();
        
        d.constData()->cljcomps.changeEnergy(*this,
                                    CLJEnergy(delta_nrgs.get<0>(), delta_nrgs.get<1>()));
        this->setClean();
    }
    else
    {
        //recalculate everything from scratch as this has been requested
        //calculate the energy from scratch
        CLJCalculator calc;
        tuple<double,double> nrgs = calc.calculate(*(d.constData()->cljfunc), cljboxes);
        //tuple<double,double> nrgs = d.constData()->cljfunc->calculate(cljboxes);

        if (not d.constData()->fixed_atoms.isEmpty())
        {
            this->regridAtoms();
            tuple<double,double> grid_nrgs = d.constData()->fixed_atoms.calculate(cljboxes);
            nrgs.get<0>() += grid_nrgs.get<0>();
            nrgs.get<1>() += grid_nrgs.get<1>();
        }
        
        d.constData()->cljcomps.setEnergy(*this, CLJEnergy(nrgs.get<0>(), nrgs.get<1>()));
        this->setClean();
    }
}

/** Function called to add a molecule to this forcefield */
void InterFF::_pvt_added(const SireMol::PartialMolecule &mol, const SireBase::PropertyMap &map)
{
    //be lazy for the moment - recalculate everything!
    mustNowReallyRecalculateFromScratch();
    
    //store the map used to get properties
    if (map.isDefault())
    {
        if (d.constData()->maps_for_mol.contains(mol.number()))
            d->maps_for_mol.remove(mol.number());
    }
    else
    {
        d->maps_for_mol.insert(mol.number(),map);
    }
    
    this->setDirty();
}

/** Function called to remove a molecule from this forcefield */
void InterFF::_pvt_removed(const SireMol::PartialMolecule &mol)
{
    //be lazy for the moment - recalculate everything!
    mustNowReallyRecalculateFromScratch();
    
    if (d.constData()->maps_for_mol.contains(mol.number()))
    {
        if (not this->contains(mol.number()))
        {
            d->maps_for_mol.remove(mol.number());
        }
    }
    
    this->setDirty();
}

/** Function called to indicate that a molecule in this forcefield has changed */
void InterFF::_pvt_changed(const SireMol::Molecule &molecule)
{
    MolNum molnum = molecule.number();

    if (changed_mols.contains(molnum))
    {
        //we are trying to change the same molecule twice in a row. This is too
        //complicated to sort out, so we will have to do everything from scratch
        mustNowReallyRecalculateFromScratch();
        return;
    }

    ChunkedHash< MolNum,QVector<CLJBoxIndex> >::const_iterator it = atom_locs.constFind(molnum);
    
    if (it == atom_locs.constEnd())
    {
        //this molecule doesn't exist? - see if recalculating from scratch will be ok
        mustNowReallyRecalculateFromScratch();
        return;
    }

    //create the new atoms
    CLJAtoms new_atoms(this->group(MGIdx(0)).molecules()[molnum],
                       d.constData()->maps_for_mol.value(molnum, PropertyMap()));
    
    //remove the old atoms from the grid
    CLJAtoms old_atoms = cljboxes.takeNegative(it.value());

    if (changed_atoms.isEmpty())
    {
        changed_atoms = CLJBoxes(old_atoms, new_atoms);
    }
    else
    {
        changed_atoms.add(old_atoms);
        changed_atoms.add(new_atoms);
    }

    changed_mols.insert(molnum, new_atoms);
    
    this->setDirty();
}

/** Function called to indicate that a list of molecules in this forcefield have changed */
void InterFF::_pvt_changed(const QList<SireMol::Molecule> &molecules)
{
    //be lazy for the moment - recalculate everything!
    mustNowReallyRecalculateFromScratch();
    
    this->setDirty();
}

/** Function called to indicate that all molecules in this forcefield have been removed */
void InterFF::_pvt_removedAll()
{
    atom_locs.clear();
    cljboxes = CLJBoxes();
    changed_atoms = CLJBoxes();
    d->maps_for_mol.clear();

    mustNowReallyRecalculateFromScratch();
    
    this->setDirty();
}

/** Function called to query whether or not a change in source properties would
    change the properties needed by this forcefield for the molecule with number 'molnum' */
bool InterFF::_pvt_wouldChangeProperties(SireMol::MolNum molnum,
                                         const SireBase::PropertyMap &map) const
{
    if (map.isDefault())
    {
        return d->maps_for_mol.contains(molnum);
    }
    else
    {
        return not d->maps_for_mol.contains(molnum);
    }
}
