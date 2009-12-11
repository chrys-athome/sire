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

#ifndef SIRECAS_CASNODE_H
#define SIRECAS_CASNODE_H

#include "Siren/object.h"
#include "Siren/objptr.hpp"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Complex;
class Rational;
}

namespace SireCAS
{

class Symbol;
class Expression;
class Identities;
class Values;
class ComplexValues;
class Symbol;
class Factor;

using SireMaths::Complex;
using SireMaths::Rational;

/** This is the virtual node class of all SireCAS nodes. A node
    is a point in a CAS expression tree (an expression is a tree
    of nodes, and is evaluated by summing the values of each node)
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT CASNode : public Siren::Extends<CASNode,Siren::Object>
{
public:
    CASNode();
    CASNode(const CASNode &other);
    
    virtual ~CASNode();

    ///////////////////////////
    // Extends Siren::Object //
    ///////////////////////////
    
    static QString typeName();
    
    void stream(Siren::Stream &s);
    
    //////////////////////
    // SireCAS::CASNode //
    //////////////////////

    virtual Expression operator-() const;

    virtual Expression differentiate(const Symbol &symbol) const;
    virtual Expression integrate(const Symbol &symbol) const;

    virtual Expression series(const Symbol &symbol, int n) const;

    virtual Expression simplify(int options=0) const;

    virtual Expression conjugate() const;

    virtual bool isFunction(const Symbol&) const;
    virtual bool isConstant() const;
    virtual bool isComplex() const;
    virtual bool isCompound() const;

    /** Evaluate this CASNode using values 'values'. Any
        missing symbols are assumed to equal zero.

        Note that an exception will be thrown if the result of the
        evaluation of this, or one of its children, is complex.

        \throw SireMaths::domain_error
    */
    virtual double evaluate(const Values &values) const=0;

    /** Evaluate this CASNode using the complex values 'values'.
        Any missing symbols are assumed to equal zero. */
    virtual Complex evaluate(const ComplexValues &values) const=0;

    /** Return an expression that has the identities in 'identities'
        substituted into this expression */
    virtual Expression substitute(const Identities &identities) const=0;

    /** Return the set of Symbols that appear in this CASNode */
    virtual QSet<Symbol> symbols() const=0;

    /** Return the child expressions of this Expression */
    virtual QList<Expression> children() const=0;
    
    /** Rearrange this expression into the form
        m x^i + n x^j + ... + constant
        and return the factors and powers of x
        
        \throw SireCAS::rearrangement_error
    */
    virtual QList<Factor> expand(const Symbol &symbol) const=0;
    
protected:
    CASNode& operator=(const CASNode &other);
    
    bool operator==(const CASNode &other) const;
    bool operator!=(const CASNode &other) const;
};

Expression operator+(const CASNode &node0, const CASNode &node1);
Expression operator+(const CASNode &node, double value);
Expression operator+(double value, const CASNode &node);
Expression operator+(const CASNode &node, const Complex &value);
Expression operator+(const Complex &value, const CASNode &node);

Expression operator-(const CASNode &node0, const CASNode &node1);
Expression operator-(const CASNode &node, double value);
Expression operator-(double value, const CASNode &node);
Expression operator-(const CASNode &node, const Complex &value);
Expression operator-(const Complex &value, const CASNode &node);

Expression operator*(const CASNode &node0, const CASNode &node1);
Expression operator*(const CASNode &node, double value);
Expression operator*(double value, const CASNode &node);
Expression operator*(const CASNode &node, const Complex &value);
Expression operator*(const Complex &value, const CASNode &node);

Expression operator/(const CASNode &node0, const CASNode &node1);
Expression operator/(const CASNode &node, double value);
Expression operator/(double value, const CASNode &node);
Expression operator/(const CASNode &node, const Complex &value);
Expression operator/(const Complex &value, const CASNode &node);

Expression pow(const CASNode &node, int n);
Expression pow(const CASNode &node, const Rational &n);
Expression pow(const CASNode &node, double n);
Expression pow(const CASNode &node, const Complex &n);
Expression pow(const CASNode &node, const CASNode &n);

Expression root(const CASNode &node, int n);
Expression sqrt(const CASNode &node);
Expression cbrt(const CASNode &node);

typedef Siren::ObjPtr<CASNode> CASNodePtr;

}

SIRE_EXPOSE_CLASS( SireCAS::CASNode )

SIRE_EXPOSE_OBJECT_PTR( SireCAS::CASNodePtr, SireCAS::CASNode )

SIRE_END_HEADER

#endif
