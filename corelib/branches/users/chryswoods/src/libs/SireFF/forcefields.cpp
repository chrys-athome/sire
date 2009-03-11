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

#include <QSet>

#include "forcefields.h"

#include "ffidx.h"
#include "ffname.h"
#include "ff3d.h"

#include "SireMol/mgnum.h"
#include "SireMol/mgidx.h"

#include "SireMol/moleculeview.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/viewsofmol.h"
#include "SireMol/molecules.h"
#include "SireMol/moleculegroup.h"

#include "tostring.h"

#include "SireMol/errors.h"
#include "SireFF/errors.h"
#include "SireBase/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <boost/shared_ptr.hpp>

#include <QDebug>
#include <QTime>

using namespace SireFF;
using namespace SireMol;
using namespace SireBase;
using namespace SireCAS;
using namespace SireStream;

using SireUnits::Dimension::MolarEnergy;

namespace SireFF
{
namespace detail
{

/** This is a private hierarchy of classes that is used just by ForceFields
    to relate a symbol to an energy component, forcefield expression or
    constant */
class FFSymbol
{
public:
    FFSymbol();
    FFSymbol(const Symbol &symbol);
    
    FFSymbol(const FFSymbol &other);
    
    virtual ~FFSymbol();
    
    virtual const char* what() const=0;
    
    virtual void load(QDataStream &ds);
    virtual void save(QDataStream &ds) const;
    
    virtual Expression toExpression() const=0;
    
    virtual double value() const=0;
    
    virtual MolarEnergy energy(QVector<FFPtr> &forcefields,
                          const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                          double scale_energy=1) const=0;
                          
    virtual void force(ForceTable &forcetable,
                       QVector<FFPtr> &forcefields,
                       const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                       double scale_force=1) const=0;

    const Symbol& symbol() const;

    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }

    template<class T>
    T& asA()
    {
        return dynamic_cast<T&>(*this);
    }

private:
    /** The symbol that this object represents */
    Symbol s;
};

/** This is an FFSymbol that holds just a single value */
class FFSymbolValue : public FFSymbol
{
public:
    FFSymbolValue();
    FFSymbolValue(const Symbol &symbol, double value);
    
    FFSymbolValue(const FFSymbolValue &other);
    
    ~FFSymbolValue();
    
    const char* what() const
    {
        return "SireFF::FFSymbolValue";
    }
    
    void load(QDataStream &ds);
    void save(QDataStream &ds) const;
    
    Expression toExpression() const;
    
    double value() const;
    
    MolarEnergy energy(QVector<FFPtr> &forcefields,
                       const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                       double scale_energy=1) const;
    
    void force(ForceTable &forcetable,
               QVector<FFPtr> &forcefields,
               const QHash<Symbol,FFSymbolPtr> &ffsymbols,
               double scale_force=1) const;
    
private:
    /** The value of this symbol */
    double v;
};

/** This is an FFSymbol that holds a single forcefield component */
class FFSymbolFF : public FFSymbol
{
public:
    FFSymbolFF();
    FFSymbolFF(FFIdx ffidx, const Symbol &component);
    
    FFSymbolFF(const FFSymbolFF &other);
    
    ~FFSymbolFF();
    
    const char* what() const
    {
        return "SireFF::FFSymbolFF";
    }
    
    void load(QDataStream &ds);
    void save(QDataStream &ds) const;

    Expression toExpression() const;
    
    FFIdx ffIdx() const;
    
    double value() const;
    
    MolarEnergy energy(QVector<FFPtr> &forcefields,
                       const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                       double scale_energy=1) const;
                  
    void force(ForceTable &forcetable, QVector<FFPtr> &forcefields,
               const QHash<Symbol,FFSymbolPtr> &ffsymbols,
               double scale_force=1) const;

private:
    /** The index of the forcefield that contains this component */
    FFIdx ffidx;
};

/** This is an FFSymbol that represents a complete forcefield expression */
class FFSymbolExpression : public FFSymbol
{
public:
    FFSymbolExpression();
    FFSymbolExpression(const Symbol &symbol, const Expression &expression);
    
    FFSymbolExpression(const FFSymbolExpression &other);
    
    ~FFSymbolExpression();
    
    const char* what() const
    {
        return "SireFF::FFSymbolExpression";
    }
    
    void load(QDataStream &ds);
    void save(QDataStream &ds) const;

    Expression toExpression() const;
    
    const Expression& expression() const;
    
    void expandInTermsOf(const QSet<Symbol> &ffsymbols);
    
    double value() const;
    
    MolarEnergy energy(QVector<FFPtr> &forcefields,
                       const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                       double scale_energy=1) const;
                  
    void force(ForceTable &forcetable, QVector<FFPtr> &forcefields,
               const QHash<Symbol,FFSymbolPtr> &ffsymbols,
               double scale_force=1) const;

private:
    class Component
    {
    public:
        Component();
        Component(const Expression &scale_factor, const Symbol &component);
        
        Component(const Component &other);
        
        ~Component();
        
        int nDependents() const;
        const QVector<Symbol>& dependents() const;
        
        double scalingFactor(const Values &values) const;
        const Expression& scalingExpression() const;
        
        const Symbol& symbol() const;
        
    private:
        /** The symbol for this component */
        Symbol s;
        
        /** The symbols used in the scaling factor */
        QVector<Symbol> deps;
        
        /** The expression for the scaling factor */
        Expression sclfac;
    };

    /** The forcefield expression */
    Expression ffexpression;
    
    /** All of the components of this expression */
    QVector<Component> components;
};

/** This is an FFSymbol that represents just a simple total of the energy
    of the forcefields */
class FFTotalExpression : public FFSymbol
{
public:
    FFTotalExpression();
    FFTotalExpression(const Symbol &symbol);
    
    FFTotalExpression(const FFTotalExpression &other);
    
    ~FFTotalExpression();
    
    const char* what() const
    {
        return "SireFF::FFTotalExpression";
    }
    
    void load(QDataStream &ds);
    void save(QDataStream &ds) const;
    
    Expression toExpression() const;

    double value() const;
    
    MolarEnergy energy(QVector<FFPtr> &forcefields,
                       const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                       double scale_energy=1) const;
                  
    void force(ForceTable &forcetable, QVector<FFPtr> &forcefields,
               const QHash<Symbol,FFSymbolPtr> &ffsymbols,
               double scale_force=1) const;
};

} // end of namespace detail

} // end of namespace SireFF

using namespace SireFF::detail;

/** Serialise an FFSymbol to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const FFSymbolPtr &ffsymbol)
{
    if (ffsymbol.get() == 0)
    {
        ds << QString::null;
    }
    else
    {
        ds << QString( ffsymbol->what() );
        ffsymbol->save(ds);
    }
    
    return ds;
}

/** Extract an FFSymbol from a binary datastream */
QDataStream& operator>>(QDataStream &ds, FFSymbolPtr &ffsymbol)
{
    QString type_name;
    
    ds >> type_name;
    
    if (type_name.isEmpty())
    {
        ffsymbol.reset();
    }
    else 
    {
        if (type_name == QLatin1String("SireFF::FFSymbolFF"))
        {
            ffsymbol.reset( new FFSymbolFF() );
        }
        else if (type_name == QLatin1String("SireFF::FFSymbolValue"))
        {
            ffsymbol.reset( new FFSymbolValue() );
        }
        else if (type_name == QLatin1String("SireFF::FFSymbolExpression"))
        {
            ffsymbol.reset( new FFSymbolExpression() );
        }
        else if (type_name == QLatin1String("SireFF::FFTotalExpression"))
        {
            ffsymbol.reset( new FFTotalExpression() );
        }
        else
        {
            throw SireError::program_bug( QObject::tr(
                "Internal error with ForceFields - it can't recognise the "
                "FFSymbol type %1.").arg(type_name), CODELOC );
        }
        
        ffsymbol->load(ds);
    }
    
    return ds;
}

///////////
/////////// Implementation of FFSymbol
///////////

FFSymbol::FFSymbol()
{}

FFSymbol::FFSymbol(const Symbol &symbol) : s(symbol)
{}

FFSymbol::FFSymbol(const FFSymbol &other) : s(other.s)
{}

FFSymbol::~FFSymbol()
{}

const Symbol& FFSymbol::symbol() const
{
    return s;
}

void FFSymbol::load(QDataStream &ds)
{
    ds >> s;
}

void FFSymbol::save(QDataStream &ds) const
{
    ds << s;
}

