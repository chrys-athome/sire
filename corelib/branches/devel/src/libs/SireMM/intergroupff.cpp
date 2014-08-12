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

#include "intergroupff.h"
#include "cljshiftfunction.h"
#include "cljcalculator.h"

#include "SireBase/booleanproperty.h"
#include "SireBase/lengthproperty.h"

#include "SireError/errors.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/molresid.h"
#include "SireMol/residue.h"
#include "SireMol/atomselection.h"
#include "SireMol/selector.hpp"

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
        class InterGroupFFData : public QSharedData
        {
        public:
            InterGroupFFData() : QSharedData(),
                                 parallel_calc(true), repro_sum(false)
            {}
            
            InterGroupFFData(const InterGroupFFData &other)
                 : cljfunc(other.cljfunc),
                   cljcomps(other.cljcomps),
                   props(other.props),
                   parallel_calc(other.parallel_calc),
                   repro_sum(other.repro_sum)
            {}
            
            ~InterGroupFFData()
            {}

            /** The function used to calculate energies */
            CLJFunctionPtr cljfunc;

            /** The energy components available for this forcefield */
            CLJComponent cljcomps;
            
            /** All of the properties in this forcefield */
            Properties props;
            
            /** Whether or not to calculate energies in parallel */
            bool parallel_calc;
            
            /** Whether or not to sum energies using a reproducible sum */
            bool repro_sum;
        };
    }
}

static RegisterMetaType<InterGroupFF> r_intergroupff;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const InterGroupFF &intergroupff)
{
    writeHeader(ds, r_intergroupff, 1);
    
    SharedDataStream sds(ds);
    
    sds << intergroupff.cljgroup[0] << intergroupff.cljgroup[1]
        << intergroupff.needs_accepting
        << intergroupff.d->cljfunc
        << intergroupff.d->parallel_calc
        << intergroupff.d->repro_sum
        << static_cast<const G2FF&>(intergroupff);

    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, InterGroupFF &intergroupff)
{
    VersionID v = readHeader(ds, r_intergroupff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> intergroupff.cljgroup[0] >> intergroupff.cljgroup[1]
            >> intergroupff.needs_accepting
            >> intergroupff.d->cljfunc
            >> intergroupff.d->parallel_calc
            >> intergroupff.d->repro_sum
            >> static_cast<G2FF&>(intergroupff);
        
        intergroupff.rebuildProps();
        intergroupff._pvt_updateName();
    }
    else
        throw version_error(v, "1", r_intergroupff, CODELOC);
    
    return ds;
}

/** Constructor */
InterGroupFF::InterGroupFF()
             : ConcreteProperty<InterGroupFF,G2FF>(),
               needs_accepting(false)
{
    cljgroup[0] = CLJGroup( CLJExtractor::EXTRACT_BY_CUTGROUP );
    cljgroup[1] = CLJGroup( CLJExtractor::EXTRACT_BY_CUTGROUP );

    d = new detail::InterGroupFFData();
    this->_pvt_updateName();
    this->setCLJFunction( CLJShiftFunction::defaultShiftFunction() );
}

/** Construct, specifying the name of the forcefield */
InterGroupFF::InterGroupFF(const QString &name)
             : ConcreteProperty<InterGroupFF, G2FF>(),
               needs_accepting(false)
{
    cljgroup[0] = CLJGroup( CLJExtractor::EXTRACT_BY_CUTGROUP );
    cljgroup[1] = CLJGroup( CLJExtractor::EXTRACT_BY_CUTGROUP );

    d = new detail::InterGroupFFData();
    G2FF::setName(name);
    this->setCLJFunction( CLJShiftFunction::defaultShiftFunction() );
}

/** Copy constructor */
InterGroupFF::InterGroupFF(const InterGroupFF &other)
             : ConcreteProperty<InterGroupFF,G2FF>(other),
               d(other.d),
               needs_accepting(other.needs_accepting)
{
    cljgroup[0] = other.cljgroup[0];
    cljgroup[1] = other.cljgroup[1];
}

/** Destructor */
InterGroupFF::~InterGroupFF()
{}

/** Function used to set the CLJFunction used to calculate the energy */
void InterGroupFF::setCLJFunction(const CLJFunction &func)
{
    if (not d.constData()->cljfunc.read().equals(func))
    {
        d->cljfunc = func;
        rebuildProps();
        this->mustNowRecalculateFromScratch();
    }
}

