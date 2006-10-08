#ifndef SIREMOL_ATOMIDGROUP_H
#define SIREMOL_ATOMIDGROUP_H

#include <QSet>

#include <boost/tuple/tuple.hpp>

#include "atomindex.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomIDGroup;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::AtomIDGroup&);
QDataStream& operator>>(QDataStream&, SireMol::AtomIDGroup&);

namespace SireMol
{

class AtomIDGroup;
typedef boost::tuple<AtomIDGroup,AtomIDGroup> AtomIDGroupPair;

/**
This class contains the IDs of all of the atoms in a molecule that belong to this group. This class is optimised for speedy addition of atoms and querying of whether or not particular atoms or residues are contained in this group. This class is used by the functions that split molecules into groups so that the groups can be moved via the internal bond/angle/dihedral moves.

@author Christopher Woods
*/
class SIREMOL_EXPORT AtomIDGroup
{

friend QDataStream& ::operator<<(QDataStream&, const AtomIDGroup&);
friend QDataStream& ::operator>>(QDataStream&, AtomIDGroup&);

public:
    AtomIDGroup();
    AtomIDGroup(const QSet<AtomIndex> &atoms);
    AtomIDGroup(const QSet<ResNum> &residues);
    AtomIDGroup(const QSet<ResNum> &residues, const QSet<AtomIndex> &atoms);
    
    ~AtomIDGroup();

    void add(ResNum resnum);
    void add(const AtomIndex &atom);

    bool contains(ResNum resnum) const;
    bool contains(const AtomIndex &atom) const;

    bool intersects(const QSet<AtomIndex> &atoms) const;

    const QSet<AtomIndex>& atoms() const;
    const QSet<ResNum>& residues() const;

    void clear();
    void simplify();

    bool isEmpty() const;

    QString inventory() const;

private:

    static bool intersects(const QSet<AtomIndex> &big,
                           const QSet<AtomIndex> &small);

    /** The set of explicitly added atoms in this group */
    QSet<AtomIndex> atms;

    /** The set of fully added residues in this group */
    QSet<ResNum> residus;
};

/** Add all of the atoms of residue 'resnum' to this group */
inline void AtomIDGroup::add(ResNum resnum)
{
    residus.insert(resnum);
}

/** Add the atom 'atom' to this group */
inline void AtomIDGroup::add(const AtomIndex &atom)
{
    atms.insert(atom);
}

/** Return whether or not this group contains the explicitly added residue
    'resnum' */
inline bool AtomIDGroup::contains(ResNum resnum) const
{
    return residus.contains(resnum);
}

/** Return whether or not this group contains the atom 'atom'
    (either added explicitly, or in an added residue) */
inline bool AtomIDGroup::contains(const AtomIndex &atom) const
{
    return residus.contains(atom.resNum()) or atms.contains(atom);
}

/** Return the list of all explicitly added atoms to this group */
inline const QSet<AtomIndex>& AtomIDGroup::atoms() const
{
    return atms;
}

/** Return the list of all explicitly added residues to this group */
inline const QSet<ResNum>& AtomIDGroup::residues() const
{
    return residus;
}

/** Small, private function, which is used to see if two QSet<AtomIndex>s intersect.
    For best speed, ensure that 'big' is the larger of the two sets, and 'small' is
    the smaller. */
inline bool AtomIDGroup::intersects(const QSet<AtomIndex> &big, const QSet<AtomIndex> &small)
{
    QList<AtomIndex> contents = small.toList();

    foreach( AtomIndex atom, contents )
    {
        if (big.contains(atom))
            return true;
    }

    return false;
}

/** Return whether or not this group has atoms in it that are also in
    'atoms' */
inline bool AtomIDGroup::intersects(const QSet<AtomIndex> &atoms) const
{
    if (atoms.count() > atms.count())
        return intersects(atoms, atms);
    else
        return intersects(atms, atoms);
}

}

Q_DECLARE_METATYPE(SireMol::AtomIDGroup)

SIRE_END_HEADER

#endif
