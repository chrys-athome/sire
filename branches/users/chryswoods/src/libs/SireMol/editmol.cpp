//
// #include "qhash_siremol.h"
//
// #include "editmol.h"
// #include "editres.h"
// #include "atomidgroup.h"
// #include "bond.h"
// #include "angle.h"
// #include "dihedral.h"
// #include "improper.h"
// #include "aabox.h"
// #include "moleculebonds.h"
// #include "residuebonds.h"
// #include "templatefunction.h"
// #include "weightfunction.h"
// #include "splitmolecule.h"
// #include "moleculesignature.h"
//
// #include "SireMaths/line.h"
// #include "SireMaths/triangle.h"
// #include "SireMaths/torsion.h"
// #include "SireMaths/angle.h"
// #include "SireMaths/vector.h"
// #include "SireMaths/quaternion.h"
//
// #include "SireStream/datastream.h"
//
// using namespace SireStream;
// using namespace SireMol;
//
// /** Function to hash an EditMol - an EditMol is hashed according to its name */
// uint SIREMOL_EXPORT qHash(const EditMol &editmol)
// {
//     return qHash(editmol.name());
// }
//
// static const RegisterMetaType<EditMol> r_editmol("SireMol::EditMol");
//
// /** Serialise an EditMol to a binary data stream */
// QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const EditMol &editmol)
// {
//     writeHeader(ds, r_editmol, 1) << editmol.moldata;
//
//     return ds;
// }
//
// /** Deserialise an EditMol from a binary datastream. Note that this
//     will detach 'editmol' - i.e. it will not overwrite any other
//     EditMol or EditRes views of 'editmol'. */
// QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, EditMol &editmol)
// {
//     VersionID v = readHeader(ds, r_editmol);
//
//     if (v == 1)
//     {
//         //detach and the editmol (we don't want to mess up any
//         //connected EditRes or EditMol objects...
//         editmol.detach();
//
//         ds >> editmol.moldata;
//     }
//     else
//         throw version_error(v, "1", r_editmol, CODELOC);
//
//     return ds;
// }
//
// /** Create an empty EditMol with name 'name' */
// EditMol::EditMol(const QString &name) : moldata(name)
// {}
//
// /** Copy constructor. Create an EditMol that is a shallow copy of 'other'.
//     Changing this EditMol will change 'other'. Use 'detach()' to make this
//     your own private copy of this EditMol. */
// EditMol::EditMol(const EditMol &other) : moldata(other.moldata)
// {}
//
// /** Create an EditMol from a Molecule */
// EditMol::EditMol(const Molecule &mol) : moldata(mol)
// {}
//
// /** Create an EditMol view on the passed EditMolData */
// EditMol::EditMol(const EditMolData &data) : moldata(data)
// {}
//
// /** Destructor */
// EditMol::~EditMol()
// {}
//
// /** Assignment operator - this creates a shallow copy of 'other'. This
//     may not be as you expect, but is a consequence of the explicit sharing
//     of this class. */
// EditMol& EditMol::operator=(const EditMol &other)
// {
//     moldata = other.moldata;
//     return *this;
// }
//
// /** Comparison operator */
// bool EditMol::operator==(const EditMol &other) const
// {
//     return moldata == other.moldata;
// }
//
// /** Comparison operator */
// bool EditMol::operator!=(const EditMol &other) const
// {
//     return moldata != other.moldata;
// }
//
// /** Return a deep copy of this EditMol */
// EditMol EditMol::deepCopy() const
// {
//     return moldata.deepCopy().editmol();
// }
//
// /** Return a shallow copy of this EditMol */
// EditMol EditMol::shallowCopy()
// {
//     return moldata.editmol();
// }
//
// /** Detach this EditMol from shared storage - this will ensure that this
//     EditMol will not be changed by any other existing views of the molecule,
//     and that changes to this EditMol will not change any other existing
//     views of the molecule. */
// void EditMol::detach()
// {
//     moldata = moldata.deepCopy();
// }
//
// /** Clear this molecule - this removes all residues, atoms and bonds.
//     Note that this will clear residues even if there are active EditRes
//     views of these residues. */
// void EditMol::clear()
// {
//     moldata.clear();
// }
//
// /** Return the residue with residue number 'resnum'. If a residue
//     with this number does not exist then an exception is thrown.
//
//     Note that the returned EditRes is a view of a shallow copy of this
//     molecule. Changing the EditRes *will* change this EditMol.
//
//     Use the 'detach()' function on this EditRes to make a deep
//     copy, (or call the deepCopy() function)
//
//     \throw SireMol::missing_residue
// */
// EditRes EditMol::residue(ResNum resnum)
// {
//     return moldata.editres(resnum);
// }
//
// /** Return the first residue in this molecule with the name 'resname'.
//
//     This will throw an exception if there is no such residue.
//
//     The returned EditRes is on a shallow copy of this EditMol.
//
//     \throw SireMol::missing_residue
// */
// EditRes EditMol::residue(const QString &resname)
// {
//     return moldata.editres(resname);
// }
//
// /** Return the number of residues in the EditMol */
// int EditMol::count() const
// {
//     return moldata.nResidues();
// }
//
// /** Return the number of residues in the EditMol */
// int EditMol::size() const
// {
//     return moldata.nResidues();
// }
//
// /** Return the number of residues in the EditMol */
// int EditMol::nResidues() const
// {
//     return moldata.nResidues();
// }
//
// /** Return the i'th residue in the EditMol. This returns
//     a shallow copy EditRes, and will throw an exception if an
//     invalid index is requested.
//
//     \throw SireError::invalid_index
// */
// EditRes EditMol::at(int i)
// {
//     return moldata.at(i);
// }
//
// /** Return the i'th residue in the EditMol - this returns an EditRes that
//     is on a shallow copy of this EditMol - changing the returned EditRes
//     will change this EditMol. */
// EditRes EditMol::operator[](int i)
// {
//     return at(i);
// }
//
// /** Add a residue to the molecule - throws an exception if this residue
//     already exists. All of the 'addResidue' functions throw an exception
//     if a residue with this number already exists.
//
//     \throw SireMol::duplicate_residue
// */
// EditRes EditMol::addResidue(ResNum resnum, const QString &resnam)
// {
//     return moldata.addResidue(resnum,resnam);
// }
//
// /** Add a residue called resnam at residue number 'resnum', using
//     template 'tmpl'. This will throw an exception if there is already
//     a residue with this number in this molecule.
//
//     \throw SireMol::duplicate_residue
// */
// EditRes EditMol::addResidue(ResNum resnum, const QString &resnam, const EditRes &tmpl)
// {
//     return moldata.addResidue(resnum,resnam,tmpl);
// }
//
// /** Add a residue with a null name
//
//     \throw SireMol::duplicate_residue
// */
// EditRes EditMol::addResidue(ResNum resnum)
// {
//     return moldata.addResidue(resnum, QObject::tr("UNK"));
// }
//
// /** Add a residue numbered 'resnum' that is a copy of the residue 'res'
//
//     \throw SireMol::duplicate_residue
// */
// EditRes EditMol::addResidue(ResNum resnum, const EditRes &res)
// {
//     return moldata.addResidue(resnum, res);
// }
//
// /** Return a new residue number that is not currently in use in this residue.
//     This will return largest_resnum + 1 (or 1 if this is an empty molecule) */
// ResNum EditMol::getNewResidueNumber() const
// {
//     ResNumList resnums = residueNumbers();
//
//     if (resnums.isEmpty())
//         return 1;
//     else
//         return resnums.last() + 1;
// }
//
// /** Add a new residue called 'resname'. The residue number will be automatically
//     obtained as largest_resnum + 1 */
// EditRes EditMol::addResidue(const QString &resnam)
// {
//     return addResidue(getNewResidueNumber(), resnam);
// }
//
// /** Add a new residue that is a copy of the template 'tmpl'. The residue number
//     will be one larger than the current largest residue number. */
// EditRes EditMol::addResidue(const EditRes &tmpl)
// {
//     return addResidue(getNewResidueNumber(), tmpl);
// }
//
// /** Add a new residue called resnam based on the template 'tmpl'. The residue
//     number will be one larger than the current largest residue number */
// EditRes EditMol::addResidue(const QString &resnam, const EditRes &tmpl)
// {
//     return addResidue(getNewResidueNumber(), resnam, tmpl);
// }
//
// /** Remove the residue with residue number 'resnum'. Does nothing if
//     there is no residue with this number */
// void EditMol::removeResidue(ResNum resnum)
// {
//     moldata.removeResidue(resnum);
// }
//
// /** Remove the first residue with residue name 'resnam'. This does nothing if there
//     is no residue with this name in the molecule. */
// void EditMol::removeResidue(const QString &resnam)
// {
//     moldata.removeResidue(resnam);
// }
//
// /** Remove all residues called 'resnam' from this molecule. This does nothing if
//     there are no residues with that name */
// void EditMol::removeAllResidues(const QString &resnam)
// {
//     moldata.removeAllResidues(resnam);
// }
//
// /** Remove the atom 'atom' */
// void EditMol::remove(const AtomIndex &atm)
// {
//     moldata.remove(atm);
// }
//
// /** Remove a whole load of atoms... */
// void EditMol::remove(const AtomIndexList &atoms)
// {
//     foreach(AtomIndex atm, atoms)
//         remove(atm);
// }
//
// /** Remove a whole load of atoms... */
// void EditMol::remove(const AtomIndexSet &atoms)
// {
//     foreach(AtomIndex atm, atoms)
//         remove(atm);
// }
//
// /** Return the total number of atoms in this molecule */
// int EditMol::nAtoms() const
// {
//     return moldata.nAtoms();
// }
//
// /** Return whether or not the molecule is empty (has no residues) */
// bool EditMol::isEmpty() const
// {
//     return moldata.isEmpty();
// }
//
// /** Return a set of all atoms in this molecule. As this copies the atoms
//     this could be slow for large molecules */
// AtomSet EditMol::atoms() const
// {
//     return moldata.atoms();
// }
//
// /** Return the set of all atoms in the residue with number 'resnum'. This
//     will throw an exception if there is no residue with this number.
//
//     \throw SireMol::missing_residue
// */
// AtomSet EditMol::atoms(ResNum resnum) const
// {
//     return moldata.atoms(resnum);
// }
//
// /** Return a vector of all atoms in this molecule. As this copies the atoms
//     this could be slow for large molecules */
// AtomVector EditMol::atomVector() const
// {
//     return moldata.atomVector();
// }
//
// /** Return the vector of all atoms in the residue with number 'resnum'. This
//     will throw an exception if there is no residue with this number.
//
//     \throw SireMol::missing_residue
// */
// AtomVector EditMol::atomVector(ResNum resnum) const
// {
//     return moldata.atomVector(resnum);
// }
//
// /** Return whether there is an atom 'atm' in this EditMol */
// bool EditMol::contains(const AtomIndex &atm) const
// {
//     return moldata.contains(atm);
// }
//
// /** Return whether there is a residue 'resnum' in this EditMol */
// bool EditMol::contains(ResNum resnum) const
// {
//     return moldata.contains(resnum);
// }
//
// /** Return whether or not this molecule contains a residue called 'resname' */
// bool EditMol::contains(const QString &resname) const
// {
//     return moldata.contains(resname);
// }
//
// /** Return a list of residue numbers (in numerical order) */
// ResNumList EditMol::residueNumbers() const
// {
//     return moldata.residueNumbers();
// }
//
// /** Return the list of residue numbers of residues that are called 'resnam' */
// ResNumList EditMol::residueNumbers(const QString &resnam) const
// {
//     return moldata.residueNumbers(resnam);
// }
//
// /** Return the names of the residues (in the order that they were added) */
// QStringList EditMol::residueNames() const
// {
//     return moldata.residueNames();
// }
//
// /** Return the ResidueIDs of all of the residues in this molecule */
// ResidueIDSet EditMol::residueIDs() const
// {
//     return moldata.residueIDs();
// }
//
// /** Return the residue name of the residue with residue number 'resnum'.
//     This will throw an exception if there is no such residue.
//
//     \throw SireMol::missing_residue
// */
// QString EditMol::residueName(ResNum resnum) const
// {
//     return moldata.residueName(resnum);
// }
//
// /** Return the name of the EditMol */
// QString EditMol::name() const
// {
//     return moldata.name();
// }
//
// /** Set the name of this EditMol */
// void EditMol::setName(const QString &n)
// {
//     moldata.setName(n);
// }
//
// /** Return a string representation of the EditMol */
// QString EditMol::toString() const
// {
//     return QObject::tr("EditMol (%1) : nResidues() == %2, nAtoms() == %3")
//                       .arg(name()).arg(nResidues()).arg(nAtoms());
// }
//
// /** Return a signature for the EditMol. This can be used to compare
//     this EditMol with another (or with Molecule) to see if the
//     two objects describe the same molecules (i.e. contain the same
//     residues, atoms and bonds) */
// MoleculeSignature EditMol::signature() const
// {
//     return moldata.signature();
// }
//
// /** Return a reference to the atom with index 'idx'. This will throw
//     an exception if there is no such atom.
//
//     \throw SireMol::missing_atom
// */
// const Atom& EditMol::atom(const AtomIndex &atm) const
// {
//     return moldata.atom(atm);
// }
//
// /** Return the connectivity of the EditMol */
// MoleculeBonds EditMol::connectivity() const
// {
//     return moldata.connectivity();
// }
//
// /** Return the connectivity of residue 'resnum' in this molecule. This will
//     throw an exception if this residue isn't in this molecule.
//
//     \throw SireMol::missing_residue
// */
// ResidueBonds EditMol::connectivity(ResNum resnum) const
// {
//     return moldata.connectivity(resnum);
// }
//
// /** Return the geometric line between the atoms of the bond.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Line EditMol::bond(const Bond &bnd) const
// {
//     return moldata.bond(bnd);
// }
//
// /** Return the geometric triangle formed by the atoms of this angle
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Triangle EditMol::angle(const Angle &ang) const
// {
//     return moldata.angle(ang);
// }
//
// /** Return the geometric torsion formed by the atoms of this dihedral
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion EditMol::dihedral(const Dihedral &dih) const
// {
//     return moldata.dihedral(dih);
// }
//
// /** Return the geometric torsion formed by the atoms of the improper
//     angle.
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Torsion EditMol::improper(const Improper &imp) const
// {
//     return moldata.improper(imp);
// }
//
// /** Return the length of the bond in this molecule
//
//     \throw SireMol::missing_atom
// */
// double EditMol::measure(const Bond &bnd) const
// {
//     return moldata.measure(bnd);
// }
//
// /** Return the size of the angle in this molecule
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle EditMol::measure(const Angle &ang) const
// {
//     return moldata.measure(ang);
// }
//
// /** Return the size of the dihedral angle in this molecule
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle EditMol::measure(const Dihedral &dih) const
// {
//     return moldata.measure(dih);
// }
//
// /** Return the size of the improper angle in this molecule
//
//     \throw SireMol::missing_atom
// */
// SireMaths::Angle EditMol::measure(const Improper &imp) const
// {
//     return moldata.measure(imp);
// }
//
// /** Add the atom 'atom'. This will add the residue if it doesn't already exist.
//     An exception will be thrown if there is already an atom with this index
//     in the molecule.
//
//     \throw SireMol::duplicate_atom
// */
// void EditMol::add(const Atom &atom)
// {
//     moldata.add(atom);
// }
//
// /** Add a whole load of atoms to this molecule...
//
//     \throw SireMol::duplicate_atom
// */
// void EditMol::add(const AtomList &atoms)
// {
//     int nats = atoms.count();
//     for (int i=0; i<nats; ++i)
//         this->add(atoms.at(i));
// }
//
// /** Add a whole load of atoms to this molecule...
//
//     \throw SireMol::duplicate_atom
// */
// void EditMol::add(const AtomSet &atoms)
// {
//     int nats = atoms.count();
//     for (int i=0; i<nats; ++i)
//         this->add(atoms.at(i));
// }
//
// /** Add a whole load of atoms to this molecule...
//
//     \throw SireMol::duplicate_atom
// */
// void EditMol::add(const AtomVector &atoms)
// {
//     int nats = atoms.count();
//     for (int i=0; i<nats; ++i)
//         this->add(atoms.at(i));
// }
//
// /** Update the atom 'atom'. This will throw an exception if there isn't such
//     an atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// void EditMol::update(const Atom &atom)
// {
//     moldata.update(atom);
// }
//
// /** Update the atom 'atm'. This will throw an exception if there isn't such
//     an atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// void EditMol::update(const AtomIndex &atom, const Atom &details)
// {
//     moldata.update(atom,details);
// }
//
// /** Update the atom 'atm'. This will throw an exception if there isn't such
//     an atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// void EditMol::update(const AtomIndex &atom, const Element &element)
// {
//     moldata.update(atom,element);
// }
//
// /** Update the atom 'atm'. This will throw an exception if there isn't such
//     an atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// void EditMol::update(const AtomIndex &atom, const Vector &coords)
// {
//     moldata.update(atom,coords);
// }
//
// /** Add the bond 'bnd'. The atoms in this bond have to be in this molecule or
//     an exception will be thrown
//
//     \throw SireMol::missing_atom
// */
// void EditMol::add(const Bond &bond)
// {
//     moldata.add(bond);
// }
//
// /** Add a whole load of bonds to the molecule.
//
//     \throw SireMol::missing_atom
// */
// void EditMol::add(const BondList &bonds)
// {
//     int nbnds = bonds.count();
//     for (int i=0; i<nbnds; ++i)
//         this->add(bonds.at(i));
// }
//
// /** Use an algorithm to automatically add the bonds between all atoms of
//     this molecule */
// void EditMol::addAutoBonds()
// {
//     //first loop over every residue in this molecule and auto-add
//     //the bonds in each residue - at the same time also calculate
//     //the AABox that encases each residue
//
//     QList<AABox> boxlist;
//     int nres = moldata.nResidues();
//     for (int i=0; i<nres; i++)
//     {
//         EditRes res = moldata.at(i);
//
//         res.addAutoBonds();
//         boxlist.append( AABox(res.atoms()) );
//     }
//
//     //now loop over all of the residue pairs and see if they are close
//     //enough together to be bonded...
//     for (int i=0; i<nres-1; i++)
//     {
//         const AABox &res0 = boxlist.at(i);
//         for (int j=i+1; j<nres; j++)
//         {
//             const AABox &res1 = boxlist.at(j);
//             //residues need to be within 5.0 A to be considered to be bonded
//             if (res0.withinDistance(5.0,res1))
//             {
//                 this->addAutoBonds(moldata.at(i), moldata.at(j));
//             }
//         }
//     }
// }
//
// /** Auto-add all of the bonds between the atoms of residues res0 and
//     res1, which are residues in this molecule. If these are not residues
//     in this molecule then an exception will be thrown. */
// void EditMol::addAutoBonds(const EditRes &res0, const EditRes &res1)
// {
//     //loop over all pair of atoms between the residues and see if they
//     //are within the vdw contact distance... If so then add the bond
//     AtomSet atoms0 = res0.atoms();
//     AtomSet atoms1 = res1.atoms();
//
//     int nats0 = atoms0.count();
//     int nats1 = atoms1.count();
//
//     for (int i=0; i<nats0; i++)
//     {
//         const Atom &atom0 = atoms0.at(i);
//
//         //do not add bonds to dummy atoms
//         if (atom0.nProtons() > 0)
//         {
//             for (int j=0; j<nats1; j++)
//             {
//                 const Atom &atom1 = atoms1.at(j);
//
//                 if (atom1.nProtons() > 0 and Atom::withinBondRadii(atom0,atom1,0.1))
//                     moldata.add( Bond(atom0,atom1) );
//             }
//         }
//     }
// }
//
// /** Remove the bond between the two atoms */
// void EditMol::remove(const Bond &bond)
// {
//     moldata.remove(bond);
// }
//
// /** Remove a whole load of bonds... */
// void EditMol::remove(const BondList &bonds)
// {
//     int nbnds = bonds.count();
//     for (int i=0; i<nbnds; ++i)
//         this->remove(bonds.at(i));
// }
//
// /** Remove all bonds involving residue 'resnum'. Useful if you want to
//     delete this residue! */
// void EditMol::removeAllBonds(ResNum resnum)
// {
//     moldata.removeAllBonds(resnum);
// }
//
// /** Remove all bonds involving atom 'atm' */
// void EditMol::removeAllBonds(const AtomIndex &atm)
// {
//     moldata.removeAllBonds(atm);
// }
//
// /** Remove all bonds */
// void EditMol::removeAllBonds()
// {
//     moldata.removeAllBonds();
// }
//
// /** Return a list of all of the Bonds in this EditMol that contain 'atom' */
// BondList EditMol::bonds(const AtomIndex &atm) const
// {
//     return connectivity().bonds(atm);
// }
//
// /** Return a list of all of the Bonds in this EditMol that act in or between
//     residue 'resnum' */
// BondList EditMol::bonds(ResNum resnum) const
// {
//     return connectivity().residue(resnum).bonds();
// }
//
// /** Return the total number of bonds in this EditMol */
// int EditMol::nBonds() const
// {
//     return connectivity().nBonds();
// }
//
// /** Return the list of all of the bonds in this EditMol */
// BondList EditMol::bonds() const
// {
//     return connectivity().bonds();
// }
//
// /** Apply the residue tempate 'tmpl' to the residue in this molecule with
//     residue number 'resnum', using the template function 'tmplfunc'
//
//     \throw SireMol::template_error
// */
// void EditMol::applyTemplate(const EditRes &tmpl, ResNum resnum,
//                             const TemplateFunction &tmplfunc)
// {
//     residue(resnum).applyTemplate(tmpl, tmplfunc);
// }
//
// /** Overloaded function - apply the template to every residue in this molecule that
//     is called 'resname'. This is useful for applying protein residue templates,
//     for example.
//
//     \throw SireMol::template_error
// */
// void EditMol::applyTemplate(const EditRes &tmpl, const QString &resname,
//                             const TemplateFunction &tmplfunc)
// {
//     //apply this template to each residue called 'resname'
//     ResNumList resnums = moldata.residueNumbers();
//
//     foreach( ResNum resnum, resnums )
//     {
//         if (moldata.residueName(resnum) == resname)
//             residue(resnum).applyTemplate(tmpl, tmplfunc);
//     }
// }
//
// /** Apply the template held in 'tmpl' to this molecule using the template function
//     'tmplfunc'.
//
//     \throw SireMol::template_error
// */
// void EditMol::applyTemplate(const EditMol &tmpl, const TemplateFunction &tmplfunc)
// {
//     tmplfunc.applyTemplate(tmpl, *this);
// }
//
// /** Return the relative weights of group0 and group1 of this molecule. An exception
//     will be thrown if the groups contains any atoms that are not in this molecule.
//
//     See EditRes::getWeight for a description of the meaning of the returned value.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
// */
// double EditMol::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
//                           const WeightFunction &weightfunc) const
// {
//     return moldata.getWeight(group0, group1, weightfunc);
// }
//
// /** Overloaded function that gets the weight using the RelFromMass function */
// double EditMol::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1) const
// {
//     return moldata.getWeight(group0, group1, RelFromMass());
// }
//
// /** Calculate and return the geometric center of the EditMol */
// Vector EditMol::center() const
// {
//     Vector minvec, maxvec;
//     bool start = true;
//
//     foreach( ResNum resnum, residueNumbers() )
//     {
//         AtomSet resatms = atoms(resnum);
//
//         int nats = resatms.count();
//
//         if (start and nats > 0)
//         {
//             //initialise minvec and maxvec to be the coordinates of
//             //the first atom in the molecule.
//             minvec = resatms.at(0);
//             maxvec = minvec;
//             start = false;
//         }
//
//         for (int j=0; j<nats; ++j)
//         {
//             const Atom &atom = resatms.at(j);
//             minvec.setMin(atom);
//             maxvec.setMax(atom);
//         }
//     }
//
//     return 0.5 * (minvec + maxvec);
// }
//
// /** Translate the whole molecule by 'delta' */
// void EditMol::translate(const Vector &delta)
// {
//     //loop over each residue and translate each one
//     moldata.translate(delta);
// }
//
// /** Translate the atom 'atm' by 'delta'. This will throw an exception if there is no such
//     atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// void EditMol::translate(const AtomIndex &atm, const Vector &delta)
// {
//     moldata.translate(atm, delta);
// }
//
// /** Translate the atoms in 'group' that are in this molecule by 'delta'. An exception
//     is thrown if this group contains atoms that are not in this molecule.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
// */
// void EditMol::translate(const AtomIDGroup &group, const Vector &delta)
// {
//     moldata.translate(group,delta);
// }
//
// /** Translate the residue with number 'resnum' by 'delta'. This throws an exception
//     if there is no such residue.
//
//     \throw SireMol::missing_residue
// */
// void EditMol::translate(ResNum resnum, const Vector &delta)
// {
//     moldata.translate(resnum,delta);
// }
//
// /** Rotate the entire molecule by the rotation matrix implied by the quaternion 'quat'
//     around the point 'point'. */
// void EditMol::rotate(const Quaternion &quat, const Vector &point)
// {
//     moldata.rotate(quat,point);
// }
//
// /** Rotate the atom 'atm' by the quaternion 'quat' around the point 'point'. This
//     will throw an exception if there is no such atom in this molecule.
//
//     \throw SireMol::missing_atom
// */
// void EditMol::rotate(const AtomIndex &atm, const Quaternion &quat, const Vector &point)
// {
//     moldata.rotate(atm,quat,point);
// }
//
// /** Rotate the atoms in the AtomIDGroup 'group' by the quaternion 'quat' around the
//     point 'point'. This will throw an exception if any of the atoms in the group are
//     also not in the molecule.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::missing_residue
// */
// void EditMol::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
// {
//     moldata.rotate(group,quat,point);
// }
//
// /** Rotate the atoms in the residue with number 'resnum' by the quaternion 'quat' about
//     the point 'point'. This will throw an exception if there is no such residue.
//
//     \throw SireMol::missing_residue
// */
// void EditMol::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
// {
//     moldata.rotate(resnum,quat,point);
// }
//
// /** Change the length of the bond 'bnd' by 'delta'. The atoms passed passed in 'anchors'
//     are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used
//     to control how the two parts of the residue that are moved (the two sides of
//     the moving bond) are weighted when they are moved.
//
//     This will throw an exception if either the atoms in the bond are not present
//     in this molecule, if both sides of the bond are anchored, or if the bond
//     is part of a ring.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
//
// */
// void EditMol::change(const Bond &bnd, double delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     //Split the molecule into two groups based on the bond
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     moldata.change(bnd, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overloaded function that changes the bond length using the RelFromMass weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::change(const Bond &bond, double delta, const AtomIndexSet &anchors)
// {
//     change(bond, delta, RelFromMass(), anchors);
// }
//
// /** Change the size of the angle 'ang' by 'delta'. The atoms passed passed in 'anchors'
//     are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
//     control how the two parts of the molecule that are moved (the two sides of the moving
//     angle) are weighted when they are moved.
//
//     This will throw an exception if either the atoms in the angle are not present
//     in this molecule, if both sides of the angle are anchored, or if the angle
//     is part of a ring.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
//
// */
// void EditMol::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     //split the molecule into two parts based on the angle
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(ang, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     moldata.change(ang, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
//                      const AtomIndexSet &anchors)
// {
//     change(ang, delta, RelFromMass(), anchors);
// }
//
// /** Change the size of the dihedral 'dih' by 'delta'. The atoms passed passed in 'anchors'
//     are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
//     control how the two parts of the molecule that are moved (the two sides of the moving
//     dihedral) are weighted when they are moved.
//
//     This will throw an exception if either the atoms in the dihedral are not present
//     in this molecule, if both sides of the angle are anchored, or if the dihedral
//     is part of a ring.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
//
// */
// void EditMol::change(const Dihedral &dih, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     //split the molecule into two parts based on the dihedral
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(dih, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     moldata.change(Bond(dih.atom1(),dih.atom2()), delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::change(const Dihedral &dih, const SireMaths::Angle &delta,
//                      const AtomIndexSet &anchors)
// {
//     change(dih,delta,RelFromMass(),anchors);
// }
//
// /** Change the size of the dihedral 'dih' by 'delta'. The atoms passed passed in 'anchors'
//     are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
//     control how the two parts of the molecule that are moved (the two sides of the moving
//     dihedral) are weighted when they are moved. This moves all atoms that are either side
//     of the dihedral.
//
//     This will throw an exception if either the atoms in the dihedral are not present
//     in this molecule, if both sides of the angle are anchored, or if the dihedral
//     is part of a ring.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::change(const Bond &bnd, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     //split the molecule into two parts based on the bond
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     moldata.change(bnd, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default WeightFunction */
// void EditMol::change(const Bond &dih, const SireMaths::Angle &delta, const AtomIndexSet &anchors)
// {
//     change(dih, delta, RelFromMass(), anchors);
// }
//
// /** Change the size of the improper angle 'improper' by 'delta'. The atoms passed passed in 'anchors'
//     are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
//     control how the two parts of the molecule that are moved (the two sides of the moving
//     improper angle) are weighted when they are moved.
//
//     This will throw an exception if either the atoms in the improper are not present
//     in this molecule, if both sides of the angle are anchored, or if the improper
//     is part of a ring.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
//
// */
// void EditMol::change(const Improper &improper, const SireMaths::Angle &delta,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     //split the molecule into two parts based on the improper
//     boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(improper, connectivity());
//
//     AtomIDGroup group0 = groups.get<0>();
//     AtomIDGroup group1 = groups.get<1>();
//
//     moldata.change(improper, delta, group0, group1, weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::change(const Improper &improper, const SireMaths::Angle &delta,
//                      const AtomIndexSet &anchors)
// {
//     change(improper, delta, RelFromMass(), anchors);
// }
//
// /** This function is similar to EditMol::changeBond, except that it sets the specified
//     bond to a particular length
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const Bond &bnd, double val,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(bnd, val-measure(bnd), weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const Bond &bnd, double val, const AtomIndexSet &anchors)
// {
//     change(bnd, val-measure(bnd), anchors);
// }
//
// /** This function is similar to EditMol::changeAngle, except that it set the specified angle
//     to a particular size. Note that this will probably change the size of other angles, so
//     if you wish to set a whole series of angles, then you will need to think carefully about
//     the order in which you set them.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const SireMol::Angle &ang, const SireMaths::Angle &val,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(ang, val-measure(ang), weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const SireMol::Angle &ang, const SireMaths::Angle &val,
//                   const AtomIndexSet &anchors)
// {
//     change(ang, val-measure(ang), anchors);
// }
//
// /** This function is similar to EditMol::changeDihedral, except that it sets the specified
//     dihedral to a particular size. Note that this will probably change the size of other
//     dihedrals, so think about the order in which you apply multiple calls of this function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const Dihedral &dih, const SireMaths::Angle &val,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(dih, val-measure(dih), weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const Dihedral &dih, const SireMaths::Angle &val,
//                   const AtomIndexSet &anchors)
// {
//     change(dih, val-measure(dih), anchors);
// }
//
// /** This function is similar to EditMol::change(const Bond&), except that it sets the specified
//     dihedral to a particular size.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::setAll(const Dihedral &dih, const SireMaths::Angle &val,
//                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(dih, val-measure(dih), weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::setAll(const Dihedral &dih, const SireMaths::Angle &val,
//                      const AtomIndexSet &anchors)
// {
//     change(dih, val-measure(dih), anchors);
// }
//
// /** This function is similar to EditMol::change(const Improper&), except that it
//     sets the specified improper angle to a particular size.
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const Improper &improper, const SireMaths::Angle &size,
//                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
// {
//     change(improper, size-measure(improper), weightfunc, anchors);
// }
//
// /** Overload of the function that uses a default weight function
//
//     \throw SireMol::missing_atom
//     \throw SireMol::anchor_error
//     \throw SireMol::ring_error
// */
// void EditMol::set(const Improper &improper, const SireMaths::Angle &size,
//                   const AtomIndexSet &anchors)
// {
//     change(improper, size-measure(improper), anchors);
// }
