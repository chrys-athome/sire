
#include "qhash_siremol.h"

#include "residue.h"
#include "molecule.h"
#include "resnum.h"
#include "resid.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "atomidgroup.h"
#include "splitmolecule.h"
#include "weightfunction.h"
#include "residuebonds.h"

#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireMol;

/** Hash a residue - this returns its residue number */
uint SIREMOL_EXPORT qHash(const Residue &res)
{
    return qHash(res.resNum());
}

static const RegisterMetaType<Residue> r_residue("SireMol::Residue");

/** Serialise a Residue to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Residue &res)
{
    writeHeader(ds, r_residue, 1) << res.rnum;

    SharedDataStream(ds) << res.d;

    return ds;
}

/** Deserialise a Residue from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Residue &res)
{
    VersionID v = readHeader(ds, r_residue);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> res.rnum >> res.d;
    }
    else
        throw version_error(v, "1", r_residue, CODELOC);

    return ds;
}

/** Construct an empty residue */
Residue::Residue() : rnum(0)
{}

/** Construct a residue that is a copy of the residue with number 'resnum'
    in the passed molecule

    \throw SireMol::missing_residue
*/
Residue::Residue(const Molecule &molecule, ResNum resnum)
        : d(molecule.d), rnum(resnum)
{
    d->info().assertResidueExists(rnum);
}

/** Construct a residue that is a copy of the residue at index 'resid' in
    the passed molecule

    \throw SireError::invalid_index
*/
Residue::Residue(const Molecule &molecule, ResID resid)
        : d(molecule.d), rnum( d->info().residueNumber(resid) )
{}

/** Construct a residue that is a copy of the first residue called 'resname'
    in the passed molecule

    \throw SireMol::missing_residue
*/
Residue::Residue(const Molecule &molecule, const QString &resname)
        : d(molecule.d), rnum( d->info().residueNumber(resname) )
{}

/** Copy constructor - this is fast as this class is implicitly shared */
Residue::Residue(const Residue &other)
        : d(other.d), rnum(other.rnum)
{}

/** Destructor */
Residue::~Residue()
{}

///// Operators /////////////////////////////////////////

