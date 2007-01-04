/**
  * @file
  *
  * C++ Implementation: AtomIDGroup
  *
  * Description:
  * Implementation of the AtomIDGroup class
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "qhash_siremol.h"

#include "atomidgroup.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AtomIDGroup> r_atomidgroup;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomIDGroup &group)
{
    writeHeader(ds, r_atomidgroup, 1) << group.atms << group.residus;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomIDGroup &group)
{
    VersionID v = readHeader(ds, r_atomidgroup);

    if (v == 1)
    {
        ds >> group.atms >> group.residus;
    }
    else
        throw version_error(v, "1", r_atomidgroup, CODELOC);

    return ds;
}

/** Construct an empty AtomIDGroup */
AtomIDGroup::AtomIDGroup()
{}

/** Destructor */
AtomIDGroup::~AtomIDGroup()
{}

/** Construct a group that contains the atoms 'atoms' */
AtomIDGroup::AtomIDGroup(const QSet<AtomIndex> &atoms)
            : atms(atoms)
{}

/** Construct a group that contains the residues whose numbers
    are in 'resnums' */
AtomIDGroup::AtomIDGroup(const QSet<ResNum> &resnums)
            : residus(resnums)
{}

/** Construct a group that contains the residues whose numbers
    are in 'resnums' and the atoms in 'atoms' */
AtomIDGroup::AtomIDGroup(const QSet<ResNum> &resnums,
                         const QSet<AtomIndex> &atoms)
            : residus(resnums), atms(atoms)
{
    for (QSet<AtomIndex>::const_iterator it = atoms.begin();
         it != atoms.end();
         ++it)
    {
        if (residus.contains(it->resNum()))
            atms.remove(*it);
    }
}

/** Clear all atoms and residues from this group */
void AtomIDGroup::clear()
{
    atms.clear();
    residus.clear();
}

/** Return whether this group is empty (contains no atoms or residues) */
bool AtomIDGroup::isEmpty() const
{
    return atms.isEmpty() and residus.isEmpty();
}

/** Simplify this group - this ensures that if a residue has been
    added to this group, then none of its atoms are in the list
    of explicitly added atoms - this ensures that an atom in
    the molecule will only appear once in this group; either
    explicitly in the list of atoms, or implicitly via the list
    of residues */
void AtomIDGroup::simplify()
{
    QList<AtomIndex> allatoms = this->atoms().toList();

    foreach( AtomIndex atom, allatoms )
    {
        if (this->contains(atom.resNum()))
            atms.remove(atom);
    }
}

/** Return a string representation of an inventory of the contents of this
    group */
QString AtomIDGroup::inventory() const
{
    QString ret = QObject::tr("AtomIDGroup contents:\n");

    int natm = 0;
    foreach( AtomIndex atm, atms )
    {
        natm++;
        ret += QObject::tr("Atom %1: %2").arg(natm).arg(atm.toString());
    }

    int nres = 0;
    foreach( ResNum resnum, residus )
    {
        nres++;
        ret += QObject::tr("Residue %1: Number = %2").arg(nres).arg(resnum.toString());
    }

    return ret;
}
