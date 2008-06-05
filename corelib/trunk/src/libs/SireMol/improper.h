#ifndef SIREMOL_IMPROPER_H
#define SIREMOL_IMPROPER_H

#include "SireMol/atom.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Improper;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::Improper&);
QDataStream& operator>>(QDataStream&, SireMol::Improper&);

uint qHash(const SireMol::Improper&);

namespace SireMol
{


/**
This class represents an improper angle (in the same way that Bond represents a bond, SireMol::Angle an angle and Dihedral a dihedral).
 
@author Christopher Woods
*/
class SIREMOL_EXPORT Improper
{

friend QDataStream& ::operator<<(QDataStream&, const Improper&);
friend QDataStream& ::operator>>(QDataStream&, Improper&);

public:
    
    typedef AtomIndex value_type;
    
    Improper();
    Improper(const AtomIndex &atom0, const AtomIndex &atom1,
             const AtomIndex &atom2, const AtomIndex &atom3);
    Improper(const tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> &tuple);
    Improper(const QString &atom0, const QString &atom1, 
             const QString &atom2, const QString &atom3, ResNum resnum);
    Improper(const QString &atm0, ResNum res0, const QString &atm1, ResNum res1,
             const QString &atm2, ResNum res2, const QString &atm3, ResNum res3);
    
    Improper(const Improper &other);
    
    ~Improper();
    
    const AtomIndex& atom0() const;
    const AtomIndex& atom1() const;
    const AtomIndex& atom2() const;
    const AtomIndex& atom3() const;
    
    const AtomIndex& operator[](int i) const;
    const AtomIndex& at(int i) const;
    int size() const;
    int count() const;
    
    bool operator==(const Improper &other) const;
    bool operator!=(const Improper &other) const;
    
    const Improper& operator=(const Improper &other);
    
    bool operator>(const Improper &other) const;
    bool operator>=(const Improper &other) const;
    bool operator<(const Improper &other) const;
    bool operator<=(const Improper &other) const;
    
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
inline const AtomIndex& Improper::atom0() const
{
    return atoms[0];
}

/** Return a reference to the second AtomIndex in the dihedral */
inline const AtomIndex& Improper::atom1() const
{
    return atoms[1];
}

/** Return a reference to the third AtomIndex in the dihedral */
inline const AtomIndex& Improper::atom2() const
{
    return atoms[2];
}

/** Return a reference to the fourth AtomIndex in the dihedral */
inline const AtomIndex& Improper::atom3() const
{
    return atoms[3];
}

/** Return the size of the Dihedral (e.g. the number of atoms held in it, being 4) */
inline int Improper::size() const
{
    return 4;
}

/** Synonym for 'size' */
inline int Improper::count() const
{
    return this->size();
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Improper::at(int i) const
{
    return atoms[ i % 4 ];
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Improper::operator[](int i) const
{
    return this->at(i);
}

/** Return whether or not this dihedral contains this AtomIndex */
inline bool Improper::contains(const AtomIndex &atom) const
{
    return (atom == atoms[0] or atom == atoms[1] or 
            atom == atoms[2] or atom == atoms[3]);
}

/** Return whether or not this improper contains an atom called 'atmnam' */
inline bool Improper::contains(const QString &atmnam) const
{
    return ( atmnam == atoms[0].name() or atmnam == atoms[1].name() or
             atmnam == atoms[2].name() or atmnam == atoms[3].name() );
}

/** Return whether or not this dihedral contains an atom in the residue 
    with residue number 'resnum' */
inline bool Improper::contains(ResNum resnum) const
{
    return (resnum == atoms[0].resNum() or resnum == atoms[1].resNum() or
            resnum == atoms[2].resNum() or resnum == atoms[3].resNum());
}
    
/** Return whether or not this is an intra-residue dihedral */
inline bool Improper::intraResidue() const
{
    return (atoms[0].resNum() == atoms[1].resNum()) and (atoms[0].resNum() == atoms[2].resNum()) 
                and (atoms[0].resNum() == atoms[3].resNum());
}

/** Return whether or not this in an inter-residue dihedral */
inline bool Improper::interResidue() const
{
    return not intraResidue();
}
    
/** Return the residue number of atom0() */
inline ResNum Improper::resNum0() const
{
    return atoms[0].resNum();
}

/** Return the residue number of atom1() */
inline ResNum Improper::resNum1() const
{
    return atoms[1].resNum();
}

/** Return the residue number of atom2() */
inline ResNum Improper::resNum2() const
{
    return atoms[2].resNum();
}

/** Return the residue number of atom3() */
inline ResNum Improper::resNum3() const
{
    return atoms[3].resNum();
}

}

Q_DECLARE_METATYPE(SireMol::Improper)

SIRE_END_HEADER

#endif
