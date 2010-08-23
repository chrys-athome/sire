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

#ifndef SIREMOVE_MOVERMOVE_H
#define SIREMOVE_MOVERMOVE_H

#include "montecarlo.h"
#include "sampler.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MoverMove;
class DofID;
}

QDataStream& operator<<(QDataStream&, const SireMove::MoverMove&);
QDataStream& operator>>(QDataStream&, SireMove::MoverMove&);
QDataStream& operator<<(QDataStream&, const SireMove::DofID&);
QDataStream& operator>>(QDataStream&, SireMove::DofID&);


namespace SireMol
{
class MoleculeGroup;
class PartialMolecule;
class AtomIdx;
class Connectivity;
class AtomID;
class BondID;
class AngleID;
class DihedralID;
}

namespace SireMove
{

class Sampler;
class ZMatrixCoords;

using SireMol::MoleculeGroup;
using SireMol::PartialMolecule;
using SireMol::Connectivity;
using SireMol::AtomIdx;
using SireMol::AtomID;
using SireMol::BondID;
using SireMol::AngleID;
using SireMol::DihedralID;

/** This class implements an intramolecular Monte Carlo move that uses 
    the mover() method to perturb intramolecular degrees of freedom and 
    that may be applied to a random molecule (or part of a molecule)
    within a MoleculeGroup. It is based on the ZMatMove class.
    
    @author Julien Michel
*/
class SIREMOVE_EXPORT MoverMove 
            : public SireBase::ConcreteProperty<MoverMove,MonteCarlo>
{

friend QDataStream& ::operator<<(QDataStream&, const MoverMove&);
friend QDataStream& ::operator>>(QDataStream&, MoverMove&);

public:
    MoverMove();
    
    MoverMove(const MoleculeGroup &molgroup);
    MoverMove(const Sampler &sampler);
    
    MoverMove(const MoverMove &other);
    
    ~MoverMove();
    
    MoverMove& operator=(const MoverMove &other);
    
    static const char* typeName();

    bool operator==(const MoverMove &other) const;
    bool operator!=(const MoverMove &other) const;

    QString toString() const;

    void setSampler(const Sampler &sampler);
    void setSampler(const MoleculeGroup &molgroup);

    const Sampler& sampler() const;
    const MoleculeGroup& moleculeGroup() const;
    
    void setGenerator(const RanGenerator &rangenerator);

    void move(System &system, int nmoves, bool record_stats=true);
    void changeDeltas(float prob, float scale);
    void setDelta(const BondID &bond, SireUnits::Dimension::Length delta);
    void setDelta(const AngleID &angle, SireUnits::Dimension::Angle delta);
    void setDelta(const DihedralID &dihedral, SireUnits::Dimension::Angle delta);

    void setBonds(const QList<BondID> &bonds);
    void setAngles(const QList<AngleID> &angles);
    void setDihedrals(const QList<DihedralID> &dihedrals);    
    void setBondDeltas(const QHash<DofID,SireUnits::Dimension::Length> &bond_deltas);
    void setAngleDeltas(const QHash<DofID,SireUnits::Dimension::Angle> &angle_deltas);

    const QList<BondID>& getBonds(); 
    const QList<AngleID>& getAngles(); 
    const QList<DihedralID>& getDihedrals(); 
    const QHash<DofID,SireUnits::Dimension::Length>& getBondDeltas();
    const QHash<DofID,SireUnits::Dimension::Angle>& getAngleDeltas();

protected:
    void _pvt_setTemperature(const SireUnits::Dimension::Temperature &temperature);

private:
    /** The sampler used to select random molecules for the move */
    SamplerPtr smplr;
    /** The bonds that can be moved */
    QList<BondID> bonds;
    /** The angles that can be moved */
    QList<AngleID> angles;
    /** The dihedrals that can be moved */
    QList<DihedralID> dihedrals;    
    /** The list of delta values for bonds*/
    QHash<DofID,SireUnits::Dimension::Length> bond_deltas;
    /** The list of delta values for angle/dihedrals*/
    QHash<DofID,SireUnits::Dimension::Angle> angle_deltas;
 };

/** This class implements an unique label of degrees of freedom based 
    on atomic indices. It is based on the IDQuad class from SireMM/fouratomfunctions.h
    
    @author Julien Michel
*/
class SIREMOVE_EXPORT DofID
{

friend QDataStream& ::operator<<(QDataStream&, const DofID&);
friend QDataStream& ::operator>>(QDataStream&, DofID&);

public:
    DofID(qint32 index0=-1, qint32 index1=-1, 
	  qint32 index2=-1, qint32 index3=-1);

    DofID(const AtomID &atom0, const AtomID &atom1);
    DofID(const AtomID &atom0, const AtomID &atom1, const AtomID &atom2);
    DofID(const AtomID &atom0, const AtomID &atom1, const AtomID &atom2, const AtomID &atom3);
    DofID(const AtomIdx &atom0, const AtomIdx &atom1);
    DofID(const AtomIdx &atom0, const AtomIdx &atom1, const AtomIdx &atom2);
    DofID(const AtomIdx &atom0, const AtomIdx &atom1, const AtomIdx &atom2, const AtomIdx &atom3);
    DofID(const BondID &bond);
    DofID(const AngleID &angle);
    DofID(const DihedralID &dihedral);

    DofID(const DofID &other);
    
    ~DofID();
    
    DofID& operator=(const DofID &other);
    
    bool operator==(const DofID &other) const;
    bool operator!=(const DofID &other) const;
    
    //uint hash() const;
    static const char* typeName();


    AtomIdx atom0() const{ return AtomIdx(idx0); }
    AtomIdx atom1() const{ return AtomIdx(idx1); }
    AtomIdx atom2() const{ return AtomIdx(idx2); }
    AtomIdx atom3() const{ return AtomIdx(idx3); } 

private:
    qint32 idx0;
    qint32 idx1;
    qint32 idx2;
    qint32 idx3; 

};

inline uint qHash(const DofID &dofid)
{
  return (dofid.atom0().value() << 24) |
           ( ( dofid.atom1().value() << 16) & 0x00FF0000) |
           ( ( dofid.atom2().value() << 8)  & 0x0000FF00) |
           ( dofid.atom3().value() & 0x000000FF); 
}


}

Q_DECLARE_METATYPE( SireMove::MoverMove )
Q_DECLARE_METATYPE( SireMove::DofID )

SIRE_EXPOSE_CLASS( SireMove::MoverMove )
SIRE_EXPOSE_CLASS( SireMove::DofID )

SIRE_END_HEADER

#endif
