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

XMLStream& operator<<(XMLStream&, const SireMM::AtomicCharges&);
XMLStream& operator>>(XMLStream&, SireMM::AtomicCharges&);

namespace SireMM
{

using SireMol::MoleculeInfo;
using SireMol::AtomSelection;
using SireMol::CGAtomID;
using SireBase::Property;

/** This class is used to hold all of the atomic charges for the
    atoms in a molecule. All of the atomic charges are held in
    groups that correspond to the CutGroups in the molecule.

    This is really just a thin-wrapper around
    QVector< QVector<ChargeParameter> >

    @author Christopher Woods
*/
class SIREMM_EXPORT AtomicCharges
        : public SireBase::ConcreteProperty<AtomicCharges,SireMol::AtomicProperties>,
          public QVector< QVector<ChargeParameter> >
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicCharges&);
friend QDataStream& ::operator>>(QDataStream&, AtomicCharges&);

friend XMLStream& ::operator<<(XMLStream&, const AtomicCharges&);
friend XMLStream& ::operator>>(XMLStream&, AtomicCharges&);

public:
    AtomicCharges();

    AtomicCharges(const QVector< QVector<ChargeParameter> > &charges);

    AtomicCharges(const QVector<ChargeParameter> &charges);

    AtomicCharges(const Property &property);

    AtomicCharges(const AtomicCharges &other);

    ~AtomicCharges();

    AtomicCharges& operator=(const QVector< QVector<ChargeParameter> > &charges);
    AtomicCharges& operator=(const QVector<ChargeParameter> &charges);

    AtomicCharges& operator=(const AtomicCharges &other);

    bool operator==(const AtomicCharges &other) const;
    bool operator!=(const AtomicCharges &other) const;

    static const char* typeName()
    {
        return "SireMM::AtomicCharges";
    }

    Property mask(const AtomSelection &selected_atoms) const;

    bool isCompatibleWith(const MoleculeInfo &molinfo) const;

    QVariant value(const CGAtomID &cgatomid) const;
};

}

Q_DECLARE_METATYPE(SireMM::AtomicCharges);

SIRE_END_HEADER

#endif
