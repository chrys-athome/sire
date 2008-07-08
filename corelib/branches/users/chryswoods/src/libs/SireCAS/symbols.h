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

#ifndef SIRECAS_SYMBOLS_H
#define SIRECAS_SYMBOLS_H

#include <QSet>

#include "symbol.h"

#include "tostring.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Trival derivation of QSet<Symbol> that adds a constructor that
    automatically adds the passed Symbol 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT Symbols : public QSet<Symbol>
{
public:
    Symbols() : QSet<Symbol>()
    {}
    
    Symbols(const Symbol &sym) : QSet<Symbol>()
    {
        this->insert(sym);
    }
    
    Symbols(const QSet<Symbol> &other) : QSet<Symbol>(other)
    {}
    
    Symbols(const QList<Symbol> &other) : QSet<Symbol>()
    {
        int n = other.count();
        for (int i=0; i<n; ++i)
            insert( other.at(i) );
    }
    
    ~Symbols()
    {}
    
    QString toString() const
    {
        return Sire::toString( QSet<Symbol>(*this) );
    }
    
    void insert(const Symbol &symbol)
    {
        QSet<Symbol>::insert(symbol);
    }
    
    void insert(const Symbols &symbols)
    {
        for (Symbols::const_iterator it = symbols.begin();
             it != symbols.end();
             ++it)
        {
            this->insert(*it);
        }
    }
};

}

SIRE_END_HEADER

#endif
