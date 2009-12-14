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

#ifndef SIRECAS_VALUES_H
#define SIRECAS_VALUES_H

#include <QHash>
#include <QList>

#include "symbol.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

class ComplexValues;

/**
    This class holds a set of Symbols and their associated values. This is used
    when numerically evaluating an equation.

    @author Christopher Woods
*/
class SIRECAS_EXPORT Values : public Siren::Implements<Values,Siren::Object>
{
public:
    Values();
    Values(const Symbol &symbol, double value);
    Values(const QHash<Symbol,double> &values);
    Values(const ComplexValues &other);

    Values(const Values &other);

    ~Values();

    Values& operator=(const Values &other);

    bool operator==(const Values &other) const;
    bool operator!=(const Values &other) const;

    Values operator+(const Values &other) const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    double value(const Symbol &sym) const;

    double operator[](const Symbol &sym) const;
    double operator()(const Symbol &sym) const;

    const QHash<SymbolID, double>& values() const;

    QList<Symbol> keys() const;
    QList<Symbol> symbols() const;

    bool isEmpty() const;

    int count() const;

    bool contains(const Symbol &symbol) const;

private:
    /** Hash mapping Symbol IDs to actual numerical values */
    QHash<SymbolID, double> vals;
};

}

Q_DECLARE_METATYPE(SireCAS::Values)

SIRE_EXPOSE_CLASS( SireCAS::Values )

SIRE_END_HEADER

#endif
