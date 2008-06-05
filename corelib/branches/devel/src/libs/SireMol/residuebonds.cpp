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

#include "qhash_siremol.h"

#include <QSharedData>
#include <QDataStream>
#include <QHash>
#include <QSet>
#include <QMultiHash>

#include "residuebonds.h"
#include "bond.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;

namespace SireMol
{
class ResidueBondsPvt;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResidueBondsPvt&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueBondsPvt&);

namespace SireMol
{

/**
This is the private implementation of the ResidueBonds class.

@author Christopher Woods
*/
class ResidueBondsPvt : public QSharedData
{
friend QDataStream& ::operator<<(QDataStream&, const ResidueBondsPvt&);
friend QDataStream& ::operator>>(QDataStream&, ResidueBondsPvt&);

public:
    ResidueBondsPvt(ResNum resnum = ResNum(0));
    ResidueBondsPvt(const ResidueBondsPvt &other);

    ~ResidueBondsPvt();

    bool operator==(const ResidueBondsPvt &other) const;
    bool operator!=(const ResidueBondsPvt &other) const;

    bool isEmpty() const;

    QString toString() const;

    ResNum resNum() const;

    QSet<ResNum> bondedResidues() const;
    QSet<ResNum> residuesBondedTo(const QString &atom) const;

    bool bondedTo(ResNum resnum) const;
    bool bonded(const AtomIndex &atom0, const AtomIndex &atom1) const;

    QList<Bond> interBonds() const;
    QList<Bond> intraBonds() const;
    QList<Bond> asymmetricBonds() const;
    QList<Bond> bonds() const;

    int nBonds() const;
    int nAsymmetricBonds() const;
    int nIntraBonds() const;
    int nInterBonds() const;

    QSet<QString> anchors() const;

    QList<Bond> bonds(const QString &atomname) const;
    QList<Bond> interBonds(const QString &atomname) const;
    QList<Bond> intraBonds(const QString &atomname) const;

    QList<Bond> bonds(ResNum rnum) const;

    bool contains(const QString &atom) const;
    bool contains(const Bond &bond) const;

  //// Functions only accessible to MoleculeBonds////////
    void add(const Bond &bond);
    void add(const QString &atom0, const QString &atom1);

    void remove(const AtomIndex &atom);
    void remove(const QString &atom);
    void remove(ResNum resnum);

    void remove(const Bond &bond);
    void remove(const QString &atom0, const QString &atom1);

    void removeIntra();
    void removeInter();

    void removeIntra(const QString &atom);
    void removeInter(const QString &atom);

    void renumber(ResNum oldnum, ResNum newnum);

    void finalise();
    void clear();
  ///////////////////////////////////////////////////////

private:

    /** The residue number of the residue that has these bonds */
    ResNum resnum;

    /** Set of intra-residue bonds in the Residue */
    QSet<Bond> intrabnds;

    /** Hash which maps inter-residue bonds to the residue number
        that this residue is bonded to */
    QHash<Bond, ResNum> interbnds;

    /** Hash which maps the residue numbers of residues that are bonded
        to this residue to all of the bonds that exist between the two
        residues */
    QMultiHash<ResNum, Bond> bondedres;

    /** Multihash of all of the intraresidue bonds, indexed by their
        contained atoms */
    QMultiHash<QString, QString> atmintrabnds;

    /** Multihash of all of the interresidue bonds, indexed by the
        atom that is in this residue */
    QMultiHash<QString, AtomIndex> atminterbnds;

    /** Set of the names of all atoms that are involved in inter-residue
        bonding - this is useful to know as these are the anchor atoms for
        an intra-residue move, and they also provide information about
        where in the chain the residue sits. */
    QSet<QString> anchrs;
};

} // end of namespace SireMol

using namespace SireMol;

static const RegisterMetaType<ResidueBonds> r_resbonds;