///////////
/////////// Implementation of FFSymbolValue
///////////

FFSymbolValue::FFSymbolValue() : FFSymbol(), v(0)
{}

FFSymbolValue::FFSymbolValue(const Symbol &symbol, double value) 
              : FFSymbol(symbol), v(value)
{}

FFSymbolValue::FFSymbolValue(const FFSymbolValue &other)
               : FFSymbol(other), v(other.v)
{}

FFSymbolValue::~FFSymbolValue()
{}

void FFSymbolValue::load(QDataStream &ds)
{
    ds >> v;
    FFSymbol::load(ds);
}

void FFSymbolValue::save(QDataStream &ds) const
{
    ds << v;
    FFSymbol::save(ds);
}

Expression FFSymbolValue::toExpression() const
{
    return Expression(v);
}

double FFSymbolValue::value() const
{
    return v;
}

MolarEnergy FFSymbolValue::energy(QVector<FFPtr> &forcefields,
                                  const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                                  double scale_energy) const
{
    return MolarEnergy(scale_energy * v);
}

void FFSymbolValue::force(ForceTable &forcetable,
                          QVector<FFPtr> &forcefields,
                          const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                          double scale_force) const
{
    return;
}

///////////
/////////// Implementation of FFSymbolFF
///////////

FFSymbolFF::FFSymbolFF() : FFSymbol(), ffidx(0)
{}

FFSymbolFF::FFSymbolFF(FFIdx ffindex, const Symbol &component)
           : FFSymbol(component), ffidx(ffindex)
{}

FFSymbolFF::FFSymbolFF(const FFSymbolFF &other)
           : FFSymbol(other), ffidx(other.ffidx)
{}

FFSymbolFF::~FFSymbolFF()
{}

void FFSymbolFF::load(QDataStream &ds)
{
    ds >> ffidx;
    FFSymbol::load(ds);
}

void FFSymbolFF::save(QDataStream &ds) const
{
    ds << ffidx;
    FFSymbol::save(ds);
}

FFIdx FFSymbolFF::ffIdx() const
{
    return ffidx;
}

Expression FFSymbolFF::toExpression() const
{
    return Expression(this->symbol());
}

double FFSymbolFF::value() const
{
    throw SireError::program_bug( QObject::tr(
        "There is no value associated with a forcefield (%1, %2)")
            .arg(ffidx).arg(this->symbol().toString()), CODELOC );
            
    return 0;
}

MolarEnergy FFSymbolFF::energy(QVector<FFPtr> &forcefields,
                               const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                               double scale_energy) const
{
    if (scale_energy != 0)
        return scale_energy * forcefields[ffidx].edit().energy(this->symbol());
    else
        return MolarEnergy(0);
}

void FFSymbolFF::force(ForceTable &forcetable, QVector<FFPtr> &forcefields,
                       const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                       double scale_force) const
{
    FFPtr &ffield = forcefields[ffidx];
    
    if (not ffield->isA<FF3D>())
        throw SireFF::missing_derivative( QObject::tr(
            "The forcefield of type %1 does not inherit from FF3D so does "
            "not provide a force function.")
                .arg(ffield->what()), CODELOC );

    ffield.edit().asA<FF3D>().force(forcetable, this->symbol(), scale_force);
}

///////////
/////////// Implementation of FFSymbolExpression
///////////

FFSymbolExpression::Component::Component()
{}

FFSymbolExpression::Component::Component(const Expression &scale_factor, 
                                         const Symbol &component)
                   : s(component), sclfac(scale_factor)
{
    Symbols dependents = scale_factor.symbols();
    
    if (not dependents.isEmpty())
    {
        deps = QVector<Symbol>( dependents.count() );
        Symbol *deps_array = deps.data();
        
        int i = 0;
        foreach (const Symbol &dependent, dependents)
        {
            deps_array[i] = dependent;
            ++i;
        }
    }
}

FFSymbolExpression::Component::Component(const Component &other)
                   : s(other.s), deps(other.deps), sclfac(other.sclfac)
{}

FFSymbolExpression::Component::~Component()
{}

int FFSymbolExpression::Component::nDependents() const
{
    return deps.count();
}

const QVector<Symbol>& FFSymbolExpression::Component::dependents() const
{
    return deps;
}

const Expression& FFSymbolExpression::Component::scalingExpression() const
{
    return sclfac;
}

double FFSymbolExpression::Component::scalingFactor(const Values &values) const
{
    return sclfac.evaluate(values);
}

const Symbol& FFSymbolExpression::Component::symbol() const
{
    return s;
}

FFSymbolExpression::FFSymbolExpression() : FFSymbol()
{}

FFSymbolExpression::FFSymbolExpression(const Symbol &symbol, 
                                       const Expression &expression)
                   : FFSymbol(symbol), ffexpression(expression)
{}

FFSymbolExpression::FFSymbolExpression(const FFSymbolExpression &other)
                   : FFSymbol(other), ffexpression(other.ffexpression),
                     components(other.components)
{}

FFSymbolExpression::~FFSymbolExpression()
{}

void FFSymbolExpression::load(QDataStream &ds)
{
    ds >> ffexpression;
    
    components.clear();
    
    FFSymbol::load(ds);
}

void FFSymbolExpression::save(QDataStream &ds) const
{
    ds << ffexpression;
    FFSymbol::save(ds);
}

const Expression& FFSymbolExpression::expression() const
{
    return ffexpression;
}

void FFSymbolExpression::expandInTermsOf(const QSet<Symbol> &ffsymbols)
{
    //get all of the symbols in which to expand this expression
    components.clear();
    
    QSet<Symbol> symbols = ffexpression.symbols();
    symbols.intersect(ffsymbols);
    
    foreach (const Symbol &symbol, symbols)
    {
        QList<Factor> factors = ffexpression.expand(symbol);
        
        foreach (const Factor &factor, factors)
        {
            if (factor.power().isZero())
                continue;
        
            if (factor.power() != Expression(1))
                throw SireError::incompatible_error( QObject::tr(
                    "You cannot raise a forcefield energy component (%1) "
                    "to any power (%2) other than 1 as this is not "
                    "dimensionally correct.")
                        .arg(symbol.toString(), factor.power().toString()),
                            CODELOC );
                
            components.append( Component(factor.factor(), symbol) );
        }
    }
}

Expression FFSymbolExpression::toExpression() const
{
    return ffexpression;
}

double FFSymbolExpression::value() const
{
    throw SireError::incompatible_error( QObject::tr(
        "You cannot multiply one forcefield component (%1) by another in "
        "the forcefield expression %2.")
            .arg(symbol().toString(), ffexpression.toString()), CODELOC );
            
    return 0;
}

MolarEnergy FFSymbolExpression::energy(QVector<FFPtr> &forcefields,
                                       const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                                       double scale_energy) const
{
    //loop over each component of the expression
    MolarEnergy nrg(0);

    if (scale_energy == 0)
        return nrg;
    
    int ncomponents = components.count();
    const Component *components_array = components.constData();
    
    Values values;

    for (int i=0; i<ncomponents; ++i)
    {
        const Component &component = components_array[i];
        
        //evaluate all of the dependent symbols...
        int ndeps = component.nDependents();
        const Symbol *deps_array = component.dependents().constData();
        
        for (int j=0; j<ndeps; ++j)
        {
            const Symbol &symbol = deps_array[j];

            if (not values.contains(symbol))
            {
                values.set( symbol, ffsymbols[symbol]->value() );
            }
        }
        
        nrg += ffsymbols[component.symbol()]->energy(forcefields, ffsymbols,
                                 scale_energy * component.scalingFactor(values));
    }

    return nrg;
}

void FFSymbolExpression::force(ForceTable &forcetable,
                               QVector<FFPtr> &forcefields,
                               const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                               double scale_force) const
{
    int ncomponents = 0;
    const Component *components_array = components.constData();
    
    Values values;
    
    for (int i=0; i<ncomponents; ++i)
    {
        const Component &component = components_array[i];
        
        //evaluate all of the dependent symbols...
        int ndeps = component.nDependents();
        const Symbol *deps_array = component.dependents().constData();
        
        for (int j=0; j<ndeps; ++j)
        {
            const Symbol &symbol = deps_array[j];

            if (not values.contains(symbol))
                values.set( symbol, ffsymbols[symbol]->value() );
        }
        
        //now evaluate the scaling factor...
        scale_force *= component.scalingFactor(values);
        
        ffsymbols[component.symbol()]->force(forcetable, forcefields, 
                                             ffsymbols, scale_force);
    }
}

