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

#ifndef SIRECAS_POWERCONSTANT_H
#define SIRECAS_POWERCONSTANT_H

#include "power.h"

#include "SireMaths/rational.h"
#include "SireMaths/complex.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This class represents a constant raised to a generic power, e.g. 10^x

    @author Christopher Woods
*/
class SIRECAS_EXPORT PowerConstant 
        : public Siren::Implements<PowerConstant,PowerFunction>
{
public:
    PowerConstant();
    PowerConstant(double val, const Expression &power);

    PowerConstant(const PowerConstant &other);

    ~PowerConstant();

    PowerConstant& operator=(const PowerConstant &other);
    
    bool operator==(const PowerConstant &other) const;
    bool operator!=(const PowerConstant &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression core() const;
    Expression power() const;

private:
    /** The constant value */
    double cre;

    /** The expression by which the constant is raised to the power */
    Expression pwr;
};

/** This class represents an expression raised to a constant power. This is the
    base class of RationalPower (expression raised to a rational power) and
    RealPower (expression raised to a non-rational power).

    @author Christopher Woods
*/
class SIRECAS_EXPORT ConstantPower 
        : public Siren::Extends<ConstantPower,PowerFunction>
{
public:
    ConstantPower();
    ConstantPower(const Expression &expression);

    ~ConstantPower();

    Expression core() const;

    void stream(Siren::Stream &s);

protected:
    ConstantPower(const ConstantPower &other);

    ConstantPower& operator=(const ConstantPower &other);
    bool operator==(const ConstantPower &other) const;
    bool operator!=(const ConstantPower &other) const;

    /** The expression that is raised to a power */
    Expression ex;
};

/** This class represents an expression raised to a constant integer power */
class SIRECAS_EXPORT IntegerPower 
        : public Siren::Implements<IntegerPower,ConstantPower>
{
public:
    IntegerPower();
    IntegerPower(const Expression &expression, int power);

    IntegerPower(const IntegerPower &other);

    ~IntegerPower();

    IntegerPower& operator=(const IntegerPower &other);
    
    bool operator==(const IntegerPower &other) const;
    bool operator!=(const IntegerPower &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression power() const;

private:
    /** The integer power */
    qint32 pwr;
};


/** This class represents an expression raised to a rational power */
class SIRECAS_EXPORT RationalPower 
        : public Siren::Implements<RationalPower,ConstantPower>
{
public:
    RationalPower();
    RationalPower(const Expression &expression, const SireMaths::Rational &power);

    RationalPower(const RationalPower &other);

    ~RationalPower();

    RationalPower& operator=(const RationalPower &other);
    
    bool operator==(const RationalPower &other) const;
    bool operator!=(const RationalPower &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression power() const;

private:
    /** The rational power */
    SireMaths::Rational pwr;
};

/** This class represents an expression raised to a real power */
class SIRECAS_EXPORT RealPower 
        : public Siren::Implements<RealPower,ConstantPower>
{
public:
    RealPower();
    RealPower(const Expression &expression, double power);

    RealPower(const RealPower &other);

    ~RealPower();

    RealPower& operator=(const RealPower &other);
    
    bool operator==(const RealPower &other) const;
    bool operator!=(const RealPower &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression power() const;

private:
    /** The real power */
    double pwr;
};

/** This class represents an expression raised to a complex power */
class SIRECAS_EXPORT ComplexPower 
            : public Siren::Implements<ComplexPower,ConstantPower>
{
public:
    ComplexPower();
    ComplexPower(const Expression &expression, const SireMaths::Complex &power);

    ComplexPower(const ComplexPower &other);

    ~ComplexPower();
    
    ComplexPower& operator=(const ComplexPower &other);
    
    bool operator==(const ComplexPower &other) const;
    bool operator!=(const ComplexPower &other) const;
    
    void stream(Siren::Stream &s);
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression power() const;

    bool isComplex() const;

private:
    /** The complex power */
    SireMaths::Complex pwr;
};

}

Q_DECLARE_METATYPE(SireCAS::PowerConstant)
Q_DECLARE_METATYPE(SireCAS::IntegerPower)
Q_DECLARE_METATYPE(SireCAS::RationalPower)
Q_DECLARE_METATYPE(SireCAS::RealPower)
Q_DECLARE_METATYPE(SireCAS::ComplexPower)

SIRE_EXPOSE_CLASS( SireCAS::PowerConstant )
SIRE_EXPOSE_CLASS( SireCAS::IntegerPower )
SIRE_EXPOSE_CLASS( SireCAS::RationalPower )
SIRE_EXPOSE_CLASS( SireCAS::RealPower )
SIRE_EXPOSE_CLASS( SireCAS::ComplexPower )

SIRE_END_HEADER

#endif