/** Serialise the ResidueBondsPvt to a datastream */
QDataStream& operator<<(QDataStream &ds, const ResidueBondsPvt &bnds)
{
    writeHeader(ds, r_resbonds, 1) << bnds.resnum << bnds.intrabnds
                                   << bnds.interbnds << bnds.bondedres
                                   << bnds.atmintrabnds << bnds.atminterbnds;

    return ds;
}

/** Deserialise a ResidueBondsPvt from a datastream */
QDataStream& operator>>(QDataStream &ds, ResidueBondsPvt &bnds)
{
    VersionID v = readHeader(ds, r_resbonds);

    if (v == 1)
    {
        ds >> bnds.resnum >> bnds.intrabnds >> bnds.interbnds
           >> bnds.bondedres >> bnds.atmintrabnds >> bnds.atminterbnds;

        //rebuild anchors
        QSet<QString> anchors;

        for (QMultiHash<QString,AtomIndex>::const_iterator it =
                                                    bnds.atminterbnds.constBegin();
             it != bnds.atminterbnds.constEnd();
             ++it)
        {
            anchors.insert(it.key());
        }

        bnds.anchrs = anchors;
    }
    else
        throw version_error(v, "1", r_resbonds, CODELOC);

    return ds;
}

/** Create an empty ResidueBondsPvt that contains no bonds */
ResidueBondsPvt::ResidueBondsPvt( ResNum rnum ) : QSharedData(), resnum( rnum )
{}

/** Copy constructor. */
ResidueBondsPvt::ResidueBondsPvt( const ResidueBondsPvt &other )
        : QSharedData(),
          resnum( other.resnum ), intrabnds( other.intrabnds ),
          interbnds( other.interbnds ), bondedres( other.bondedres ),
          atmintrabnds( other.atmintrabnds ), atminterbnds( other.atminterbnds ),
          anchrs( other.anchrs )
{}

/** Destructor. */
ResidueBondsPvt::~ResidueBondsPvt()
{}

/** Comparison operator */
bool ResidueBondsPvt::operator==(const ResidueBondsPvt &other) const
{
    return this == &other or
           (resnum == other.resnum and intrabnds == other.intrabnds and
                      interbnds == other.interbnds);
}

/** Comparison operator */
bool ResidueBondsPvt::operator!=(const ResidueBondsPvt &other) const
{
    return this != &other and
           (resnum != other.resnum or intrabnds != other.intrabnds or
                      interbnds != other.interbnds);
}

/** Return whether or not there are no bonds in this ResidueBondsPvt */
bool ResidueBondsPvt::isEmpty() const
{
    return intrabnds.isEmpty() and interbnds.isEmpty();
}

/** Return the number of intraresidue bonds */
inline int ResidueBondsPvt::nIntraBonds() const
{
    return intrabnds.count();
}

/** Return the number of interresidue bonds */
inline int ResidueBondsPvt::nInterBonds() const
{
    return interbnds.count();
}

/** Return a string representation of the ResidueBonds */
QString ResidueBondsPvt::toString() const
{
    int nintra = nIntraBonds();
    int ninter = nInterBonds();

    return QObject::tr("ResidueBonds(%1): nIntraBonds() == %2, nInterBonds() == %3")
                              .arg(resNum().toString()).arg(nintra).arg(ninter);
}

/** Return the residue number of the residue whose bonds these are */
ResNum ResidueBondsPvt::resNum() const
{
    return resnum;
}

/** Return the set of anchor atoms for this residue - these are the
    atom names of atoms that are involved in inter-residue bonds */
QSet<QString> ResidueBondsPvt::anchors() const
{
    return anchrs;
}

/** Return the residue numbers of all residues that this residue is bonded to */
QSet<ResNum> ResidueBondsPvt::bondedResidues() const
{
    return bondedres.keys().toSet();
}

/** Return the list of residue numbers of all residues bonded to the atom 'atom' */
QSet<ResNum> ResidueBondsPvt::residuesBondedTo(const QString &atom) const
{
    QSet<ResNum> resnums;

    foreach( AtomIndex atm, atminterbnds.values(atom) )
        resnums.insert(atm.resNum());

    return resnums;
}

