//
// #include "qhash_siremol.h"
//
// #include <QSharedData>
// #include <QMap>
//
// #include "atom.h"
// #include "editmoldata.h"
// #include "atomvector.h"
// #include "editmol.h"
// #include "editres.h"
// #include "moleculebonds.h"
// #include "atomidgroup.h"
// #include "bond.h"
// #include "angle.h"
// #include "dihedral.h"
// #include "improper.h"
// #include "weightfunction.h"
// #include "moleculesignature.h"
// #include "molecule.h"
//
// #include "SireMol/errors.h"
//
// #include "SireMaths/vector.h"
// #include "SireMaths/quaternion.h"
// #include "SireMaths/matrix.h"
// #include "SireMaths/line.h"
// #include "SireMaths/triangle.h"
// #include "SireMaths/torsion.h"
// #include "SireMaths/rotate.h"
//
// #include "SireStream/datastream.h"
// using namespace SireStream;
//
// static const
// RegisterMetaType<SireMol::EditMolData> r_editmoldata("SireMol::EditMolData", MAGIC_ONLY);
//
// namespace SireMol
// {
// class EditMolDataPvt;
// }
//
// QDataStream& operator<<(QDataStream&, const SireMol::EditMolDataPvt&);
// QDataStream& operator>>(QDataStream&, SireMol::EditMolDataPvt&);
//
// namespace SireMol
// {
//
// using SireMaths::Matrix;
//
// /**
// This class holds the private implementation of EditMolData. This allows the
// EditMolData class to be implicitly shared.
//
// @author Christopher Woods
// */
// class EditMolDataPvt : public QSharedData
// {
//
// friend QDataStream& ::operator<<(QDataStream&, const EditMolDataPvt&);
// friend QDataStream& ::operator>>(QDataStream&, EditMolDataPvt&);
//
// public:
//     EditMolDataPvt(const QString &molname = QString::null);
//     EditMolDataPvt(const Molecule &mol);
//     EditMolDataPvt(const EditMolDataPvt &other);
//
//     ~EditMolDataPvt();
//
//     bool operator==(const EditMolDataPvt &other) const;
//     bool operator!=(const EditMolDataPvt &other) const;
//
//     const MoleculeBonds& connectivity() const;
//     ResidueBonds connectivity(ResNum resnum) const;
//
//     MoleculeSignature signature() const;
//
//     const QString& name() const;
//     void setName(const QString &name);
//
//     QList<ResNum> residueNumbers() const;
//     QStringList residueNames() const;
//
//     void clear();
//     void clean();
//
//     const Atom& atom(const AtomIndex &atm) const;
//
//     QVector<Atom> atoms(ResNum resnum) const;
//     QVector<Vector> coordinates(ResNum resnum) const;
//
//     void add(const Atom &atom);
//     void remove(const AtomIndex &atom);
//
//     void add(const Bond &bond);
//     void remove(const Bond &bond);
//
//     void removeAllBonds(ResNum resnum);
//     void removeAllBonds(const AtomIndex &atom);
//     void removeAllBonds();
//
//     void update(const Atom &atom);
//     void update(const AtomIndex &atm, const Atom &details);
//     void update(const AtomIndex &atm, const Vector &coords);
//     void update(const AtomIndex &atm, const Element &element);
//
//     bool contains(ResNum resnum) const;
//     bool contains(const AtomIndex &atm) const;
//     bool contains(const Bond &bond) const;
//
//     void addResidue(ResNum resnum, const QString &resnam);
//
//     void removeResidue(ResNum resnum);
//     void removeResidue(const QString &resname);
//     void removeAllResidues(const QString &resname);
//
//     void translate(const AtomIndex &atom, const Vector &delta);
//     void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
//
//     void translate(const AtomIDGroup &group, const Vector &delta);
//     void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
//
//     void translate(ResNum resnum, const Vector &delta);
//     void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
//
//     void translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
//     void rotate(ResNum resnum, const QStringList &atoms,
//                 const Quaternion &quat, const Vector &point);
//
//     void translate(const Vector &delta);
//     void rotate(const Quaternion &quat, const Vector &point);
//
//     QString residueName(ResNum resnum) const;
//     void setResidueName(ResNum resnum, const QString &name);
//
//     void renumberResidue(ResNum resnum, ResNum newnum);
//
//     void clearResidue(ResNum resnum);
//
//     bool isEmpty() const;
//     bool isEmptyResidue(ResNum resnum) const;
//
//     const Atom& at(ResNum resnum, int i) const;
//
//     int nAtoms() const;
//     int nAtoms(ResNum resnum) const;
//
//     int nResidues() const;
//
//     SireMaths::Line bond(const Bond &bnd) const;
//     SireMaths::Triangle angle(const SireMol::Angle &ang) const;
//     SireMaths::Torsion dihedral(const Dihedral &dih) const;
//     SireMaths::Torsion improper(const Improper &improper) const;
//
//     QStringList atomNames(ResNum resnum) const;
//     QList<ResNum> residueNumbers(const QString &resnam) const;
//
//     QVector<Atom> atoms() const;
//     QVector<Vector> coordinates() const;
//
//     void finalise();
//
//     double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
//                      const WeightFunction &weightfunc) const;
//
//     void change(const Bond &bnd, double delta, const AtomIDGroup &group0,
//                 const AtomIDGroup &group1, const WeightFunction &weightfunc,
//                 const QSet<AtomIndex> &anchors);
//
//     void change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                 const AtomIDGroup &group0,
//                 const AtomIDGroup &group1, const WeightFunction &weightfunc,
//                 const QSet<AtomIndex> &anchors);
//
//     void change(const Bond &dih, const SireMaths::Angle &delta, const AtomIDGroup &group0,
//                 const AtomIDGroup &group1, const WeightFunction &weightfunc,
//                 const QSet<AtomIndex> &anchors);
//
//     void change(const Improper &improper, const SireMaths::Angle &delta,
//                 const AtomIDGroup &group0,
//                 const AtomIDGroup &group1, const WeightFunction &weightfunc,
//                 const QSet<AtomIndex> &anchors);
//
//     void checkResidue(ResNum resnum) const;
//
// private:
//
//     EditResAtomSet& residue(ResNum resnum);
//
//     ERAtomAccessor atom(const AtomIndex &atm);
//
//     void rotate(const AtomIndex &atm, const Matrix &rotmat, const Vector &point);
//     void rotate(ResNum resnum, const Matrix &rotmat, const Vector &point);
//     void rotate(ResNum resnum, const QStringList &atoms, const Matrix &rotmat,
//                 const Vector &point);
//     void rotate(const Matrix &rotmat, const Vector &point);
//
//     /** The name of this molecule */
//     QString molnme;
//
//     /** Set of all atoms in each residue, indexed by residue number, and sorted
//         by the order in which the atoms were added to the residue. */
//     QMap< ResNum, QHash<QString,Atom> > atms;
//
//     /** Hash mapping residue numbers to residue names */
//     QMap<ResNum, QString> resnmes;
//
//     /** Sorted list of residue numbers of residues in this molecule */
//     QList<ResNum> resnums;
//
//     /** The bonding within the molecule */
//     MoleculeBonds molbnds;
// };
//
// } //End of namespace SireMol
//
// using namespace SireMol;
//
// ////////////////
// //////////////// Implmentation of EditMolDataPvt
// ////////////////
//
// /** Serialise an EditMolDataPvt to a datastream */
// QDataStream& operator<<(QDataStream &ds, const EditMolDataPvt &mol)
// {
//     writeHeader(ds, r_editmoldata, 1) << mol.molnme << mol.atms
//                                       << mol.resnmes << mol.resnums << mol.molbnds;
//
//     return ds;
// }
//
// /** Deserialise an EditMolDataPvt from a datastream */
// QDataStream& operator>>(QDataStream &ds, EditMolDataPvt &mol)
// {
//     VersionID v = readHeader(ds, r_editmoldata);
//
//     if (v == 1)
//     {
//         ds >> mol.molnme >> mol.atms >> mol.resnmes
//            >> mol.resnums >> mol.molbnds;
//     }
//     else
//         throw version_error(v, "1", r_editmoldata, CODELOC);
//
//     return ds;
// }
//
// /** Construct an EditMolPvt for the molecule called 'molname' */
// EditMolDataPvt::EditMolDataPvt(const QString &molname) : QSharedData(), molnme(molname)
// {}
//
// /** Construct an EditMolDataPvt that contains the data that represents the molecule 'mol' */
// EditMolDataPvt::EditMolDataPvt(const Molecule &mol) : QSharedData(), molnme(mol.name())
// {
//     //loop over each residue in the molecule and add it in turn...
//     int nres = mol.nResidues();
//     QVector<ResNum> resnums = mol.residueNumbers();
//     QStringList resnames = mol.residueNames();
//
//     for (int i=0; i<nres; ++i)
//     {
//         this->addResidue(resnums.at(i),resnames.at(i));
//
//         QVector<Atom> atms = mol.atoms(resnums.at(i));
//         int nats = atms.count();
//
//         for (int j=0; j<nats; ++j)
//             this->add(atms.at(j));
//     }
//
//     molbnds = mol.connectivity();
// }
//
// /** Copy constructor */
// EditMolDataPvt::EditMolDataPvt(const EditMolDataPvt &other)
//                : QSharedData(),
//                  molnme(other.molnme), atms(other.atms),
//                  resnmes(other.resnmes), resnums(other.resnums), molbnds(other.molbnds)
// {}
//
// /** Destructor */
// EditMolDataPvt::~EditMolDataPvt()
// {}
//
// /** Comparison operator */
// bool EditMolDataPvt::operator==(const EditMolDataPvt &other) const
// {
//     return (this == &other) or
//            (molnme == other.molnme and atms == other.atms and resnmes == other.resnmes and
//             resnums == other.resnums and molbnds == other.molbnds);
// }
//
// bool EditMolDataPvt::operator!=(const EditMolDataPvt &other) const
// {
//     return (this != &other) and
//            (molnme != other.molnme or atms != other.atms or resnmes != other.resnmes or
//             resnums != other.resnums or molbnds != other.molbnds);
// }
//
// /** Return the name of the molecule */
// inline const QString& EditMolDataPvt::name() const
// {
//     return molnme;
// }
//
// /** Check that the residue with number 'resnum' is in this molecule. Throw
//     an exception if it isn't.
//
//     \throw SireMol::missing_residue
// */
// inline void EditMolDataPvt::checkResidue(ResNum resnum) const
// {
//     if (not resnmes.contains(resnum))
//         throw SireMol::missing_residue(QObject::tr(
//             "EditMol \"%1\" does not contain a residue with number \"%2\"")
//                 .arg(name(),resnum.toString()), CODELOC);
// }
//
// /** Return a reference to the EditResAtomSet for the residue with number 'resnum'.
//     This automatically creates the residue if it doesn't already exist. */
// EditResAtomSet& EditMolDataPvt::residue(ResNum resnum)
// {
//     if (not resnmes.contains(resnum))
//     {
//         addResidue(resnum, QObject::tr("UNK"));
//     }
//
//     return atms[resnum];
// }
//
// /** Return the list of atoms in the residue 'resnum', sorted in the same order
//     as they were added to this molecule. This will throw an exception if there
//     is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
// */
// QVector<Atom> EditMolDataPvt::atoms(ResNum resnum) const
// {
//     checkResidue(resnum);
//     return AtomSet(atms[resnum].values());
// }
//
// /** Return a vector of atoms in the residue 'resnum', sorted in the same order
//     as they were added to this molecule. This will throw an exception if there
//     is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
// */
// AtomVector EditMolDataPvt::atomVector(ResNum resnum) const
// {
//     checkResidue(resnum);
//     return atms[resnum].values();
// }
//
// /** Return a const reference to the bond data of the molecule */
// inline const MoleculeBonds& EditMolDataPvt::connectivity() const
// {
//     return molbnds;
// }
//
// /** Return the connectivity of the residue with number 'resnum'. This will
//     throw an exception if there is no such residue */
// inline ResidueBonds EditMolDataPvt::connectivity(ResNum resnum) const
// {
//     checkResidue(resnum);
//     return molbnds.residue(resnum);
// }
//
// /** Return a signature for the EditMol. This can be used to compare
//     this EditMol with another (or with Molecule) to see if the
//     two objects describe the same molecules (i.e. contain the same
//     residues, atoms and bonds) */
// inline MoleculeSignature EditMolDataPvt::signature() const
// {
//     QHash<ResidueID,QStringSet> atmnames;
//
//     //loop over all of the residues
//     QMapIterator<ResNum,QString> it(resnmes);
//     while(it.hasNext())
//     {
//         it.next();
//
//         //get the atom names in this residue
//         atmnames.insert( ResidueID(it.value(),it.key()), atms[it.key()].keys().toSet() );
//     }
//
//     return MoleculeSignature(molnme, atmnames, molbnds);
// }
//
// /** Set the name of the molecule */
// inline void EditMolDataPvt::setName(const QString &name)
// {
//     molnme = name;
// }
//
// /** Return whether or not this molecule contains a residue with residue
//     number 'resnum' */
// inline bool EditMolDataPvt::contains(ResNum resnum) const
// {
//     return resnmes.contains(resnum);
// }
//
// /** Return whether or not this molecule contains an atom with index 'atm'.
//     Note that this returns true only if there is an exact match (case-sensitive
//     match) */
// inline bool EditMolDataPvt::contains(const AtomIndex &atm) const
// {
//     return atms.contains(atm.resNum()) and atms[atm.resNum()].has_key(atm.name());
// }
//
// /** Return whether or not this molecule contains the bond 'bond'. */
// inline bool EditMolDataPvt::contains(const Bond &bond) const
// {
//     return molbnds.contains(bond);
// }
//
// /** Return the atom in this residue with the name 'atomname'. This will first
//     try to find an atom that matches the name exactly - i.e. a case sensitive
//     match. If that fails, it will then try to match an all upper-case version
//     of the name, then an all lower-case version of the name. If these fail,
//     then it will try and match the first any-case version of the name that it
//     can find. It is always much better and faster to match the exact name.
//
//     This 'sloppy' matching is to make it easier for the user working with
//     the code. It may however lead to confusion if you really want to use
//     different atoms with the same name but with different case. I strongly
//     urge against this, but I know that some people do like to use case
//     to distinguish atoms, so I will not prevent it.
//
//     \throw SireMol::missing_atom
// */
// inline const Atom& EditMolDataPvt::atom(const AtomIndex &atm) const
// {
//     if (not atms.contains(atm.resNum()))
//         throw SireMol::missing_atom(QObject::tr(
//             "Molecule \"%1\" does not contain atom %2 as it doesn't contain the residue!")
//                 .arg(name()).arg(atm.toString()), CODELOC);
//     else
//     {
//         //get the atoms in the residue
//         EditResAtomSet resatms = atms[atm.resNum()];
//
//         if (resatms.has_key(atm.name()))
//             //exact match!
//             return resatms.at_key(atm.name());
//         else
//         {
//             //try an all uppercase match
//             QString upper = atm.name().toUpper();
//             if (resatms.has_key(upper))
//                 return resatms.at_key(upper);
//
//             //try an all lowercase match
//             QString lower = atm.name().toLower();
//             if (resatms.has_key(lower))
//                 return resatms.at_key(lower);
//
//             //that hasn't worked. We will now scan all atoms in the residue
//             //and see if there is an any-case match.
//             int nats = resatms.count();
//             for (int i=0; i<nats; ++i)
//             {
//                 if (resatms[i].name().toLower() == lower)
//                     return resatms[i];
//             }
//
//             throw SireMol::missing_atom(QObject::tr(
//                 "There is no atom called \"%1\" in residue %2 in molecule \"%3\"")
//                     .arg(atm.name(),atm.resNum().toString(),name()), CODELOC);
//         }
//     }
// }
//
// /** Internal function used to get a non-const reference to the atom
//     with atomindex 'atm'. This throws an exception if there is no such
//     atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// inline ERAtomAccessor EditMolDataPvt::atom(const AtomIndex &atm)
// {
//     if (not this->contains(atm))
//         throw SireMol::missing_atom(QObject::tr(
//             "There is no atom called \"%1\" in residue %2 in molecule \"%3\"")
//                 .arg(atm.name(),atm.resNum().toString(),name()), CODELOC);
//
//     ERAtomAccessor it(atms[atm.resNum()]);
//     it.findKey(atm.name());
//
//     return it;
// }
//
// /** Return the name of the residue with number 'resnum'. This throws an
//     exception if there is no residue with this number.
//
//     \throw SireMol::missing_residue
// */
// QString EditMolDataPvt::residueName(ResNum resnum) const
// {
//     if (not resnmes.contains(resnum))
//         throw SireMol::missing_residue(QObject::tr(
//             "Molecule %1 is missing a residue numbered %2")
//                 .arg(name(),resnum.toString()), CODELOC);
//
//     return resnmes[resnum];
// }
//
// /** Return the list of all of the residue numbers in this molecule */
// QList<ResNum> EditMolDataPvt::residueNumbers() const
// {
//     return resnums;
// }
//
// /** Return the list of all residue names */
// QStringList EditMolDataPvt::residueNames() const
// {
//     return resnmes.values();
// }
//
// /** Return the set of residue IDs of all of the residues in this molecule */
// ResidueIDSet EditMolDataPvt::residueIDs() const
// {
//     ResidueIDSet resids;
//     int nres = resnums.count();
//     resids.reserve(nres);
//
//     for (int i=0; i<nres; ++i)
//     {
//         ResNum resnum = resnums[i];
//         resids.insert( ResidueID(resnmes[resnum], resnum) );
//     }
//
//     return resids;
// }
//
// /** Return the list of residue numbers of residues called 'resnam'. This will
//     return an empty list if there are no residues with this name */
// QList<ResNum> EditMolDataPvt::residueNumbers(const QString &resnam) const
// {
//     QList<ResNum> rnums;
//
//     QMapIterator<ResNum, QString> it(resnmes);
//
//     while( it.hasNext() )
//     {
//         it.next();
//
//         if (it.value() == resnam)
//             rnums.append(it.key());
//     }
//
//     return rnums;
// }
//
// /** Completely clear this molecule */
// void EditMolDataPvt::clear()
// {
//     atms.clear();
//     molbnds.clear();
//     resnmes.clear();
// }
//
// /** Clean up the EditMolData - this removes all empty residues (those that don't
//     contain any atoms) */
// void EditMolDataPvt::clean()
// {
//     //delete all empty residues
//     QMutableMapIterator< ResNum, EditResAtomSet > it(atms);
//
//     while (it.hasNext())
//     {
//         if (it.value().isEmpty())
//         {
//             //remove the residue from the list of residue names
//             resnmes.remove(it.key());
//             //remove the residue
//             it.remove();
//         }
//     }
// }
//
// /** Finalise a molecule - this cleans up the molecule by removing empty residues
//     and squeezing memory usage so that as the molecule consumes as little memory as
//     possible. */
// void EditMolDataPvt::finalise()
// {
//     this->clean();
//
//     molbnds.finalise();
//
//     QMutableMapIterator<ResNum,EditResAtomSet> it(atms);
//
//     while(it.hasNext())
//     {
//         it.next();
//         it.value().squeeze();
//     }
// }
//
// /** Add the atom 'atom' to the molecule. This will automatically create
//     the residue if it is missing. This will throw an exception if an atom
//     with this name already exists.
//
//     \throw SireMol::duplicate_atom
// */
// void EditMolDataPvt::add(const Atom &atom)
// {
//     EditResAtomSet &resatms = residue(atom.resNum());
//
//     if (resatms.has_key(atom.name()))
//         throw SireMol::duplicate_atom(QObject::tr(
//             "Cannot add another atom called \"%1\" to molecule \"%2\" (atom = %3)")
//                 .arg(atom.name(),name(),atom.toString()), CODELOC);
//
//     resatms.insert(atom);
// }
//
// /** Remove the atom 'atom' from this molecule. This does nothing if this
//     atom is not present in the molecule. */
// void EditMolDataPvt::remove(const AtomIndex &atom)
// {
//     if (atms.contains(atom.resNum()))
//     {
//         EditResAtomSet &resatms = atms[atom.resNum()];
//
//         if (resatms.has_key(atom.name()))
//         {
//             resatms.remove_key(atom.name());
//             this->removeAllBonds(atom);
//         }
//     }
// }
//
// /** Add a bond - this will throw an exception if either of the atoms in the
//     bond are missing
//
//     \throw SireMol::missing_atom
// */
// void EditMolDataPvt::add(const Bond &bond)
// {
//      if ( not (this->contains(bond.atom0()) and this->contains(bond.atom1())) )
//      {
//         QString err;
//         if (not this->contains(bond.atom0()))
//             err = QObject::tr("No atom %1 (%2) in molecule \"%3\". ")
//                 .arg(bond.atom0().name(),bond.atom0().resNum().toString(),name());
//
//         if (not this->contains(bond.atom1()))
//             err += QObject::tr("No atom %1 (%2) in molecule \"%3\".")
//                 .arg(bond.atom1().name(),bond.atom1().resNum().toString(),name());
//
//         throw SireMol::missing_atom(err, CODELOC);
//      }
//
//      molbnds.add(bond);
// }
//
// /** Remove a bond - this will do nothing if the bond doesn't exist */
// void EditMolDataPvt::remove(const Bond &bond)
// {
//     molbnds.remove(bond);
// }
//
// /** Remove all bonds involving the residue with residue number 'resnum' - this
//     does nothing if this residue doesn't exist, or if it already has no bonds. */
// void EditMolDataPvt::removeAllBonds(ResNum resnum)
// {
//     molbnds.removeAll(resnum);
// }
//
// /** Remove all bonds involving the atom 'atom'. This does nothing if this
//     atom is not in this molecule, or if it doesn't have any bonds */
// void EditMolDataPvt::removeAllBonds(const AtomIndex &atom)
// {
//     molbnds.removeAll(atom);
// }
//
// /** Remove all bonding information from this molecule */
// void EditMolDataPvt::removeAllBonds()
// {
//     molbnds.clear();
// }
//
// /** Update the atom information for 'atom'. This will throw an exception
//     if this atom doesn't already exist.
//
//     \throw SireMol::missing_atom
// */
// void EditMolDataPvt::update(const Atom &atm)
// {
//     ERAtomAccessor it = atom(atm);
//
//     it.value().element() = atm.element();
//     it.value().vector() = atm.vector();
// }
//
// /** Update the atom 'atm' with the contents of 'details' */
// void EditMolDataPvt::update(const AtomIndex &atm, const Atom &details)
// {
//     atom(atm).value() = Atom(details.number(), atm, details.element(), details.vector());
// }
//
// /** Update the information for atom 'atm' - change the element */
// void EditMolDataPvt::update(const AtomIndex &atm, const Element &element)
// {
//     atom(atm).value().element() = element;
// }
//
// /** Update the information for atom 'atm' - change the coordinates */
// void EditMolDataPvt::update(const AtomIndex &atm, const Vector &coords)
// {
//     atom(atm).value().vector() = coords;
// }
//
// /** Add a residue with residue number 'resnum' and name 'resnam' to this molecule.
//     This will throw an exception if a residue with this number already exists
//
//     \throw SireMol::duplicate_residue
// */
// void EditMolDataPvt::addResidue(ResNum resnum, const QString &resnam)
// {
//     if (resnmes.contains(resnum))
//         throw SireMol::duplicate_residue(QObject::tr(
//             "A residue with number %1 already exists. Cannot replace %2(%1) with %4(%1)")
//                 .arg(resnum.toString(),resnmes[resnum],resnam), CODELOC);
//
//     resnmes.insert(resnum, resnam);
//     resnums.append(resnum);
//     qSort(resnums);
//     atms.insert(resnum, EditResAtomSet());
// }
//
// /** Remove the residue with number 'resnum' from this molecule. This remove all of the
//     residue's atoms and all of the bonds to the residue. This does nothing if there
//     is no residue with this number in the molecule. */
// void EditMolDataPvt::removeResidue(ResNum resnum)
// {
//     if (resnmes.contains(resnum))
//     {
//         //remove all bonding involving this residue
//         molbnds.removeAll(resnum);
//
//         //remove the residue's atoms
//         atms.remove(resnum);
//
//         //remove the residue
//         resnmes.remove(resnum);
//     }
// }
//
// /** Remove the first residue with name 'resnam'. Does nothing if there are no residues
//     called 'resnam'. */
// void EditMolDataPvt::removeResidue(const QString &resnam)
// {
//     QList<ResNum> resnums = residueNumbers(resnam);
//
//     if (resnums.count() > 0)
//         removeResidue(resnums[0]);
// }
//
// /** Remove all residues called 'resnam'. Does nothing if there are no residues with this
//     name. */
// void EditMolDataPvt::removeAllResidues(const QString &resnam)
// {
//     QList<ResNum> resnums = residueNumbers(resnam);
//
//     foreach( ResNum resnum, resnums )
//         removeResidue(resnum);
// }
//
// /** Renumber the residue with current number 'resnum' to the new number 'newnum'.
//
//     This will throw an exception if either there is no current residue with number
//     'resnum' or if there is an existing residue with number 'newnum'
//
//     \throw SireMol::missing_residue
//     \throw SireMol::duplicate_residue
// */
// void EditMolDataPvt::renumberResidue(ResNum resnum, ResNum newnum)
// {
//     throw SireError::incomplete_code(QObject::tr(
//           "Need to write code to renumber residues..."), CODELOC);
// }
//
// /** Return the vector of all of the atoms that are in this molecule. The atoms
//     are sorted according to their residue number, then in the order that
//     they were added to the residue. */
// AtomVector EditMolDataPvt::atomVector() const
// {
//     //get the number of atoms in the molecule
//     int nats = nAtoms();
//
//     //reserve sufficient space in the vector
//     AtomVector allatoms;
//     allatoms.reserve(nats);
//
//     QMapIterator<ResNum, EditResAtomSet> it(atms);
//
//     while( it.hasNext() )
//     {
//         it.next();
//
//         foreach( Atom atm, it.value().values() )
//             allatoms.append(atm);
//     }
//
//     return allatoms;
// }
//
// /** Return the set of all of the atoms that are in this molecule. The atoms
//     are sorted according to their residue number, then in the order that
//     they were added to the residue. */
// AtomSet EditMolDataPvt::atoms() const
// {
//     return AtomSet(atomVector());
// }
//
// /** Translate the atom 'atm' by 'delta'. This throws an exception if there is
//     no such atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// inline void EditMolDataPvt::translate(const AtomIndex &atm, const Vector &delta)
// {
//     atom(atm).value() += delta;
// }
//
// /** Rotate the atom 'atm' by the rotation matrix 'rotmat' around the point 'point'. This
//     will throw an exception if there is no such atom.
//
//     \throw SireMol::missing_atom
// */
// inline void EditMolDataPvt::rotate(const AtomIndex &atm, const Matrix &rotmat, const Vector &point)
// {
//     ERAtomAccessor it = atom(atm);
//     Atom &rotatm = it.value();
//     rotatm = SireMaths::rotate(rotatm, rotmat, point);
// }
//
// /** Rotate the atom 'atm' by the quaternion 'quat' around the point 'point'. This
//     will throw an exception if there is no such atom.
//
//     \throw SireMol::missing_atom
// */
// inline void EditMolDataPvt::rotate(const AtomIndex &atm, const Quaternion &quat, const Vector &point)
// {
//     rotate(atm, quat.toMatrix(), point);
// }
//
// /** Translate all of the atoms in the IDGroup 'group' by 'delta'. This will
//     throw an exception if any of the atoms are missing from this molecule.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
// */
// void EditMolDataPvt::translate(const AtomIDGroup &group, const Vector &delta)
// {
//     if (delta.isZero())
//         return;
//
//     //translate all of the residues
//     for (ResNumSet::const_iterator it = group.residues().begin();
//          it != group.residues().end(); ++it)
//     {
//         this->translate(*it, delta);
//     }
//
//     //now translate all of the atoms...
//     for (QSet<AtomIndex>::const_iterator it = group.atoms().begin();
//          it != group.atoms().end(); ++it)
//     {
//         this->translate(*it, delta);
//     }
// }
//
// /** Rotate all of the atoms in the IDGroup 'group' by 'delta'. This will
//     throw an exception if any of the atoms are missing from this molecule.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
// */
// void EditMolDataPvt::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
// {
//     if (quat.isIdentity())
//         return;
//
//     //as an optimisation, rotate using the matrix form of the quaternion
//     Matrix rotmat = quat.toMatrix();
//
//     //rotate all of the residues
//     for (ResNumSet::const_iterator it = group.residues().begin();
//          it != group.residues().end(); ++it)
//     {
//         this->rotate(*it, rotmat, point);
//     }
//
//     //now rotate all of the atoms...
//     for (QSet<AtomIndex>::const_iterator it = group.atoms().begin();
//          it != group.atoms().end(); ++it)
//     {
//         this->rotate(*it, rotmat, point);
//     }
// }
//
// /** Translate all of the atoms in residue 'resnum'. This will throw an exception if
//     there is no residue with this number in this molecule.
//
//     \throw SireMol::missing_residue
// */
// void EditMolDataPvt::translate(ResNum resnum, const Vector &delta)
// {
//     if (not contains(resnum))
//         throw SireMol::missing_residue(QObject::tr(
//             "There is no residue with number \"%1\" in molecule \"%2\"")
//                 .arg(resnum.toString(),name()), CODELOC);
//
//     if (delta.isZero())
//         return;
//
//     EditResAtomSet &resatms = atms[resnum];
//
//     ERAtomAccessor it(resatms);
//     while (it.hasNext())
//     {
//         it.next();
//         it.value() += delta;
//     }
// }
//
// /** Translate the atoms with names in 'atoms' in residue 'resnum' by delta. This will
//     throw an exception if there is no residue with this number, or if any of the
//     atoms are not in the residue.
//
//     \throw SireMol::missing_residue
//     \throw SireMol::missing_atom
// */
// void EditMolDataPvt::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
// {
//     if (not contains(resnum))
//         throw SireMol::missing_residue(QObject::tr(
//             "There is no residue with number \"%1\" in molecule \"%2\"")
//                 .arg(resnum.toString(),name()), CODELOC);
//
//     if (delta.isZero())
//         return;
//
//     EditResAtomSet &resatms = atms[resnum];
//
//     ERAtomAccessor it(resatms);
//
//     foreach( QString atmname, atoms )
//     {
//         it.findKey(atmname);
//
//         if (not it.isValid())
//             throw SireMol::missing_atom(QObject::tr(
//                 "No atom called \"%1\" in residue \"%2(%3)\" in molecule \"%4\"")
//                     .arg(atmname,residueName(resnum),resnum.toString(),name()), CODELOC);
//
//         it.value() += delta;
//     }
// }
//
// /** Rotate all of the atoms in the residue 'resnum' using the rotation matrix 'rotmat'
//     about the point 'point'. This will throw an exception if there is no such
//     residue in the molecule.
//
//     \throw SireMol::missing_residue
// */
// void EditMolDataPvt::rotate(ResNum resnum, const Matrix &rotmat, const Vector &point)
// {
//     if (not contains(resnum))
//         throw SireMol::missing_residue(QObject::tr(
//             "There is no residue with number \"%1\" in molecule \"%2\"")
//                 .arg(resnum.toString(),name()), CODELOC);
//
//     EditResAtomSet &resatms = atms[resnum];
//
//     ERAtomAccessor it(resatms);
//     while( it.hasNext() )
//     {
//         it.next();
//         Atom &rotatm = it.value();
//         rotatm = SireMaths::rotate(rotatm, rotmat, point);
//     }
// }
//
// /** Rotate all of the atoms in the residue 'resnum' using the quaternion 'quat'
//     about the point 'point'. This will throw an exception if there is no such
//     residue in the molecule.
//
//     \throw SireMol::missing_residue
// */
// inline void EditMolDataPvt::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
// {
//     if (not quat.isIdentity())
//         this->rotate(resnum, quat.toMatrix(), point);
// }
//
// /** Rotate the atoms with names in 'atoms' and in residue 'resnum' using the rotation
//     matrix 'rotmat' about the point 'point'. This will throw an exception if any of the atoms
//     are not in the residue, or if there is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
//     \throw SireMol::missing_atom
// */
// inline void EditMolDataPvt::rotate(ResNum resnum, const QStringList &atoms,
//                                    const Matrix &rotmat, const Vector &point)
// {
//     if (not contains(resnum))
//         throw SireMol::missing_residue(QObject::tr(
//             "There is no residue with number \"%1\" in molecule \"%2\"")
//                 .arg(resnum.toString(),name()), CODELOC);
//
//     EditResAtomSet &resatms = atms[resnum];
//     ERAtomAccessor it( resatms );
//
//     foreach( QString atmnam, atoms )
//     {
//         it.findKey(atmnam);
//         if (not it.isValid())
//             throw SireMol::missing_atom(QObject::tr(
//                 "No atom called \"%1\" in residue \"%2(%3)\" in molecule \"%4\"")
//                     .arg(atmnam,residueName(resnum),resnum.toString(),name()), CODELOC);
//
//         Atom &rotatm = it.value();
//         rotatm = SireMaths::rotate(rotatm, rotmat, point);
//     }
// }
//
// /** Rotate the atoms with names in 'atoms' and in residue 'resnum' using the quaternion
//     'quat' about the point 'point'. This will throw an exception if any of the atoms
//     are not in the residue, or if there is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
//     \throw SireMol::missing_atom
// */
// inline void EditMolDataPvt::rotate(ResNum resnum, const QStringList &atoms,
//                                    const Quaternion &quat, const Vector &point)
// {
//     if (not quat.isIdentity())
//         this->rotate(resnum, atoms, quat.toMatrix(), point);
// }
//
// /** Translate the entire molecule by 'delta' */
// void EditMolDataPvt::translate(const Vector &delta)
// {
//     if (not delta.isZero())
//     {
//         QMutableMapIterator<ResNum,EditResAtomSet> it(atms);
//
//         while( it.hasNext() )
//         {
//             it.next();
//             EditResAtomSet &resatms = it.value();
//
//             ERAtomAccessor it2(resatms);
//             while (it2.hasNext())
//             {
//                 it2.next();
//                 it2.value() += delta;
//             }
//         }
//     }
// }
//
// /** Rotate the entire molecule by the rotation matrix 'rotmat' about the point 'point'. */
// void EditMolDataPvt::rotate(const Matrix &rotmat, const Vector &point)
// {
//     QMutableMapIterator<ResNum,EditResAtomSet> it(atms);
//
//     while( it.hasNext() )
//     {
//         it.next();
//
//         EditResAtomSet &resatms = it.value();
//
//         ERAtomAccessor it2(resatms);
//         while( it2.hasNext() )
//         {
//             it2.next();
//             Atom &rotatm = it2.value();
//             rotatm = SireMaths::rotate(rotatm, rotmat, point);
//         }
//     }
// }
//
// /** Rotate the entire molecule by the rotation matrix implied by the quaternion 'quat'
//     about the point 'point' */
// inline void EditMolDataPvt::rotate(const Quaternion &quat, const Vector &point)
// {
//     if (not quat.isIdentity())
//         rotate(quat.toMatrix(), point);
// }
//
// /** Set the name of residue 'resnum' to 'resname'. This will throw an
//     exception if there is no residue with this number in the molecule.
//
//     \throw SireMol::missing_residue
// */
// void EditMolDataPvt::setResidueName(ResNum resnum, const QString &resname)
// {
//     checkResidue(resnum);
//     resnmes[resnum] = resname;
// }
//
// /** Clear the residue 'resnum'. Note that this doesn't delete the residue,
//     it just removes all atoms and bonds in the residue, and all inter-residue
//     bonds to the residue. This does nothing if there is no such residue
//     in this molecule. */
// void EditMolDataPvt::clearResidue(ResNum resnum)
// {
//     molbnds.removeAll(resnum);
//
//     if (atms.contains(resnum))
//         atms[resnum].clear();
// }
//
// /** Return whether or not this molecule is empty (has no atoms or residues)
//     (note that a molecule with empty residues is not itself empty) */
// bool EditMolDataPvt::isEmpty() const
// {
//     return atms.isEmpty();
// }
//
// /** Return whether or not the residue with residue number 'resnum' is empty.
//     This returns true if there is no such residue with this name, or if there
//     are no atoms in this residue. */
// bool EditMolDataPvt::isEmptyResidue(ResNum resnum) const
// {
//     return (not atms.contains(resnum)) or atms[resnum].isEmpty();
// }
//
// /** Return the total number of residues in this molecule - this includes
//     empty residues */
// int EditMolDataPvt::nResidues() const
// {
//     return resnmes.count();
// }
//
// /** Return the total number of atoms in the molecule */
// int EditMolDataPvt::nAtoms() const
// {
//     QMapIterator<ResNum, EditResAtomSet> it(atms);
//
//     int nats = 0;
//
//     while (it.hasNext())
//     {
//         it.next();
//         nats += it.value().count();
//     }
//
//     return nats;
// }
//
// /** Return the number of atoms in the residue with number 'resnum'. This will
//     throw an exception if there is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
// */
// inline int EditMolDataPvt::nAtoms(ResNum resnum) const
// {
//     checkResidue(resnum);
//     return atms[resnum].count();
// }
//
// /** Return the 'ith' atom in the residue 'resnum'. This will throw an exception
//     if there is no such residue with this number, or if i < 0 or i >= natoms(resnum)
//
//     \throw SireMol::missing_residue
//     \throw SireMol::invalid_index
// */
// const Atom& EditMolDataPvt::at(ResNum resnum, int i) const
// {
//     if (i < 0 or i >= nAtoms(resnum))
//         throw SireError::invalid_index(QObject::tr(
//             "Invalid index (%1) for EditRes[i] (%2:%3 in molecule \"%4\") - natoms() == %5")
//                 .arg(i).arg(residueName(resnum),resnum.toString(),name())
//                     .arg(nAtoms(resnum)), CODELOC);
//
//     return atms[resnum].at(i);
// }
//
// /** Return the names of all of the atoms in the residue, in an arbitrary order.
//     This will throw an exception if there is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
// */
// inline QStringList EditMolDataPvt::atomNames(ResNum resnum) const
// {
//     checkResidue(resnum);
//     return atms[resnum].keys();
// }
//
// /** Return the geometric line representing the bond 'bnd'. This will throw
//     an exception if the bond refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Line EditMolDataPvt::bond(const Bond &bnd) const
// {
//     return SireMaths::Line( atom(bnd.atom0()), atom(bnd.atom1()) );
// }
//
// /** Return the geometric triangle representing the angle 'ang'. This will throw
//     an exception if the angle refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Triangle EditMolDataPvt::angle(const SireMol::Angle &ang) const
// {
//     return SireMaths::Triangle( atom(ang.atom0()), atom(ang.atom1()), atom(ang.atom2()) );
// }
//
// /** Return the geometric torsion representing the dihedral 'dih'. This will throw
//     an exception if the dihedral refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion EditMolDataPvt::dihedral(const Dihedral &dih) const
// {
//     return SireMaths::Torsion( atom(dih.atom0()), atom(dih.atom1()),
//                                atom(dih.atom2()), atom(dih.atom3()) );
// }
//
// /** Return the geometric torsion representing the improper angle 'improper'. This will throw
//     an exception if the improper angle refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion EditMolDataPvt::improper(const Improper &improper) const
// {
//     return SireMaths::Torsion( atom(improper.atom0()), atom(improper.atom1()),
//                                atom(improper.atom2()), atom(improper.atom3()) );
// }
//
// /** Return the relative weights of group0 and group1 of this molecule. An exception
//     will be thrown if the groups contains any atoms that are not in this molecule.
//
//     The returned weight will lie between 0 and 1. If the value is 0, then all
//     of the weight is on group0. If the weight is 1, then all of the weight is
//     on group1. If the weight is 0.5, then the weight is shared evenly between
//     group0 and group1. If the weight is 0.25, then group0 has 75% of the weight
//     while group1 has 25%. If the weight if 0.75 then group0 has 25% of the
//     weight while group1 has 75%. (so in essence, it is the percentage of the
//     weight that group1 has)
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
// */
// double EditMolDataPvt::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
//                                  const WeightFunction &weightfunc) const
// {
//     //create a calculator workspace for the weight calculation
//     WeightCalculator calc(weightfunc);
//
//     //add all of atoms in the groups to the calculator
//     foreach( AtomIndex atm, group0.atoms() )
//     {
//         calc.addToA( atom(atm) );
//     }
//
//     foreach( AtomIndex atm, group1.atoms() )
//     {
//         calc.addToB( atom(atm) );
//     }
//
//     //now add all of the residues
//     foreach( ResNum resnum, group0.residues() )
//     {
//         calc.addToA( atomVector(resnum) );
//     }
//
//     foreach( ResNum resnum, group1.residues() )
//     {
//         calc.addToB( atomVector(resnum) );
//     }
//
//     return calc.weight();
// }
//
// /** Translate the atoms in group0 and group1 along the vector of the bond 'bnd', ensuring
//     weighting the translation of the two sides by 'weight' and ensuring that the
//     atoms in 'anchors' are not moved.
//
//     This will throw an exception if either the atoms in 'bnd' or any of the atoms in
//     group0 or group1 are not in this molecule.
//
//     An exception will also be thrown if there are anchor atoms in both group0 and group1.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
//     \throw SireMol::anchor_error
// */
// void EditMolDataPvt::change(const Bond &bnd, double delta,
//                             const AtomIDGroup &group0, const AtomIDGroup &group1,
//                             const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     //get the anchor status
//     bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
//     bool anchor1 = group1.isEmpty() or group1.intersects(anchors);
//
//     double weight;
//
//     if (anchor0 and anchor1)
//         //this bond is immovable
//         throw SireMol::anchor_error(QObject::tr(
//             "Cannot change bond \"%1\" as both groups are either anchored or empty.\n%2\n%3")
//                 .arg(bnd.toString(),group0.inventory(),group1.inventory()), CODELOC);
//     else if (anchor0)
//         weight = 0.0;
//     else if (anchor1)
//         weight = 1.0;
//     else
//         weight = getWeight(group0, group1, weightfunc);
//
//     //get the vector of the bond...
//     SireMaths::Line line = this->bond(bnd);
//     Vector bondvec = line.vector();
//
//     if (bondvec.isZero())
//         //if we have a zero bondvector, then translate along the x axis
//         bondvec = Vector(1.0,0.0,0.0);
//     else
//         bondvec = bondvec.normalise();
//
//     //translate group0 by (weight-1) * delta * bondvec
//     this->translate(group0, (weight-1.0)*delta*bondvec);
//
//     //translate group1 by weight * delta * bondvec
//     this->translate(group1, weight*delta*bondvec);
// }
//
// /** Rotate the atoms in group0 and group1 around the angle 'ang', ensuring
//     weighting the rotation of the two sides by 'weight' and ensuring that the
//     atoms in 'anchors' are not moved.
//
//     This will throw an exception if any of the atoms in 'ang' or any of the atoms in
//     group0 or group1 are not in this molecule.
//
//     An exception will also be thrown if there are anchor atoms in both group0 and group1.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
//     \throw SireMol::anchor_error
// */
// void EditMolDataPvt::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                             const AtomIDGroup &group0, const AtomIDGroup &group1,
//                             const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     //get the anchor status - this will override the passed weight
//     bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
//     bool anchor1 = group1.isEmpty() or group1.intersects(anchors);
//
//     double weight;
//
//     if (anchor0 and anchor1)
//         //this bond is immovable
//         throw SireMol::anchor_error(QObject::tr(
//             "Cannot change angle \"%1\" as both groups are either anchored or empty.\n%2\n%3")
//                 .arg(ang.toString(),group0.inventory(),group1.inventory()), CODELOC);
//     else if (anchor0)
//         weight = 0.0;
//     else if (anchor1)
//         weight = 1.0;
//     else
//         weight = getWeight(group0, group1, weightfunc);
//
//     //get the geometry of the angle...
//     SireMaths::Triangle trig = angle(ang);
//     Vector angvec = trig.vector();
//
//     if (angvec.isZero())
//         //if we have a zero angle vector, then rotate about the z axis
//         angvec = Vector(0.0,0.0,1.0);
//     else
//         angvec = angvec.normalise();
//
//     //rotate group0 by (weight-1)*delta around angvec, about the central atom
//     //of the angle
//     this->rotate( group0, Quaternion((weight-1.0)*delta,angvec), trig[1] );
//
//     //rotate group1 by weight*delta around angvec, about the central atom of the angle
//     this->rotate( group1, Quaternion(weight*delta,angvec), trig[1] );
// }
//
// /** Rotate the atoms in group0 and group1 around the bond 'dih', ensuring
//     weighting the rotation of the two sides by 'weight' and ensuring that the
//     atoms in 'anchors' are not moved.
//
//     This will throw an exception if any of the atoms in 'dih' or any of the atoms in
//     group0 or group1 are not in this molecule.
//
//     An exception will also be thrown if there are anchor atoms in both group0 and group1.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
//     \throw SireMol::anchor_error
// */
// void EditMolDataPvt::change(const Bond &dih, const SireMaths::Angle &delta,
//                             const AtomIDGroup &group0, const AtomIDGroup &group1,
//                             const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     //get the anchor status - this will override the passed weight
//     bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
//     bool anchor1 = group1.isEmpty() or group1.intersects(anchors);
//
//     double weight;
//
//     if (anchor0 and anchor1)
//         //this bond is immovable
//         throw SireMol::anchor_error(QObject::tr(
//             "Cannot change dihedral centered about \"%1\" as both groups are "
//             "either anchored or empty.\n%2\n%3")
//                 .arg(dih.toString(),group0.inventory(),group1.inventory()), CODELOC);
//     else if (anchor0)
//         weight = 0.0;
//     else if (anchor1)
//         weight = 1.0;
//     else
//         weight = getWeight(group0, group1, weightfunc);
//
//     //get the geometry of the dihedral...
//     SireMaths::Line line = bond(dih);
//     Vector dihvec = line.vector();
//
//     if (dihvec.isZero())
//         //if we have a zero dihedral vector, then rotate about the x axis
//         dihvec = Vector(1.0,0.0,0.0);
//     else
//         dihvec = dihvec.normalise();
//
//     //rotate group0 by (weight-1)*delta around dihvec, about the first atom of the bond
//     this->rotate( group0, Quaternion((weight-1.0)*delta,dihvec), line[0] );
//
//     //rotate group1 by weight*delta around dihvec, about the second atom of the bond
//     this->rotate( group1, Quaternion(weight*delta,dihvec), line[1] );
// }
//
// /** Rotate the atoms in group0 and group1 around the improper angle 'improper', ensuring
//     weighting the rotation of the two sides by 'weight' and ensuring that the
//     atoms in 'anchors' are not moved.
//
//     This will throw an exception if any of the atoms in 'improper' or any of the atoms in
//     group0 or group1 are not in this molecule.
//
//     An exception will also be thrown if there are anchor atoms in both group0 and group1.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
//     \throw SireMol::anchor_error
// */
// void EditMolDataPvt::change(const Improper &improper, const SireMaths::Angle &delta,
//                             const AtomIDGroup &group0, const AtomIDGroup &group1,
//                             const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     throw SireError::incomplete_code(QObject::tr(
//             "Need to write code to change improper angles!"), CODELOC);
// }
//
// ///////////////
// /////////////// Implementation of EditMolData
// ///////////////
//
// /** Serialise an EditMolData to a datastream */
// QDataStream& operator<<(QDataStream &ds, const EditMolData &mol)
// {
//     ds << mol.d();
//     return ds;
// }
//
// /** Deserialise an EditMolData from a datastream - create a new
//     copy to deserialise into... */
// QDataStream& operator>>(QDataStream &ds, EditMolData &mol)
// {
//     mol.detach();
//     ds >> mol.d();
//     return ds;
// }
//
// /** Construct an empty molecule called 'molname' */
// EditMolData::EditMolData(const QString &molname)
//             : ptr( new QSharedDataPointer<EditMolDataPvt>(new EditMolDataPvt(molname)) )
// {}
//
// /** Construct an EditMol that represents the same molecule as 'mol' */
// EditMolData::EditMolData(const Molecule &mol)
//             : ptr( new QSharedDataPointer<EditMolDataPvt>(new EditMolDataPvt(mol)) )
// {}
//
// /** Copy constructor - This makes a shallow copy of other. Beware, as this may not be
//     what you expect. However, it is a consequence of this class being explicitly
//     shared. If you wish to get your own copy then ensure that you 'detach()' after
//     making this copy. */
// EditMolData::EditMolData(const EditMolData &other)
//             : ptr( other.ptr )
// {}
//
// /** Destructor */
// EditMolData::~EditMolData()
// {}
//
// /** Make a shallow copy of 'other' - see shallowCopy() */
// EditMolData& EditMolData::operator=(const EditMolData &other)
// {
//     ptr = other.ptr;
//     return *this;
// }
//
// /** Private function used to easily retrieve a const reference to the
//     underlying EditMolDataPvt object */
// const EditMolDataPvt& EditMolData::d() const
// {
//     return *(*ptr);
// }
//
// /** Private function used to easily retrieve a reference to the
//     underlying EditMolDataPvt object */
// EditMolDataPvt& EditMolData::d()
// {
//     return *(*ptr);
// }
//
// /** Comparison operator */
// bool EditMolData::operator==(const EditMolData &other) const
// {
//     //two editmols are equal if they have the same atoms,
//     //residues and bonds...
//     return d() == other.d();
// }
//
// /** Comparison operator */
// bool EditMolData::operator!=(const EditMolData &other) const
// {
//     return d() != other.d();
// }
//
// /** Detach this copy of the EditMolData from shared storage. You are guarnteed
//     to be the only holder of the data after this call. */
// void EditMolData::detach()
// {
//     ptr.reset( new QSharedDataPointer<EditMolDataPvt>( *ptr ) );
// }
//
// /** Return a deep copy of this EditMolData. The deep copy will have its own
//     copy of the data, and changing it will not change the original. */
// EditMolData EditMolData::deepCopy() const
// {
//     EditMolData mol(*this);
//     mol.detach();
//     return mol;
// }
//
// /** Return a shallow copy of this EditMolData. The shallow copy will share
//     data with the original, and changing the copy will change the original. */
// EditMolData EditMolData::shallowCopy()
// {
//     return EditMolData(*this);
// }
//
// /** Return an EditMol view of this data */
// EditMol EditMolData::editmol()
// {
//     return EditMol(*this);
// }
//
// /** Return an EditRes view of residue 'resnum' in this molecule. This will throw
//     an exception if there is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
// */
// EditRes EditMolData::editres(ResNum resnum)
// {
//     //ensure that there is a residue with this number
//     d().checkResidue(resnum);
//     return EditRes(EditMolData(*this), resnum);
// }
//
// /** Return an EditRes view of the first residue called 'resnam'. This will throw
//     an exception if there is no such residue in this molecule.
//
//     \throw SireMol::missing_residue
// */
// EditRes EditMolData::editres(const QString &resnam)
// {
//     QList<ResNum> resnums = residueNumbers(resnam);
//
//     if (resnums.count() == 0)
//         throw SireMol::missing_residue(QObject::tr(
//             "There is no residue called \"%1\" in molecule \"%2\"")
//                 .arg(resnam).arg(name()), CODELOC);
//
//     return editres(resnums[0]);
// }
//
// /** Return a list of EditRes that are bonded to residue 'rnum'. This throws an exception
//     if there is no such residue. This returns a list of residues that are on a shallow
//     copy of the molecule.
//
//     \throw SireMol::missing_residue
// */
// EditResSet EditMolData::residuesBondedTo(ResNum resnum)
// {
//     QList<ResNum> resnums = connectivity(resnum).residuesBondedTo();
//
//     EditResSet residus;
//     foreach( ResNum rnum, resnums )
//         residus.insert( editres(rnum) );
//
//     return residus;
// }
//
// /** Extract the residue with residue number 'resnum' - this returns an editres
//     that is a deep copy of the residue in this molecule, but the returned
//     editres is alone in the deep copy */
// EditRes EditMolData::extract(ResNum resnum) const
// {
//     EditMolData newres = deepCopy();
//
//     QList<ResNum> resnums = newres.residueNumbers();
//
//     foreach( ResNum rnum, resnums )
//     {
//         if (rnum != resnum)
//             newres.removeResidue(resnum);
//     }
//
//     return EditRes(newres, resnum);
// }
//
// /** Renumber the residue 'resnum' to 'newnum'.
//
//     An exception will be thrown if either there isn't a residue with number 'resnum',
//     or if a residue with number 'newnum' already exists.
//
//     \throw SireMol::missing_residue
//     \throw SireMol::duplicate_residue
// */
// void EditMolData::renumberResidue(ResNum resnum, ResNum newnum)
// {
//     d().renumberResidue(resnum,newnum);
// }
//
// /** Return the 'ith' residue in the molecule as an EditRes. This will
//     return a shallow copy EditRes (changing it will change this molecule).
//     An exception will be thrown if i<0 or i>=nresidues
//
//     \throw SireError::invalid_index
// */
// EditRes EditMolData::at(int i)
// {
//     QList<ResNum> resnums = d().residueNumbers();
//
//     if (i < 0 or i >= resnums.count())
//         throw SireError::invalid_index(QObject::tr(
//             "Index error accessing i=%1 for molecule \"%2\" (nresidues == %3)")
//                 .arg(i).arg(name()).arg(resnums.count()), CODELOC);
//
//     return editres(resnums.at(i));
// }
//
// void EditMolData::setName(const QString &name)
// {
//     d().setName(name);
// }
//
// void EditMolData::setResidueName(ResNum resnum, const QString &name)
// {
//     d().setResidueName(resnum,name);
// }
//
// void EditMolData::clear()
// {
//     d().clear();
// }
//
// void EditMolData::clean()
// {
//     d().clean();
// }
//
// void EditMolData::clearResidue(ResNum resnum)
// {
//     d().clearResidue(resnum);
// }
//
// void EditMolData::finalise()
// {
//     d().finalise();
// }
//
// void EditMolData::add(const Atom &atom)
// {
//     d().add(atom);
// }
//
// EditRes EditMolData::addResidue(ResNum resnum, const QString &resnam)
// {
//     d().addResidue(resnum,resnam);
//     return editres(resnum);
// }
//
// EditRes EditMolData::addResidue(ResNum resnum, const EditRes &tmpl)
// {
//     //add the residue
//     EditRes newres = addResidue(resnum, tmpl.name());
//
//     //now add the atoms and bonds of the template to the new residue
//     newres.add(tmpl);
//
//     return newres;
// }
//
// EditRes EditMolData::addResidue(ResNum resnum, const QString &resnam, const EditRes &tmpl)
// {
//     //add the residue
//     EditRes newres = addResidue(resnum, resnam);
//
//     //now add the atoms and bonds of the template to the new residue
//     newres.add(tmpl);
//
//     return newres;
// }
//
// void EditMolData::remove(const AtomIndex &atom)
// {
//     d().remove(atom);
// }
//
// void EditMolData::removeResidue(ResNum resnum)
// {
//     d().removeResidue(resnum);
// }
//
// void EditMolData::removeResidue(const QString &resnam)
// {
//     d().removeResidue(resnam);
// }
//
// void EditMolData::removeAllResidues(const QString &resnam)
// {
//     d().removeAllResidues(resnam);
// }
//
// void EditMolData::add(const Bond &bond)
// {
//     d().add(bond);
// }
//
// void EditMolData::remove(const Bond &bond)
// {
//     d().remove(bond);
// }
//
// void EditMolData::removeAllBonds(ResNum resnum)
// {
//     d().removeAllBonds(resnum);
// }
//
// void EditMolData::removeAllBonds(const AtomIndex &atom)
// {
//     d().removeAllBonds(atom);
// }
//
// void EditMolData::removeAllBonds()
// {
//     d().removeAllBonds();
// }
//
// void EditMolData::update(const Atom &atom)
// {
//     d().update(atom);
// }
//
// void EditMolData::update(const AtomIndex &atm, const Atom &details)
// {
//     d().update(atm,details);
// }
//
// void EditMolData::update(const AtomIndex &atm, const Element &element)
// {
//     d().update(atm,element);
// }
//
// void EditMolData::update(const AtomIndex &atm, const Vector &coords)
// {
//     d().update(atm,coords);
// }
//
// MoleculeBonds EditMolData::connectivity() const
// {
//     return d().connectivity();
// }
//
// ResidueBonds EditMolData::connectivity(ResNum resnum) const
// {
//     return d().connectivity(resnum);
// }
//
// MoleculeSignature EditMolData::signature() const
// {
//     return d().signature();
// }
//
// AtomSet EditMolData::atoms(ResNum resnum) const
// {
//     return d().atoms(resnum);
// }
//
// AtomVector EditMolData::atomVector(ResNum resnum) const
// {
//     return d().atomVector(resnum);
// }
//
// const QString& EditMolData::name() const
// {
//     return d().name();
// }
//
// bool EditMolData::isEmpty() const
// {
//     return d().isEmpty();
// }
//
// bool EditMolData::isEmptyResidue(ResNum resnum) const
// {
//     return d().isEmptyResidue(resnum);
// }
//
// QString EditMolData::residueName(ResNum resnum) const
// {
//     return d().residueName(resnum);
// }
//
// QList<ResNum> EditMolData::residueNumbers() const
// {
//     return d().residueNumbers();
// }
//
// QList<ResNum> EditMolData::residueNumbers(const QString &resnam) const
// {
//     return d().residueNumbers(resnam);
// }
//
// QStringList EditMolData::residueNames() const
// {
//     return d().residueNames();
// }
//
// ResidueIDSet EditMolData::residueIDs() const
// {
//     return d().residueIDs();
// }
//
// const Atom& EditMolData::atom(const AtomIndex &atm) const
// {
//     return d().atom(atm);
// }
//
// bool EditMolData::contains(ResNum resnum) const
// {
//     return d().contains(resnum);
// }
//
// bool EditMolData::contains(const AtomIndex &atm) const
// {
//     return d().contains(atm);
// }
//
// bool EditMolData::contains(const Bond &bond) const
// {
//     return d().contains(bond);
// }
//
// const Atom& EditMolData::at(ResNum resnum, int i) const
// {
//     return d().at(resnum,i);
// }
//
// int EditMolData::nResidues() const
// {
//     return d().nResidues();
// }
//
// int EditMolData::nAtoms() const
// {
//     return d().nAtoms();
// }
//
// int EditMolData::nAtoms(ResNum resnum) const
// {
//     return d().nAtoms(resnum);
// }
//
// QStringList EditMolData::atomNames(ResNum resnum) const
// {
//     return d().atomNames(resnum);
// }
//
// AtomSet EditMolData::atoms() const
// {
//     return d().atoms();
// }
//
// AtomVector EditMolData::atomVector() const
// {
//     return d().atomVector();
// }
//
// void EditMolData::translate(const AtomIndex &atom, const Vector &delta)
// {
//     d().translate(atom, delta);
// }
//
// void EditMolData::rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point)
// {
//     d().rotate(atom, quat, point);
// }
//
// void EditMolData::translate(const AtomIDGroup &group, const Vector &delta)
// {
//     d().translate(group, delta);
// }
//
// void EditMolData::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
// {
//     d().rotate(group, quat, point);
// }
//
// void EditMolData::translate(ResNum resnum, const Vector &delta)
// {
//     d().translate(resnum, delta);
// }
//
// void EditMolData::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
// {
//     d().rotate(resnum, quat, point);
// }
//
// void EditMolData::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
// {
//     d().translate(resnum,atoms,delta);
// }
//
// void EditMolData::rotate(ResNum resnum, const QStringList &atoms,
//                          const Quaternion &quat, const Vector &point)
// {
//     d().rotate(resnum,atoms,quat,point);
// }
//
// void EditMolData::translate(const Vector &delta)
// {
//     d().translate(delta);
// }
//
// void EditMolData::rotate(const Quaternion &quat, const Vector &point)
// {
//     d().rotate(quat,point);
// }
//
// double EditMolData::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
//                             const WeightFunction &weightfunc) const
// {
//     return d().getWeight(group0,group1,weightfunc);
// }
//
// /** Return the geometric line representing the bond 'bnd'. This will throw
//     an exception if the bond refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Line EditMolData::bond(const Bond &bnd) const
// {
//     return d().bond(bnd);
// }
//
// /** Return the geometric triangle representing the angle 'ang'. This will throw
//     an exception if the angle refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Triangle EditMolData::angle(const SireMol::Angle &ang) const
// {
//     return d().angle(ang);
// }
//
// /** Return the geometric torsion representing the dihedral 'dih'. This will throw
//     an exception if the dihedral refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion EditMolData::dihedral(const Dihedral &dih) const
// {
//     return d().dihedral(dih);
// }
//
// /** Return the geometric torsion representing the improper angle 'improper'. This will throw
//     an exception if the improper angle refers to atoms or residues that are not in this
//     molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion EditMolData::improper(const Improper &improper) const
// {
//     return d().improper(improper);
// }
//
// /** Return the length of the bond 'bnd'. This will throw an exception if this
//     bond refers to atoms that are not in this molecule.
//
//     \throw SireMol::missing_atom
// */
// double EditMolData::measure(const Bond &bnd) const
// {
//     return bond(bnd).length();
// }
//
// /** Return the size of the angle 'ang'. This will throw an exception if this
//     angle refers to atoms that are not in this molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle EditMolData::measure(const SireMol::Angle &ang) const
// {
//     return angle(ang).angle1();
// }
//
// /** Return the size of the dihedral 'dih'. This will throw an exception if this
//     dihedral refers to atoms that are not in this molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle EditMolData::measure(const Dihedral &dih) const
// {
//     return dihedral(dih).angle();
// }
//
// /** Return the size of the improper angle 'ang'. This will throw an exception if this
//     improper angle refers to atoms that are not in this molecule.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle EditMolData::measure(const Improper &improper) const
// {
//     throw SireError::incomplete_code(QObject::tr(
//         "Need to write code to measure an improper angle..."), CODELOC);
//
//     return 0.0;
// }
//
// void EditMolData::change(const Bond &bnd, double delta,
//                          const AtomIDGroup &group0, const AtomIDGroup &group1,
//                          const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     d().change(bnd, delta, group0, group1, weightfunc, anchors);
// }
//
// void EditMolData::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                          const AtomIDGroup &group0, const AtomIDGroup &group1,
//                          const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     d().change(ang, delta, group0, group1, weightfunc, anchors);
// }
//
// void EditMolData::change(const Bond &bnd, const SireMaths::Angle &delta,
//                          const AtomIDGroup &group0, const AtomIDGroup &group1,
//                          const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     d().change(bnd, delta, group0, group1, weightfunc, anchors);
// }
//
// void EditMolData::change(const Improper &improper, const SireMaths::Angle &delta,
//                          const AtomIDGroup &group0, const AtomIDGroup &group1,
//                          const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
// {
//     d().change(improper, delta, group0, group1, weightfunc, anchors);
// }

#include "SireMol/editmoldata.h"

using namespace SireMol;

EditMolData::EditMolData(const QString&)
{}

EditMolData::~EditMolData()
{}

