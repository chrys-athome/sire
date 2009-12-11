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

#ifndef SIRECAS_FACTOR_H
#define SIRECAS_FACTOR_H

#include "symbol.h"
#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This is a small class that can hold the factor and power of a symbol

    @author Christopher Woods
*/
class SIRECAS_EXPORT Factor : public Siren::Implements<Factor,Siren::Object>
{
public:
    Factor();
    
    Factor(const Symbol &symbol, double factor, double power);
    Factor(const Symbol &symbol,
           const Expression &factor, const Expression &power);
    
    Factor(const Factor &other);
    
    ~Factor();
    
    Factor& operator=(const Factor &other);
    
    bool operator==(const Factor &other) const;
    bool operator!=(const Factor &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    const Symbol& symbol() const;

    const Expression& factor() const;
    
    const Expression& power() const;

private:
    /** The symbol for the factor */
    Symbol s;

    /** The factor and power */
    Expression f, p;
};

}

Q_DECLARE_METATYPE( SireCAS::Factor )

SIRE_EXPOSE_CLASS( SireCAS::Factor )

SIRE_END_HEADER

#endif
