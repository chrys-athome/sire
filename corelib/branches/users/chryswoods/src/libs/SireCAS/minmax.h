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

#ifndef SIRECAS_MINMAX_H
#define SIRECAS_MINMAX_H

#include "doublefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Minimum value of two expressions */
class SIRECAS_EXPORT Min : public Siren::Implements<Min,DoubleFunc>
{
public:
    Min();
    Min(const Expression &x, const Expression &y);

    Min(const Min &other);

    ~Min();

    Min& operator=(const Min &other);
    
    bool operator==(const Min &other) const;
    bool operator!=(const Min &other) const;
    
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &x, const Expression &y) const;
    QString stringRep() const;
};

/** Maximum value of two expressions */
class SIRECAS_EXPORT Max : public Siren::Implements<Max,DoubleFunc>
{
public:
    Max();
    Max(const Expression &x, const Expression &y);

    Max(const Max &other);

    ~Max();

    Max& operator=(const Max &other);
    
    bool operator==(const Max &other) const;
    bool operator!=(const Max &other) const;

    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &x, const Expression &y) const;
    QString stringRep() const;
};

}

Q_DECLARE_METATYPE( SireCAS::Min )
Q_DECLARE_METATYPE( SireCAS::Max )

SIRE_EXPOSE_CLASS( SireCAS::Min )
SIRE_EXPOSE_CLASS( SireCAS::Max )

SIRE_END_HEADER

#endif
