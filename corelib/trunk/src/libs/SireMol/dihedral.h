#ifndef SIREMOL_DIHEDRAL_H
#define SIREMOL_DIHEDRAL_H

#include "atomindex.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Dihedral;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::Dihedral&);
QDataStream& operator>>(QDataStream&, SireMol::Dihedral&);

uint qHash(const SireMol::Dihedral&);

namespace SireMol
{

/** 
This class represents a dihedral between atoms - this provides the AtomIndexes of the atoms between which the 
dihedral is made. Note that this *only* provides the AtomIndexes, and not the actual Atoms. This
means that it is not possible to get the size of this dihedral using only this class. To
get the size of the dihedral you must do;

\code
SireMaths::Angle ang = mol.measure(dihedral);
\endcode

Note that the atoms are sorted (so the first atom is < the fourth atom).
This is to ensure that the dihedral between atom0-atom1-atom2-atom3 is identical to the dihedral
between atom3-atom2-atom1-atom0.

@author Christopher Woods
*/
class SIREMOL_EXPORT Dihedral
{

friend QDataStream& ::operator<<(QDataStream&, const Dihedral&);
friend QDataStream& ::operator>>(QDataStream&, Dihedral&);

public:
    
    typedef AtomIndex value_type;
    
    Dihedral();
    Dihedral(const AtomIndex &atom0, const AtomIndex &atom1,
             const AtomIndex &atom2, const AtomIndex &atom3);
    Dihedral(const tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> &tuple);
    Dihedral(const QString &atom0, const QString &atom1, 
             const QString &atom2, const QString &atom3, ResNum resnum);
    Dihedral(const QString &atm0, ResNum res0, const QString &atm1, ResNum res1,
             const QString &atm2, ResNum res2, const QString &atm3, ResNum res3);
    
    Dihedral(const Dihedral &other);
    
    ~Dihedral();
    
    const AtomIndex& atom0() const;
    const AtomIndex& atom1() const;
    const AtomIndex& atom2() const;
    const AtomIndex& atom3() const;
    
    const AtomIndex& operator[](int i) const;
    const AtomIndex& at(int i) const;
    int size() const;
    int count() const;
    
    bool operator==(const Dihedral &other) const;
    bool operator!=(const Dihedral &other) const;
    
    const Dihedral& operator=(const Dihedral &other);
    
    bool operator>(const Dihedral &other) const;
    bool operator>=(const Dihedral &other) const;
    bool operator<(const Dihedral &other) const;
    bool operator<=(const Dihedral &other) const;
    
    bool contains(const AtomIndex &atom) const;
    bool contains(const QString &atmnam) const;
    bool contains(ResNum resnum) const;
    
    bool intraResidue() const;
    bool interResidue() const;
    
    ResNum resNum0() const;
    ResNum resNum1() const;
    ResNum resNum2() const;
    ResNum resNum3() const;
    
    QString toString() const;
    
protected:
    void create(const AtomIndex &atom0, const AtomIndex &atom1,
                const AtomIndex &atom2, const AtomIndex &atom3);
    
    /** Indexes of the atoms in the dihedral */
    AtomIndex atoms[4];
};

/** Return a reference to the first AtomIndex in the dihedral */
inline const AtomIndex& Dihedral::atom0() const
{
    return atoms[0];
}

/** Return a reference to the second AtomIndex in the dihedral */
inline const AtomIndex& Dihedral::atom1() const
{
    return atoms[1];
}

/** Return a reference to the third AtomIndex in the dihedral */
inline const AtomIndex& Dihedral::atom2() const
{
    return atoms[2];
}

/** Return a reference to the fourth AtomIndex in the dihedral */
inline const AtomIndex& Dihedral::atom3() const
{
    return atoms[3];
}

/** Return the size of the Dihedral (e.g. the number of atoms held in it, being 4) */
inline int Dihedral::size() const
{
    return 4;
}

/** Synonym for 'size' */
inline int Dihedral::count() const
{
    return this->size();
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Dihedral::at(int i) const
{
    return atoms[ i % 4 ];
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Dihedral::operator[](int i) const
{
    return this->at(i);
}

/** Return whether or not this dihedral contains this AtomIndex */
inline bool Dihedral::contains(const AtomIndex &atom) const
{
    return (atom == atoms[0] or atom == atoms[1] or 
            atom == atoms[2] or atom == atoms[3]);
}

/** Return whether or not this dihedral contains an atom called 'atmnam' */
inline bool Dihedral::contains(const QString &atmnam) const
{
    return ( atmnam == atoms[0].name() or atmnam == atoms[1].name() or
             atmnam == atoms[2].name() or atmnam == atoms[3].name() );
}

/** Return whether or not this dihedral contains an atom in the residue 
    with residue number 'resnum' */
inline bool Dihedral::contains(ResNum resnum) const
{
    return (resnum == atoms[0].resNum() or resnum == atoms[1].resNum() or
            resnum == atoms[2].resNum() or resnum == atoms[3].resNum());
}
    
/** Return whether or not this is an intra-residue dihedral */
inline bool Dihedral::intraResidue() const
{
    return (atoms[0].resNum() == atoms[1].resNum()) and (atoms[0].resNum() == atoms[2].resNum()) 
                and (atoms[0].resNum() == atoms[3].resNum());
}

/** Return whether or not this in an inter-residue dihedral */
inline bool Dihedral::interResidue() const
{
    return not intraResidue();
}
    
/** Return the residue number of atom0() */
inline ResNum Dihedral::resNum0() const
{
    return atoms[0].resNum();
}

/** Return the residue number of atom1() */
inline ResNum Dihedral::resNum1() const
{
    return atoms[1].resNum();
}

/** Return the residue number of atom2() */
inline ResNum Dihedral::resNum2() const
{
    return atoms[2].resNum();
}

/** Return the residue number of atom3() */
inline ResNum Dihedral::resNum3() const
{
    return atoms[3].resNum();
}

}

Q_DECLARE_METATYPE(SireMol::Dihedral)

SIRE_END_HEADER

#endif