///////////
/////////// Implementation of FFTotalExpression
///////////

FFTotalExpression::FFTotalExpression() : FFSymbol()
{}

FFTotalExpression::FFTotalExpression(const Symbol &symbol)
                  : FFSymbol(symbol)
{}

FFTotalExpression::FFTotalExpression(const FFTotalExpression &other)
                  : FFSymbol(other)
{}
  
FFTotalExpression::~FFTotalExpression()
{}

void FFTotalExpression::load(QDataStream &ds)
{
    FFSymbol::load(ds);
}

void FFTotalExpression::save(QDataStream &ds) const
{
    FFSymbol::save(ds);
}

Expression FFTotalExpression::toExpression() const
{
    return Expression(this->symbol());
}

double FFTotalExpression::value() const
{
    throw SireError::program_bug( QObject::tr(
        "An FFTotalExpression should never be used in a situation where "
        "its value must be determined..."), CODELOC );
        
    return 0;
}

MolarEnergy FFTotalExpression::energy(QVector<FFPtr> &forcefields,
                                      const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                                      double scale_energy) const
{
    MolarEnergy nrg(0);
    
    int nffields = forcefields.count();
    
    FFPtr *ffields_array = forcefields.data();
    
    for (int i=0; i<nffields; ++i)
    {
        nrg += ffields_array[i].edit().energy();
    }
    
    return nrg * scale_energy;
}

void FFTotalExpression::force(ForceTable &forcetable,
                              QVector<FFPtr> &forcefields,
                              const QHash<Symbol,FFSymbolPtr> &ffsymbols,
                              double scale_force) const
{
    int nffields = forcefields.count();
    FFPtr *ffields_array = forcefields.data();
    
    for (int i=0; i<nffields; ++i)
    {
        FFPtr &ffield = ffields_array[i];
        
        if (ffield->isA<FF3D>())
            ffield.edit().asA<FF3D>().force(forcetable, scale_force);
    }
}

///////////
/////////// Implementation of ForceFields
///////////

static const RegisterMetaType<ForceFields> r_ffields;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ForceFields &ffields)
{
    writeHeader(ds, r_ffields, 1);
    
    SharedDataStream sds(ds);
    
    //write out all of the forcefields
    sds << ffields.ffields_by_idx
        << ffields.ffsymbols;
    
    return ds;
}
 
/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ForceFields &ffields)
{
    VersionID v = readHeader(ds, r_ffields);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);

        //read into a new object, as a lot can go wrong!
        ForceFields new_ffields;

        //read in the forcefields
        sds >> new_ffields.ffields_by_idx
            >> new_ffields.ffsymbols;

        //rebuild the index
        new_ffields.rebuildIndex();
        
        ffields = new_ffields;
    }
    else
        throw version_error(v, "1", r_ffields, CODELOC);

    return ds;
}

Symbol ForceFields::total_component("E_{total}");

/** Return the symbol representing the total energy component */
const Symbol& ForceFields::totalComponent()
{
    return total_component;
}

/** Constructor */
ForceFields::ForceFields() : ConcreteProperty<ForceFields,MolGroupsBase>()
{}

/** Internal function used to return the ith forcefield
    (this performs no bounds checking!) */
const FF& ForceFields::_pvt_forceField(int i) const
{
    return ffields_by_idx.constData()[i].read();
}

/** Internal function used to return the ith forcefield
    (this performs no bounds checking!) */
FF& ForceFields::_pvt_forceField(int i)
{
    return ffields_by_idx.data()[i].edit();
}

/** Internal function used to return the forcefield with name 'ffname'.
    This does not check to see if this forcefield exists */
const FF& ForceFields::_pvt_forceField(const FFName &ffname) const
{
    return this->_pvt_forceField( *(ffields_by_name.constFind(ffname)) );
}

/** Internal function used to return the forcefield with name 'ffname'.
    This does not check to see if this forcefield exists */
FF& ForceFields::_pvt_forceField(const FFName &ffname)
{
    return this->_pvt_forceField( *(ffields_by_name.constFind(ffname)) );
}

/** Internal function used to return the forcefield that contains the
    molecule group with number 'mgnum'. This does not check to see 
    if such a group exists */
const FF& ForceFields::_pvt_forceField(const MGNum &mgnum) const
{
    return this->_pvt_forceField( *(mgroups_by_num.constFind(mgnum)) );
}

/** Internal function used to return the forcefield that contains the
    molecule group with number 'mgnum'. This does not check to see 
    if such a group exists */
FF& ForceFields::_pvt_forceField(const MGNum &mgnum)
{
    return this->_pvt_forceField( *(mgroups_by_num.constFind(mgnum)) );
}

/** Reindex the moleculegroups and molecules */
void ForceFields::reindex()
{
    MolGroupsBase::clearIndex();

    int nffields = ffields_by_idx.count();
    const FFPtr *ffields_array = ffields_by_idx.constData();
    
    for (int i=0; i<nffields; ++i)
    {
        const FFPtr &ffield = ffields_array[i];
        
        for (int j=0; j<ffield->nGroups(); ++j)
        {
            MolGroupsBase::addToIndex( ffield->at(MGIdx(j)) );
        }
    }
}

/** Rebuild the index of forcefields and groups */
void ForceFields::rebuildIndex()
{
    ffields_by_name.clear();
    mgroups_by_num.clear();
    
    int nffields = ffields_by_idx.count();
    const FFPtr *ffields_array = ffields_by_idx.constData();
    
    for (int i=0; i<nffields; ++i)
    {
        const FFPtr &ffield = ffields_array[i];
        
        if (ffields_by_name.contains(ffield->name()))
        {
            const FFPtr &old_ffield = this->_pvt_forceField(ffield->name());
        
            throw SireFF::duplicate_forcefield( QObject::tr(
                "Cannot have two forcefields in the same set that both "
                "have the same name! (%1, %2 version %3 vs. %4 version %5)")
                    .arg(ffield->name())
                    .arg(ffield->UID().toString()).arg(ffield->version())
                    .arg(old_ffield->UID().toString()).arg(old_ffield->version()),
                            CODELOC );
        }
        
        ffields_by_name.insert( ffield->name(), i );
        
        foreach (MGNum mgnum, ffield->mgNums())
        {
            if (mgroups_by_num.contains(mgnum))
            {
                const FFPtr &old_ffield = this->_pvt_forceField(mgnum);
                
                throw SireMol::duplicate_group( QObject::tr(
                    "Cannot have two different forcefields containing the same "
                    "molecule group - %1 (%2 version %3 %4 vs. "
                    "%5 version %6 %7)")
                        .arg(mgnum)
                        .arg(ffield->name()).arg(ffield->version())
                        .arg(ffield->UID().toString())
                        .arg(old_ffield->name()).arg(old_ffield->version())
                        .arg(old_ffield->UID().toString()), CODELOC );
            }
        
            mgroups_by_num.insert( mgnum, ffield->name() );
        }
    }
    
    ffields_by_name.squeeze();
    mgroups_by_num.squeeze();

    //now rebuild the index of molecules and molecule groups
    MolGroupsBase::clearIndex();
    
    for (int i=0; i<nffields; ++i)
    {
        const FFPtr &ffield = ffields_array[i];
        
        for (int j=0; j<ffield->nGroups(); ++j)
        {
            MolGroupsBase::addToIndex( ffield->at(MGIdx(j)) );
        }
    }
    
    //now rebuild the index of forcefield expressions and symbols
    QHash<Symbol,FFSymbolPtr> new_symbols;

    //first copy in all of the symbols representing all of the forcefield
    //components
    QSet<Symbol> all_symbols;
    
    for (FFIdx i(0); i<nffields; ++i)
    {
        Symbols symbols = ffields_array[i]->components().symbols();

        foreach (const Symbol &symbol, symbols)
        {
            if (new_symbols.contains(symbol))
                throw SireError::program_bug( QObject::tr(
                    "It should not be possible for two forcefields to have "
                    "the same component symbol... (%1)")
                        .arg(symbol.toString()), CODELOC );
        
            new_symbols.insert( symbol, FFSymbolPtr(new FFSymbolFF(i, symbol)) );
            all_symbols.insert(symbol);
        }
    }

    //copy in the non-forcefield symbols from the old array
    for (QHash<Symbol,FFSymbolPtr>::const_iterator it = ffsymbols.constBegin();
         it != ffsymbols.constEnd();
         ++it)
    {
        if ( not (it.value()->isA<FFSymbolFF>() or 
                  it.value()->isA<FFTotalExpression>()) )
        {
            if (new_symbols.contains(it.key()))
                throw SireFF::duplicate_component( QObject::tr(
                    "You cannot use the symbol %1 to represent a forcefield "
                    "expression or parameter as it already in use to represent "
                    "an energy component of a forcefield.")
                        .arg(it.key().toString()), CODELOC );
            
            new_symbols.insert(it.key(), it.value());
        }
    }
    
    //now process each forcefield expression...
    for (QHash<Symbol,FFSymbolPtr>::iterator it = new_symbols.begin();
         it != new_symbols.end();
         ++it)
    {
        if (it.value()->isA<FFSymbolExpression>())
        {
            it.value()->asA<FFSymbolExpression>().expandInTermsOf(all_symbols);
        }
    }
    
    //if there isn't a total energy component, then add the default one
    if (not new_symbols.contains( this->totalComponent() ))
    {
        new_symbols.insert(this->totalComponent(),
                           FFSymbolPtr(new FFTotalExpression()));
    }
    
    ffsymbols = new_symbols;
}

