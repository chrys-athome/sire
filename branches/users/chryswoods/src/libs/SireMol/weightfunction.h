/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOL_WEIGHTFUNCTION_H
#define SIREMOL_WEIGHTFUNCTION_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class WeightFunction;

/** This is the base class of all weight functions. A weight function
    is a simple function that takes two groups in a molecule, and
    returns the relative weight of those two groups (0 == 100% group A,
    0.5 == 50% group A, 50% group B, 1 == 100% groupB)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT WeightFuncBase : public SireBase::PropertyBase
{
public:
    WeightFuncBase();
    
    WeightFuncBase(const WeightFuncBase &other);
    
    virtual ~WeightFuncBase();
    
    static const char* typeName()
    {
        return "SireMol::WeightFuncBase";
    }
    
    virtual WeightFuncBase* clone() const=0;
    
    /** Return the relative weight of group0 and group1 in the 
        molecule whose data is in 'moldata', using the supplied
        map to find the required properties
        
        \throw SireError::incompatible_error
        \throw SireBase::missing_property
    */
    virtual double operator()(const MoleculeData &moldata,
                              const AtomSelection &group0,
                              const AtomSelection &group1,
                              const PropertyMap &map = PropertyMap()) const=0;

    /** Return the relative weight of two molecule views (view0
        and view1), using map0 to find the required properties
        from view0, and map1 to find the required properties from
        view1.
        
        \throw SireBase::missing_property
    */
    virtual double operator()(const MoleculeView &view0, 
                              const PropertyMap &map0,
                              const MoleculeView &view1,
                              const PropertyMap &map1) const=0;

    /** Return the relative weight of the two molecule views
        (view0 and view1) using the supplied map to find the 
        required properties from both views
        
        \throw SireBase::missing_property
    */
    double operator()(const MoleculeView &view0,
                      const MoleculeView &view1,
                      const PropertyMap &map = PropertyMap()) const
    {
        return this->operator()(view0, map, view1, map);
    }
};

/** This is the wrapper class that holds a generic WeightFuncBase
    Property
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT WeightFunction
{

friend QDataStream& ::operator<<(QDataStream&, const WeightFunction&);
friend QDataStream& ::operator>>(QDataStream&, WeightFunction&);

public:
    WeightFunction();
    WeightFunction(const WeightFuncBase &weightfunc);
    
    WeightFunction(const WeightFunction &other);
    
    ~WeightFunction();

    WeightFunction& operator=(const WeightFuncBase &weightfunc);
    WeightFunction& operator=(const WeightFunction &weightfunc);
    
    WeightFunction& operator=(const Property &other);

    bool operator==(const WeightFunction &other) const;
    bool operator!=(const WeightFunction &other) const;

    bool operator==(const Property &other) const;
    bool operator!=(const Property &other) const;

    const WeightFuncBase& base() const
    {
        return *d;
    }

    const char* what() const
    {
        return d->what();
    }

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

private:
    /** Shared pointer to the actual property */
    SireBase::SharedPolyPointer<WeightFuncBase> d;
};

/** This class calculates the weight by assigning all of the weight to
    the group with the largest number of atoms.

    @author Christopher Woods
*/
class SIREMOL_EXPORT AbsFromNumber 
            : public SireBase::ConcreteProperty<AbsFromNumber,WeightFuncBase>
{
public:
    AbsFromNumber();
    AbsFromNumber(const AbsFromNumber &other);

    ~AbsFromNumber();

    static const char* typeName()
    {
        return "SireMol::AbsFromNumber";
    }

    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;
    
    AbsFromNumber& operator=(const AbsFromNumber&)
    {
        return *this;
    }
    
    bool operator==(const AbsFromNumber&) const
    {
        return true;
    }
    
    bool operator!=(const AbsFromNumber&) const
    {
        return false;
    }
    
    AbsFromNumber* clone() const
    {
        return new AbsFromNumber(*this);
    }
    
    double operator()(const MoleculeData &moldata,
                      const AtomSelection &group0,
                      const AtomSelection &group1,
                      const PropertyMap &map = PropertyMap()) const;

    double operator()(const MoleculeView &view0, 
                      const PropertyMap &map0,
                      const MoleculeView &view1,
                      const PropertyMap &map1) const;

private:
    double weight(int nats0, int nats1) const;
};

