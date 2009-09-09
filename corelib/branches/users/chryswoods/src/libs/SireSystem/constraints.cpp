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

#include "constraints.h"
#include "moleculeconstraint.h"
#include "system.h"

#include "SireMol/molecules.h"

#include "SireID/index.h"

#include "SireBase/savestate.h"

#include "SireSystem/errors.h" 
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireSystem;
using namespace SireMol;
using namespace SireID;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<Constraints> r_constraints;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const Constraints &constraints)
{
    writeHeader(ds, r_constraints, 2);
    
    SharedDataStream sds(ds);
    
    sds << constraints.cons << constraints.molcons
        << static_cast<const Property&>(constraints);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, Constraints &constraints)
{
    VersionID v = readHeader(ds, r_constraints);
    
    if (v == 2)
    {
        SharedDataStream sds(ds);
        sds >> constraints.cons >> constraints.molcons
            >> static_cast<Property&>(constraints);
    }
    else if (v == 1)
    {
        SharedDataStream sds(ds);

        QList<ConstraintPtr> cons;

        sds >> cons;
        
        constraints.cons = cons.toVector();
        constraints.molcons = QVector<ConstraintPtr>();
    }
    else
        throw version_error(v, "1", r_constraints, CODELOC);
        
    return ds;
}

/** Null constructor */
Constraints::Constraints() : ConcreteProperty<Constraints,Property>()
{}

/** Construct to contain just the constraint 'constraint' */
Constraints::Constraints(const Constraint &constraint)
            : ConcreteProperty<Constraints,Property>()
{
    if (constraint.isA<MoleculeConstraint>())
        molcons.append(constraint);
    else
        cons.append( constraint );
}

/** Construct from the passed list of constraints */
Constraints::Constraints(const QVector<ConstraintPtr> &constraints)
            : ConcreteProperty<Constraints,Property>()
{
    foreach (const ConstraintPtr constraint, constraints)
    {
        this->add( *constraint );
    }
}

/** Construct from the passed list of constraints */
Constraints::Constraints(const QList<ConstraintPtr> &constraints)
            : ConcreteProperty<Constraints,Property>()
{
    foreach (const ConstraintPtr constraint, constraints)
    {
        this->add( *constraint );
    }
}

/** Copy constructor */
Constraints::Constraints(const Constraints &other)
            : ConcreteProperty<Constraints,Property>(other),
              cons(other.cons), molcons(other.molcons)
{}

/** Destructor */
Constraints::~Constraints()
{}

