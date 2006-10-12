#ifndef SIREMOL_BOND_H
#define SIREMOL_BOND_H

#include "SireMol/atomindex.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Bond;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::Bond&);
QDataStream& operator>>(QDataStream&, SireMol::Bond&);

uint qHash(const SireMol::Bond&);

namespace SireMol
{

/**
This class represents a bond - this provides the AtomIndexes of the atoms between which the
bond is made. Note that this *only* provides the AtomIndexes, and not the actual Atoms. This
means that it is not possible to get the length of this bond using only this class. To
get the length of the bond you must do;

\code
double lgth = mol->length(bond);
\endcode

Note that the atoms are sorted (so the first atom is < the second atom).
This is to ensure that the bond between atom0-atom1 is identical to the bond
between atom1-atom0.

@author Christopher Woods
*/
class SIREMOL_EXPORT Bond
{

friend QDataStream& ::operator<<(QDataStream&, const Bond&);
friend QDataStream& ::operator>>(QDataStream&, Bond&);

public:

    typedef AtomIndex value_type;

    Bond();
    Bond(const AtomIndex &atom0, const AtomIndex &atom1);
    Bond(const tuple<AtomIndex,AtomIndex> &atoms);
    Bond(const QString &atom0, const QString &atom1, ResNum resnum);
    Bond(const QString &nam0, ResNum res0, const QString &nam1, ResNum res1);
    Bond(const Bond &other);

    ~Bond();

    const AtomIndex& atom0() const;
    const AtomIndex& atom1() const;

    const AtomIndex& other(const AtomIndex &atm) const;
    const AtomIndex& other(ResNum resnum) const;
    ResNum otherRes(ResNum resnum) const;

    const AtomIndex& atom(ResNum resnum) const;

    const AtomIndex& operator[](int i) const;
    const AtomIndex& at(int i) const;

    const AtomIndex& operator[](ResNum resnum) const;
    const AtomIndex& at(ResNum resnum) const;

    int size() const;
    int count() const;

    bool operator==(const Bond &other) const;
    bool operator!=(const Bond &other) const;

    const Bond& operator=(const Bond &other);

    bool operator>(const Bond &other) const;
    bool operator>=(const Bond &other) const;
    bool operator<(const Bond &other) const;
    bool operator<=(const Bond &other) const;

    bool contains(const AtomIndex &atom) const;
    bool contains(const QString &atmnam) const;
    bool contains(ResNum resnum) const;

    bool intraResidue() const;
    bool interResidue() const;

    ResNum resNum0() const;
    ResNum resNum1() const;

    QString toString() const;

    Bond renumber(ResNum oldnum, ResNum newnum) const;

protected:
    void create(const AtomIndex &atom0, const AtomIndex &atom1);

    /** Index of the atoms in the bond */
    AtomIndex atoms[2];
};

/** Return a reference to the first AtomIndex in the bond */
inline const AtomIndex& Bond::atom0() const
{
    return atoms[0];
}

/** Return a reference to the second AtomIndex in the bond */
inline const AtomIndex& Bond::atom1() const
{
    return atoms[1];
}

/** Return the size of the Bond (e.g. the number of atoms held in it, being 2) */
inline int Bond::size() const
{
    return 2;
}

/** Synonym for 'size' */
inline int Bond::count() const
{
    return this->size();
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Bond::at(int i) const
{
    return atoms[ i % 2 ];
}

/** Return atm0 if i is even, else return atm1 */
inline const AtomIndex& Bond::operator[](int i) const
{
    return this->at(i);
}

/** Return whether or not this bond contains this AtomIndex */
inline bool Bond::contains(const AtomIndex &atom) const
{
    return (atom == atoms[0] or atom == atoms[1]);
}

/** Return whether or not this bond contains an atom called 'atmnam' */
inline bool Bond::contains(const QString &atmnam) const
{
    return ( atmnam == atoms[0].name() or atmnam == atoms[1].name() );
}

/** Return whether or not this bond contains an atom in the residue
    with residue number 'resnum' */
inline bool Bond::contains(ResNum resnum) const
{
    return (resnum == atoms[0].resNum() or resnum == atoms[1].resNum());
}

/** Return whether or not this is an intra-residue bond */
inline bool Bond::intraResidue() const
{
    return atoms[0].resNum() == atoms[1].resNum();
}

/** Return whether or not this in an inter-residue bond */
inline bool Bond::interResidue() const
{
    return not intraResidue();
}

/** Return the residue number of atom0() */
inline ResNum Bond::resNum0() const
{
    return atoms[0].resNum();
}

/** Return the residue number of atom1() */
inline ResNum Bond::resNum1() const
{
    return atoms[1].resNum();
}

}

/** Provide a function to hash a bond (for use in a QSet or QHash) */
inline uint qHash(const SireMol::Bond &bond)
{
    //assume a 32bit uint
    //  give 16 bits from each atom

    uint atm0 = qHash(bond.atom0());
    uint atm1 = qHash(bond.atom1());

    return (atm0 << 16) | (atm1 & 0x0000FFFF);
}

Q_DECLARE_METATYPE(SireMol::Bond)

SIRE_END_HEADER

#endif
