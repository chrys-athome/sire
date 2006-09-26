
#include "qhash_siremol.h"

#include <QDataStream>
#include <boost/assert.hpp>

#include "moleculebonds.h"
#include "atom.h"
#include "bond.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<MoleculeBonds> r_molbonds("SireMol::MoleculeBonds");

/** Serialise a MoleculeBonds */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeBonds &molbnds)
{
    writeHeader(ds, r_molbonds, 1) << molbnds.resbnds;

    return ds;
}

/** Deserialise a MoleculeBonds */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeBonds &molbnds)
{
    VersionID v = readHeader(ds, r_molbonds);

    if (v == 1)
    {
        ds >> molbnds.resbnds;
    }
    else
        throw version_error(v, "1", r_molbonds, CODELOC);

    return ds;
}

/** Construct an empty set of MoleculeBonds */
MoleculeBonds::MoleculeBonds()
{}

/** Copy constructor */
MoleculeBonds::MoleculeBonds(const MoleculeBonds &other) : resbnds(other.resbnds)
{}

/** Destructor */
MoleculeBonds::~MoleculeBonds()
{}

/** Comparison operator */
bool MoleculeBonds::operator==(const MoleculeBonds &other) const
{
    return resbnds == other.resbnds;
}

/** Comparison operator */
bool MoleculeBonds::operator!=(const MoleculeBonds &other) const
{
    return resbnds != other.resbnds;
}

/** Private function used to return a non-const reference to the ResidueBonds
    for the residue with residue number 'resnum'. This creates a new ResidueBonds
    if one does not already exist for this residue. */
ResidueBonds& MoleculeBonds::getResidue(ResNum resnum)
{
    if (not resbnds.contains(resnum))
        resbnds.insert( resnum, ResidueBonds(resnum) );

    return resbnds[resnum];
}

/** Return a string representation of the MoleculeBonds */
QString MoleculeBonds::toString() const
{
    int nbnds = nBonds();
    int nres = nResidues();

    return QObject::tr("MoleculeBonds: nBonds() == %1 for nResidues() == %2")
                                .arg(nbnds).arg(nres);
}

/** Add a bond between atoms 'atom0' and 'atom1'. This does nothing if a bond already
    exists between these atoms. */
void MoleculeBonds::add(const AtomIndex &atom0, const AtomIndex &atom1)
{
    getResidue(atom0.resNum()).add(atom0,atom1);

    if (atom0.resNum() != atom1.resNum())
        getResidue(atom1.resNum()).add(atom0,atom1);
}

/** Overloaded function - add a bond between atoms called 'atom0' and 'atom1' in
    residue 'resnum' */
void MoleculeBonds::add(ResNum resnum, const QString &atom0, const QString &atom1)
{
    getResidue(resnum).add(atom0,atom1);
}

/** Overloaded function - add the bond 'bond' to the molecule */
void MoleculeBonds::add(const Bond &bond)
{
    this->add(bond.atom0(),bond.atom1());
}

/** Remove the bond between atoms 'atom0' and 'atom1'. This does nothing if there
    is no such bond. */
void MoleculeBonds::remove(const AtomIndex &atom0, const AtomIndex &atom1)
{
    if ( resbnds.contains(atom0.resNum()) )
    {
        ResidueBonds &res = resbnds[atom0.resNum()];
        res.remove(atom0,atom1);

        if (res.isEmpty())
            //there cannot be any inter-residue bonds, so it is
            //safe to remove this residue
            resbnds.remove(atom0.resNum());
    }

    if (atom0.resNum() != atom1.resNum())
    {
        ResidueBonds &res = resbnds[atom1.resNum()];
        res.remove(atom0,atom1);

        if (res.isEmpty())
            //there cannot be any inter-residue bonds, so it is
            //safe to remove this residue
            resbnds.remove(atom1.resNum());
    }
}

/** Overloaded function - remove the bond between atoms called 'atom0' and 'atom1' in
    residue with residue number 'resnum'. This does nothing if there is no
    such bond in this molecule. */
