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

#ifndef SIRECAS_INVTRIGFUNCS_H
#define SIRECAS_INVTRIGFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Inverse-cosine */
class SIRECAS_EXPORT ArcCos : public Siren::Implements<ArcCos,SingleFunc>
{
public:
    ArcCos();
    ArcCos(const Expression &ex);

    ArcCos(const ArcCos &other);

    ~ArcCos();

    ArcCos& operator=(const ArcCos &other);
    
    bool operator==(const ArcCos &other) const;
    bool operator!=(const ArcCos &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-sine */
class SIRECAS_EXPORT ArcSin : public Siren::Implements<ArcSin,SingleFunc>
{
public:
    ArcSin();
    ArcSin(const Expression &ex);

    ArcSin(const ArcSin &other);

    ~ArcSin();

    ArcSin& operator=(const ArcSin &other);
    
    bool operator==(const ArcSin &other) const;
    bool operator!=(const ArcSin &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-tangent */
class SIRECAS_EXPORT ArcTan : public Siren::Implements<ArcTan,SingleFunc>
{
public:
    ArcTan();
    ArcTan(const Expression &ex);

    ArcTan(const ArcTan &other);

    ~ArcTan();

    ArcTan& operator=(const ArcTan &other);
    
    bool operator==(const ArcTan &other) const;
    bool operator!=(const ArcTan &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-secant */
class SIRECAS_EXPORT ArcSec : public Siren::Implements<ArcSec,SingleFunc>
{
public:
    ArcSec();
    ArcSec(const Expression &ex);

    ArcSec(const ArcSec &other);

    ~ArcSec();

    ArcSec& operator=(const ArcSec &other);
    
    bool operator==(const ArcSec &other) const;
    bool operator!=(const ArcSec &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-cosecant */
class SIRECAS_EXPORT ArcCsc : public Siren::Implements<ArcCsc,SingleFunc>
{
public:
    ArcCsc();
    ArcCsc(const Expression &ex);

    ArcCsc(const ArcCsc &other);

    ~ArcCsc();

    ArcCsc& operator=(const ArcCsc &other);
    
    bool operator==(const ArcCsc &other) const;
    bool operator!=(const ArcCsc &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const;
    QString stringRep() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-cotangent */
class SIRECAS_EXPORT ArcCot : public Siren::Implements<ArcCot,SingleFunc>
{
public:
    ArcCot();
    ArcCot(const Expression &ex);

    ArcCot(const ArcCot &other);

    ~ArcCot();

    ArcCot& operator=(const ArcCot &other);
    
    bool operator==(const ArcCot &other) const;
    bool operator!=(const ArcCot &other) const;

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

Q_DECLARE_METATYPE(SireCAS::ArcCos)
Q_DECLARE_METATYPE(SireCAS::ArcSin)
Q_DECLARE_METATYPE(SireCAS::ArcTan)
Q_DECLARE_METATYPE(SireCAS::ArcCsc)
Q_DECLARE_METATYPE(SireCAS::ArcSec)
Q_DECLARE_METATYPE(SireCAS::ArcCot)

SIRE_EXPOSE_CLASS( SireCAS::ArcCos )
SIRE_EXPOSE_CLASS( SireCAS::ArcSin )
SIRE_EXPOSE_CLASS( SireCAS::ArcTan )
SIRE_EXPOSE_CLASS( SireCAS::ArcCsc )
SIRE_EXPOSE_CLASS( SireCAS::ArcSec )
SIRE_EXPOSE_CLASS( SireCAS::ArcCot )

SIRE_END_HEADER

#endif
