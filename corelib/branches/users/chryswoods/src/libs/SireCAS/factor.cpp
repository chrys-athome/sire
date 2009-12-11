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

#include "factor.h"

using namespace SireCAS;

////////
//////// Implementation of Factor
////////

Factor::Factor() : f(1), p(1)
{}

Factor::Factor(const Symbol &symbol,
               const Expression &factor, const Expression &power)
       : s(symbol), f(factor), p(power)
{}

Factor::Factor(const Symbol &symbol, 
               double factor, double power) 
       : s(symbol), f(factor), p(power)
{}

Factor::Factor(const Factor &other) : s(other.s), f(other.f), p(other.p)
{}

Factor::~Factor()
{}

Factor& Factor::operator=(const Factor &other)
{
    f = other.f;
    p = other.p;
    s = other.s;
    
    return *this;
}

bool Factor::operator==(const Factor &other) const
{
    return f == other.f and p == other.p and s == other.s;
}

bool Factor::operator!=(const Factor &other) const
{
    return f != other.f or p != other.p or s != other.s;
}

static QString get_string(const Expression &expression)
{
    if (expression.isCompound())
    {
        return QString("[%1]").arg(expression.toString());
    }
    else
        return expression.toString();
}

QString Factor::toString() const
{
    return QString("%1 %2^%3").arg(::get_string(f), s.toString(), ::get_string(p));
}
