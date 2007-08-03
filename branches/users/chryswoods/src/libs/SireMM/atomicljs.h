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

#ifndef SIREMM_ATOMICLJS_H
#define SIREMM_ATOMICLJS_H

#include <QVector>

#include "SireMol/atomicproperties.h"

#include "ljparameter.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class AtomicLJs;
}

QDataStream& operator<<(QDataStream&, const SireMM::AtomicLJs&);
QDataStream& operator>>(QDataStream&, SireMM::AtomicLJs&);

XMLStream& operator<<(XMLStream&, const SireMM::AtomicLJs&);
XMLStream& operator>>(XMLStream&, SireMM::AtomicLJs&);

namespace SireMM
{

using SireMol::MoleculeInfo;
using SireMol::AtomSelection;
using SireMol::CGAtomID;

using SireBase::Property;

/** This class is used to hold all of the atomic LJ parameters for the
    atoms in a molecule. All of the atomic LJ parameters are held in
    groups that correspond to the CutGroups in the molecule.

    This is really just a thin-wrapper around
    QVector< QVector<LJParameter> >

    @author Christopher Woods
*/
class SIREMM_EXPORT AtomicLJs
      : public SireBase::ConcreteProperty<AtomicLJs,SireMol::AtomicProperties>,
        public QVector< QVector<LJParameter> >
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicLJs&);
friend QDataStream& ::operator>>(QDataStream&, AtomicLJs&);

friend XMLStream& ::operator<<(XMLStream&, const AtomicLJs&);
friend XMLStream& ::operator>>(XMLStream&, AtomicLJs&);

public:
    AtomicLJs();

    AtomicLJs(const QVector< QVector<LJParameter> > &ljparams);

    AtomicLJs(const QVector<LJParameter> &ljparams);

    AtomicLJs(const Property &property);

    AtomicLJs(const AtomicLJs &other);

    ~AtomicLJs();

    AtomicLJs& operator=(const QVector< QVector<LJParameter> > &ljparams);
    AtomicLJs& operator=(const QVector<LJParameter> &ljparams);

    AtomicLJs& operator=(const AtomicLJs &other);

    bool operator==(const AtomicLJs &other) const;
    bool operator!=(const AtomicLJs &other) const;

    static const char* typeName()
    {
        return "SireMM::AtomicLJs";
    }

    Property mask(const AtomSelection &selected_atoms) const;

    bool isCompatibleWith(const MoleculeInfo &molinfo) const;

    QVariant value(const CGAtomID &cgatomid) const;
};

}

Q_DECLARE_METATYPE(SireMM::AtomicLJs);

SIRE_END_HEADER

#endif