/** This class calculates the weight by assigning the weight based on the
    ratio of the number of atoms in the two groups.

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResFromNumber 
            : public SireBase::ConcreteProperty<ResFromNumber,WeightFuncBase>
{
public:
    ResFromNumber();
    ResFromNumber(const ResFromNumber &other);

    ~ResFromNumber();

    static const char* typeName()
    {
        return "SireMol::ResFromNumber";
    }

    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;
    
    ResFromNumber& operator=(const ResFromNumber&)
    {
        return *this;
    }
    
    bool operator==(const ResFromNumber&) const
    {
        return true;
    }
    
    bool operator!=(const ResFromNumber&) const
    {
        return false;
    }
    
    ResFromNumber* clone() const
    {
        return new ResFromNumber(*this);
    }
    
    double operator()(const MoleculeData &moldata,
                      const AtomSelection &group0,
                      const AtomSelection &group1,
                      const PropertyMap &map = PropertyMap()) const;

    double operator()(const MoleculeView &view0, 
                      const PropertyMap &map0,
                      const MoleculeView &view1,
                      const PropertyMap &map1) const;

private:
    double weight(int nats0, int nats1) const;
};

/** This class calculates the weight by assigning all of the weight to
    the group with the most mass

    @author Christopher Woods
*/
class SIREMOL_EXPORT AbsFromMass 
            : public SireBase::ConcreteProperty<AbsFromMass,WeightFuncBase>
{
public:
    AbsFromMass();
    AbsFromMass(const AbsFromMass &other);

    ~AbsFromMass();

    static const char* typeName()
    {
        return "SireMol::AbsFromMass";
    }

    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;
    
    AbsFromMass& operator=(const AbsFromMass&)
    {
        return *this
    }
    
    bool operator==(const AbsFromMass&) const
    {
        return true;
    }
    
    bool operator!=(const AbsFromMass&) const
    {
        return false;
    }
    
    AbsFromMass* clone() const
    {
        return new AbsFromMass(*this);
    }
    
    double operator()(const MoleculeData &moldata,
                      const AtomSelection &group0,
                      const AtomSelection &group1,
                      const PropertyMap &map = PropertyMap()) const;

    double operator()(const MoleculeView &view0, 
                      const PropertyMap &map0,
                      const MoleculeView &view1,
                      const PropertyMap &map1) const;

private:
    double weight(double mass0, double mass1) const;
};

/** This class calculates the weight by assigning the weight based on the
    ratio of the masses of the two groups.

    @author Christopher Woods
*/
class SIREMOL_EXPORT RelFromMass 
            : public SireBase::ConcreteProperty<RelFromMass,WeightFuncBase>
{
public:
    RelFromMass();
    RelFromMass(const RelFromMass &other);

    ~RelFromMass();

    static const char* typeName()
    {
        return "SireMol::RelFromMass";
    }

    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;
    
    RelFromMass& operator=(const RelFromMass&)
    {
        return *this;
    }
    
    bool operator==(const RelFromMass&) const
    {
        return true;
    }
    
    bool operator!=(const RelFromMass&) const
    {
        return false;
    }
    
    RelFromMass* clone() const
    {
        return new RelFromMass(*this);
    }
    
    double operator()(const MoleculeData &moldata,
                      const AtomSelection &group0,
                      const AtomSelection &group1,
                      const PropertyMap &map = PropertyMap()) const;

    double operator()(const MoleculeView &view0, 
                      const PropertyMap &map0,
                      const MoleculeView &view1,
                      const PropertyMap &map1) const;

private:
    double weight(double mass0, double mass1) const;
};

}

SIRE_END_HEADER

#endif
