
#include "qhash_siremol.h"

#include "atomidgroup.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AtomIDGroup> r_atomidgroup("SireMol::AtomIDGroup");

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
    AtomIndexList allatoms = this->atoms().toList();
    
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
