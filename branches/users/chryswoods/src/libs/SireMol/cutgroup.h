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

#ifndef SIREMOL_CUTGROUP_H
#define SIREMOL_CUTGROUP_H

#include <QVector>

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CutGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::CutGroup&);
QDataStream& operator>>(QDataStream&, SireMol::CutGroup&);

namespace SireMol
{

/**
A CutGroup is a logical grouping of Atoms into a single group that is considered for
intermolecular non-bonded cutting, and for periodic boundaries.

@author Christopher Woods
*/
class SIREMOL_EXPORT CutGroup : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const CutGroup&);
friend QDataStream& ::operator>>(QDataStream&, CutGroup&);

public:
    CutGroup();

    CutGroup(const MoleculeData &moldata, const CGID &cgid);

    CutGroup(const CutGroup &other);

    ~CutGroup();

    CutGroup& operator=(const CutGroup &other);
    
    bool operator==(const CutGroup &other) const;
    bool operator!=(const CutGroup &other) const;
    
    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeData &moldata);
    
    CGName name() const;
    CGIdx index() const;
    
    CutGroupInfo info() const;
    
    template<class T>
    T property(const PropertyName &key) const;
    
    template<class T>
    T metadata(const PropertyName &metakey) const;
    
    template<class T>
    T metadata(const PropertyName &key,
               const PropertyName &metakey) const;
    
    Mover<CutGroup> move() const;
    Evaluator evaluate() const;
    Editor<CutGroup> edit() const;

    Atom select(const AtomID &atomid) const;
    AtomsInMol selectAll(const AtomID &atomid) const;
    AtomsInMol selectAll() const;
    
    Atom atom(const AtomID &atomid) const;
    AtomsInMol atoms(const AtomID &atomid) const;
    AtomsInMol atoms() const;
    
    Molecule molecule() const;

protected:
    template<class T>
    void setProperty(const QString &key, const T &value);

    template<class T>
    void setMetadata(const QString &key, const QString &metakey,
                     const T &value);
                     
    template<class T>
    void setMetadata(const QString &metakey, const T &value);

private:
    /** The index of the CutGroup */
    CGIdx cgidx;
};

}

Q_DECLARE_METATYPE(SireMol::CutGroup)

SIRE_END_HEADER

#endif
