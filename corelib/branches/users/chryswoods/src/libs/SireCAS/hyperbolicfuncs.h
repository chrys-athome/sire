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

#ifndef SIRECAS_HYPERBOLICFUNCS_H
#define SIRECAS_HYPERBOLICFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Hyperbolic cosine */
class SIRECAS_EXPORT Cosh : public Siren::Implements<Cosh,SingleFunc>
{
public:
    Cosh();
    Cosh(const Expression &ex);

    Cosh(const Cosh &other);

    ~Cosh();
    
    Cosh& operator=(const Cosh &other);
    
    bool operator==(const Cosh &other) const;
    bool operator!=(const Cosh &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic sine */
class SIRECAS_EXPORT Sinh : public Siren::Implements<Sinh,SingleFunc>
{
public:
    Sinh();
    Sinh(const Expression &ex);

    Sinh(const Sinh &other);

    ~Sinh();

    Sinh& operator=(const Sinh &other);
    
    bool operator==(const Sinh &other) const;
    bool operator!=(const Sinh &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic tangent */
class SIRECAS_EXPORT Tanh : public Siren::Implements<Tanh,SingleFunc>
{
public:
    Tanh();
    Tanh(const Expression &ex);

    Tanh(const Tanh &other);

    ~Tanh();

    Tanh& operator=(const Tanh &other);
    
    bool operator==(const Tanh &other) const;
    bool operator!=(const Tanh &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic secant */
class SIRECAS_EXPORT Sech : public Siren::Implements<Sech,SingleFunc>
{
public:
    Sech();
    Sech(const Expression &ex);

    Sech(const Sech &other);

    ~Sech();

    Sech& operator=(const Sech &other);
    
    bool operator==(const Sech &other) const;
    bool operator!=(const Sech &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic cosecant */
class SIRECAS_EXPORT Csch : public Siren::Implements<Csch,SingleFunc>
{
public:
    Csch();
    Csch(const Expression &ex);

    Csch(const Csch &other);

    ~Csch();

    Csch& operator=(const Csch &other);
    
    bool operator==(const Csch &other) const;
    bool operator!=(const Csch &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic cotangent */
class SIRECAS_EXPORT Coth : public Siren::Implements<Coth,SingleFunc>
{
public:
    Coth();
    Coth(const Expression &ex);

    Coth(const Coth &other);

    ~Coth();

    Coth& operator=(const Coth &other);
    
    bool operator==(const Coth &other) const;
    bool operator!=(const Coth &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

}

Q_DECLARE_METATYPE(SireCAS::Cosh)
Q_DECLARE_METATYPE(SireCAS::Sinh)
Q_DECLARE_METATYPE(SireCAS::Tanh)
Q_DECLARE_METATYPE(SireCAS::Csch)
Q_DECLARE_METATYPE(SireCAS::Sech)
Q_DECLARE_METATYPE(SireCAS::Coth)

SIRE_EXPOSE_CLASS( SireCAS::Cosh )
SIRE_EXPOSE_CLASS( SireCAS::Sinh )
SIRE_EXPOSE_CLASS( SireCAS::Tanh )
SIRE_EXPOSE_CLASS( SireCAS::Csch )
SIRE_EXPOSE_CLASS( SireCAS::Sech )
SIRE_EXPOSE_CLASS( SireCAS::Coth )

SIRE_END_HEADER

#endif