/** Return the set of inter-residue bonds */
QList<Bond> ResidueBondsPvt::interBonds() const
{
    return interbnds.keys();
}

/** Return the set of intra-residue bonds */
QList<Bond> ResidueBondsPvt::intraBonds() const
{
    return intrabnds.values();
}

/** Return the set of all inter- and intra-residue bonds */
QList<Bond> ResidueBondsPvt::bonds() const
{
    return intraBonds() + interBonds();
}

/** Add the intra-residue bond 'bond' */
void ResidueBondsPvt::add(const Bond &bond)
{
    if (bond.intraResidue() and bond.resNum0() == resnum)
    {
        intrabnds.insert(bond);
        atmintrabnds.insert(bond.atom0().name(), bond.atom1().name());
        atmintrabnds.insert(bond.atom1().name(), bond.atom0().name());
    }
    else if ( bond.interResidue() and
                (bond.resNum0() == resnum or bond.resNum1() == resnum) )
    {
        ResNum otherres = bond.otherRes(resnum);

        interbnds.insert(bond, otherres);
        bondedres.insert(otherres, bond);

        if (bond.atom0().resNum() == resnum)
        {
            atminterbnds.insert(bond.atom0().name(), bond.atom1());
            anchrs.insert( bond.atom0().name() );
        }
        else
        {
            atminterbnds.insert(bond.atom1().name(), bond.atom0());
            anchrs.insert( bond.atom1().name() );
        }
    }
}

/** Add the intra-residue bond between atoms called atom0-atom1 */
void ResidueBondsPvt::add(const QString &atom0, const QString &atom1)
{
    intrabnds.insert( Bond(atom0, atom1, resnum) );
}

/** Remove the Atom 'atm'. This will remove the bond containing this
    atom from this Residue. If there
    are no bonds with this Atom in this residue, then nothing happens. */
void ResidueBondsPvt::remove(const AtomIndex &atom)
{
    if (atom.resNum() == resnum)
    {
        //remove this atom from all of the intra-residue bonds
        QList<Bond> bonds = intrabnds.toList();

        foreach( Bond bond, bonds )
        {
            if (bond.contains(atom))
                intrabnds.remove(bond);
        }

        //remove this atom from the list of anchors
        anchrs.remove( atom.name() );

        //remove the atom from the 'atm-' hashs
        atmintrabnds.remove(atom.name());
        atminterbnds.remove(atom.name());
    }

    //remove this atom from any of the interbonds
    QMutableHashIterator<Bond, ResNum> it( interbnds );

    while ( it.hasNext() )
    {
        it.next();
        Bond bond = it.key();

        BOOST_ASSERT(bond.interResidue());

        if (bond.contains(atom))
        {
            //we must remove this bond from interbonds
            it.remove();

            //find the atom from this residue
            const AtomIndex &myatom = bond[resnum];

            //remove the bond from atminterbnds...
            QList<AtomIndex> otheratms = atminterbnds.values(myatom.name());

            if (otheratms.count() <= 1)
            {
                atminterbnds.remove(myatom.name());
                anchrs.remove(myatom.name());
            }
            else
            {
                otheratms.removeAll( bond.other(resnum) );
                atminterbnds.remove(myatom.name());

                foreach (AtomIndex otheratm, otheratms)
                {
                    atminterbnds.insert( myatom.name(), otheratm );
                }
            }

            //now remove this value from the multihash
            QMutableHashIterator<ResNum,Bond> it2( bondedres );

            while( it2.findNext(bond) )
                it2.remove();
        }
    }
}

/** Remove all bonds involving the atom with the name 'atomname' */
void ResidueBondsPvt::remove(const QString &atomname)
{
    this->remove( AtomIndex(atomname, resnum) );
}

/** Remove all bonds involving the residue with number 'rnum' */
void ResidueBondsPvt::remove(ResNum rnum)
{
    //if this is this residue, then clear it
    if (rnum == resnum)
    {
        this->clear();
        return;
    }

    //get the list of inter-residue bonds to the residue
    QList<Bond> bnds = bonds(rnum);

    //loop through each bond and remove it
    foreach( Bond bnd, bnds )
        this->remove(bnd);
}

