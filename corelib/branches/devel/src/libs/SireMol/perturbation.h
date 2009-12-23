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

#ifndef SIREMOL_PERTURBATION_H
#define SIREMOL_PERTURBATION_H

#include <QList>

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireCAS/expression.h"
#include "SireCAS/symbol.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Perturbation;
class NullPerturbation;

class Perturbations;
}

QDataStream& operator<<(QDataStream&, const SireMol::Perturbation&);
QDataStream& operator>>(QDataStream&, SireMol::Perturbation&);

QDataStream& operator<<(QDataStream&, const SireMol::NullPerturbation&);
QDataStream& operator>>(QDataStream&, SireMol::NullPerturbation&);

QDataStream& operator<<(QDataStream&, const SireMol::Perturbations&);
QDataStream& operator>>(QDataStream&, SireMol::Perturbations&);

namespace SireCAS
{
class Values;
}

namespace SireMol
{

class Molecule;
class MolEditor;

using SireBase::PropertyMap;

using SireCAS::Expression;
using SireCAS::Symbol;
using SireCAS::Values;

class SIREMOL_EXPORT PerturbationSymbols
{
public:
    PerturbationSymbols();
    ~PerturbationSymbols();

    const Symbol& lambda() const;
    const Symbol& initial() const;
    const Symbol& final() const;

private:
    Symbol lam, init, finl;
};

/** This is the base class of all perturbation objects. A Perturbation
    is a rule for changing a property of a molecule with respect
    to a driving (reaction) coordinate. Perturbations can be used
    to implement single topology free energy calculations
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT Perturbation : public SireBase::Property
{

friend class Perturbations;   // so can call perturbMolecule directly

friend QDataStream& ::operator<<(QDataStream&, const Perturbation&);
friend QDataStream& ::operator>>(QDataStream&, Perturbation&);

public:
    Perturbation();
    Perturbation(const Perturbation &other);
    
    virtual ~Perturbation();
    
    static const char* typeName();
    
    virtual Perturbation* clone() const=0;
    
    static const Expression& defaultFunction();
    static const PerturbationSymbols& symbols();
    
    const Expression& mappingFunction() const;
    
    const PropertyMap& propertyMap() const;
    
    Molecule perturb(const Molecule &molecule, const Values &values) const;

    static const NullPerturbation& null();

protected:
    Perturbation(const PropertyMap &map);
    Perturbation(const Expression &equation, const PropertyMap &map = PropertyMap());

    Perturbation& operator=(const Perturbation &other);
    
    virtual void perturbMolecule(MolEditor &molecule, const Values &values) const=0;
    
    bool operator==(const Perturbation &other) const;
    bool operator!=(const Perturbation &other) const;

private:
    /** The equation used to control the perturbation */
    Expression mapping_eqn;
    
    /** The property map used to find the properties 
        used in this perturbation */
    PropertyMap map;
};

/** This is a null perturbation that does nothing */
class SIREMOL_EXPORT NullPerturbation
        : public SireBase::ConcreteProperty<NullPerturbation,Perturbation>
{
friend QDataStream& ::operator<<(QDataStream&, const NullPerturbation&);
friend QDataStream& ::operator>>(QDataStream&, NullPerturbation&);

public:
    NullPerturbation();
    NullPerturbation(const NullPerturbation &other);
    
    ~NullPerturbation();
    
    static const char* typeName();
    
    NullPerturbation& operator=(const NullPerturbation &other);
    
    bool operator==(const NullPerturbation &other) const;
    bool operator!=(const NullPerturbation &other) const;
    
protected:
    void perturbMolecule(MolEditor &molecule, const Values &values) const;
};

typedef SireBase::PropPtr<Perturbation> PerturbationPtr;

/** This class holds a collection of perturbations that can
    be applied to a molecule
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT Perturbations 
        : public SireBase::ConcreteProperty<Perturbations,Perturbation>
{

friend QDataStream& ::operator<<(QDataStream&, const Perturbations&);
friend QDataStream& ::operator>>(QDataStream&, Perturbations&);

public:
    Perturbations();
    Perturbations(const Perturbation &perturbation);
    Perturbations(const QList<PerturbationPtr> &perturbations);

    Perturbations(const Perturbation &perturbation, const Expression &equation);
    Perturbations(const QList<PerturbationPtr> &perturbations, const Expression &equation);
    
    Perturbations(const Perturbations &other);
    
    ~Perturbations();
    
    Perturbations& operator=(const Perturbations &other);
    
    bool operator==(const Perturbations &other) const;
    bool operator!=(const Perturbations &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    QList<PerturbationPtr> perturbations() const;

protected:    
    void perturbMolecule(MolEditor &molecule, const Values &values) const;

private:
    void makeSane();
    
    /** All of the perturbations used in this object */
    QList<PerturbationPtr> perts;
};

}

Q_DECLARE_METATYPE( SireMol::NullPerturbation )
Q_DECLARE_METATYPE( SireMol::Perturbations )

SIRE_EXPOSE_CLASS( SireMol::Perturbation )
SIRE_EXPOSE_CLASS( SireMol::NullPerturbation )
SIRE_EXPOSE_CLASS( SireMol::Perturbations )
SIRE_EXPOSE_CLASS( SireMol::PerturbationSymbols )

SIRE_EXPOSE_PROPERTY( SireMol::PerturbationPtr, SireMol::Perturbation )

SIRE_END_HEADER

#endif