/** Construct a group that holds just a single forcefield */
ForceFields::ForceFields(const FF& forcefield)
            : ConcreteProperty<ForceFields,MolGroupsBase>()
{
    ffields_by_idx.append(forcefield);
    this->rebuildIndex();
}

/** Construct a group that holds lots of forcefields */
ForceFields::ForceFields(const QList<FFPtr> &forcefields)
            : ConcreteProperty<ForceFields,MolGroupsBase>()
{
    ffields_by_idx = forcefields.toVector();
    
    int nffields = ffields_by_idx.count();
    
    if (nffields > 1)
    {
        Molecules mols;
        
        FFPtr *ffields_array = ffields_by_idx.data();
        
        for (int i=1; i<nffields; ++i)
        {
            mols += ffields_array[i-1]->molecules();
            ffields_array[i].edit().update(mols);
        }
    }
    
    this->rebuildIndex();
}

/** Construct a group that holds lots of forcefields */
ForceFields::ForceFields(const QVector<FFPtr> &forcefields)
            : ConcreteProperty<ForceFields,MolGroupsBase>(),
              ffields_by_idx(forcefields)
{
    this->rebuildIndex();
}

/** Copy constructor */
ForceFields::ForceFields(const ForceFields &other)
            : ConcreteProperty<ForceFields,MolGroupsBase>(other),
              ffields_by_idx(other.ffields_by_idx),
              ffields_by_name(other.ffields_by_name),
              mgroups_by_num(other.mgroups_by_num),
              ffsymbols(other.ffsymbols)
{}

/** Destructor */
ForceFields::~ForceFields()
{}

