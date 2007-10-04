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

#ifndef SIREMOL_SEGMENT_H
#define SIREMOL_SEGMENT_H

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Segment;
}

QDataStream& operator<<(QDataStream&, const SireMol::Segment&);
QDataStream& operator>>(QDataStream&, SireMol::Segment&);

namespace SireMol
{

/** This is a view of a single segment within a molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT Segment : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Segment&);
friend QDataStream& ::operator>>(QDataStream&, Segment&);

public:
    Segment();
    
    Segment(const MoleculeData &data, const SegID &segid);
    
    Segment(const Segment &other);
    
    ~Segment();
    
    Segment& operator=(const Segment &other);
    
    bool operator==(const Segment &other) const;
    bool operator!=(const Segment &other) const;
    
    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeData &moldata) const;
    
    SegName name() const;
    SegIdx index() const;
    
    SegmentInfo info() const;
    
    Mover<Segment> move() const;
    Evaluator evaluate() const;
    Editor<Segment> edit() const;
    
    Molecule molecule() const;
    
    Atom select(const AtomID &atomid) const;
    AtomsInMol selectAll(const AtomID &atomid) const;
    AtomsInMol selectAll() const;
    
    Atom atom(const AtomID &atomid) const;
    AtomsInMol atoms(const AtomID &atomid) const;
    AtomsInMol atoms() const;

    template<class T>
    T property(const PropertyName &key) const;
    
    template<class T>
    T metadata(const PropertyName &key,
               const PropertyName &metakey) const;

protected:
    template<class T>
    void setProperty(const QString &key, const T &value);
    
    template<class T>
    void setMetadata(const QString &key, const QString &metakey,
                     const T &value);

private:
    /** The index of the segment in the molecule */
    SegIdx segidx;
    
    /** The atoms that are part of this segment */
    AtomSelection selected_atoms;
};

}

Q_DECLARE_METATYPE(SireMol::Segment);

SIRE_END_HEADER

#endif