/** Return the function used to calculate the energy */
const CLJFunction& InterGroupFF::cljFunction() const
{
    return d.constData()->cljfunc.read();
}

/** Internal function called when the name of the forcefield changes */
void InterGroupFF::_pvt_updateName()
{
    d->cljcomps = CLJComponent( this->name() );
    G2FF::_pvt_updateName();
}

const char* InterGroupFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<InterGroupFF>() );
}

const char* InterGroupFF::what() const
{
    return InterGroupFF::typeName();
}

/** Copy assignment operator */
InterGroupFF& InterGroupFF::operator=(const InterGroupFF &other)
{
    if (this != &other)
    {
        cljgroup[0] = other.cljgroup[0];
        cljgroup[1] = other.cljgroup[1];
        needs_accepting = other.needs_accepting;
        d = other.d;
        G2FF::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool InterGroupFF::operator==(const InterGroupFF &other) const
{
    return (this == &other) or
           (G2FF::operator==(other) and
            cljgroup[0] == other.cljgroup[0] and
            cljgroup[1] == other.cljgroup[1] and
            needs_accepting == other.needs_accepting);
}

/** Comparison operator */
bool InterGroupFF::operator!=(const InterGroupFF &other) const
{
    return not operator==(other);
}

InterGroupFF* InterGroupFF::clone() const
{
    return new InterGroupFF(*this);
}

/** Return the energy components of this forcefield */
const CLJComponent& InterGroupFF::components() const
{
    return d->cljcomps;
}

/** Internal function used to rebuild the properties object that 
    stores all of the properties of this forcefield */
void InterGroupFF::rebuildProps()
{
    d->props = d->cljfunc.read().properties();
    d->props.setProperty("cljFunction", d->cljfunc);
    d->props.setProperty("parallelCalculation", BooleanProperty(d->parallel_calc));
    d->props.setProperty("reproducibleCalculation", BooleanProperty(d->repro_sum));
}

/** Set the forcefield property called 'name' to the value 'property' */
bool InterGroupFF::setProperty(const QString &name, const Property &property)
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
    else if (name == "parallelCalculation")
    {
        bool parallel_calc = property.asA<BooleanProperty>().value();
        
        if (parallel_calc != d.constData()->parallel_calc)
        {
            d->parallel_calc = parallel_calc;
            d->props.setProperty("parallelCalculation", property);
            return true;
        }
        else
            return false;
    }
    else if (name == "reproducibleCalculation")
    {
        bool repro_sum = property.asA<BooleanProperty>().value();
        
        if (repro_sum != d.constData()->repro_sum)
        {
            d->repro_sum = repro_sum;
            d->props.setProperty("reproducibleCalculation", property);
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
const Property& InterGroupFF::property(const QString &name) const
{
    return d->props.property(name);
}

/** Return whether or not this forcefield contains the property 'property' */
bool InterGroupFF::containsProperty(const QString &name) const
{
    return d->props.hasProperty(name);
}

/** Return all of the properties of this function */
const Properties& InterGroupFF::properties() const
{
    return d->props;
}

/** Set whether or not to use a multicore parallel algorithm
    to calculate the energy */
void InterGroupFF::setUseParallelCalculation(bool on)
{
    if (on != usesParallelCalculation())
    {
        d->parallel_calc = on;
        d->props.setProperty("parallelCalculation", BooleanProperty(on));
    }
}

/** Turn on use of a multicore parallel calculation of the energy.
    This is on by default, and spreads the energy calculations over
    available cores */
void InterGroupFF::enableParallelCalculation()
{
    this->setUseParallelCalculation(true);
}

/** Turn off use of a multicore parallel calculation of the energy.
    This may be quicker if you have few atoms in the forcefield,
    or if you are only planning on allocating one core per forcefield */
void InterGroupFF::disableParallelCalculation()
{
    this->setUseParallelCalculation(false);
}

/** Return whether or not a parallel algorithm is used to calculate energies */
bool InterGroupFF::usesParallelCalculation() const
{
    return d->parallel_calc;
}

/** Turn on an energy summing algorithm that guarantees the same energy
    regardless of whether a single core or multicore calculation is being
    performed (i.e. rounding errors in both cases will be identical) */
void InterGroupFF::enableReproducibleCalculation()
{
    setUseReproducibleCalculation(true);
}

/** Turn off an energy summing algorithm that guarantees the same energy
    regardless of whether a single core or multicore calculation is being
    performed (i.e. rounding errors in both cases will not be identical) */
void InterGroupFF::disableReproducibleCalculation()
{
    setUseReproducibleCalculation(false);
}

/** Switch on or off use of an energy summing algorithm that guarantees the 
    same energy regardless of whether a single core or multicore calculation 
    is being performed */
void InterGroupFF::setUseReproducibleCalculation(bool on)
{
    if (on != d->repro_sum)
    {
        d->repro_sum = on;
        d->props.setProperty("reproducibleCalculation", BooleanProperty(on));
    }
}

/** Return whether or not a reproducible energy summing algorithm is being
    used to accumulate the energies */
bool InterGroupFF::usesReproducibleCalculation() const
{
    return d->repro_sum;
}

/** Signal that this forcefield must now be recalculated from scratch */
void InterGroupFF::mustNowRecalculateFromScratch()
{
    cljgroup[0].mustRecalculateFromScratch();
    cljgroup[1].mustRecalculateFromScratch();
    needs_accepting = false;

    this->setDirty();
}

/** Signal to completely do everything from scratch */
void InterGroupFF::mustNowReallyRecalculateFromScratch()
{
    cljgroup[0].mustReallyRecalculateFromScratch();
    cljgroup[1].mustReallyRecalculateFromScratch();
    needs_accepting = false;
    
    this->setDirty();
}

/** Recalculate the energy of this forcefield */
void InterGroupFF::recalculateEnergy()
{
    if (cljgroup[0].recalculatingFromScratch() or cljgroup[1].recalculatingFromScratch())
    {
        //calculate the energy from first principles
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
        
        if (cljgroup[0].isEmpty() or cljgroup[1].isEmpty())
        {
            //no atoms
            d.constData()->cljcomps.setEnergy(*this, CLJEnergy(0,0));
            this->setClean();
            return;
        }
        
        //calculate the energy from scratch
        tuple<double,double> nrgs(0,0);
        
        if (d.constData()->parallel_calc)
        {
            CLJCalculator calc(d->repro_sum);
            nrgs = calc.calculate(*(d.constData()->cljfunc),
                                  cljgroup[0].cljBoxes(),
                                  cljgroup[1].cljBoxes());
        }
        else
        {
            nrgs = d.constData()->cljfunc->calculate(cljgroup[0].cljBoxes(),
                                                     cljgroup[1].cljBoxes());
        }

        d.constData()->cljcomps.setEnergy(*this, CLJEnergy(nrgs.get<0>(), nrgs.get<1>()));
        this->setClean();
    }
    else if (cljgroup[0].needsAccepting() and cljgroup[1].needsAccepting())
    {
        //recalculate everything from scratch until I write optimised code that
        //handles the differences between the two groups
        tuple<double,double> nrgs(0,0);
        
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
        
        if (d.constData()->parallel_calc)
        {
            CLJCalculator calc(d.constData()->repro_sum);
            nrgs = calc.calculate(*(d.constData()->cljfunc),
                                  cljgroup[0].cljBoxes(),
                                  cljgroup[1].cljBoxes());
        }
        else
        {
            nrgs = d.constData()->cljfunc.read().calculate(cljgroup[0].cljBoxes(),
                                                           cljgroup[1].cljBoxes());
        }
        
        d.constData()->cljcomps.setEnergy(*this, CLJEnergy(nrgs.get<0>(), nrgs.get<1>()));
        this->setClean();
    }
    else if (cljgroup[0].needsAccepting())
    {
        //group 1 shouldn't need accepting, but we will do it anyway to be sure
        cljgroup[1].accept();
    
        //we can calculate using just the change in energy
        tuple<double,double> delta_nrgs(0,0);

        CLJAtoms changed_atoms = cljgroup[0].changedAtoms();
        
        if (d.constData()->parallel_calc)
        {
            CLJCalculator calc(d.constData()->repro_sum);
            delta_nrgs = calc.calculate(*(d.constData()->cljfunc),
                                        changed_atoms, cljgroup[1].cljBoxes());
        }
        else
        {
            delta_nrgs = d.constData()->cljfunc.read().calculate(changed_atoms,
                                                                 cljgroup[1].cljBoxes());
        }
        
        d.constData()->cljcomps.changeEnergy(*this,
                                    CLJEnergy(delta_nrgs.get<0>(), delta_nrgs.get<1>()));

        //the CLJGroup needs to be accepted before we can change anything else
        needs_accepting = true;

        this->setClean();
    }
    else if (cljgroup[1].needsAccepting())
    {
        //group 0 shouldn't need accepting, but we will do it anyway to be sure
        cljgroup[0].accept();
    
        //we can calculate using just the change in energy
        tuple<double,double> delta_nrgs(0,0);

        CLJAtoms changed_atoms = cljgroup[1].changedAtoms();
        
        if (d.constData()->parallel_calc)
        {
            CLJCalculator calc(d.constData()->repro_sum);
            delta_nrgs = calc.calculate(*(d.constData()->cljfunc),
                                        changed_atoms, cljgroup[0].cljBoxes());
        }
        else
        {
            delta_nrgs = d.constData()->cljfunc.read().calculate(changed_atoms,
                                                                 cljgroup[0].cljBoxes());
        }
        
        d.constData()->cljcomps.changeEnergy(*this,
                                    CLJEnergy(delta_nrgs.get<0>(), delta_nrgs.get<1>()));

        //the CLJGroup needs to be accepted before we can change anything else
        needs_accepting = true;

        this->setClean();
    }
    else
    {
        //recalculate everything from scratch as this has been requested
        //calculate the energy from scratch
        tuple<double,double> nrgs(0,0);
        
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
        
        if (d.constData()->parallel_calc)
        {
            CLJCalculator calc(d.constData()->repro_sum);
            nrgs = calc.calculate(*(d.constData()->cljfunc),
                                  cljgroup[0].cljBoxes(),
                                  cljgroup[1].cljBoxes());
        }
        else
        {
            nrgs = d.constData()->cljfunc.read().calculate(cljgroup[0].cljBoxes(),
                                                           cljgroup[1].cljBoxes());
        }
        
        d.constData()->cljcomps.setEnergy(*this, CLJEnergy(nrgs.get<0>(), nrgs.get<1>()));
        this->setClean();
    }
}

/** Function called to add a molecule to this forcefield */
void InterGroupFF::_pvt_added(quint32 group_id,
                              const SireMol::PartialMolecule &mol,
                              const SireBase::PropertyMap &map)
{
    if (needs_accepting)
    {
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
    }

    cljgroup[group_id].add(mol, map);
    setDirty();
}

/** Function called to remove a molecule from this forcefield */
void InterGroupFF::_pvt_removed(quint32 group_id, const SireMol::PartialMolecule &mol)
{
    if (needs_accepting)
    {
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
    }

    cljgroup[group_id].remove(mol);
    setDirty();
}

/** Function called to indicate that the passed molecule has changed */
void InterGroupFF::_pvt_changed(quint32 group_id, const Molecule &molecule, bool auto_update)
{
    if (needs_accepting)
    {
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
    }

    cljgroup[group_id].update(molecule);
    setDirty();
}

/** Function called to indicate that a list of molecules in this forcefield have changed */
void InterGroupFF::_pvt_changed(quint32 group_id,
                                const QList<SireMol::Molecule> &molecules, bool auto_update)
{
    if (needs_accepting)
    {
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
    }

    foreach (const Molecule &molecule, molecules)
    {
        cljgroup[group_id].update(molecule);
    }
    
    setDirty();
}

/** Function called to indicate that all molecules in this forcefield have been removed */
void InterGroupFF::_pvt_removedAll(quint32 group_id)
{
    if (needs_accepting)
    {
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
    }

    cljgroup[group_id].removeAll();
    this->setDirty();
}

/** Function called to query whether or not a change in source properties would
    change the properties needed by this forcefield for the molecule with number 'molnum' */
bool InterGroupFF::_pvt_wouldChangeProperties(quint32 group_id, SireMol::MolNum molnum,
                                              const SireBase::PropertyMap &map) const
{
    return cljgroup[group_id].mapForMolecule(molnum) != map;
}

/** Return whether or not this forcefield is using a temporary workspace that 
    needs to be accepted */
bool InterGroupFF::needsAccepting() const
{
    return needs_accepting or G2FF::needsAccepting();
}

/** Tell the forcefield that the last move was accepted. This tells the
    forcefield to make permanent any temporary changes that were used a workspace
    to avoid memory allocation during a move */
void InterGroupFF::accept()
{
    if (needs_accepting)
    {
        cljgroup[0].accept();
        cljgroup[1].accept();
        needs_accepting = false;
    }
    
    G2FF::accept();
}