/** Copy assignment operator */
ForceFields& ForceFields::operator=(const ForceFields &other)
{
    if (this != &other)
    {
        ffields_by_idx = other.ffields_by_idx;
        ffields_by_name = other.ffields_by_name;
        mgroups_by_num = other.mgroups_by_num;
        ffsymbols = other.ffsymbols;
        
        MolGroupsBase::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool ForceFields::operator==(const ForceFields &other) const
{
    return ffields_by_idx == other.ffields_by_idx;
}

/** Comparison operator */
bool ForceFields::operator!=(const ForceFields &other) const
{
    return ffields_by_idx != other.ffields_by_idx;
}

/** Internal function used to return the group with number 'mgnum' */
const MoleculeGroup& ForceFields::getGroup(MGNum mgnum) const
{
    if (not mgroups_by_num.contains(mgnum))
        throw SireMol::missing_group( QObject::tr(
            "None of the forcefields in this set contain a molecule group "
            "with number %1. Available groups are %2.")
                .arg(mgnum).arg(Sire::toString(mgroups_by_num.keys())), 
                    CODELOC );

    return this->_pvt_forceField(mgnum).group(mgnum);
}

/** Internal function used to get the pointers to lots of groups */
void ForceFields::getGroups(const QList<MGNum> &mgnums,
                            QVarLengthArray<const MoleculeGroup*,10> &groups) const
{
    groups.clear();
    
    foreach (MGNum mgnum, mgnums)
    {
        groups.append( &(this->getGroup(mgnum)) );
    }
}

/** Internal function used to get pointers to all of the groups
    in all of the forcefields of this set */
QHash<MGNum,const MoleculeGroup*> ForceFields::getGroups() const
{
    QHash<MGNum,const MoleculeGroup*> groups;
    
    for (QHash<MGNum,FFName>::const_iterator it = mgroups_by_num.constBegin();
         it != mgroups_by_num.constEnd();
         ++it)
    {
        groups.insert( it.key(), &(this->_pvt_forceField(it.value()).group(it.key())) );
    }
    
    return groups;
}

/** Return the forcefield with name 'ffname'

    \throw SireFF::missing_forcefield
*/
const FF& ForceFields::operator[](const FFName &ffname) const
{
    if (not ffields_by_name.contains(ffname))
        throw SireFF::missing_forcefield( QObject::tr(
            "There is no forcefield called \"%1\" in this set. Available "
            "forcefields are called %2.")
                .arg(ffname).arg( Sire::toString(ffields_by_name.keys()) ),
                    CODELOC );
                    
    return this->_pvt_forceField(ffname);
}

/** Return the forcefield at index 'ffidx'

    \throw SireError::invalid_index
*/
const FF& ForceFields::operator[](const FFIdx &ffidx) const
{
    return this->_pvt_forceField( ffidx.map( ffields_by_idx.count() ) );
}

/** Return the forcefield with ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FF& ForceFields::operator[](const FFID &ffid) const
{
    return this->_pvt_forceField( this->ffIdx(ffid) );
}

/** Return the forcefield with name 'ffname'

    \throw SireFF::missing_forcefield
*/
const FF& ForceFields::at(const FFName &ffname) const
{
    return this->operator[](ffname);
}

/** Return the forcefield at index 'ffidx'

    \throw SireError::invalid_index
*/
const FF& ForceFields::at(const FFIdx &ffidx) const
{
    return this->operator[](ffidx);
}

/** Return the forcefield with ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FF& ForceFields::at(const FFID &ffid) const
{
    return this->operator[](ffid);
}

/** Return the forcefield with name 'ffname'

    \throw SireFF::missing_forcefield
*/
const FF& ForceFields::forceField(const FFName &ffname) const
{
    return this->operator[](ffname);
}

/** Return the forcefield at index 'ffidx'

    \throw SireError::invalid_index
*/
const FF& ForceFields::forceField(const FFIdx &ffidx) const
{
    return this->operator[](ffidx);
}

/** Return the forcefield with ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FF& ForceFields::forceField(const FFID &ffid) const
{
    return this->operator[](ffid);
}

/** Return the forcefield that contains the molecule group with
    number 'mgnum'
    
    \throw SireMol::missing_group
*/
const FF& ForceFields::forceField(const MGNum &mgnum) const
{
    if (not mgroups_by_num.contains(mgnum))
        throw SireMol::missing_group( QObject::tr(
            "None of the forcefields in this set contain a molecule group "
            "with number %1. Available groups are %2.")
                .arg(mgnum).arg(Sire::toString(mgroups_by_num.keys())), 
                    CODELOC );

    return this->_pvt_forceField(mgnum);
}

/** Return the number of forcefields in this set */
int ForceFields::nForceFields() const
{
    return ffields_by_idx.count();
}
    
/** Return the index of the forcefield with name 'ffname'

    \throw SireFF::missing_forcefield 
    \throw SireFF::duplicate_forcefield
*/
FFIdx ForceFields::ffIdx(const FFName &ffname) const
{
    QList<FFIdx> ffidxs = ffname.map(*this);
    
    if (ffidxs.count() > 1)
        throw SireFF::duplicate_forcefield( QObject::tr(
            "There is more than one forcefield that matches the name "
            "\"%1\".").arg(ffname), CODELOC );

    return ffidxs.first();
}

/** Simple function that allows a shortcut for ffIdx(FFIdx)

    \throw SireError::invalid_index
*/
FFIdx ForceFields::ffIdx(const FFIdx &ffidx) const
{
    return FFIdx( ffidx.map(this->nForceFields()) );
}

/** Return the FFIdx of the forcefield that matches the ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
FFIdx ForceFields::ffIdx(const FFID &ffid) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);
    
    if (ffidxs.count() > 1)
        throw SireFF::duplicate_forcefield( QObject::tr(
            "More than one forcefield in this set matches the ID %1. "
            "Matching forcefield have indicies %2.")
                .arg(ffid.toString(), Sire::toString(ffidxs)),
                    CODELOC );
                    
    return ffidxs.at(0);
}
    
QList<FFIdx> ForceFields::map(const FFID &ffid) const
{
    return ffid.map(*this);
}

QList<FFIdx> ForceFields::map(const FFIdx &ffidx) const
{
    QList<FFIdx> ffidxs;
    
    ffidxs.append( this->ffIdx(ffidx) );
    
    return ffidxs;
}

QList<FFIdx> ForceFields::map(const FFName &ffname) const
{
    QList<FFIdx> ffidxs;

    if (ffname.isCaseSensitive())
    {
        QHash<QString,int>::const_iterator it = ffields_by_name.constFind(ffname);
        
        if (it != ffields_by_name.constEnd())
            ffidxs.append( FFIdx(it.value()) );
    }
    else
    {
        QString lower_name = QString(ffname).toLower();
        
        for (QHash<QString,int>::const_iterator it = ffields_by_name.constBegin();
             it != ffields_by_name.constEnd();
             ++it)
        {
            if (it.key().toLower() == lower_name)
                ffidxs.append( FFIdx(it.value()) );
        }
    }

    if (ffidxs.isEmpty())
        throw SireFF::missing_forcefield( QObject::tr(
            "There are no forcefields in this set called %1. "
            "Available forcefields are called %2.")
                .arg(ffname).arg( Sire::toString(ffields_by_name.keys()) ),
                    CODELOC );

    return ffidxs;
}

/** Simple function that short cuts ffName(FFName) 

    \throw SireFF::missing_forcefield
*/
const FFName& ForceFields::ffName(const FFName &ffname) const
{
    QList<FFIdx> ffidxs = this->map(ffname);

    if (ffidxs.count() > 1)
        throw SireFF::duplicate_forcefield( QObject::tr(
            "More than one forcefield in this set has the name %1. "
            "Matching forcefield have indicies %2.")
                .arg(ffname.toString(), Sire::toString(ffidxs)),
                    CODELOC );

    return ffname;
}

/** Return the name of the forcefield at index 'ffidx'

    \throw SireError::invalid_index
*/
const FFName& ForceFields::ffName(const FFIdx &ffidx) const
{
    return this->_pvt_forceField( ffidx.map(this->nForceFields()) ).name();
}

/** Return the name of the forcefield that matches the ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FFName& ForceFields::ffName(const FFID &ffid) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);

    if (ffidxs.count() > 1)
        throw SireFF::duplicate_forcefield( QObject::tr(
            "More than one forcefield in this set matches the ID %1. "
            "Matching forcefield have indicies %2.")
                .arg(ffid.toString(), Sire::toString(ffidxs)),
                    CODELOC );

    return this->_pvt_forceField( ffidxs.at(0).map(this->nForceFields()) ).name();
}

/** Return a string representation of this set */
QString ForceFields::toString() const
{
    return QObject::tr("FFPtr( nForceFields() == %1 )").arg(this->nForceFields());
}

/** Set the component represented by the symbol 'symbol' equal to the 
    value 'value'. This replaces any existing component with this value.
    Note that an exception will be raised if you try to replace a component
    that exists in one of the constituent forcefields.
    
    \throw SireFF::duplicate_component
*/
void ForceFields::setComponent(const Symbol &symbol, double value)
{
    ForceFields old_state( *this );

    try
    {
        ffsymbols.insert( symbol, 
                          FFSymbolPtr(new FFSymbolValue(symbol, value)) );
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the component represented by the symbol 'symbol' equal to the expression
    contained in 'expression'. This replaces any existing component with 
    this expression. Note that an exception will be raised if you try to 
    replace a component that exists in one of the constituent forcefields
    
    \throw SireFF::duplicate_component
*/
void ForceFields::setComponent(const Symbol &symbol, const Expression &expression)
{
    if (expression.isConstant())
    {
        this->setComponent(symbol, expression.evaluate(Values()));
        return;
    }

    ForceFields old_state( *this );
    
    try
    {
        ffsymbols.insert( symbol, 
                          FFSymbolPtr(new FFSymbolExpression(symbol, expression)) );
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Return the symbols representing all of the energy components */
Symbols ForceFields::components() const
{
    return ffsymbols.keys().toSet();
}

/** Return whether or not there is a forcefield component with symbol 'symbol' */
bool ForceFields::hasComponent(const Symbol &symbol) const
{
    return ffsymbols.contains(symbol);
}

/** Return the forcefield component symbol, value or expression that matches
    the component represented by the symbol 'symbol'
    
    \throw SireFF::missing_component
*/
Expression ForceFields::getComponent(const Symbol &symbol) const
{
    if (not ffsymbols.contains(symbol))
        throw SireFF::missing_component( QObject::tr(
            "There is no component represented by the symbol %1. "
            "Available components are %2.")
                .arg(symbol.toString(), Sire::toString(ffsymbols.keys())),
                    CODELOC );
                    
    return ffsymbols[symbol]->toExpression();
}

/** Return the energy associated with the symbol 'component'. This component 
    may either be a component of one of the constituent forcefields,
    or it may represent a function of the forcefield components
    
    \throw SireFF::missing_component
*/
SireUnits::Dimension::MolarEnergy ForceFields::energy(const Symbol &component)
{
    if (not ffsymbols.contains(component))
        throw SireFF::missing_component( QObject::tr(   
            "There is no component of the energy represented by the "
            "symbol %1. Available components are %2.")
                .arg(component.toString(), Sire::toString(ffsymbols.keys())),
                    CODELOC );

    return ffsymbols.value(component)->energy(ffields_by_idx, ffsymbols);
}

/** Return the energy of this set of forcefields. This uses the supplied
    total energy function to calculate the energy, if one exists,
    or it just calculates the sum of the total energies of all of the
    contained forcefields */
SireUnits::Dimension::MolarEnergy ForceFields::energy()
{
    return this->energy( this->totalComponent() );
}

/** Return the energies of all of the components whose symbols are 
    listed in 'components'
    
    \throw SireFF::missing_component
*/
Values ForceFields::energies(const QSet<Symbol> &components)
{
    Values vals;
    
    foreach (const Symbol &component, components)
    {
        vals.set(component, this->energy(component).value());
    }
    
    return vals;
}

/** Return the energies of all of the components of all of the forcefields,
    constants and expressions */
Values ForceFields::energies()
{
    return this->energies( ffsymbols.keys().toSet() );
}

/** Add the force due to the component 'component' to the molecules
    in the force table 'forcetable', scaled by 'scale_force'
    
    \throw SireFF::missing_component
*/
void ForceFields::force(ForceTable &forcetable, const Symbol &component,
                        double scale_force)
{
    if (not ffsymbols.contains(component))
        throw SireFF::missing_component( QObject::tr(   
            "There is no component of the energy represented by the "
            "symbol %1. Available components are %2.")
                .arg(component.toString(), Sire::toString(ffsymbols.keys())),
                    CODELOC );

    ffsymbols.value(component)->force(forcetable, ffields_by_idx, 
                                      ffsymbols, scale_force);
}

/** Add the forces due to the forcefields in this set to the molecules
    in the force table 'forcetable', scaled by 'scale_force' */
void ForceFields::force(ForceTable &forcetable, double scale_force)
{
    this->force(forcetable, this->totalComponent(), scale_force);
}

/** Set the property 'name' to have the value 'value' in *all* of the 
    forcefields contained in this set
    
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
void ForceFields::setProperty(const QString &name, const Property &value)
{
    QVector<FFPtr> new_ffields = ffields_by_idx;
    
    int nffields = new_ffields.count();
    FFPtr *new_ffields_array = new_ffields.data();
    
    for (int i=0; i<nffields; ++i)
    {
        if (new_ffields_array[i].read().containsProperty(name))
            new_ffields_array[i].edit().setProperty(name, value);
    }
    
    //everything went ok!
    ffields_by_idx = new_ffields;
}

/** Set the property 'name' to have the value 'value' in all of the forcefields
    in this set that match the ID 'ffid'
    
    \throw SireFF::missing_forcefield
    \throw SireError::invalid_index
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
void ForceFields::setProperty(const FFID &ffid, const QString &name, 
                              const Property &value)
{
    QVector<FFPtr> old_state( ffields_by_idx );

    try
    {
        QList<FFIdx> ffidxs = ffid.map(*this);
        
        foreach (const FFIdx &ffidx, ffidxs)
        {
            if (this->_pvt_forceField(ffidx).containsProperty(name))
                this->_pvt_forceField(ffidx).setProperty(name, value);
        }
    }
    catch(...)
    {
        ffields_by_idx = old_state;
        throw;
    }
}

/** Return the value of the property with name 'name'. This returns
    the property if it exists in at least one forcefield, and
    if all occurances of the property have the same value

    \throw SireBase::duplicate_property
    \throw SireBase::missing_property
*/
const Property& ForceFields::property(const QString &name) const
{
    int nffields = ffields_by_idx.count();
    const FFPtr *ffields_array = ffields_by_idx.constData();
    
    const Property *p = &(Property::null());
    
    for (int i=0; i<nffields; ++i)
    {
        if (ffields_array[i]->containsProperty(name))
        {
            if ( p->equals(Property::null()) )
            {
                p = &(ffields_array[i]->property(name));
            }
            else if ( not p->equals(ffields_array[i]->property(name)) )
            {
                throw SireBase::duplicate_property( QObject::tr(
                    "More than one forcefield contains the property (%1), and "
                    "it has a different value in these forcefields. "
                    "You will have to search for this property "
                    "individually in each forcefield (e.g. using the "
                    "ForceFields::forceFieldsWithProperty(...) member function).")
                        .arg(name), CODELOC );
            }
        }
    }
    
    if ( p->equals(Property::null()) )
    {
        QStringList property_keys;
    
        for (int i=0; i<nffields; ++i)
        {
            property_keys += ffields_array[i]->propertyKeys();
        }
    
        throw SireBase::missing_property( QObject::tr(
            "None of the contained forcefields have a property called %1. "
            "Available properties are %2.")
                .arg(name, Sire::toString(property_keys) ), CODELOC );
    }
    
    return *p;
}

/** Return the value of the property 'name' in the forcefields identified
    by the ID 'ffid'
    
    \throw SireBase::duplicate_property
    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireBase::missing_property
*/
const Property& ForceFields::property(const FFID &ffid, const QString &name) const
{
    //get the list of forcefields that match this ID
    QList<FFIdx> ffidxs = ffid.map(*this);

    const Property *p = &(Property::null());

    foreach (FFIdx ffidx, ffidxs)
    {
        const FF &ff = this->_pvt_forceField(ffidx);
        
        if (ff.containsProperty(name))
        {
            if ( p->equals(Property::null()) )
            {
                p = &(ff.property(name));
            }
            else if ( not p->equals(ff.property(name)) )
            {
                throw SireBase::duplicate_property( QObject::tr(
                    "More than one of the forcefields that match the ID %1 "
                    "contain the property %2, and it has a different value "
                    "in at least two of these forcefields. You will need "
                    "to search for this property one forcefield at a time "
                    "(e.g. using the ForceFields::forceFieldsWithProperty(...) "
                    "member function).")
                        .arg(ffid.toString(), name), CODELOC );
            }
        }
    }
    
    if ( p->equals(Property::null()) )
        throw SireBase::missing_property( QObject::tr(
            "None of the forcefields that match the ID '%1' contain "
            "a property called %2.")
                .arg(ffid.toString(), name), CODELOC );

    return *p;
}

/** Return whether or not any of the forcefields contain the property
    with name 'name' */
bool ForceFields::containsProperty(const QString &name) const
{
    int nffields = ffields_by_idx.count();
    const FFPtr *ffields_array = ffields_by_idx.constData();
    
    for (int i=0; i<nffields; ++i)
    {
        if (ffields_array[i]->containsProperty(name))
            return true;
    }
    
    return false;
}

/** Return whether or not any of the forcefields that match the ID 'ffid'
    contain the property with name 'name'
    
    \throw SireFF::missing_forcefield
    \throw SireError::invalid_index
*/
bool ForceFields::containsProperty(const FFID &ffid, const QString &name) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);
    
    foreach (const FFIdx &ffidx, ffidxs)
    {
        if (this->_pvt_forceField(ffidx).containsProperty(name))
            return true;
    }
    
    return false;
}

/** Return the names of all of the properties in all of the forcefields */
QStringList ForceFields::propertyKeys() const
{
    int nffields = this->nForceFields();

    if (nffields == 0)
    {
        return QStringList();
    }
    else if (nffields == 1)
    {
        return this->_pvt_forceField(0).propertyKeys();
    }
    
    QSet<QString> keys;
    
    for (int i=0; i<nffields; ++i)
    {
        keys.unite( this->_pvt_forceField(i).propertyKeys().toSet() );
    }
    
    return QStringList( keys.toList() );
}

/** Return the names of all of the properties in the forcefields 
    identified by the ID 'ffid'
    
    \throw SireFF::missing_forcefield
    \throw SireError::invalid_index
*/
QStringList ForceFields::propertyKeys(const FFID &ffid) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);
    
    if (ffidxs.count() == 1)
    {
        return this->_pvt_forceField(ffidxs.at(0)).propertyKeys();
    }
    
    QSet<QString> keys;
    
    foreach (FFIdx idx, ffidxs)
    {
        keys.unite( this->_pvt_forceField(idx).propertyKeys().toSet() );
    }
    
    return QStringList( keys.toList() );
}

/** Return all of the properties in all of the forcefields. This will raise
    an error if there are properties with the same name in different 
    forcefields that have different values.
    
    \throw SireBase::duplicate_property
*/
Properties ForceFields::properties() const
{
    if (this->nForceFields() == 1)
    {
        return this->_pvt_forceField(0).properties();
    }

    Properties props;
    
    QStringList keys = this->propertyKeys();
    
    foreach (QString key, keys)
    {
        props.setProperty(key, this->property(key));
    }
    
    return props;
}

/** Return all of the properties in all of the forcefields identified by
    the ID 'ffid'. This will raise an error if there are properties with
    the same name in different forcefields that have different values.
    
    \throw SireBase::duplicate_property
*/
Properties ForceFields::properties(const FFID &ffid) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);
    
    if (ffidxs.count() == 1)
    {
        return this->_pvt_forceField(ffidxs.at(0)).properties();
    }
    
    QStringList keys = this->propertyKeys(ffid);
    
    Properties props;
    
    foreach (QString key, keys)
    {
        props.setProperty(key, this->property(ffid, key));
    }
    
    return props;
}

