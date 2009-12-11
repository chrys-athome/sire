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

#ifndef SIRECAS_POWER_H
#define SIRECAS_POWER_H

#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This is the base class of all power expressions, 
    e.g. x^y (all of the form core^power). There are several 
    sub-classes that depend on exactly what is being raised 
    to which power, e.g. Exp is e^y, Power is x^y, PowerConstant 
    is c^y and ConstantPower is x^c (with ConstantPower further 
    derived into RationalPower and RealPower based on whether 
    the constant is rational). All of these can be constructed 
    transparently by creating a Power and then calling 'reduce' 
    on the resulting object.
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT PowerFunction : public Siren::Extends<PowerFunction,CASNode>
{
public:
    PowerFunction();

    ~PowerFunction();

    virtual Expression core() const=0;
    virtual Expression power() const=0;

    static QString typeName();

    QSet<Symbol> symbols() const;

    bool isCompound() const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    Expression substitute(const Identities &identities) const;

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    QList<Expression> children() const;

    bool isFunction(const Symbol &symbol) const;
    bool isConstant() const;

    Expression reduce() const;
    
    QList<Factor> expand(const Symbol &symbol) const;
};

/** This class represents an expression raised to a generic 
    power (e.g. x^y). This is also the route to raising expressions 
    to real-number powers, and the base of the implementation of 
    the exp() and invlog_10() functions.

    @author Christopher Woods
*/
class SIRECAS_EXPORT Power : public Siren::Implements<Power,PowerFunction>
{
public:
    Power();
    Power(const Expression &base, const Expression &power);

    Power(const Power &other);

    ~Power();

    Power& operator=(const Power &other);
    
    bool operator==(const Power &other) const;
    bool operator!=(const Power &other) const;

    uint hashCode() const;
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

    Expression core() const;
    Expression power() const;

    bool isCompound() const;

private:
    /** The core expression */
    Expression ex;

    /** The power */
    Expression pwr;
};

}

Q_DECLARE_METATYPE(SireCAS::Power)

SIRE_EXPOSE_CLASS( SireCAS::PowerFunction )
SIRE_EXPOSE_CLASS( SireCAS::Power )

SIRE_END_HEADER

#endif
