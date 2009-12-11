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

#ifndef SIRECAS_INVHYPERBOLICFUNCS_H
#define SIRECAS_INVHYPERBOLICFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Inverse-hyperbolic-cosine */
class SIRECAS_EXPORT ArcCosh : public Siren::Implements<ArcCosh,SingleFunc>
{
public:
    ArcCosh();
    ArcCosh(const Expression &ex);

    ArcCosh(const ArcCosh &other);

    ~ArcCosh();

    ArcCosh& operator=(const ArcCosh &other);
    
    bool operator==(const ArcCosh &other) const;
    bool operator!=(const ArcCosh &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-sine */
class SIRECAS_EXPORT ArcSinh : public Siren::Implements<ArcSinh,SingleFunc>
{
public:
    ArcSinh();
    ArcSinh(const Expression &ex);

    ArcSinh(const ArcSinh &other);

    ~ArcSinh();

    ArcSinh& operator=(const ArcSinh &other);
    
    bool operator==(const ArcSinh &other) const;
    bool operator!=(const ArcSinh &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-tangent */
class SIRECAS_EXPORT ArcTanh : public Siren::Implements<ArcTanh,SingleFunc>
{
public:
    ArcTanh();
    ArcTanh(const Expression &ex);

    ArcTanh(const ArcTanh &other);

    ~ArcTanh();

    ArcTanh& operator=(const ArcTanh &other);
    
    bool operator==(const ArcTanh &other) const;
    bool operator!=(const ArcTanh &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-secant */
class SIRECAS_EXPORT ArcSech : public Siren::Implements<ArcSech,SingleFunc>
{
public:
    ArcSech();
    ArcSech(const Expression &ex);

    ArcSech(const ArcSech &other);

    ~ArcSech();

    ArcSech& operator=(const ArcSech &other);
    
    bool operator==(const ArcSech &other) const;
    bool operator!=(const ArcSech &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-cosecant */
class SIRECAS_EXPORT ArcCsch : public Siren::Implements<ArcCsch,SingleFunc>
{
public:
    ArcCsch();
    ArcCsch(const Expression &ex);

    ArcCsch(const ArcCsch &other);

    ~ArcCsch();

    ArcCsch& operator=(const ArcCsch &other);
    
    bool operator==(const ArcCsch &other) const;
    bool operator!=(const ArcCsch &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-cotangent */
class SIRECAS_EXPORT ArcCoth : public Siren::Implements<ArcCoth,SingleFunc>
{
public:
    ArcCoth();
    ArcCoth(const Expression &ex);

    ArcCoth(const ArcCoth &other);

    ~ArcCoth();

    ArcCoth& operator=(const ArcCoth &other);
    
    bool operator==(const ArcCoth &other) const;
    bool operator!=(const ArcCoth &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

}

Q_DECLARE_METATYPE(SireCAS::ArcCosh)
Q_DECLARE_METATYPE(SireCAS::ArcSinh)
Q_DECLARE_METATYPE(SireCAS::ArcTanh)
Q_DECLARE_METATYPE(SireCAS::ArcCsch)
Q_DECLARE_METATYPE(SireCAS::ArcSech)
Q_DECLARE_METATYPE(SireCAS::ArcCoth)

SIRE_EXPOSE_CLASS( SireCAS::ArcCosh )
SIRE_EXPOSE_CLASS( SireCAS::ArcSinh )
SIRE_EXPOSE_CLASS( SireCAS::ArcTanh )
SIRE_EXPOSE_CLASS( SireCAS::ArcCsch )
SIRE_EXPOSE_CLASS( SireCAS::ArcSech )
SIRE_EXPOSE_CLASS( SireCAS::ArcCoth )

SIRE_END_HEADER

#endif