/** Return the list of all forcefields that contain a property with name 'name'

    \throw SireBase::missing_property
*/
QVector<FFPtr> ForceFields::forceFieldsWithProperty(const QString &name) const
{
    QVector<FFPtr> ffs;
    
    int nffields = this->nForceFields();
    
    for (int i=0; i<nffields; ++i)
    {
        if (this->_pvt_forceField(i).containsProperty(name))
            ffs.append( this->_pvt_forceField(i) );
    }
    
    if (ffs.isEmpty())
        throw SireBase::missing_property( QObject::tr(
            "No forcefields contain the property %1. Available properties "
            "are %2.")
                .arg(name, Sire::toString(this->propertyKeys())), CODELOC );
                
    return ffs;
}

/** Return the list of forcefields that match the ID "ffid" and that
    contain the property with name 'name'
    
    \throw SireFF::missing_forcefield
    \throw SireBase::missing_property
    \throw SireError::invalid_index
*/
QVector<FFPtr> ForceFields::forceFieldsWithProperty(const FFID &ffid, 
                                                    const QString &name) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);
    
    QVector<FFPtr> ffs;
    
    foreach (FFIdx ffidx, ffidxs)
    {
        if (this->_pvt_forceField(ffidx).containsProperty(name))
            ffs.append( this->_pvt_forceField(ffidx) );
    }
    
    if (ffs.isEmpty())
        throw SireBase::missing_property( QObject::tr(
            "None of the forcefields that match the ID %1 contain the "
            "property called %2. Available properties are %3.")
                .arg(ffid.toString(), name, 
                     Sire::toString(this->propertyKeys(ffid))), CODELOC );
                     
    return ffs;
}
   
/** Return the list of all forcefields that match the ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireError::invalid_index
*/
QVector<FFPtr> ForceFields::forceFields(const FFID &ffid) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);
    
    QVector<FFPtr> ffs;
    ffs.reserve(ffidxs.count());
    
    foreach (FFIdx ffidx, ffidxs)
    {
        ffs.append( this->_pvt_forceField(ffidx) );
    }
    
    return ffs;
}

