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

#ifndef SIREMM_POSITIONALRESTRAINT_H
#define SIREMM_POSITIONALRESTRAINT_H

#include "restraint.h"

#include "SireMol/atomselection.h"
#include "SireMol/cgatomidx.h"

#include "SireCAS/expression.h"

#include "SireMaths/vector.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class PositionalRestraint;
class AtomicRestraint;
class CenterOfGeometryRestraint;
class CenterOfMassRestraint;
}

QDataStream& operator<<(QDataStream&, const SireMM::PositionalRestraint&);
QDataStream& operator>>(QDataStream&, SireMM::PositionalRestraint&);

QDataStream& operator<<(QDataStream&, const SireMM::AtomicRestraint&);
QDataStream& operator>>(QDataStream&, SireMM::AtomicRestraint&);

QDataStream& operator<<(QDataStream&, const SireMM::CenterOfGeometryRestraint&);
QDataStream& operator>>(QDataStream&, SireMM::CenterOfGeometryRestraint&);

QDataStream& operator<<(QDataStream&, const SireMM::CenterOfMassRestraint&);
QDataStream& operator>>(QDataStream&, SireMM::CenterOfMassRestraint&);

namespace SireCAS
{
class Symbol;
}

namespace SireMM
{

using SireMol::AtomSelection;
using SireMol::Atom;
using SireMol::MoleculeView;
using SireMol::CGAtomIdx;

using SireCAS::Expression;
using SireCAS::Symbol;

using SireMaths::Vector;

/** This is the base class of all simple 
    positional restraints, that restrains a molecule
    (or part of a molecule) to a point in 3D space.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT PositionalRestraint : public Restraint3D
{

friend QDataStream& ::operator<<(QDataStream&, const PositionalRestraint&);
friend QDataStream& ::operator>>(QDataStream&, PositionalRestraint&);

public:
    PositionalRestraint();
    
    PositionalRestraint(const PositionalRestraint &other);
    
    ~PositionalRestraint();
    
    static const char* typeName()
    {
        return "SireMM::PositionalRestraint";
    }
    
    static const Symbol& r();

    const Expression& restraintFunction() const;
    const Expression& differentialRestraintFunction() const;

    const Vector& restraintPoint() const;
    const Vector& moleculePoint() const;
    
    SireUnits::Dimension::MolarEnergy energy() const;

protected:
    PositionalRestraint(const Vector &restraint_point,
                        const Expression &restraint_expression);

    PositionalRestraint(const Vector &restraint_point,
                        const Expression &restraint_expression,
                        const Expression &differential_restraint_expression);

    PositionalRestraint& operator=(const PositionalRestraint &other);
    
    bool operator==(const PositionalRestraint &other) const;
    bool operator!=(const PositionalRestraint &other) const;

    void update(const Vector &new_molecule_point);

    Vector restraintForce() const;

private:
    /** The location of the restraint */
    Vector restraint_point;
    
    /** The current location of the anchor for the molecule */
    Vector molecule_point;
    
    /** The energy expression for the restraint */
    Expression restraint_function;
    
    /** The differential of the restraint function as a function 
        of distance */
    Expression diff_restraint_function;
};

// typedef the unit of a harmonic force constant ( MolarEnergy / Length^2 )
typedef SireUnits::Dimension::PhysUnit<1,0,-2,0,0,-1,0> HarmonicPositionForceConstant;

/** This is a restraint that restrains an atom to a point in 3D space */
class SIREMM_EXPORT AtomicRestraint
        : public SireBase::ConcreteProperty<AtomicRestraint,PositionalRestraint>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicRestraint&);
friend QDataStream& ::operator>>(QDataStream&, AtomicRestraint&);

public:
    AtomicRestraint();
    
    AtomicRestraint(const Atom &atom, const Vector &restraint_point,
                    const Expression &restraint_expression,
                    const PropertyMap &map = PropertyMap());
                    
    AtomicRestraint(const AtomicRestraint &other);
    
    ~AtomicRestraint();
    
    static const char* typeName();
    
    AtomicRestraint& operator=(const AtomicRestraint &other);
    
    bool operator==(const AtomicRestraint &other) const;
    bool operator!=(const AtomicRestraint &other) const;
    
    static AtomicRestraint 
            harmonic(const Atom &atom, 
                     const Vector &restraint_point,
                     const HarmonicPositionForceConstant &force_constant,
                     const PropertyMap &map = PropertyMap());

    static AtomicRestraint
            halfHarmonic(const Atom &atom,
                         const Vector &restraint_point,
                         const SireUnits::Dimension::Length &distance,
                         const HarmonicPositionForceConstant &force_constant,
                         const PropertyMap &map = PropertyMap());

    void update(const MoleculeData &moldata, const PropertyMap &map);
    
    void force(MolForceTable &forcetable, double scale_force=1) const;

protected:
    AtomicRestraint(const Atom &atom, const Vector &restraint_point,
                    const Expression &restraint_expression,
                    const Expression &diff_restraint_expression,
                    const PropertyMap &map = PropertyMap());

