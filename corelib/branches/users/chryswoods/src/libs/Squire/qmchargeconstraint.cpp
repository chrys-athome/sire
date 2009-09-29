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

#include "qmchargeconstraint.h"

#include "SireMol/molecules.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/atomcharges.h"
#include "SireMol/mgname.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/moleditor.h"

#include "SireSystem/system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireSystem;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<QMChargeConstraint> r_qmchgconstraint;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds,
                                      const QMChargeConstraint &qmchgconstraint)
{
    writeHeader(ds, r_qmchgconstraint, 1);
    
    SharedDataStream sds(ds);
    
    sds << qmchgconstraint.charge_calculator
        << static_cast<const ChargeConstraint&>(qmchgconstraint);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds,
                                      QMChargeConstraint &qmchgconstraint)
{
    VersionID v = readHeader(ds, r_qmchgconstraint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> qmchgconstraint.charge_calculator
            >> static_cast<ChargeConstraint&>(qmchgconstraint);
            
        qmchgconstraint.must_recalc_from_scratch = true;
    }
    else
        throw version_error(v, "1", r_qmchgconstraint, CODELOC);
        
    return ds;
}

/** Constructor */
QMChargeConstraint::QMChargeConstraint()
                   : ConcreteProperty<QMChargeConstraint,ChargeConstraint>(),
                     must_recalc_from_scratch(true)
{}

/** Construct to constrain the charges for the molecules in the 
    molecule group 'molgroup' using the optionally supplied property
    map to find the necessary properteis */
QMChargeConstraint::QMChargeConstraint(const MoleculeGroup &molgroup,
                                       const PropertyMap &map)
                   : ConcreteProperty<QMChargeConstraint,ChargeConstraint>(molgroup,
                                                                           map),
                     must_recalc_from_scratch(true)
{}

/** Construct to constrain the charges for the molecules in the     
    molecule group 'molgroup' to those calculated using the 
    QM charge calculator 'chargecalculator', using the optionally
    supplied property map to find the necessary properties */
QMChargeConstraint::QMChargeConstraint(const MoleculeGroup &molgroup,
                                       const QMChargeCalculator &chargecalculator,
                                       const PropertyMap &map)
                   : ConcreteProperty<QMChargeConstraint,ChargeConstraint>(molgroup,
                                                                           map),
                     charge_calculator(chargecalculator),
                     must_recalc_from_scratch(true)
{}
                                                                           
/** Copy constructor */
QMChargeConstraint::QMChargeConstraint(const QMChargeConstraint &other)
                   : ConcreteProperty<QMChargeConstraint,ChargeConstraint>(other),
                     charge_calculator(other.charge_calculator),
                     must_recalc_from_scratch(other.must_recalc_from_scratch)
{}

/** Destructor */
QMChargeConstraint::~QMChargeConstraint()
{}

const char* QMChargeConstraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<QMChargeConstraint>() );
}