/** Copy assignment operator */
Constraints& Constraints::operator=(const Constraints &other)
{
    if (this != &other)
    {
        cons = other.cons;
        molcons = other.molcons;
        Property::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Constraints::operator==(const Constraints &other) const
{
    return cons == other.cons and molcons == other.molcons;
}

/** Comparison operator */
bool Constraints::operator!=(const Constraints &other) const
{
    return not this->operator==(other);
}

/** Return the ith constraint in this set

    \throw SireError::invalid_index
*/
const Constraint& Constraints::operator[](int i) const
{
    i = Index(i).map( this->nConstraints() );
    
    if (i >= cons.count())
        return molcons.at( i - cons.count() ).read();
    else
        return cons.at( i ).read();
}

/** Syntactic sugar for Constraints::add */
Constraints& Constraints::operator+=(const Constraint &constraint)
{
    this->add(constraint);
    return *this;
}

/** Syntactic sugar for Constraints::add */
Constraints& Constraints::operator+=(const Constraints &constraints)
{
    this->add(constraints);
    return *this;
}

/** Syntactic sugar for Constraints::remove */
Constraints& Constraints::operator-=(const Constraint &constraint)
{
    this->remove(constraint);
    return *this;
}

/** Syntactic sugar for Constraints::remove */
Constraints& Constraints::operator-=(const Constraints &constraints)
{
    this->remove(constraints);
    return *this;
}

/** Return the number of constraints in this set */
int Constraints::nConstraints() const
{
    return cons.count() + molcons.count();
}

/** Return the number of constraints in this set */
int Constraints::count() const
{
    return this->nConstraints();
}

/** Return the number of constraints in this set */
int Constraints::size() const
{
    return this->nConstraints();
}

/** Return the list of all of the constraints in this set */
QVector<ConstraintPtr> Constraints::constraints() const
{
    return cons + molcons;
}
  
/** Return the list of all of the constraints that affect
    just the components and properties of the system */
QVector<ConstraintPtr> Constraints::componentConstraints() const
{
    return cons;
}

/** Return the list of all of the constraints that affect
    just the coordinates or properties of the molecules 
    of the system */
QVector<ConstraintPtr> Constraints::moleculeConstraints() const
{
    return molcons;
}

/** Add the passed constraint to this set - this is only added
    if it does not exist in this set */
void Constraints::add(const Constraint &constraint)
{
    if (constraint.isA<MoleculeConstraint>())
    {
        molcons.append(constraint);
        molcons.squeeze();
    }
    else
    {
        foreach (const ConstraintPtr con, cons)
        {
            if (con->equals(constraint))
                return;
        }
        
        cons.append(constraint);
        cons.squeeze();
    }
}

/** Add all of the passed constraints to this set. This only
    adds the constraints that are not already part of this set */
void Constraints::add(const Constraints &constraints)
{
    for (QVector<ConstraintPtr>::const_iterator it = constraints.cons.constBegin();
         it != constraints.cons.constEnd();
         ++it)
    {
        this->add(it->read());
    }

    for (QVector<ConstraintPtr>::const_iterator it = constraints.molcons.constBegin();
         it != constraints.molcons.constEnd();
         ++it)
    {
        this->add(it->read());
    }
}

/** Remove the constraint 'constraint' from this set - this
    does nothing if this constraint is not part of this set */
void Constraints::remove(const Constraint &constraint)
{
    if (constraint.isA<MoleculeConstraint>())
    {
        QMutableVectorIterator<ConstraintPtr> it(molcons);
    
        while (it.hasNext())
        {
            it.next();
        
            if (it.value()->equals(constraint))
                it.remove();
        }
        
        molcons.squeeze();
    }
    else
    {
        QMutableVectorIterator<ConstraintPtr> it(cons);
    
        while (it.hasNext())
        {
            it.next();
        
            if (it.value()->equals(constraint))
                it.remove();
        }

        cons.squeeze();
    }
}

/** Remove all of the constraints in 'constraints' from this
    set - this ignores constraints that are not in this set */
void Constraints::remove(const Constraints &constraints)
{
    for (QVector<ConstraintPtr>::const_iterator it = constraints.cons.constBegin();
         it != constraints.cons.constEnd();
         ++it)
    {
        this->remove( it->read() );
    }

    for (QVector<ConstraintPtr>::const_iterator it = constraints.molcons.constBegin();
         it != constraints.molcons.constEnd();
         ++it)
    {
        this->remove( it->read() );
    }
}

/** Remove the constraint at index 'i' */
void Constraints::removeAt(int i)
{
    i = Index(i).map( this->nConstraints() );
    
    if (i >= cons.count())
    {
        molcons.remove( i - cons.count() );
        molcons.squeeze();
    }
    else
    {
        cons.remove(i);
        cons.squeeze();
    }
}

/** Return whether or not all of the constraints in this set are
    satisfied in the passed system */
bool Constraints::areSatisfied(const System &system) const
{
    for (QVector<ConstraintPtr>::const_iterator it = cons.constBegin();
         it != cons.constEnd();
         ++it)
    {
        if (not it->read().isSatisfied(system))
            return false;
    }

    for (QVector<ConstraintPtr>::const_iterator it = molcons.constBegin();
         it != molcons.constEnd();
         ++it)
    {
        if (not it->read().isSatisfied(system))
            return false;
    }
    
    return true;
}

/** Assert that all of the constraints in this set are satisfied
    in the passed system
    
    \throw SireSystem::constraint_error
*/
void Constraints::assertSatisfied(const System &system) const
{
    QStringList broken_constraints;
    
    for (QVector<ConstraintPtr>::const_iterator it = cons.constBegin();
         it != cons.constEnd();
         ++it)
    {
        if (not it->read().isSatisfied(system))
        {
            broken_constraints.append( QObject::tr("%1 : %2")
                        .arg(broken_constraints.count() + 1)
                        .arg(it->read().toString()) );
        }
    }
    
    for (QVector<ConstraintPtr>::const_iterator it = molcons.constBegin();
         it != molcons.constEnd();
         ++it)
    {
        if (not it->read().isSatisfied(system))
        {
            broken_constraints.append( QObject::tr("%1 : %2")
                        .arg(broken_constraints.count() + 1)
                        .arg(it->read().toString()) );
        }
    }
    
    if (not broken_constraints.isEmpty())
    {
        throw SireSystem::constraint_error( QObject::tr(
            "Some of the constraints are not satisfied in the system %1. "
            "The number of unsatisfied constraints in %2. Here they are;\n%3")
                .arg(system.name())
                .arg(broken_constraints.count())
                .arg(broken_constraints.join("\n")), CODELOC );
    }
}

/** Return whether or not all of the molecule constraints in this set are
    satisfied in the passed system */
bool Constraints::moleculeConstraintsAreSatisfied(const System &system) const
{
    for (QVector<ConstraintPtr>::const_iterator it = molcons.constBegin();
         it != molcons.constEnd();
         ++it)
    {
        if (not it->read().isSatisfied(system))
            return false;
    }
    
    return true;
}

/** Assert that all of the molecule constraints in this set are satisfied
    in the passed system
    
    \throw SireSystem::constraint_error
*/
void Constraints::assertMoleculeConstraintsAreSatisfied(const System &system) const
{
    QStringList broken_constraints;

    for (QVector<ConstraintPtr>::const_iterator it = molcons.constBegin();
         it != molcons.constEnd();
         ++it)
    {
        if (not it->read().isSatisfied(system))
        {
            broken_constraints.append( QObject::tr("%1 : %2")
                        .arg(broken_constraints.count() + 1)
                        .arg(it->read().toString()) );
        }
    }
    
    if (not broken_constraints.isEmpty())
    {
        throw SireSystem::constraint_error( QObject::tr(
            "Some of the molecule constraints are not satisfied in the system %1. "
            "The number of unsatisfied constraints in %2. Here they are;\n%3")
                .arg(system.name())
                .arg(broken_constraints.count())
                .arg(broken_constraints.join("\n")), CODELOC );
    }
}

/** Internal function used to resolve clashes between molecule constraints */
void Constraints::resolveMoleculeConstraints(System &system,
                                             Molecules old_changed_mols)
{
    const int max_loops = 10;
    
    for (int i=0; i<max_loops; ++i)
    {
        Molecules changed_mols;

        if (old_changed_mols.isEmpty())
            return;
            
        else if (old_changed_mols.count() == 1)
        {
            MolNum changed_molnum = old_changed_mols.constBegin()->data().number();
            
            for (QVector<ConstraintPtr>::iterator it = molcons.begin();
                 it != molcons.end();
                 ++it)
            {
                changed_mols += it->edit()
                                    .asA<MoleculeConstraint>().update(system, 
                                                                      changed_molnum);
            }
        }
        else
        {
            for (QVector<ConstraintPtr>::iterator it = molcons.begin();
                 it != molcons.end();
                 ++it)
            {
                changed_mols += it->edit()
                                    .asA<MoleculeConstraint>().update(system, 
                                                                      old_changed_mols);
            }
        }
        
        if (changed_mols.isEmpty())
            //nothing needs changing - all constraints must be satisfied
            return;
    
        //update the system with the new molecules
        system.update(changed_mols);
        
        //save the changed molecules as a hint for the next pass
        old_changed_mols = changed_mols;
    }
    
    //we only get here if the constraints are still not satisfied
    QStringList broken_constraints;

    for (QVector<ConstraintPtr>::const_iterator it = molcons.constBegin();
         it != molcons.constEnd();
         ++it)
    {
        if (not it->read().isSatisfied(system))
        {
            broken_constraints.append( QObject::tr("%1 : %2")
                        .arg(broken_constraints.count() + 1)
                        .arg(it->read().toString()) );
        }
    }

    if (not broken_constraints.isEmpty())
        throw SireSystem::constraint_error( QObject::tr(
            "Some of the molecule constraints are still not satisfied, despite "
            "%1 passes, in the system %2. "
            "The number of unsatisfied constraints in %3. Here they are;\n%4")
                .arg(max_loops)
                .arg(system.name())
                .arg(broken_constraints.count())
                .arg(broken_constraints.join("\n")), CODELOC );
}

/** Apply all of the molecule constraints to the system 'system' */
void Constraints::applyMoleculeConstraints(System &system)
{
    if (molcons.count() == 0)
        return;

    SaveState old_system = SaveState::save(system);
    QVector<ConstraintPtr> old_molcons( molcons );
    
    try
    {
        //perform a first pass through the constraints
        Molecules changed_mols;
    
        for (QVector<ConstraintPtr>::iterator it = molcons.begin();
             it != molcons.end();
             ++it)
        {
            changed_mols += it->edit()
                                .asA<MoleculeConstraint>().update(system);
        }
        
        if (not changed_mols.isEmpty())
        {
            system.update(changed_mols);
            this->resolveMoleculeConstraints(system, changed_mols);
        }
    }
    catch(...)
    {
        molcons = old_molcons;
        old_system.restore(system);
        throw;
    }
}

/** Apply all of the molecule constraints to the system 'system',
    providing the hint that only the molecule with number 'molnum'
    has changed since the last application. It is a very bad idea
    to provide a hint that is not true! */
void Constraints::applyMoleculeConstraints(System &system, MolNum molnum)
{
    if (molcons.count() == 0)
        return;

    SaveState old_system = SaveState::save(system);
    QVector<ConstraintPtr> old_molcons( molcons );
    
    try
    {
        //perform a first pass through the constraints
        Molecules changed_mols;
    
        for (QVector<ConstraintPtr>::iterator it = molcons.begin();
             it != molcons.end();
             ++it)
        {
            changed_mols += it->edit()
                                .asA<MoleculeConstraint>().update(system, molnum);
        }
        
        if (not changed_mols.isEmpty())
        {
            system.update(changed_mols);
            this->resolveMoleculeConstraints(system, changed_mols);
        }
    }
    catch(...)
    {
        molcons = old_molcons;
        old_system.restore(system);
        throw;
    }
}

/** Apply all of the molecule constraints to the system 'system',
    providing the hint that only the molecules in 'molecules'
    have changed since the last application. It is a very bad idea
    to provide a hint that is not true! */
void Constraints::applyMoleculeConstraints(System &system, const Molecules &molecules)
{
    if (molcons.count() == 0)
        return;

    SaveState old_system = SaveState::save(system);
    QVector<ConstraintPtr> old_molcons( molcons );
    
    try
    {
        //perform a first pass through the constraints
        Molecules changed_mols;
    
        for (QVector<ConstraintPtr>::iterator it = molcons.begin();
             it != molcons.end();
             ++it)
        {
            changed_mols += it->edit()
                                .asA<MoleculeConstraint>().update(system, molecules);
        }
        
        if (not changed_mols.isEmpty())
        {
            system.update(changed_mols);
            this->resolveMoleculeConstraints(system, changed_mols);
        }
    }
    catch(...)
    {
        molcons = old_molcons;
        old_system.restore(system);
        throw;
    }
}

/** Apply all of the contained constraints to the system 'system' */
void Constraints::apply(System &system)
{
    if (cons.count() == 0)
    {
        this->applyMoleculeConstraints(system);
        return;
    }

    SaveState old_system = SaveState::save(system);
    
    try
    {
        //first apply all of the component constraints - we take multiple
        //sweeps as changing one component may then change the value of 
        //another constraint
        const int max_loops = 10;
        
        for (int i=0; i<max_loops; ++i)
        {
            bool something_changed = false;

            for (QVector<ConstraintPtr>::const_iterator it = cons.constBegin();
                 it != cons.constEnd();
                 ++it)
            {
                bool this_changed = it->read().apply(system);
                something_changed = something_changed or this_changed;
            }
            
            if (not something_changed)
            {
                //the components are ok - we can now apply
                //the molecule constraints
                this->applyMoleculeConstraints(system);
                return;
            }
        }
        
        //we only get here if we have exceeded max_loops attempts
        // - get a list of the broken constraints
        QStringList broken_constraints;
    
        for (QVector<ConstraintPtr>::const_iterator it = cons.constBegin();
             it != cons.constEnd();
             ++it)
        {
            if (not it->read().isSatisfied(system))
            {
                broken_constraints.append( QObject::tr("%1 : %2")
                                  .arg(broken_constraints.count() + 1)
                                  .arg(it->read().toString()) );
            }
        }
                
        if (not broken_constraints.isEmpty())
            throw SireSystem::constraint_error( QObject::tr(
                        "Despite %1 attempts, some of the constraints in the system "
                        "cannot be satisfied:\n%2")
                            .arg(max_loops)
                            .arg(broken_constraints.join("\n")), CODELOC );

		//we got here, but there were no broken constraints - we must
        //now apply the molecule constraints
        this->applyMoleculeConstraints(system);
    }
    catch(...)
    {
        old_system.restore(system);
        throw;
    }
}

const char* Constraints::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Constraints>() );
}