/** Return the names of all of the forcefields that match the ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireError::invalid_index
*/
QList<FFName> ForceFields::ffNames(const FFID &ffid) const
{
    QList<FFIdx> ffidxs = ffid.map(*this);

    QList<FFName> names;
    
    foreach (FFIdx ffidx, ffidxs)
    {
        names.append( this->_pvt_forceField(ffidx).name() );
    }
    
    return names;
}

/** Return an array containing all of the forcefields in this set, ordered
    in the same order as they appear in this set */
const QVector<FFPtr>& ForceFields::forceFields() const
{
    return ffields_by_idx;
}

/** Return an array containing all of the forcefields in this set, ordered
    in the same order as they appear in this set */
const QVector<FFPtr>& ForceFields::list() const
{
    return this->forceFields();
}

/** Return a list of all of the forcefield names, ordered in the same
    order as the forcefields appear in this set */
QList<FFName> ForceFields::ffNames() const
{
    QList<FFName> ffnames;
    
    for (QHash<QString,int>::const_iterator it = ffields_by_name.constBegin();
         it != ffields_by_name.constEnd();
         ++it)
    {
        ffnames.append( FFName(it.key()) );
    }

    return ffnames;
}

/** Return a list of all of the forcefield names, ordered in the same
    order as the forcefields appear in this set */
QList<FFName> ForceFields::names() const
{
    return this->ffNames();
}

/** Tell all of the forcefields that they must now recalculate their
    energies from scratch. This is a good way to debug the forcefields,
    but may also speed up cases where you know in advance that you will
    be moving most (or all) of the molecules between energy calculations */
void ForceFields::mustNowRecalculateFromScratch()
{
    int nffields = ffields_by_idx.count();
    FFPtr *ffields_array = ffields_by_idx.data();
    
    for (int i=0; i<nffields; ++i)
    {
        ffields_array[i].edit().mustNowRecalculateFromScratch();
    }
}

/** Return whether or not any of the forcefields in this set are dirty
    (the molecules have changed since the last energy calculation) */
bool ForceFields::isDirty() const
{
    int nffields = ffields_by_idx.count();
    const FFPtr *ffields_array = ffields_by_idx.constData();
    
    for (int i=0; i<nffields; ++i)
    {
        if (ffields_array[i]->isDirty())
            return true;
    }
    
    return false;
}

/** Return whether or not all of the forcefields in this set are clean
    (there have been no changes since the last energy evaluation) */
bool ForceFields::isClean() const
{
    return not this->isDirty();
}

