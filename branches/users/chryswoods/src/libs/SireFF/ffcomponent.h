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

#ifndef SIREFF_FFCOMPONENT_H
#define SIREFF_FFCOMPONENT_H

#include "SireCAS/symbol.h"

#include <QLatin1String>

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This is the base class of all Symbols that represent forcefield
    components.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT FFComponent : public SireCAS::Symbol
{
public:
    virtual ~FFComponent();
    
    static const char* typeName()
    {
        return "SireFF::FFComponent";
    }
    
    quint64 UID() const;

    QString name() const;

protected:
    FFComponent(quint64 ffuid, const QLatin1String &name);
    
    FFComponent(const FFComponent &other);
    
    static QString symbolName(quint64 ffuid, const QLatin1String &name)
    {
        return QString("E^{FF:%1}_{%2}").arg(ffuid).arg(name);
    }
};

}

SIRE_END_HEADER

#endif
