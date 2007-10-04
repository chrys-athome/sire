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

#ifndef SIREMOL_RESIDUE_H
#define SIREMOL_RESIDUE_H

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Residue;
}

QDataStream& operator<<(QDataStream&, const SireMol::Residue&);
QDataStream& operator>>(QDataStream&, SireMol::Residue&);

namespace SireMol
{

/**
This class represents a Residue in a Molecule.

@author Christopher Woods
*/
class SIREMOL_EXPORT Residue : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Residue&);
friend QDataStream& ::operator>>(QDataStream&, Residue&);

public:
    Residue();
    
    Residue(const MoleculeData &moldata, const ResID &resid);

    Residue(const Residue &other);

    ~Residue();

    Residue& operator=(const Residue &other);
    
    bool operator==(const Residue &other) const;
    bool operator!=(const Residue &other) const;
    
    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeData &moldata);

    ResName name() const;
    ResNum number() const;
    ResIdx index() const;
    
    ResidueInfo info() const;
    
    template<class T>
    T property(const PropertyName &key) const;
    
    template<class T>
    T metadata(const PropertyName &metakey) const;
    
    template<class T>
    T metadata(const PropertyName &key,
               const PropertyName &metakey) const;
    
    Mover<Residue> move() const;
    Evaluator evaluate() const;
    Editor<Residue> edit() const;
    
    Molecule molecule() const;
    Chain chain() const;
    
    Atom select(const AtomID &atomid) const;
    
    AtomsInMol selectAll(const AtomID &atomid) const;
    AtomsInMol selectAll() const;
    
    Atom atom(const AtomID &atomid) const;
    
    AtomsInMol atoms(const AtomID &atomid) const;
    AtomsInMol atoms() const;

protected:
    template<class T>
    void setProperty(const QString &key, const T &value);

    template<class T>
    void setMetadata(const QString &metakey, const T &value);
    
    template<class T>
    void setMetadata(const QString &key, const QString &metakey,
                     const T &value);

private:
    /** The index of the residue in the molecule */
    ResIdx residx;
    
    /** The atoms that are selected as part of this residue */
    AtomSelection selected_atoms;
};

}

Q_DECLARE_METATYPE(SireMol::Residue)

SIRE_END_HEADER

#endif