/** Remove all intra-residue bonds from this residue */
void ResidueBondsPvt::removeIntra()
{
    QSet<Bond> orig_intrabonds( intrabnds );

    foreach (Bond bond, orig_intrabonds)
    {
        this->remove(bond);
    }
}

/** Remove all inter-residue bonds from this residue */
void ResidueBondsPvt::removeInter()
{
    QList<Bond> orig_interbonds = interbnds.keys();

    foreach (Bond bond, orig_interbonds)
    {
        this->remove(bond);
    }
}

/** Remove all intra-residue bonds that involve the atom 'atom' */
void ResidueBondsPvt::removeIntra(const QString &atom)
{
    QList<Bond> bonds = this->intraBonds(atom);

    foreach (Bond bond, bonds)
    {
        this->remove(bond);
    }
}

/** Remove all of the inter-residue bonds that involve the atom 'atom' */
void ResidueBondsPvt::removeInter(const QString &atom)
{
    QList<Bond> bonds = this->interBonds(atom);

    foreach (Bond bond, bonds)
    {
        this->remove(bond);
    }
}

/** Remove the bond 'bond' from this ResidueBondsPvt */
void ResidueBondsPvt::remove(const Bond &bond)
{
    if (intrabnds.contains(bond))
    {
        //remove the bond from intrabnds
        intrabnds.remove(bond);

        //now remove the bond from atmintrabnds
        QMutableHashIterator<QString,QString> it( atmintrabnds );
        while( it.findNext(bond.atom0().name()) )
        {
            if (it.key() == bond.atom1().name())
                it.remove();
        }

        it.toFront();
        while( it.findNext(bond.atom1().name()) )
        {
            if (it.key() == bond.atom0().name())
                it.remove();
        }
    }
    else if (interbnds.contains(bond))
    {
        interbnds.remove(bond);

        //remove the bond from bondedres
        QMutableHashIterator<ResNum,Bond> it( bondedres );
        while( it.findNext(bond) )
            it.remove();

        //now remove the bond from atminterbnds
        QString myatm;
        AtomIndex otheratm;

        if (bond.atom0().resNum() == resnum and bond.atom1().resNum() != resnum)
        {
            myatm = bond.atom0().name();
            otheratm = bond.atom1();
        }
        else if (bond.atom0().resNum() != resnum and bond.atom1().resNum() == resnum)
        {
            myatm = bond.atom1().name();
            otheratm = bond.atom0();
        }
        else
            throw SireError::program_bug(QObject::tr("Should never get here!"), CODELOC);

        QMutableHashIterator<QString,AtomIndex> it2( atminterbnds );
        while( it2.findNext(otheratm) )
        {
            if (it2.key() == myatm)
              it2.remove();
        }
    }
}

/** Remove the bond between atoms called 'atom0' and 'atom1' in this residue */
void ResidueBondsPvt::remove(const QString &atom0, const QString &atom1)
{
    this->remove( Bond(atom0,atom1,resnum) );
}

/** Return whether or not this contains information about the bonding to
    atom with name 'atom' */
inline bool ResidueBondsPvt::contains(const QString &atom) const
{
    return atmintrabnds.contains(atom) or atminterbnds.contains(atom);
}

/** Return whether or not this residue contains the bond 'bond' */
inline bool ResidueBondsPvt::contains(const Bond &bond) const
{
    return intrabnds.contains(bond) or interbnds.contains(bond);
}