/** Add the forcefield 'forcefield' to this set. This will raise
    an exception if this forcefield (or one with the same name)
    is already present in this set. Note that if the added
    forcefield will be updated to contain the versions of 
    any molecules that are already present in any of the 
    other forcefields.

    \throw SireFF::duplicate_forcefield
    \throw SireMol::duplicate_group
*/
void ForceFields::add(const FF &forcefield)
{
    FFPtr ff( forcefield );
    ff.edit().update( this->matchToExistingVersion(forcefield.molecules()) );

    ForceFields old_state( *this );
    
    try
    {
        ffields_by_idx.append(ff);
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Internal function used to remove the ith forcefield */
void ForceFields::_pvt_remove(int i)
{
    ForceFields old_state( *this );
    
    try
    {
        ffields_by_idx.remove(i);
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove the forcefield at index 'ffidx' from this set

    \throw SireError::invalid_index
*/
void ForceFields::remove(const FFIdx &ffidx)
{
    this->_pvt_remove( ffidx.map(ffields_by_idx.count()) );
}

/** Remove the forcefield with name 'ffname'.

    \throw SireFF::missing_forcefield
*/
void ForceFields::remove(const FFName &ffname)
{
    this->_pvt_remove( this->ffIdx(ffname) );
}

/** Remove the forcefield(s) that match the ID 'ffid' 

    \throw SireFF::missing_forcefield
    \throw SireError::invalid_index
*/
void ForceFields::remove(const FFID &ffid)
{
    QList<FFIdx> ffidxs = ffid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const FFIdx &ffidx, ffidxs)
        {
            this->_pvt_remove(ffidx);
        }
        
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove all of the forcefields from this set */
void ForceFields::removeAllForceFields()
{
    ForceFields old_state(*this);
    
    try
    {
        ffields_by_idx.clear();
        ffields_by_name.clear();
        mgroups_by_num.clear();
        this->clearIndex();
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Return the molecule group that has number 'mgnum'

    \throw SireMol::missing_group
*/
const MoleculeGroup& ForceFields::at(MGNum mgnum) const
{
    if (not mgroups_by_num.contains(mgnum))
        throw SireMol::missing_group( QObject::tr(
            "None of the forcefields in this set contain a molecule group "
            "with number %1. Available groups have numbers %2.")
                .arg(mgnum).arg( Sire::toString(mgroups_by_num.keys()) ),
                    CODELOC );
                    
    return this->_pvt_forceField(mgnum).at(mgnum);
}

/** Add the molecule view 'molview' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const MoleculeView &molview, const MGID &mgid,
                      const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);

    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(view.data()) );
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).add(view, mgnum, map);
            MolGroupsBase::addToIndex(mgnum, view.data().number());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}
 
/** Add the views of the molecule in 'molviews' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const ViewsOfMol &molviews, const MGID &mgid,
                      const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);

    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(views.data()) );
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).add(views, mgnum, map);
            MolGroupsBase::addToIndex(mgnum, views.number());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Add the molecules in 'molecules' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const Molecules &molecules, const MGID &mgid,
                      const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    Molecules mols = this->matchToExistingVersion(molecules);

    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).add(mols, mgnum, map);

            MolGroupsBase::addToIndex(mgnum, mols.molNums());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Add the molecules in the molecule group 'molgroup' to the molecule 
    groups identified by 'mgid', using the supplied property map to find the 
    properties required for the forcefield
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const MoleculeGroup &molgroup, const MGID &mgid,
                      const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    //update the group to match the molecule versions
    //already present in this group...
    MolGroupPtr group(molgroup);
    group.edit().update( this->matchToExistingVersion(group.read().molecules()) );

    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).add(group, mgnum, map);
            
            MolGroupsBase::addToIndex(mgnum, group->molNums().toList().toSet());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Add the molecule view 'molview' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield. This only adds the view to groups 
    that don't already contain this view.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const MoleculeView &molview, const MGID &mgid,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);

    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(view.data()) );
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).addIfUnique(view, mgnum, map);
                                                     
            MolGroupsBase::addToIndex(mgnum, view.data().number());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Add the molecule views in 'molviews' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield. This only adds the view to groups 
    that don't already contain this view.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);

    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(views.data()) );
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).addIfUnique(views, mgnum, map);
            
            MolGroupsBase::addToIndex(mgnum, views.number());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}
                              
/** Add the molecules in 'molecules' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield. This only adds the view to groups 
    that don't already contain this view.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const Molecules &molecules, const MGID &mgid,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    Molecules mols = this->matchToExistingVersion(molecules);
    QSet<MolNum> molnums = mols.molNums();

    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).addIfUnique(mols, mgnum, map);
            
            MolGroupsBase::addToIndex(mgnum, mols.molNums());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Add the molecules in the molecule group 'molgroup' to the molecule 
    groups identified by 'mgid', using the supplied property map to 
    find the properties required for the forcefield. This only adds the 
    view to groups that don't already contain this view.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const MoleculeGroup &molgroup, const MGID &mgid,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);

    //update the group...
    MolGroupPtr group(molgroup);
    group.edit().update( this->matchToExistingVersion(molgroup.molecules()) );
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).addIfUnique(group, mgnum, map);
            
            MolGroupsBase::addToIndex(mgnum, group.read().molNums().toList().toSet());
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Add the molecule view 'molview' to the molecule groups identified 
    by 'mgid', using the default locations to find any properties 
    required by the forcefields. 
        
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const MoleculeView &molview, const MGID &mgid)
{
    this->add(molview, mgid, PropertyMap());
}

/** Add the molecule views in 'molviews' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const ViewsOfMol &molviews, const MGID &mgid)
{
    this->add(molviews, mgid, PropertyMap());
}

/** Add the molecules in 'molecules' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const Molecules &molecules, const MGID &mgid)
{
    this->add(molecules, mgid, PropertyMap());
}

/** Add the molecules in the molecule group 'molgroup' to the molecule 
    groups identified by 'mgid', using the supplied property map to 
    find the properties required for the forcefield
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::add(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->add(molgroup, mgid, PropertyMap());
}

/** Add the molecule view 'molview' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield. Only views that aren't already in
    the forcefield are added.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const MoleculeView &molview, const MGID &mgid)
{
    this->addIfUnique(molview, mgid, PropertyMap());
}

/** Add the molecule views in 'molviews' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield. Only views that aren't already in
    the forcefield are added.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid)
{
    this->addIfUnique(molviews, mgid, PropertyMap());
}

/** Add the molecules in 'molecules' to the molecule groups identified 
    by 'mgid', using the supplied property map to find the properties
    required for the forcefield. Only views that aren't already in
    the forcefield are added.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const Molecules &molecules, const MGID &mgid)
{
    this->addIfUnique(molecules, mgid, PropertyMap());
}

/** Add the molecules in the molecule group 'molgroup' to the molecule 
    groups identified by 'mgid', using the supplied property map to 
    find the properties required for the forcefield. Only views that 
    aren't already in the forcefield are added.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_parameter
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::addIfUnique(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->addIfUnique(molgroup, mgid, PropertyMap());
}

/** Remove all of the molecule views that are contained in the molecule
    groups identified by the ID 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::removeAll(const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).removeAll(mgnum);
            
            MolGroupsBase::clearIndex(mgnum);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove the view 'molview' from the specified groups in this
    forcefield. Note that this only removes the specific view
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::remove(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            FF &ff = this->_pvt_forceField(mgnum);
            
            ff.remove(molview, mgnum);
            
            if (not ff.group(mgnum).contains(molview.data().number()))
            {
                MolGroupsBase::removeFromIndex(mgnum, molview.data().number());
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove the views in 'molviews' from the specified groups in this
    forcefield. Note that this only removes the specific views
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::remove(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            FF &ff = this->_pvt_forceField(mgnum);
            
            ff.remove(molviews, mgnum);
            
            if (not ff.group(mgnum).contains(molviews.number()))
            {
                MolGroupsBase::removeFromIndex(mgnum, molviews.number());
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove them molecules in 'molecules' from the specified groups in this
    forcefield. Note that this only removes the specific views
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::remove(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            FF &ff = this->_pvt_forceField(mgnum);
            
            ff.remove(molecules, mgnum);
            
            const MoleculeGroup &molgroup = ff.group(mgnum);
            
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                if (not molgroup.contains(it->number()))
                {
                    MolGroupsBase::removeFromIndex(mgnum, it->number());
                }
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove the views in the molecule group 'molgroup' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::remove(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->remove(molgroup.molecules(), mgid);
}

/** Remove the all copies of the view in 'molview' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    - it does not remove the atoms in this view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::removeAll(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            FF &ff = this->_pvt_forceField(mgnum);
            
            ff.removeAll(molview, mgnum);
            
            if (not ff.group(mgnum).contains(molview.data().number()))
            {
                MolGroupsBase::removeFromIndex(mgnum, molview.data().number());
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove the all copies of the views in 'molviews' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    - it does not remove the atoms in this view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::removeAll(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            FF &ff = this->_pvt_forceField(mgnum);
            
            ff.removeAll(molviews, mgnum);
            
            if (not ff.group(mgnum).contains(molviews.number()))
            {
                MolGroupsBase::removeFromIndex(mgnum, molviews.number());
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove the all copies of the molecules in 'molecules' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    - it does not remove the atoms in this view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::removeAll(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            FF &ff = this->_pvt_forceField(mgnum);
            
            ff.removeAll(molecules, mgnum);
            
            const MoleculeGroup &group = ff.group(mgnum);
            
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                if (not group.contains(it->number()))
                {
                    MolGroupsBase::removeFromIndex(mgnum, it->number());
                }
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Remove the all copies of the molecules in the molecule group 'molgroup' 
    from the specified groups in this forcefield. Note that this only removes
    the specific views - it does not remove the atoms in this view from all 
    of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::removeAll(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->removeAll(molgroup, mgid);
}

/** Remove all views of the molecule with number 'molnum' from the molecule
    groups identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::remove(MolNum molnum, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        MGNum mgnum = mgnums.at(0);
        this->_pvt_forceField(mgnum).remove(molnum, mgnum);
        MolGroupsBase::removeFromIndex(mgnum, molnum);
    }
    else
    {
        ForceFields old_state( *this );
    
        try
        {
            foreach (const MGNum &mgnum, mgnums)
            {
                this->_pvt_forceField(mgnum).remove(molnum, mgnum);
            
                MolGroupsBase::removeFromIndex(mgnum, molnum);
            }
        }
        catch(...)
        {
            this->operator=(old_state);
            throw;
        }
    }
}

/** Remove all of the molecules whose numbers are in 'molnums' from
    all of the molecule groups identified by the ID 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void ForceFields::remove(const QSet<MolNum> &molnums, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).remove(molnums, mgnum);
            
            MolGroupsBase::removeFromIndex(mgnum, molnums);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Update all of the forcefields so that they use the version of the data 
    of the molecule held in 'moldata'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::update(const MoleculeData &moldata)
{
    if (not this->contains(moldata.number()))
        return;
        
    const QList<MGNum> &mgnums = this->groupsContaining(moldata.number());

    BOOST_ASSERT(not mgnums.isEmpty());

    if (mgnums.count() == 1)
    {
        this->_pvt_forceField(mgnums.at(0)).update(moldata);
    }
    else
    {
        ForceFields old_state( *this );
    
        try
        {
            foreach (const MGNum &mgnum, mgnums)
            {
                this->_pvt_forceField(mgnum).update(moldata);
            }
        }
        catch(...)
        {
            this->operator=(old_state);
            throw;
        }
    }
}

/** Update all of the forcefields in this group so that they have the 
    same version of the molecules that are present in 'molecules'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::update(const Molecules &molecules)
{
    if (molecules.isEmpty())
        return;
    
    else if (molecules.count() == 1)
    {
        this->update( molecules.constBegin()->data() );
    }
    else
    {
        ForceFields old_state( *this );
        
        try
        {
            int nffields = ffields_by_idx.count();
            FFPtr *ffields_array = ffields_by_idx.data();
            
            for (int i=0; i<nffields; ++i)
            {
                ffields_array[i].edit().update(molecules);
            }
        }
        catch(...)
        {
            this->operator=(old_state);
            throw;
        }
    }
}

/** Update all of the forcefields in this group so that they have the 
    same version of the molecules that are present in the molecule
    group 'molgroup'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::update(const MoleculeGroup &molgroup)
{
    this->update( molgroup.molecules() );
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the view of the molecule in 'molview'.
    The passed property map is used to find any properties that are
    needed by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const MoleculeView &molview,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(view.data()) );

    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).setContents(mgnum, view, map);
        }
        
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the views of the molecule in 'molviews'.
    The passed property map is used to find any properties that are
    needed by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const ViewsOfMol &molviews,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(views.data()) );

    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).setContents(mgnum, views, map);
        }
        
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the molecules in 'molecules'.
    The passed property map is used to find any properties that are
    needed by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const Molecules &molecules,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);

    Molecules mols = this->matchToExistingVersion(molecules);
    
    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).setContents(mgnum, mols, map);
        }
        
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the molecules in the group 'molgroup'.
    The passed property map is used to find any properties that are
    needed by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const MoleculeGroup &molgroup,
                              const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    MoleculeGroup group(molgroup);
    group.update( this->matchToExistingVersion(group.molecules()) );

    ForceFields old_state( *this );
    
    try
    {
        foreach (const MGNum &mgnum, mgnums)
        {
            this->_pvt_forceField(mgnum).setContents(mgnum, group, map);
        }
        
        this->rebuildIndex();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the view of the molecule in 'molview'.
    Properties required by the forcefields are searched for in the
    default properties.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const MoleculeView &molview)
{
    this->setContents(mgid, molview, PropertyMap());
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the views of the molecule in 'molviews'.
    Properties required by the forcefields are searched for in the
    default properties.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const ViewsOfMol &molviews)
{
    this->setContents(mgid, molviews, PropertyMap());
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the molecules in 'molecules'.
    Properties required by the forcefields are searched for in the
    default properties.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const Molecules &molecules)
{
    this->setContents(mgid, molecules, PropertyMap());
}

/** Set the contents of the molecule groups identified by the ID 'mgid'
    so that they only contain the molecules in the molecule group 'molgroup'.
    Properties required by the forcefields are searched for in the
    default properties.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void ForceFields::setContents(const MGID &mgid, const MoleculeGroup &molgroup)
{
    this->setContents(mgid, molgroup, PropertyMap());
}
