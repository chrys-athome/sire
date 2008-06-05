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

#ifndef SIREFF_FFCOMPONENT_H
#define SIREFF_FFCOMPONENT_H

#include "SireCAS/function.h"

#include "forcefieldid.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFComponent;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFComponent&);
QDataStream& operator>>(QDataStream&, SireFF::FFComponent&);

namespace SireFF
{

class FFBase;

using SireCAS::Function;
using SireCAS::Symbol;
using SireCAS::Symbols;

/** This is a specialisation of a SireCAS::Function that is used
    to represent a component of a forcefield. This controls the
    name of the function, allowing the encoding and decoding
    of the forcefield name, component name and forcefield ID
    number into the function's name.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFComponent : public Function
{

friend QDataStream& ::operator<<(QDataStream&, const FFComponent&);
friend QDataStream& ::operator>>(QDataStream&, FFComponent&);

public:
    FFComponent();

    FFComponent(const FFBase &ffield);

    FFComponent(const FFBase &ffield, const Symbols &symbols);

    FFComponent(const FFBase &ffield,
                const Symbol &sym0);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                const Symbol &sym9);

    FFComponent(const FFBase &ffield,
                const QString &component_name);

    FFComponent(const FFBase &ffield,
                const QString &component_name, const Symbols &symbols);

    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                const Symbol &sym9);

    FFComponent(const SireCAS::Function &function);

    FFComponent(const FFComponent &other);

    ~FFComponent();

    FFComponent& operator=(const FFComponent &other);
    FFComponent& operator=(const SireCAS::Function &function);

    QString forceFieldName() const;
    QString forceFieldComponent() const;

    ForceFieldID forceFieldID() const;

private:
    /** The ID number of the forcefield */
    ForceFieldID ffid;
};

}

Q_DECLARE_METATYPE(SireFF::FFComponent);

SIRE_END_HEADER

#endif