/** Copy assignment operator */
QMChargeConstraint& QMChargeConstraint::operator=(const QMChargeConstraint &other)
{
    if (this != &other)
    {
        charge_calculator = other.charge_calculator;
        must_recalc_from_scratch = other.must_recalc_from_scratch;
        
        ChargeConstraint::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool QMChargeConstraint::operator==(const QMChargeConstraint &other) const
{
    return charge_calculator == other.charge_calculator and
           must_recalc_from_scratch == other.must_recalc_from_scratch and
           ChargeConstraint::operator==(other);
}

/** Comparison operator */
bool QMChargeConstraint::operator!=(const QMChargeConstraint &other) const
{
    return not QMChargeConstraint::operator==(other);
}

/** Return a string representation of this constraint */
QString QMChargeConstraint::toString() const
{
    return QObject::tr("QMChargeConstraint( moleculeGroup() : %1, "
                       "chargeCalculator() : %2 )")
                            .arg(this->moleculeGroup().name())
                            .arg(this->chargeCalculator().toString());
}

/** Return the calculator used to calculate atomic partial charges
    from an underlying QM calculation */
const QMChargeCalculator& QMChargeConstraint::chargeCalculator() const
{
    return charge_calculator.read();
}

/** Set the charge calculator used to calculate atomic partial charges
    from an underlying QM calculation */
void QMChargeConstraint::setChargeCalculator(const QMChargeCalculator &chargecalculator)
{
    charge_calculator = chargecalculator;
    must_recalc_from_scratch = true;
}

/** Internal function used to calculate the charges of the passed
    molecule. This returns the molecule with the new charges, or
    an empty molecule if the charges haven't changed */
Molecule QMChargeConstraint::_pvt_calculateCharges(const PartialMolecule &molecule) const
{
    AtomCharges new_chgs = charge_calculator.read().calculate(molecule,
                                                              this->propertyMap());
    
    const PropertyName &charge_property = this->propertyMap()["charge"];
    
    if (molecule.hasProperty(charge_property))
    {
        const Property &old_chgs = molecule.property(charge_property);
        
        if (old_chgs.isA<AtomCharges>())
        {  
            if (old_chgs.asA<AtomCharges>() == new_chgs)
                //the charges haven't changed
                return Molecule();
        }
    }
    
    return molecule.molecule().edit()
                              .setProperty(charge_property, new_chgs)
                              .commit();
                                
}

/** Internal function used to update the constraint to match the new system */
void QMChargeConstraint::_pvt_update(const System &system)
{
    if (must_recalc_from_scratch)
    {
        this->updateGroup(system);
        
        mols_to_change = Molecules();
        
        const Molecules &mols = this->moleculeGroup().molecules();
        
        for (Molecules::const_iterator it = mols.constBegin();
             it != mols.constEnd();
             ++it)
        {
            Molecule new_mol = this->_pvt_calculateCharges(*it);
            
            if (not new_mol.isEmpty())
                mols_to_change.add(new_mol);
        }
    }
    else if (this->changesGroup(system))
    {
        //this system changes the group - we need to see if
        //any of the molecules have changed
        const MoleculeGroup &old_group = this->moleculeGroup();
        const MoleculeGroup &new_group = system[ old_group.number() ];
        
        const Molecules &old_mols = old_group.molecules();
        const Molecules &new_mols = new_group.molecules();
        
        //loop over all of the new molecules, looking to see whether
        //or not we have to calculate new charges for each one,
        //or if the previously recommended changes have been applied
        for (Molecules::const_iterator it = new_mols.constBegin();
             it != new_mols.constEnd();
             ++it)
        {
            if (mols_to_change.contains(it.key()))
            {
                //this molecule is already registered as having changed
                const ViewsOfMol &changed_mol = mols_to_change[it.key()];
                const ViewsOfMol &new_mol = it.value();
                
                // - if it is the same version, then the change has been
                //   applied and nothing more needs doing
                if (changed_mol.version() == new_mol.version())
                {
                    mols_to_change.remove(it.key());
                }
                else if (not charge_calculator.read()
                                              .mayChangeCharges(new_mol, changed_mol,
                                                                this->propertyMap()) )
                {
                    //any changes haven't affected the charges, so the 
                    //changed mol is ok
                    mols_to_change.remove(it.key());
                }
                else
                {
                    //the molecule has changed some more - need to calculate
                    //new charges
                    Molecule new_new_mol = this->_pvt_calculateCharges(new_mol);
                    
                    mols_to_change.remove(it.key());
                    
                    if (not new_new_mol.isEmpty())
                        mols_to_change.add(new_new_mol);
                }
            }
            else if (old_mols.contains(it.key()))
            {
                //this molecule already exists in the group
                const ViewsOfMol &old_mol = old_mols[it.key()];
                const ViewsOfMol &new_mol = it.value();
                
                //has the molecule changed?
                if (old_mol.version() != new_mol.version())
                {
                    //does this change affect the charges?
                    if (charge_calculator.read()
                                         .mayChangeCharges(old_mol, new_mol,
                                                           this->propertyMap()))
                    {
                        //we need to calculate new charges
                        Molecule new_new_mol = this->_pvt_calculateCharges(new_mol);
                        
                        if (not new_new_mol.isEmpty())
                            mols_to_change.add(new_new_mol);
                    }
                }
            }
        }
        
        //now loop over 'mols_to_change' and make sure that any molecules
        //which should change, but which are no longer in the group are
        //discarded
        foreach (MolNum molnum, mols_to_change.molNums())
        {
            if (not new_mols.contains(molnum))
                mols_to_change.remove(molnum);
        }
        
        if (mols_to_change.isEmpty())
            mols_to_change = Molecules();
            
        this->updateGroup(system);
    }
}

/** Update this constraint to use the system 'system', returning the molecules
    that would need to change to satisfy this constraint */
Molecules QMChargeConstraint::update(const System &system)
{
    if (system.UID() != this->sysUID() or
        system.version() != this->sysVersion())
    {
        QMChargeConstraint old_state(*this);
        
        try
        {
            this->updatedFrom(system);
            this->_pvt_update(system);
        }
        catch(...)
        {
            QMChargeConstraint::operator=(old_state);
            throw;
        }
    }

    return mols_to_change;
}

/** Update this constraint to use the system 'system', returning the molecules
    that would need to change to satisfy this constraint, providing the hint
    that the molecule with number 'changed_mol' is the only thing changed
    since the last update. If more than 'changed_mol' has changed then
    weird things may happen! */
Molecules QMChargeConstraint::update(const System &system, MolNum)
{
    return QMChargeConstraint::update(system);
}

/** Update this constraint to use the system 'system', returning the molecules
    that would need to change to satisfy this constraint, providing the hint
    that the molecules in 'molecules' are the only things changed
    since the last update. If more than 'molecules' have changed then
    weird things may happen! */
Molecules QMChargeConstraint::update(const System &system, const Molecules&)
{
    return QMChargeConstraint::update(system);
}

/** Return whether or not this constraint is satisfied for 
    the passed system */
bool QMChargeConstraint::isSatisfied(const System &system) const
{
    if (this->sysUID() == system.UID() and
        this->sysVersion() == system.version())
    {
        return mols_to_change.isEmpty();
    }
    else
    {
        QMChargeConstraint c(*this);
        return c.update(system).isEmpty();
    }
}
