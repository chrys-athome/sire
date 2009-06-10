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

#ifndef SIRECAS_CONDITIONAL_H
#define SIRECAS_CONDITIONAL_H

#include "exbase.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Conditional;
class Condition;

class GreaterThan;
class LessThan;
class GreaterOrEqualThan;
class LessOrEqualThan;
class EqualTo;
class NotEqualTo;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Conditional&);
QDataStream& operator>>(QDataStream&, SireCAS::Conditional&);

QDataStream& operator<<(QDataStream&, const SireCAS::Condition&);
QDataStream& operator>>(QDataStream&, SireCAS::Condition&);

QDataStream& operator<<(QDataStream&, const SireCAS::GreaterThan&);
QDataStream& operator>>(QDataStream&, SireCAS::GreaterThan&);

QDataStream& operator<<(QDataStream&, const SireCAS::LessThan&);
QDataStream& operator>>(QDataStream&, SireCAS::LessThan&);

QDataStream& operator<<(QDataStream&, const SireCAS::GreaterOrEqualThan&);
QDataStream& operator>>(QDataStream&, SireCAS::GreaterOrEqualThan&);

QDataStream& operator<<(QDataStream&, const SireCAS::LessOrEqualThan&);
QDataStream& operator>>(QDataStream&, SireCAS::LessOrEqualThan&);

QDataStream& operator<<(QDataStream&, const SireCAS::EqualTo&);
QDataStream& operator>>(QDataStream&, SireCAS::EqualTo&);

QDataStream& operator<<(QDataStream&, const SireCAS::NotEqualTo&);
QDataStream& operator>>(QDataStream&, SireCAS::NotEqualTo&);

namespace SireCAS
{

/** This is the base class of all conditional statements 
    (e.g. x < 2*y, or z == 3*w)
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT Condition : public ExBase
{

friend QDataStream& ::operator<<(QDataStream&, const Condition&);
friend QDataStream& ::operator>>(QDataStream&, const Condition&);

public:
    Condition();
    Condition(const Expression &lhs, const Expression &rhs);
    
    Condition(const Condition &other);
    
    ~Condition();

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;

    bool isNull() const;

    uint hash() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;

    Symbols symbols() const;
    Functions functions() const;
    Expressions children() const;

    QList<Factor> expand(const Symbol &symbol) const;
    
    bool evaluateCondition(const Values &values) const;
    bool evaluateCondition(const ComplexValues &values) const;
    
protected:
    Condition& operator=(const Condition &other);
    
    bool operator==(const Condition &other) const;
    bool operator!=(const Condition &other) const;
    
    virtual bool compareValues(double val0, double val1) const=0;
};

/** This is a conditional function that returns whether or 
    not the first expression is greater than the second
    expression 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT GreaterThan : public Condition
{

friend QDataStream& ::operator<<(QDataStream&, const GreaterThan&);
friend QDataStream& ::operator>>(QDataStream&, GreaterThan&);

public:
    GreaterThan();
    GreaterThan(const Expression &ex0, const Expression &ex1);
    
    GreaterThan(const GreaterThan &other);
    
    ~GreaterThan();
    
    GreaterThan& operator=(const GreaterThan &other);
    
    bool operator==(const GreaterThan &other) const;
    bool operator==(const ExBase &other) const;

    static const char* typeName();
    
    GreaterThan* clone() const
    {
        return new GreaterThan(*this);
    }

protected:
    bool compareValues(double val0, double val1) const;
};

}

Q_DECLARE_METATYPE( SireCAS::Conditional )

Q_DECLARE_METATYPE( SireCAS::GreaterThan )
Q_DECLARE_METATYPE( SireCAS::LessThan )
Q_DECLARE_METATYPE( SireCAS::GreaterOrEqualThan )
Q_DECLARE_METATYPE( SireCAS::LessOrEqualThan )
Q_DECLARE_METATYPE( SireCAS::EqualTo )
Q_DECLARE_METATYPE( SireCAS::NotEqualTo )

SIRE_EXPOSE_CLASS( SireCAS::Conditional )
SIRE_EXPOSE_CLASS( SireCAS::Condition )
SIRE_EXPOSE_CLASS( SireCAS::GreaterThan )
SIRE_EXPOSE_CLASS( SireCAS::LessThan )
SIRE_EXPOSE_CLASS( SireCAS::GreaterOrEqualThan )
SIRE_EXPOSE_CLASS( SireCAS::LessOrEqualThan )
SIRE_EXPOSE_CLASS( SireCAS::EqualTo )
SIRE_EXPOSE_CLASS( SireCAS::NotEqualTo )

#endif
