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

#ifndef SIRECAS_TRIGFUNCS_H
#define SIRECAS_TRIGFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Cosine */
class SIRECAS_EXPORT Cos : public Siren::Implements<Cos,SingleFunc>
{
public:
    Cos();
    Cos(const Expression &ex);

    Cos(const Cos &other);

    ~Cos();

    Cos& operator=(const Cos &other);
    
    bool operator==(const Cos &other) const;
    bool operator!=(const Cos &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Sine */
class SIRECAS_EXPORT Sin : public Siren::Implements<Sin,SingleFunc>
{
public:
    Sin();
    Sin(const Expression &ex);

    Sin(const Sin &other);

    ~Sin();

    Sin& operator=(const Sin &other);
    
    bool operator==(const Sin &other) const;
    bool operator!=(const Sin &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Tangent */
class SIRECAS_EXPORT Tan : public Siren::Implements<Tan,SingleFunc>
{
public:
    Tan();
    Tan(const Expression &ex);

    Tan(const Tan &other);

    ~Tan();

    Tan& operator=(const Tan &other);
    
    bool operator==(const Tan &other) const;
    bool operator!=(const Tan &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Secant */
class SIRECAS_EXPORT Sec : public Siren::Implements<Sec,SingleFunc>
{
public:
    Sec();
    Sec(const Expression &ex);

    Sec(const Sec &other);

    ~Sec();

    Sec& operator=(const Sec &other);
    
    bool operator==(const Sec &other) const;
    bool operator!=(const Sec &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Cosecant */
class SIRECAS_EXPORT Csc : public Siren::Implements<Csc,SingleFunc>
{
public:
    Csc();
    Csc(const Expression &ex);

    Csc(const Csc &other);

    ~Csc();

    Csc& operator=(const Csc &other);
    
    bool operator==(const Csc &other) const;
    bool operator!=(const Csc &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Cotangent */
class SIRECAS_EXPORT Cot : public Siren::Implements<Cot,SingleFunc>
{
public:
    Cot();
    Cot(const Expression &ex);

    Cot(const Cot &other);

    ~Cot();

    Cot& operator=(const Cot &other);
    
    bool operator==(const Cot &other) const;
    bool operator!=(const Cot &other) const;
    
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

Q_DECLARE_METATYPE(SireCAS::Cos)
Q_DECLARE_METATYPE(SireCAS::Sin)
Q_DECLARE_METATYPE(SireCAS::Tan)
Q_DECLARE_METATYPE(SireCAS::Csc)
Q_DECLARE_METATYPE(SireCAS::Sec)
Q_DECLARE_METATYPE(SireCAS::Cot)

SIRE_EXPOSE_CLASS( SireCAS::Cos )
SIRE_EXPOSE_CLASS( SireCAS::Sin )
SIRE_EXPOSE_CLASS( SireCAS::Tan )
SIRE_EXPOSE_CLASS( SireCAS::Csc )
SIRE_EXPOSE_CLASS( SireCAS::Sec )
SIRE_EXPOSE_CLASS( SireCAS::Cot )

SIRE_END_HEADER

#endif
