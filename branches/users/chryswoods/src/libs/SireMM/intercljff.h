/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMM_INTERCLJFF_H
#define SIREMM_INTERCLJFF_H

#include "cljff.h"
#include "inter2bodyff.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterCLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterCLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterCLJFF&);

namespace SireMM
{

/** An InterCLJFF is used to calculate the
    intermolecular coulomb and LJ
    energy of a group of molecules.

    @author Christopher Woods
*/
class SIREMM_EXPORT InterCLJFF : public Inter2BodyFF<CLJFF>
{

friend QDataStream& ::operator<<(QDataStream&, const InterCLJFF&);
friend QDataStream& ::operator>>(QDataStream&, InterCLJFF&);

public:
    InterCLJFF();

    InterCLJFF(const Space &space, const SwitchingFunction &switchingfunction);

    InterCLJFF(const InterCLJFF &other);

    ~InterCLJFF();

    static const char* typeName()
    {
        return "SireMM::InterCLJFF";
    }

    const char* what() const
    {
        return InterCLJFF::typeName();
    }

    InterCLJFF* clone() const
    {
        return new InterCLJFF(*this);
    }
};

}

Q_DECLARE_METATYPE(SireMM::InterCLJFF);

SIRE_END_HEADER

#endif