void MoleculeBonds::remove(ResNum resnum, const QString &atom0, const QString &atom1)
{
    if ( resbnds.contains(resnum) )
    {
        ResidueBonds &res = resbnds[resnum];
        res.remove(atom0,atom1);

        if (res.isEmpty())
            //there cannot be any inter-residue bonds, so it is
            //safe to remove this residue
            resbnds.remove(resnum);
    }
}

/** Overloaded function - remove the bond 'bond' from this molecule. This does nothing
    if there is no such bond. */
void MoleculeBonds::remove(const Bond &bond)
{
    this->remove(bond.atom0(),bond.atom1());
}

/** Remove all bonds from the molecule that involve atom 'atom' */
void MoleculeBonds::removeAll(const AtomIndex &atom)
{
    if ( resbnds.contains(atom.resNum()) )
    {
        //get the residue containing this atom
        ResidueBonds &res = resbnds[atom.resNum()];

        //get the list of residue numbers of residues bonded to this atom
        QSet<ResNum> bondedres = res.residuesBondedTo(atom.name());

        //remove this atom from its residue
        res.remove(atom.name());

        //if this residue has no other bonds, then it is safe to remove it
        if (res.isEmpty())
            resbnds.remove(res.resNum());

        //now remove the atom from all of the bonded residues
        foreach (ResNum resnum, bondedres)
        {
            ResidueBonds &bondedres = getResidue(resnum);
            bondedres.remove(atom);

            if (bondedres.isEmpty())
                resbnds.remove(bondedres.resNum());
        }
    }
}

/** Remove all bonds to the residue with residue number 'resnum' */
void MoleculeBonds::removeAll(ResNum resnum)
{
    if ( resbnds.contains(resnum) )
    {
        //get the residue...
        ResidueBonds &res = resbnds[resnum];

        //find all of the residues bonded to this residue
        QSet<ResNum> resnums = res.bondedResidues();

        //remove this residue
        resbnds.remove(resnum);

        //now remove this residue from all of the bonded residues
        foreach( ResNum rnum, resnums )
        {
            ResidueBonds &bondedres = getResidue(rnum);

            bondedres.remove(rnum);
            if (bondedres.isEmpty())
                resbnds.remove(bondedres.resNum());
        }
    }
}

/** Completely clear all of the bonding information */
void MoleculeBonds::clear()
{
    resbnds.clear();
}

/** Finalise the bonding information. This compacts the data structure to
    minimise the memory usage of the bonding data. Note that this will make
    it slower to modify. */
void MoleculeBonds::finalise()
{
    //finalise each of the ResidueBonds objects...
    QMutableHashIterator<ResNum,ResidueBonds> it(resbnds);

    while (it.hasNext())
    {
        it.next();
        it.value().finalise();
    }

    resbnds.squeeze();
}

/** Return whether or not this is empty (contains no bonds) */
bool MoleculeBonds::isEmpty() const
{
    return resbnds.isEmpty();
}

/** Return the ResidueBonds for the residue with residue number 'resnum'. Note that
    this will return an empty ResidueBonds if there are no bonds in this residue */
ResidueBonds MoleculeBonds::residue(ResNum resnum) const
{
    return resbnds.value(resnum);
}

/** Return the list of ResidueBonds that contain the bonding of residues that are
    bonded to the residue with residue number 'resnum' */
QList<ResidueBonds> MoleculeBonds::bondedResidues(ResNum resnum) const
{
    if (resbnds.contains(resnum))
    {
        QSet<ResNum> resnums = resbnds[resnum].bondedResidues();

        QList<ResidueBonds> bondedres;

        foreach( ResNum rnum, resnums )
        {
            BOOST_ASSERT(resbnds.contains(rnum));
            bondedres.append(resbnds[rnum]);
        }

        return bondedres;
    }
    else
        return QList<ResidueBonds>();
}

/** Return the total number of bonds in the molecule */
int MoleculeBonds::nBonds() const
{
    int nbnds = 0;

    QHashIterator<ResNum, ResidueBonds> it(resbnds);

    while (it.hasNext())
    {
        it.next();
        nbnds += it.value().nAsymmetricBonds();
    }

    return nbnds;
}