/** Renumber the residue 'oldnum' to 'newnum' */
void ResidueBondsPvt::renumber(ResNum oldnum, ResNum newnum)
{
    if (resnum == oldnum)
    {
        //This is the residue that is being renumbered

        //renumber the residue itself
        resnum = newnum;

        //now renumber all of the intra-residue bonds
        QSet<Bond> renumbered_intrabnds;
        renumbered_intrabnds.reserve(intrabnds.count());

        for (QSet<Bond>::const_iterator it = intrabnds.constBegin();
             it != intrabnds.constEnd();
             ++it)
        {
            renumbered_intrabnds.insert( it->renumber(oldnum, newnum) );
        }

        intrabnds = renumbered_intrabnds;
    }

    if (bondedres.contains(oldnum) or resnum == oldnum)
    {
        //This residue is either bonded to the residue being
        //renumbered, or is indeed the residue that is being renumbered
        //We must thus update all of the inter-residue datait->

        QHash<Bond, ResNum> renumbered_interbnds;
        renumbered_interbnds.reserve(interbnds.count());

        for (QHash<Bond,ResNum>::const_iterator it = interbnds.constBegin();
             it != interbnds.constEnd();
             ++it)
        {
            ResNum resnum = it.value();
            if (resnum == oldnum)
                resnum == newnum;

            renumbered_interbnds.insert( it.key().renumber(oldnum,newnum), resnum );
        }

        interbnds = renumbered_interbnds;

        //now renumber 'bondedres'
        QMultiHash<ResNum,Bond> renumbered_bondedres;
        renumbered_bondedres.reserve(bondedres.count());

        for (QMultiHash<ResNum,Bond>::const_iterator it = bondedres.constBegin();
             it != bondedres.constEnd();
             ++it)
        {
            ResNum resnum = it.key();
            if (resnum == oldnum)
                resnum = newnum;

            renumbered_bondedres.insert( resnum, it->renumber(oldnum,newnum) );
        }

        bondedres = renumbered_bondedres;

        //finally, atminterbnds
        QMultiHash<QString,AtomIndex> renumbered_atminterbnds;
        renumbered_atminterbnds.reserve(atminterbnds.count());

        for (QMultiHash<QString,AtomIndex>::const_iterator it = atminterbnds.constBegin();
             it != atminterbnds.constEnd();
             ++it)
        {
            AtomIndex atom = it.value();
            if (atom.resNum() == oldnum)
                atom = AtomIndex(atom.name(), newnum);

            renumbered_atminterbnds.insert(it.key(), atom);
        }

        atminterbnds = renumbered_atminterbnds;
    }

}

/** Finalise this ResidueBondsPvt - this cleans up any unused memory, and
    effectively puts this group into a read-only mode. This is to
    switch the group from being read-write, as it used in an EditMol,
    to being read-only, as is used in a Molecule.
*/
void ResidueBondsPvt::finalise()
{
    interbnds.squeeze();
    intrabnds.squeeze();
    bondedres.squeeze();
    atmintrabnds.squeeze();
    atminterbnds.squeeze();
}

/** Clear this ResidueBondsPvt - this removes all bonding information */
void ResidueBondsPvt::clear()
{
    interbnds.clear();
    intrabnds.clear();
    bondedres.clear();
    atmintrabnds.clear();
    atminterbnds.clear();
}

/** Return the set of all bonds that involve the atom called 'atomname' in this residue */
QList<Bond> ResidueBondsPvt::bonds(const QString &atomname) const
{
    return intraBonds(atomname) + interBonds(atomname);
}

/** Return the list of inter-residue bonds that involve the atom called 'atomname' in
    this residue */
QList<Bond> ResidueBondsPvt::interBonds(const QString &atomname) const
{
    QList<Bond> bnds;

    foreach( AtomIndex atm, atminterbnds.values(atomname) )
    {
        bnds.append( Bond( AtomIndex(atomname,resnum), atm ) );
    }

    return bnds;
}

/** Return the list of intra-residue bonds that involve the atom called 'atomname' in
    this residue. This returns an empty list if there are no intramolecular bonds
    to this atom. */
QList<Bond> ResidueBondsPvt::intraBonds(const QString &atomname) const
{
    QList<Bond> bnds;

    foreach( QString atmnam, atmintrabnds.values(atomname) )
    {
        bnds.append( Bond( AtomIndex(atmnam,resnum), AtomIndex(atomname,resnum) ) );
    }

    return bnds;
}

