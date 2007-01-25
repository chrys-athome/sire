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

#ifndef SIREMM_ATOMICCHARGES_H
#define SIREMM_ATOMICCHARGES_H

#include <QVector>

#include "SireMol/atomicproperties.h"

#include "chargeparameter.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class AtomicCharges;
}

QDataStream& operator<<(QDataStream&, const SireMM::AtomicCharges&);
QDataStream& operator>>(QDataStream&, SireMM::AtomicCharges&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::CGAtomID;
using SireMol::Property;

/** This class is used to hold all of the atomic charges for the
    atoms in a molecule. All of the atomic charges are held in
    groups that correspond to the CutGroups in the molecule.

    This is really just a thin-wrapper around
    QVector< QVector<ChargeParameter> >

    @author Christopher Woods
*/
class SIREMM_EXPORT AtomicCharges : public SireMol::AtomicProperties,
                                    public QVector< QVector<ChargeParameter> >
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicCharges&);
friend QDataStream& ::operator>>(QDataStream&, AtomicCharges&);

public:
    AtomicCharges();

    AtomicCharges(const QVector< QVector<ChargeParameter> > &charges);

    AtomicCharges(const QVector<ChargeParameter> &charges);

    AtomicCharges(const Property &property);

    AtomicCharges(const AtomicCharges &other);

    ~AtomicCharges();

    AtomicCharges& operator=(const QVector< QVector<ChargeParameter> > &charges);
    AtomicCharges& operator=(const QVector<ChargeParameter> &charges);
    AtomicCharges& operator=(const Property &property);

    AtomicCharges& operator=(const AtomicCharges &other);

    static const char* typeName()
    {
        return "SireMM::AtomicCharges";
    }

    const char* what() const
    {
        return AtomicCharges::typeName();
    }

    AtomicCharges* clone() const
    {
        return new AtomicCharges(*this);
    }

    bool isCompatibleWith(const Molecule &molecule) const;

    QVariant value(const CGAtomID &cgatomid) const;
};

}

Q_DECLARE_METATYPE(SireMM::AtomicCharges);

SIRE_END_HEADER

#endif