private:
    /** The index of the atom in the molecule to restrain */
    CGAtomIdx cgatomidx;
};

/** This is a retraint that restrains a molecule (or part of a molecule)
    to a point in 3D space, restraining the geometric center of the 
    molecule to that point
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CenterOfGeometryRestraint
     : public SireBase::ConcreteProperty<CenterOfGeometryRestraint,PositionalRestraint>
{

friend QDataStream& ::operator<<(QDataStream&, const CenterOfGeometryRestraint&);
friend QDataStream& ::operator>>(QDataStream&, CenterOfGeometryRestraint&);

public:
    CenterOfGeometryRestraint();
    
    CenterOfGeometryRestraint(const MoleculeView &molview,
                              const Vector &restraint_point,
                              const Expression &restraint_expression,
                              const PropertyMap &map = PropertyMap());
                              
    CenterOfGeometryRestraint(const CenterOfGeometryRestraint &other);
    
    ~CenterOfGeometryRestraint();
    
    static const char* typeName();
    
    CenterOfGeometryRestraint& operator=(const CenterOfGeometryRestraint &other);
    
    bool operator==(const CenterOfGeometryRestraint &other) const;
    bool operator!=(const CenterOfGeometryRestraint &other) const;
    
    static CenterOfGeometryRestraint
            harmonic(const MoleculeView &molview,
                     const Vector &restraint_point,
                     const HarmonicPositionForceConstant &force_constant,
                     const PropertyMap &map = PropertyMap());

    static CenterOfGeometryRestraint
            halfHarmonic(const MoleculeView &molview,
                         const Vector &restraint_point,
                         const SireUnits::Dimension::Length &distance,
                         const HarmonicPositionForceConstant &force_constant,
                         const PropertyMap &map = PropertyMap());

    void update(const MoleculeData &moldata, const PropertyMap &map);

    void force(MolForceTable &forcetable, double scale_force=1) const;

protected:
    CenterOfGeometryRestraint(const MoleculeView &molview,
                              const Vector &restraint_point,
                              const Expression &restraint_expression,
                              const Expression &diff_restraint_expression,
                              const PropertyMap &map = PropertyMap());

private:
    /** The atoms that are affected by this restraint */
    AtomSelection selected_atoms;
};

/** This is a retraint that restrains a molecule (or part of a molecule)
    to a point in 3D space, restraining the center of mass of the 
    molecule to that point
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CenterOfMassRestraint
       : public SireBase::ConcreteProperty<CenterOfMassRestraint,PositionalRestraint>
{

friend QDataStream& ::operator<<(QDataStream&, const CenterOfMassRestraint&);
friend QDataStream& ::operator>>(QDataStream&, CenterOfMassRestraint&);

public:
    CenterOfMassRestraint();
    
    CenterOfMassRestraint(const MoleculeView &molview,
                          const Vector &restraint_point,
                          const Expression &restraint_expression,
                          const PropertyMap &map = PropertyMap());
                          
    CenterOfMassRestraint(const CenterOfMassRestraint &other);
    
    ~CenterOfMassRestraint();
    
    static const char* typeName();
    
    CenterOfMassRestraint& operator=(const CenterOfMassRestraint &other);
    
    bool operator==(const CenterOfMassRestraint &other) const;
    bool operator!=(const CenterOfMassRestraint &other) const;
    
    static CenterOfMassRestraint
                harmonic(const MoleculeView &molview,
                         const Vector &restraint_point,
                         const HarmonicPositionForceConstant &force_constant,
                         const PropertyMap &map = PropertyMap());
                         
    static CenterOfMassRestraint
                halfHarmonic(const MoleculeView &molview,
                             const Vector &restraint_point,
                             const SireUnits::Dimension::Length &distance,
                             const HarmonicPositionForceConstant &force_constant,
                             const PropertyMap &map = PropertyMap());

    void update(const MoleculeData &moldata, const PropertyMap &map);
                             
    void force(MolForceTable &forcetable, double scale_force=1) const;

protected:
    CenterOfMassRestraint(const MoleculeView &molview,
                          const Vector &restraint_point,
                          const Expression &restraint_expression,
                          const Expression &diff_restraint_expression,
                          const PropertyMap &map = PropertyMap());

private:
    /** The atoms affected by this restraint */
    AtomSelection selected_atoms;
};

}

Q_DECLARE_METATYPE( SireMM::AtomicRestraint )
Q_DECLARE_METATYPE( SireMM::CenterOfGeometryRestraint )
Q_DECLARE_METATYPE( SireMM::CenterOfMassRestraint )

SIRE_EXPOSE_CLASS( SireMM::PositionalRestraint )
SIRE_EXPOSE_CLASS( SireMM::AtomicRestraint )
SIRE_EXPOSE_CLASS( SireMM::CenterOfMassRestraint )
SIRE_EXPOSE_CLASS( SireMM::CenterOfGeometryRestraint )

SIRE_END_HEADER

#endif
