#ifndef SIREMOL_ANGLE_H
#define SIREMOL_ANGLE_H

#include "SireMol/atomindex.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Angle;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::Angle&);
QDataStream& operator>>(QDataStream&, SireMol::Angle&);

uint qHash(const SireMol::Angle&);

namespace SireMol
{

/** 
This class represents an angle between atoms - this provides the AtomIndexes of the atoms between which the 
angle is made. Note that this *only* provides the AtomIndexes, and not the actual Atoms. This
means that it is not possible to get the size of this angle using only this class. To
get the size of the angle you must do;

\code
SireMaths::Angle ang = mol->size(angle);
\endcode

Note that the atoms are sorted (so the first atom is < the third atom).
This is to ensure that the angle between atom0-atom1-atom2 is identical to the angle
between atom2-atom1-atom0.

@author Christopher Woods
*/
class SIREMOL_EXPORT Angle
{

friend QDataStream& ::operator<<(QDataStream&, const SireMol::Angle&);
friend QDataStream& ::operator>>(QDataStream&, SireMol::Angle&);

public:
    
    typedef AtomIndex value_type;
    
    Angle();
    Angle(const AtomIndex &atom0, const AtomIndex &atom1,
          const AtomIndex &atom2);
    Angle(const tuple<AtomIndex,AtomIndex,AtomIndex> &tuple);
    Angle(const QString &atom0, const QString &atom1, 
          const QString &atom2, ResNum resnum);
    Angle(const QString &atm0, ResNum res0, const QString &atm1, ResNum res1,
          const QString &atm2, ResNum res2);
    Angle(const Angle &other);
    
    ~Angle();
    
    const AtomIndex& atom0() const;
    const AtomIndex& atom1() const;
    const AtomIndex& atom2() const;
    
    const AtomIndex& operator[](int i) const;
    const AtomIndex& at(int i) const;
    int size() const;
    int count() const;
    
    bool operator==(const Angle &other) const;
    bool operator!=(const Angle &other) const;
    
    const Angle& operator=(const Angle &other);
    
    bool operator>(const Angle &other) const;
    bool operator>=(const Angle &other) const;
    bool operator<(const Angle &other) const;
    bool operator<=(const Angle &other) const;
    
    bool contains(const AtomIndex &atom) const;
    bool contains(const QString &atom) const;
    bool contains(ResNum resnum) const;
    
    bool intraResidue() const;
    bool interResidue() const;
    
    ResNum resNum0() const;
    ResNum resNum1() const;
    ResNum resNum2() const;
    
    QString toString() const;
    
protected:
    void create(const AtomIndex &atom0, const AtomIndex &atom1,
                const AtomIndex &atom2);
    
    /** Index of the atoms in the angle */
    AtomIndex atoms[3];
};

/** Return a reference to the first AtomIndex in the angle */
inline const AtomIndex& Angle::atom0() const
{
    return atoms[0];
}

/** Return a reference to the second AtomIndex in the angle */
inline const AtomIndex& Angle::atom1() const
{
    return atoms[1];
}

/** Return a reference to the third AtomIndex in the angle */
inline const AtomIndex& Angle::atom2() const
{
    return atoms[2];
}

/** Return the size of the Angle (e.g. the number of atoms held in it, being 3) */
inline int Angle::size() const
{
    return 3;
}

/** Synonym for 'size' */
inline int Angle::count() const
{
    return this->size();
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Angle::at(int i) const
{
    return atoms[ i & 3 ];
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Angle::operator[](int i) const
{
    return this->at(i);
}

/** Return whether or not this angle contains this AtomIndex */
inline bool Angle::contains(const AtomIndex &atom) const
{
    return (atom == atoms[0] or atom == atoms[1] or atom == atoms[2]);
}

/** Return whether or not this angle contains an atom called 'atmnam' */
inline bool Angle::contains(const QString &atmnam) const
{
    return ( atmnam == atoms[0].name() or atmnam == atoms[1].name() or
             atmnam == atoms[2].name() );
}

/** Return whether or not this angle contains an atom in the residue 
    with residue number 'resnum' */
inline bool Angle::contains(ResNum resnum) const
{
    return (resnum == atoms[0].resNum() or resnum == atoms[1].resNum() or
            resnum == atoms[2].resNum());
}
    
/** Return whether or not this is an intra-residue angle */
inline bool Angle::intraResidue() const
{
    return (atoms[0].resNum() == atoms[1].resNum()) and 
           (atoms[0].resNum() == atoms[2].resNum()) ;
}

/** Return whether or not this in an inter-residue angle */
inline bool Angle::interResidue() const
{
    return not intraResidue();
}
    
/** Return the residue number of atom0() */
inline ResNum Angle::resNum0() const
{
    return atoms[0].resNum();
}

/** Return the residue number of atom1() */
inline ResNum Angle::resNum1() const
{
    return atoms[1].resNum();
}

/** Return the residue number of atom2() */
inline ResNum Angle::resNum2() const
{
    return atoms[2].resNum();
}

}

Q_DECLARE_METATYPE(SireMol::Angle)

SIRE_END_HEADER

#endif
