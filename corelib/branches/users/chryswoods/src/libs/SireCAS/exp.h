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

#ifndef SIRECAS_EXP_H
#define SIRECAS_EXP_H

#include "power.h"
#include "singlefunc.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This is the exponential function, e^x

    @author Christopher Woods
*/
class SIRECAS_EXPORT Exp : public Siren::Implements<Exp,PowerFunction>
{
public:
    Exp();
    Exp(const Expression &power);

    Exp(const Exp &other);

    ~Exp();

    Exp& operator=(const Exp &other);
    
    bool operator==(const Exp &other) const;
    bool operator!=(const Exp &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    Expression core() const;
    Expression power() const;

private:
    /** The expression to which 'e' is raised to */
    Expression pwr;
};

/** This is the natural logarithm (ln) function

    @author Christopher Woods
*/
class SIRECAS_EXPORT Ln : public Siren::Implements<Ln,SingleFunc>
{
public:
    Ln();
    Ln(const Expression &expression);

    Ln(const Ln &other);
    ~Ln();

    Ln& operator=(const Ln &other);
    
    bool operator==(const Ln &other) const;
    bool operator!=(const Ln &other) const;
    
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

Q_DECLARE_METATYPE(SireCAS::Exp)
Q_DECLARE_METATYPE(SireCAS::Ln)

SIRE_EXPOSE_CLASS( SireCAS::Exp )
SIRE_EXPOSE_CLASS( SireCAS::Ln )

SIRE_END_HEADER

#endif