/** Return the list of bonds between this residue and the residue with number 'rnum' */
QList<Bond> ResidueBondsPvt::bonds(ResNum rnum) const
{
    if (resnum == rnum)
    {
        //this is this residue - return all of the intra-residue bonds
        return intraBonds();
    }
    else
    {
        return bondedres.values(rnum);
    }
}

/** Return whether the residue is bonded to the residue with number 'resnum' */
bool ResidueBondsPvt::bondedTo(ResNum rnum) const
{
    return rnum == resnum or bondedres.contains(rnum);
}

/** Return whether or not the two atoms are bonded - this will only return true
    if at least one of the atoms is in this residue */
bool ResidueBondsPvt::bonded(const AtomIndex &atom0, const AtomIndex &atom1) const
{
    return ( atom0.resNum() == resnum and (
                   (atom1.resNum() == resnum and
                        atmintrabnds.values(atom0.name()).contains(atom1.name()))
                                              or
                   (atom1.resNum() != resnum and
                        atminterbnds.values(atom0.name()).contains(atom1))
                                          )
           )
           or
           ( atom1.resNum() == resnum and atminterbnds.values(atom1.name()).contains(atom0) );
}

/** Return the total number of inter- and intra-residue bonds in this BondGroup */
int ResidueBondsPvt::nBonds() const
{
    return intrabnds.count() + interbnds.count();
}

/** Return the asymmetric count of the total number of bonds. Asymmetric bonds are the
    list of intra-residue bonds in this Residue, and the list of inter-residue bonds
    to Residues with a higher residue number. You can get the list of all bonds in
    a molecule by adding together the asymmetric bonds of each Residue
    (as you can guarantee that there will be no duplication) */
int ResidueBondsPvt::nAsymmetricBonds() const
{
    int nbnds = intrabnds.count();

    QHashIterator<Bond, ResNum> it(interbnds);
    while ( it.hasNext() )
    {
        it.next();
        //add the bond if the connected residue has a higher residue number
        if (it.value() > resnum)
            nbnds++;
    }

    return nbnds;
}

/** Return the list of asymmetric bonds in this Residue. Asymmetric bonds are the
    list of intra-residue bonds in the Residue, and the list of inter-residue bonds
    to residues with a higher residue number. You can get the list of all bonds in
    a molecule by adding together the asymmetric bonds of each Residue (as you can
    guarantee that there will be no duplication) */
QList<Bond> ResidueBondsPvt::asymmetricBonds() const
{
    QList<Bond> bonds = intraBonds();

    QHashIterator<Bond, ResNum> it(interbnds);
    while ( it.hasNext() )
    {
        it.next();
        //add the bond if the connected residue has a higher residue number
        if (it.value() > resnum)
            bonds.append(it.key());
    }

    return bonds;
}

////////////////////
//////////////////// Implementation of ResidueBonds
////////////////////

/** Serialise ResidueBonds */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResidueBonds &bnds)
{
    ds << *(bnds.d);
    return ds;
}

/** Deserialise ResidueBonds */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResidueBonds &bnds)
{
    ds >> *(bnds.d);
    return ds;
}

/** Global pointer to a null ResidueBondsPvt */
static QSharedDataPointer<ResidueBondsPvt> shared_null( new ResidueBondsPvt() );

/** Construct a null ResidueBonds */
ResidueBonds::ResidueBonds() : d( shared_null )
{}

/** Construct an empty ResidueBonds, for the residue with residue number 'resnum' */
ResidueBonds::ResidueBonds(ResNum resnum) : d( new ResidueBondsPvt(resnum) )
{}

/** Copy constructor - this is fast as this class is implicitly shared */
ResidueBonds::ResidueBonds(const ResidueBonds &other) : d( other.d )
{}

/** Destructor */
ResidueBonds::~ResidueBonds()
{}