/** Comparison operator */
// bool Residue::operator==(const Residue &other) const
// {
//     return rnum == other.rnum and
//            ( d.data() == other.d.data() or *d == *(other.d) );
// }
//
// /** Comparison operator */
// bool Residue::operator!=(const Residue &other) const
// {
//     return rnum != other.rnum or
//            ( d.data() != other.d.data() and *d != *(other.d) );
// }
//
// /** Make a shallow copy of 'other' */
// Residue& Residue::operator=(const Residue &other)
// {
//     d = other.d;
//     rnum = other.rnum;
//     return *this;
// }
//
// /** Return the 'ith' atom of this residue
//
//     \throw SireError::invalid_index
// */
// const Atom& Residue::operator[](int i) const
// {
//     return d->at(rnum, i);
// }
//
// /////////////////////////////////////////////////////////
//
//
// ///// Memory mangement and interface with molecule ///////
//
// /** Return a deep copy of this residue. Changing the deep copy will not
//     change the original */
// Residue Residue::deepCopy() const
// {
//     return Residue(moldata.deepCopy(), rnum);
// }
//
// /** Return a shallow copy of this residue. Changing the shallow copy
//     will change the original */
// Residue Residue::shallowCopy()
// {
//     return Residue(moldata.shallowCopy(), rnum);
// }
//
// /** Detach this residue from shared storage. Any changes to this residue will
//     now no longer effect the other views of the Molecule. */
// void Residue::detach()
// {
//     moldata.detach();
// }
//
// /** Return a shallow copy Molecule view of the Molecule to which this residue
//     belongs. Any changes to the molecule will change this residue. */
// Molecule Residue::molecule()
// {
//     return moldata.molecule();
// }
//
// /////////////////////////////////////////////////////////
//
//
// ///// Querying the residue //////////////////////////////
//
// /** Return the name of this residue */
// QString Residue::name() const
// {
//     return moldata.residueName(rnum);
// }
//
// /** Return a string representation of this residue */
// QString Residue::toString() const
// {
//     return QObject::tr("%1(%2): nAtoms() == %3, nBonds() == %4")
//                       .arg(name(), qstr(number()), qstr(nAtoms()), qstr(nBonds()));
// }
//
// /** Return whether or not the residue is null (has no atoms) */
// bool Residue::isNull() const
// {
//     return nAtoms() == 0;
// }
//
// /** Return whether or not the molecule is empty (has no atoms) */
// bool Residue::isEmpty() const
// {
//     return nAtoms() == 0;
// }
//
// /** Return the residue number of this residue */
// ResNum Residue::number() const
// {
//     return rnum;
// }
//
// /** Return the residue number of this residue */
// ResNum Residue::resNum() const
// {
//     return rnum;
// }
//
// /** Return the set of all CutGroups that are involved in this residue. Note that
//     the CutGroups may contain atoms in other residues as there is not a one-to-one
//     correspondance between CutGroups and Residues. */
// CutGroupSet Residue::cutGroups() const
// {
//     return moldata.cutGroups(rnum);
// }
//
// /** Return the 'ith' atom in this residue
//
//     \throw SireError::invalid_index
// */
// const Atom& Residue::at(int i) const
// {
//     return moldata.at(rnum,i);
// }
//
// /** Return the number of atoms in this residue */
// int Residue::count() const
// {
//     return moldata.nAtoms(rnum);
// }
//
// /** Return the number of atoms in this residue */
// int Residue::size() const
// {
//     return moldata.nAtoms(rnum);
// }
//
// /** Return the number of atoms in this residue */
// int Residue::nAtoms() const
// {
//     return moldata.nAtoms(rnum);
// }
//
// /** Return a list of the names of all of the atoms in the residue, in the
//     same order as the atoms are contained in this residue. */
// QStringList Residue::atomNames() const
// {
//     return moldata.atomNames(rnum);
// }
//
// /** Return the number of bonds in this residue */
// int Residue::nBonds() const
// {
//     return moldata.connectivity(rnum).nBonds();
// }
//
// /** Return the connectivity of this residue. */
// ResidueBonds Residue::connectivity() const
// {
//     return moldata.connectivity(rnum);
// }
//
// /** Return whether or not this residue contains an atom called 'name' */
// bool Residue::contains(const QString &name) const
// {
//     return moldata.contains( AtomIndex(name,rnum) );
// }
//
// /** Return the atom in this residue called 'name'
//
//     \throw SireMol::missing_atom
// */
// const Atom& Residue::atom(const QString &name) const
// {
//     return moldata.atom( AtomIndex(name,rnum) );
// }
//
// /** Return the ith atom in this residue.
//
//     \throw SireError::invalid_index
// */
// const Atom& Residue::atom(int i) const
// {
//     return moldata.at(rnum, i);
// }
//
// /** Return the geometric line representing the bond from atoms called atom0-atom1.
//     Note that there doesn't actually have to be a real bond between these atoms.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Line Residue::bond(const QString &atom0, const QString &atom1) const
// {
//     return moldata.bond( Bond(atom0,atom1,rnum) );
// }
//
// /** Return the geometric triangle representing the angle from atoms called atom0-atom1-atom2
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Triangle Residue::angle(const QString &atom0, const QString &atom1,
//                                    const QString &atom2) const
// {
//     return moldata.angle( SireMol::Angle(atom0,atom1,atom2,rnum) );
// }
//
// /** Return the geometric torsion representing the dihedral from atoms called
//     atom0-atom1-atom2-atom3
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion Residue::dihedral(const QString &atom0, const QString &atom1,
//                                     const QString &atom2, const QString &atom3) const
// {
//     return moldata.dihedral( Dihedral(atom0,atom1,atom2,atom3,rnum) );
// }
//
// /** Return the geometric torsion representing the improper angle from atoms
//     called atom0-atom1-atom2-atom3
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion Residue::improper(const QString &atom0, const QString &atom1,
//                                     const QString &atom2, const QString &atom3) const
// {
//     return moldata.improper( Improper(atom0,atom1,atom2,atom3,rnum) );
// }
//
// /** Return the length of the bond between atoms called 'atom0' and 'atom1'
//
//     \throw SireMol::missing_atom
// */
// double Residue::measure(const QString &atom0, const QString &atom1) const
// {
//     return moldata.measure( Bond(atom0,atom1,rnum) );
// }
//
// /** Return the size of the angle between atoms called atom0-atom1-atom2
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle Residue::measure(const QString &atom0, const QString &atom1,
//                                   const QString &atom2) const
// {
//     return moldata.measure( SireMol::Angle(atom0,atom1,atom2,rnum) );
// }
//
// /** Return the size of the dihedral angle for atoms called atom0-atom1-atom2-atom3
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle Residue::measure(const QString &atom0, const QString &atom1,
//                                   const QString &atom2, const QString &atom3) const
// {
//     return moldata.measure( Dihedral(atom0,atom1,atom2,atom3,rnum) );
// }
//
// /** Return the size of the improper angle for atoms called atom0-atom1-atom2-atom3
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle Residue::measureImproper(const QString &atom0, const QString &atom1,
//                                           const QString &atom2, const QString &atom3) const
// {
//     return moldata.measure( Improper(atom0,atom1,atom2,atom3,rnum) );
// }
//
// /** Return the set of atoms in this residue */
// AtomSet Residue::atoms() const
// {
//     return moldata.atoms(rnum);
// }
//
// /** Return the vector of atoms in this residue */
// AtomVector Residue::atomVector() const
// {
//     return moldata.atomVector(rnum);
// }
//
// /** Return the list of bonds that involve this residue */
// BondList Residue::bonds() const
// {
//     return moldata.connectivity(rnum).bonds();
// }
//
// /** Return a list of residues that are bonded to this residue. Note that the
//     residues will be on a shallow copy of the data of this molecule, so changing
//     any of the residues may change this residue. This returns an empty list
//     if there aren't any residues bonded to this one. */
// ResidueSet Residue::residuesBondedTo()
// {
//     return moldata.residuesBondedTo(rnum);
// }
//
// /////////////////////////////////////////////////////////
//
//
// //// Moving the residue /////////////////////////////////
//
// /** Translate all of the atoms in this residue by 'delta' */
// void Residue::translate(const Vector &delta)
// {
//     moldata.translate(rnum,delta);
// }
//
// /** Translate the atom called 'atm' by 'delta'. This will throw an exception if
//     there is no atom with this name in this residue.
//
//     \throw SireMol::missing_atom
// */
// void Residue::translate(const QString &atm, const Vector &delta)
// {
//     AtomIndexSet set;
//     set.insert( AtomIndex(atm,rnum) );
//     moldata.translate( set, delta );
// }
//
// /** Translate the atoms in 'atoms' by 'delta'. An exception is thrown
//     if this group contains atoms that are not in this residue.
//
//     \throw SireMol::missing_atom
// */
// void Residue::translate(const QStringList &atoms, const Vector &delta)
// {
//     moldata.translate(rnum, atoms, delta);
// }
//
// /** Rotate the entire residue by the quaternion 'quat' around the point 'point'. */
// void Residue::rotate(const Quaternion &quat, const Vector &point)
// {
//     moldata.rotate(rnum, quat, point);
// }
//
// /** Rotate the single atom 'atm' in this residue by the quaternion 'quat' around
//     the point 'point'. An exception will be thrown if there is no such atom in this residue.
//
//     \throw SireMol::missing_atom
// */
// void Residue::rotate(const QString &atm, const Quaternion &quat, const Vector &point)
// {
//     AtomIndexSet set;
//     set.insert( AtomIndex(atm,rnum) );
//     moldata.rotate( set, quat, point );
// }
//
// /** Rotate all of the atoms in 'atoms' by the quaternion 'quat'  around the point 'point'.
//     This will throw an exception if 'atoms' contains atoms that are not in this
//     residue.
//
//     \throw SireMol::missing_atom
// */
// void Residue::rotate(const QStringList &atoms, const Quaternion &quat, const Vector &point)
// {
//     moldata.rotate(rnum, atoms, quat, point);
// }
//
// /////////////////////////////////////////////////////////
//
// //// Internal geometry moves ////////////////////////////
//
// /** Change the bond 'bnd', only moving the atoms in this residue. This will not
//     move any other atoms in the molecule. Note that the move will ignore any
//     inter-residue bonding, so the move may (badly) change interresidue bond
//     lengths and angles.
//
//     If you wish to perform a move that will not change any inter-residue bonds
//     or angles, then you need to add the atoms that are involved in inter-residue
//     bonds to the list of anchors. The list of atoms in inter-residue bonds
//     can be obtained from the editres.connectivity().anchors() function, e.g.
//     call
//
//     Note that the bond must contain at least one atom that is in this residue
//     or else an exception will be thrown
//
//     \code
//     residue.change(bnd, delta, weightfunc, residue.connectivity().anchors());
//     \endcode
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
//
// */
// void Residue::change(const Bond &bnd, double delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     if (SireMaths::isZero(delta))
//         return;
//
//     //split the residue into two based on the two atoms of the bond
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(bnd, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     //tell the moldata to move the two groups by delta along the vector of the bond...
//     moldata.change(bnd, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overloaded function that uses the default RelFromMass weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const Bond &bnd, double delta, const AtomIndexSet &anchors)
// {
//     change(bnd, delta, RelFromMass(), anchors);
// }
//
// /** Change the angle 'ang' by an angle of 'delta'.
//
//     This has the same restrictions as Residue::change(Bond), i.e. the move will
//     only move atoms in this residue
//
//     Note that the angle must contain at least one atom that is in this residue
//     or an exception will be thrown
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     if (delta.isZero())
//         return;
//
//     //split the residue into two parts...
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(ang, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     //tell the moldata to move the two groups by delta about this angle
//     moldata.change(ang, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overloaded function that uses the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                      const AtomIndexSet &anchors)
// {
//     change(ang, delta, RelFromMass(), anchors);
// }
//
// /** Change the dihedral 'dih' by an angle of 'delta'.
//
//     This has the same restrictions as Residue::change(Bond), i.e. the move
//     will only move atoms in this residue
//
//     Note that the dihedral must contain at least one atom that is in this
//     residue or else an exception will be thrown
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const Dihedral &dih, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     if (delta.isZero())
//         return;
//
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(dih, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     //tell the moldata to move the two groups around the dihedral
//     moldata.change(Bond(dih.atom1(),dih.atom2()), delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overloaded function used to use the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const Dihedral &dih, const SireMaths::Angle &delta,
//                      const AtomIndexSet &anchors)
// {
//     change(dih, delta, RelFromMass(), anchors);
// }
//
// /** Overloaded function that moves all atoms of a dihedral (about the central
//     bond of the dihedral)
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const Bond &dih, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     if (delta.isZero())
//         return;
//
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(dih, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     //tell the moldata to move the two groups around the dihedral
//     moldata.change(dih, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overloaded function use to use the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const Bond &bnd, const SireMaths::Angle &delta, const AtomIndexSet &anchors)
// {
//     change(bnd, delta, RelFromMass(), anchors);
// }
//
// /** Change the improper angle by an angle of 'delta'.
//
//     This has the same restrictions as Residue::change(Bond), i.e. the move
//     will only move atoms in this residue
//
//     Note that the improper angle must contain at least one atom that is in this
//     residue or else an exception will be thrown
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const Improper &improper, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     if (delta.isZero())
//         return;
//
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(improper, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     //tell the moldata to move the two groups around the dihedral
//     moldata.change(improper, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overloaded function used to use the default RelFromMass() function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::change(const Improper &improper, const SireMaths::Angle &delta,
//                      const AtomIndexSet &anchors)
// {
//     change(improper, delta, RelFromMass(), anchors);
// }
//
// /** This is a convienience function that allows you to directly set the length of
//     a bond. Note that this uses Residue::change(Bond), so has the same restrictions.
//
//     Note that at least one of the atoms in the bond must be in this residue
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const Bond &bnd, double size,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(bnd, size-moldata.measure(bnd), weightfunc, anchors);
// }
//
// /** Overloaded function used to use the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const Bond &bnd, double size, const AtomIndexSet &anchors)
// {
//     set(bnd, size, RelFromMass(), anchors);
// }
//
// /** This is a convienience function that allows you to directly set the size of
//     an angle. Note that this uses Residue::change(Angle), so has the same restrictions.
//
//     Note that at least one of the atoms in the angle must be in this residue
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(ang, size-moldata.measure(ang), weightfunc, anchors);
// }
//
// /** Overloaded function used to use the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
//                   const AtomIndexSet &anchors)
// {
//     set(ang, size, RelFromMass(), anchors);
// }
//
// /** This is a convienience function that allows you to directly set the size of
//     a dihedral. Note that this uses Residue::change(Dihedral), so has the same restrictions.
//
//     Note that at least one of the atoms in the dihedral must be in this residue
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const Dihedral &dih, const SireMaths::Angle &size,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(dih, size-moldata.measure(dih), weightfunc, anchors);
// }
//
// /** Overloaded function used to use the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const Dihedral &dih, const SireMaths::Angle &size, const AtomIndexSet &anchors)
// {
//     change(dih, size-moldata.measure(dih), anchors);
// }
//
// /** This is a convienience function that allows you to directly set the size of
//     a dihedral. Note that this uses Residue::change(Bond,SireMaths::Angle),
//     so has the same restrictions.
//
//     Note that this moves all of the atoms either side of the central bond of the dihedral
//
//     Note that at least one of the atoms in the dihedral must be in this residue
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::setAll(const Dihedral &dih, const SireMaths::Angle &size,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(dih, size-moldata.measure(dih), weightfunc, anchors);
// }
//
// /** Overloaded function used to use the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::setAll(const Dihedral &dih, const SireMaths::Angle &size,
//                      const AtomIndexSet &anchors)
// {
//     setAll(dih, size, RelFromMass(), anchors);
// }
//
// /** This is a convienience function that allows you to directly set the size of
//     an improper angle. Note that this uses Residue::change(Improper),
//     so has the same restrictions.
//
//     Note that at least one of the atoms in the improper angle must be in this residue
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const Improper &improper, const SireMaths::Angle &size,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     set(improper, size-moldata.measure(improper), weightfunc, anchors);
// }
//
// /** Overloaded function used to use the default RelFromMass() weight function
//
//     \throw SireMol::anchor_error
//     \throw SireMol::missing_atom
//     \throw SireMol::ring_error
// */
// void Residue::set(const Improper &improper, const SireMaths::Angle &size,
//                   const AtomIndexSet &anchors)
// {
//     set(improper, size, RelFromMass(), anchors);
// }

/////////////////////////////////////////////////////////
