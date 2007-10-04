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

#ifndef SIREMOL_EVALUATOR_H
#define SIREMOL_EVALUATOR_H

#include <QVector>
#include <QHash>

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Evaluator;
}

QDataStream& operator<<(QDataStream&, const SireMol::Evaluator&);
QDataStream& operator>>(QDataStream&, SireMol::Evaluator&);

namespace SireMol
{

/** This class is used to add a nice API to the MoleculeView based classes to
    allow the evaluation of various properties of the molecule (without the
    need to clutter up the MoleculeView-based classes' APIs).

    e.g. can type mol.evaluate().center() rather than mol.center()

    @author Christopher Woods
*/
class SIREMOL_EXPORT Evaluator : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Evaluator&);
friend QDataStream& ::operator>>(QDataStream&, Evaluator&);

public:
    Evaluator();

    Evaluator(const MoleculeView &molecule);

    Evaluator(const Evaluator &other);

    ~Evaluator();

    Evaluator& operator=(const Evaluator &other);
    Evaluator& operator=(const MoleculeView &other);

    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeData &moldata) const;

    double mass(const PropertyMap &map = PropertyMap()) const;

    Vector center(const PropertyMap &map = PropertyMap()) const;
    AABox aaBox(const PropertyMap &map = PropertyMap()) const;
    Sphere sphere(const PropertyMap &map = PropertyMap()) const;

    Vector centerOfGeometry(const PropertyMap &map = PropertyMap()) const;
    Vector centerOfMass(const PropertyMap &map = PropertyMap()) const;

private:
    /** The atoms over which the properties will be 
        evaluated */
    AtomSelection selected_atoms;
};

}

Q_DECLARE_METATYPE(SireMol::Evaluator)

SIRE_END_HEADER

#endif
