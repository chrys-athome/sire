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

#ifndef SIRECAS_SINGLEFUNC_H
#define SIRECAS_SINGLEFUNC_H

#include "symbol.h"
#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Base class of all single-expression functions (e.g. g( f(??) ))

    @author Christopher Woods
*/
class SIRECAS_EXPORT SingleFunc : public Siren::Extends<SingleFunc,CASNode>
{
public:
    SingleFunc();
    SingleFunc(const Expression &ex);

    SingleFunc(const SingleFunc &other);

    ~SingleFunc();

    static QString typeName();

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    const Expression& argument() const;
    const Expression& x() const;

    Expression conjugate() const;

    bool isFunction(const Symbol &symbol) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    Expression substitute(const Identities &identities) const;
    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    QList<Factor> expand(const Symbol &symbol) const;

protected:
    SingleFunc& operator=(const SingleFunc &other);
    
    bool operator==(const SingleFunc &other) const;
    bool operator!=(const SingleFunc &other) const;

    virtual Expression functionOf(const Expression &arg) const=0;
    virtual QString stringRep() const=0;

    virtual Expression diff() const;
    virtual Expression integ() const;

    /** The expression that this function operates on */
    Expression ex;
};

/** To declare a new function, copy the below;

class MyFunc : public Siren::Implements<MyFunc,SingleFunc>
{
public:
    MyFunc();
    MyFunc(const Expression &ex);

    MyFunc(const MyFunc &other);

    ~MyFunc();

    /// optional functions
    //Expression series(const Symbol &symbol, int n) const;
    //Expression simplify(int options=0) const;

    /// required functions
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return *this;
        else
            return MyFunc(arg);
    }

    //optional
    Expression diff() const;
    Expression integ() const;

    QString stringRep() const
    {
        return "myfunc";
    }
};

Q_DECLARE_METATYPE( MyFunc )

SIRE _ EXPOSE_CLASS( MyFunc )

static RegisterObject<MyFunc> r_myfunc;

*/

}

SIRE_EXPOSE_CLASS( SireCAS::SingleFunc )

SIRE_END_HEADER

#endif