/** Copy assignment operator */
ResidueBonds& ResidueBonds::operator=(const ResidueBonds &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool ResidueBonds::operator==(const ResidueBonds &other) const
{
    return *d == *(other.d);
}

/** Comparison operator */
bool ResidueBonds::operator!=(const ResidueBonds &other) const
{
    return *d != *(other.d);
}

/** Return whether this ResidueBonds is empty - i.e. that it contains no bonds */
bool ResidueBonds::isEmpty() const
{
    return d->isEmpty();
}

QString ResidueBonds::toString() const
{
    return d->toString();
}

ResNum ResidueBonds::resNum() const
{
    return d->resNum();
}

QSet<ResNum> ResidueBonds::bondedResidues() const
{
    return d->bondedResidues();
}

QSet<ResNum> ResidueBonds::residuesBondedTo(const QString &atom) const
{
    return d->residuesBondedTo(atom);
}

bool ResidueBonds::bondedTo(ResNum resnum) const
{
    return d->bondedTo(resnum);
}

bool ResidueBonds::bonded(const AtomIndex &atom0, const AtomIndex &atom1) const
{
    return d->bonded(atom0, atom1);
}

QList<Bond> ResidueBonds::interBonds() const
{
    return d->interBonds();
}

QList<Bond> ResidueBonds::intraBonds() const
{
    return d->intraBonds();
}

QList<Bond> ResidueBonds::asymmetricBonds() const
{
    return d->asymmetricBonds();
}

QList<Bond> ResidueBonds::bonds() const
{
    return d->bonds();
}

int ResidueBonds::nBonds() const
{
    return d->nBonds();
}

int ResidueBonds::nAsymmetricBonds() const
{
    return d->nAsymmetricBonds();
}

int ResidueBonds::nIntraBonds() const
{
    return d->nIntraBonds();
}

int ResidueBonds::nInterBonds() const
{
    return d->nInterBonds();
}

QList<Bond> ResidueBonds::bonds(const QString &atomname) const
{
    return d->bonds(atomname);
}

QList<Bond> ResidueBonds::interBonds(const QString &atomname) const
{
    return d->interBonds(atomname);
}

QList<Bond> ResidueBonds::intraBonds(const QString &atomname) const
{
    return d->intraBonds(atomname);
}

QList<Bond> ResidueBonds::bonds(ResNum rnum) const
{
    return d->bonds(rnum);
}

QSet<QString> ResidueBonds::anchors() const
{
    return d->anchors();
}

QSet<QString> ResidueBonds::interBondedAtoms() const
{
    return d->anchors();
}

bool ResidueBonds::contains(const QString &atom) const
{
    return d->contains(atom);
}

bool ResidueBonds::contains(const Bond &bond) const
{
    return d->contains(bond);
}

void ResidueBonds::add(const Bond &bond)
{
    d->add(bond);
}

void ResidueBonds::add(const QString &atom0, const QString &atom1)
{
    d->add(atom0, atom1);
}

void ResidueBonds::add(const AtomIndex &atom0, const AtomIndex &atom1)
{
    d->add( Bond(atom0, atom1) );
}

void ResidueBonds::remove(const AtomIndex &atom)
{
    d->remove(atom);
}

void ResidueBonds::remove(const QString &atom)
{
    d->remove(atom);
}

void ResidueBonds::remove(const Bond &bond)
{
    d->remove(bond);
}

void ResidueBonds::remove(const QString &atom0, const QString &atom1)
{
    d->remove(atom0,atom1);
}

void ResidueBonds::remove(const AtomIndex &atom0, const AtomIndex &atom1)
{
    d->remove( Bond(atom0, atom1) );
}

void ResidueBonds::remove(ResNum resnum)
{
    d->remove(resnum);
}

void ResidueBonds::removeIntra()
{
    d->removeIntra();
}

void ResidueBonds::removeInter()
{
    d->removeInter();
}

void ResidueBonds::removeIntra(const QString &atom)
{
    d->removeIntra(atom);
}

void ResidueBonds::removeInter(const QString &atom)
{
    d->removeInter(atom);
}

void ResidueBonds::renumber(ResNum oldnum, ResNum newnum)
{
    d->renumber(oldnum,newnum);
}

void ResidueBonds::finalise()
{
    d->finalise();
}

void ResidueBonds::clear()
{
    d->clear();
}