/** Return the total number of intra-residue bonds in this molecule */
int MoleculeBonds::nIntraBonds() const
{
    //loop over each residue and sum the number of intra-bonds
    int nintra = 0;

    for (QHash<ResNum,ResidueBonds>::const_iterator it = resbnds.begin();
         it != resbnds.end();
         ++it)
    {
        nintra += it->nIntraBonds();
    }

    return nintra;
}

/** Return the total number of inter-residue bonds in this molecule */
int MoleculeBonds::nInterBonds() const
{
    //loop over each residue and sum the number of
    //asymmetric inter-residue bonds
    int ninter = 0;

    for (QHash<ResNum,ResidueBonds>::const_iterator it = resbnds.begin();
         it != resbnds.end();
         ++it)
    {
        ninter += it->nAsymmetricBonds();
    }

    return ninter;
}

/** Return the number of residues */
int MoleculeBonds::nResidues() const
{
    return resbnds.count();
}

/** Return the list of all of the bonds in this molecule */
QList<Bond> MoleculeBonds::bonds() const
{
    QList<Bond> bnds;

    QHashIterator<ResNum, ResidueBonds> it(resbnds);

    while (it.hasNext())
    {
        it.next();
        bnds += it.value().asymmetricBonds();
    }

    return bnds;
}

/** Return all of the bonds that involve the atom 'atom' */
QList<Bond> MoleculeBonds::bonds(const AtomIndex &atom) const
{
    if (resbnds.contains(atom.resNum()))
        return resbnds[atom.resNum()].bonds(atom.name());
    else
        return QList<Bond>();
}

/** Return all of the bonds that involve the residue with number 'resnum' */
QList<Bond> MoleculeBonds::bonds(ResNum resnum) const
{
    if (resbnds.contains(resnum))
        return resbnds[resnum].bonds();
    else
        return QList<Bond>();
}

/** Return whether or not the two residues with residue numbers 'resnum0' and resnum1'
    are bonded together */
bool MoleculeBonds::bonded(ResNum resnum0, ResNum resnum1) const
{
    if (resbnds.contains(resnum0))
        return resbnds[resnum0].bondedTo(resnum1);
    else
        return false;
}

/** Return whether or not this contains the bond 'bond' */
bool MoleculeBonds::contains(const Bond &bond) const
{
    return resbnds.contains(bond.atom0().resNum()) and
              resbnds[bond.atom0().resNum()].bonded(bond.atom0(),bond.atom1());
}

/** Return whether or not this contains bonding information for atom 'atm' */
bool MoleculeBonds::contains(const AtomIndex &atom) const
{
    return resbnds.contains(atom.resNum()) and resbnds[atom.resNum()].contains(atom.name());
}

/** Return whether or not this contains bonding information for the residue
    with number 'resnum' */
bool MoleculeBonds::contains(ResNum resnum) const
{
    return resbnds.contains(resnum);
}

/** Return whether or not the atom 'atom0' is bonded to the atom 'atom1' in this
    molecule. */
bool MoleculeBonds::bonded(const AtomIndex &atom0, const AtomIndex &atom1) const
{
    return resbnds.contains(atom0.resNum()) and
              resbnds[atom0.resNum()].bonded(atom0,atom1);
}

/** Return the list of residue numbers of residues bonded to the residue with
    residue number 'resnum' */
QSet<ResNum> MoleculeBonds::resNumsBondedTo(ResNum resnum) const
{
    if (resbnds.contains(resnum))
        return resbnds[resnum].bondedResidues();
    else
        return QSet<ResNum>();
}

/** Return the complete list of residue numbers in this molecule
    (note that this only returns the residue numbers of residues that contain
    some bonding) */
QSet<ResNum> MoleculeBonds::resNums() const
{
    return resbnds.keys().toSet();
}

/** Return the complete list of atoms that are involved in bonding in this molecule */
QSet<AtomIndex> MoleculeBonds::atoms() const
{
    QSet<AtomIndex> atms;
    QHashIterator<ResNum,ResidueBonds> it(resbnds);

    while(it.hasNext())
    {
        it.next();
        atms.unite(it.value().atoms());
    }

    return atms;
}
