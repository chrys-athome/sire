
#include "qhash_siremol.h"

#include "editres.h"
#include "templatefunction.h"
#include "weightfunction.h"
#include "atomidgroup.h"
#include "editmol.h"
#include "residuebonds.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "splitmolecule.h"

#include "SireMol/errors.h"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireStream;

/** Function to hash an EditRes - an Editres is hashed according to its residue number */
uint SIREMOL_EXPORT qHash(const EditRes &editres)
{
    return qHash(editres.resNum());
}

static const RegisterMetaType<EditRes> r_editres("SireMol::EditRes");

/** Serialise an EditRes to a binary data stream (this will serialise the entire
    molecule - there is thus no need to serialise every single residue in 
    a molecule!) */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const EditRes &editres)
{
    writeHeader(ds, r_editres, 1) << editres.rnum << editres.moldata;
    
    return ds;
}

/** Deserialise an EditRes from a binary datastream. Note that this 
    will detach 'editres' - i.e. it will not overwrite any other
    EditMol or EditRes views of 'editres'. */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, EditRes &editres)
{
    VersionID v = readHeader(ds, r_editres);
    
    if (v == 1)
    {
        //detach and the editres (we don't want to mess up any
        //connected EditRes or EditMol objects...
        editres.detach();
        
        ds >> editres.rnum >> editres.moldata;
    }
    else
        throw version_error(v, "1", r_editres, CODELOC);
    
    return ds;
}

/** Create an empty residue with residue number 'resnum' and residue name 'resname'
    in a new, empty molecule */
EditRes::EditRes(ResNum resnum, const QString &resname) 
        : moldata(), rnum(resnum)
{
    moldata.addResidue(resnum, resname);
}

/** Create an empty residue called 'resnam', with residue number '1', in a new, empty
    molecule. */
EditRes::EditRes(const QString &resnam) : moldata(), rnum(1)
{
    moldata.addResidue(rnum, resnam);
}

/** Create an EditRes view of residue number 'resnum' of the EditMolData - this 
    is a private constructor that can only be called by the EditMolData class */
EditRes::EditRes(const EditMolData &mdata, ResNum resnum) : moldata(mdata), rnum(resnum)
{}

/** Shallow copy copy-constructor - call detach() or deepCopy() to get your
    own personal copy of this EditRes. */
EditRes::EditRes(const EditRes &other) : moldata(other.moldata), rnum(other.rnum)
{}

/** Destructor */
EditRes::~EditRes()
{}

/** Assignment operator - shallow copy. */
EditRes& EditRes::operator=(const EditRes &other)
{
    moldata = other.moldata;
    rnum = other.rnum;
    return *this;
}

/** Comparison operator */
bool EditRes::operator==(const EditRes &other) const
{
    return rnum == other.rnum and moldata == other.moldata;
}

/** Comparison operator */
bool EditRes::operator!=(const EditRes &other) const
{
    return rnum != other.rnum or moldata != other.moldata;
}

/** Return a deep copy of this EditRes */
EditRes EditRes::deepCopy() const
{
    return moldata.deepCopy().editres(rnum);
}

/** Return a shallow copy of this EditRes */
EditRes EditRes::shallowCopy()
{
    return EditRes(moldata, rnum);
}

/** Finalise this EditRes - this reduces the memory requirement, at
    the cost of making it slightly more expensive to edit */
void EditRes::finalise()
{
    moldata.finalise();
}

/** Detach this EditRes from shared storage. This does the same as EditMol::detach() */
void EditRes::detach()
{
    moldata = moldata.deepCopy();
}

/** Return the residue number for the residue - this is useful when creating 
    bonds involving this residue (e.g. can write Bond("CA","HA", editres)) */
EditRes::operator ResNum() const
{
    return resNum();
}

/** Return the residue number of this residue */
ResNum EditRes::resNum() const
{
    return rnum;
}

/** Return the molecule containing this residue. This returns a shallow copy 
    of the molecule, i.e. changing the returned EditMol may change this EditRes.
    Detach the molecule, or call this function on a deepCopy() to get a new
    copy of the molecule that is independent of this residue. */
EditMol EditRes::molecule()
{
    return moldata.editmol();
}
  
/** Clear this residue - this removes all atoms and bonds */
void EditRes::clear()
{
    moldata.clearResidue(rnum);
}

/** Check that there isn't an atom called 'nam' in this residue - throw 
    an exception if there is.
    
    \throw SireMol::duplicate_atom
*/
void EditRes::checkAtom(const QString &nam) const
{
    if (moldata.contains( AtomIndex(nam,rnum) ))
        throw SireMol::duplicate_atom(QObject::tr(
            "Residue %1(%2) already contains an atom called \"%2\"")
                .arg(name(),number().toString()).arg(nam), CODELOC);
}

/** Set the name of this residue */
void EditRes::setName(QString name)
{
    moldata.setResidueName(rnum, name);
}

/** Add a copy of 'newatm' to this residue.
    The atom name must be unique within this residue, or an exception will be thrown.
    
    Note that the atom names are case-sensitive, so you can have an atom called 
    "Ha" as well as an atom called "ha". I wouldn't recommend it though, as I 
    suspect that this would lead to confusion!
    
    Note also that if the residue number of 'newatm' does not match the residue
    number of this residue, then the residue number of the atom will be changed
    so that it does match this residue.
    
    \throw SireMol::duplicate_atom
*/
void EditRes::add(const Atom &newatm)
{
    moldata.add( Atom(newatm, rnum) );
}

/** Add a new atom called 'atm' 

    \throw SireMol::duplicate_atom
*/
void EditRes::add(const QString &atm)
{
    moldata.add( Atom( AtomIndex(atm,rnum) ) );
}

/** Add a whole load of atoms - note that the atom's will become part
    of this residue
     
    \throw SireMol::duplicate_atom
*/
void EditRes::add(const AtomVector &atoms)
{
    int nats = atoms.count();
    for (int i=0; i<nats; ++i)
        add(atoms.at(i));
}

/** Add a whole load of atoms 

    \throw SireMol::duplicate_atom
*/
void EditRes::add(const QStringList &atoms)
{
    foreach( QString atm, atoms )
        add(atm);
}

/** Add a whole load of atoms

    \throw SireMol::duplicate_atom
*/
void EditRes::add(const AtomList &atoms)
{
    foreach( Atom atm, atoms )
        add(atm);
}

/** Add the atoms and bonds that are in 'other' to this residue. Note that none of 
    the atoms that are in 'other' should be in this residue. If there are shared atoms,
    then consider using the 'merge' function instead. Note that this won't add any
    of the inter-residue bonds - just the intra-residue bonds.
    
    \throw SireMol::duplicate_atom
*/
void EditRes::add(const EditRes &other)
{
    //get the atoms and bonds that are in this residue
    AtomSet atms = other.atoms();
    BondList bnds = other.connectivity().intraBonds();

    //check that there are no duplicate atoms... 
    // (initial check ensures that this function is atomic - I should
    //  really do this with the other functions as well...)
    int nats = atms.count();
    for (int i=0; i<nats; ++i)
        checkAtom( atms.at(i).name() );
        
    //now add the atom and bonds...
    for (int i=0; i<nats; ++i)
        add( atms.at(i) );
    
    foreach( Bond bnd, bnds )
        addBond( bnd.atom0().name(), bnd.atom1().name() );
}

/** This merges the atoms and bonds from 'other' into this residue. */
void EditRes::merge(const EditRes &other)
{
    AtomSet atms = other.atoms();
    BondList bnds = other.connectivity().intraBonds();
    
    //add all of the atoms
    int nats = atms.count();
    for (int i=0; i<nats; ++i)
    {
        const Atom &atm = atms.at(i);
    
        //only add the atom if it doesn't already exist...
        if (not moldata.contains(AtomIndex(atm.name(),rnum)))
            add(atm);
    }
    
    //add all of the bonds
    foreach( Bond bnd, bnds )
    {
        addBond( bnd.atom0().name(), bnd.atom1().name() );
    }
}

/** Update the atom 'atm' with the element and coordinate data held in 'atm'. This will 
    throw an exception if this residue does not have an atom with this name.
    Note that the atom is matched to the name using the same rules as 
    the EditRes::atom function. Note also that the residue number of 'newatm' is 
    ignored, so the atom is matched purely on the atom's name.
     
    \throw SireMol::missing_atom
*/
void EditRes::update(const Atom &newatm)
{
    moldata.update( Atom(newatm, rnum) );
}

/** Update the atom called 'nam' to be identical to the atom 'details' (only keeping
    its original name and residue number) 
    
    \throw SireMol::missing_atom
*/
void EditRes::update(const QString &nam, const Atom &details)
{
    moldata.update( AtomIndex(nam,rnum), details );
}

/** Update the atom with name 'nam' to have coordinates 'coords'. This will 
    throw an exception if there is no such atom in this residue.
     
    \throw SireMol::missing_atom     
*/
void EditRes::update(const QString &nam, const Vector &coords)
{
    moldata.update( AtomIndex(nam,rnum), coords );
}

/** Update the atom with name 'nam' to have Element 'element'. This will
    throw an exception if there is no such atom in this residue.
     
    \throw SireMol::missing_atom
*/
void EditRes::update(const QString &nam, const Element &element)
{
    moldata.update( AtomIndex(nam,rnum), element );
}

/** Remove the atom with the name 'nam' from this residue. This does nothing if there
    is no atom with this *exact* name (name is case-sensitive, e.g. remove("ha") would
    not remove the atom "HA") */
void EditRes::remove(const QString &atom)
{
    moldata.remove( AtomIndex(atom,rnum) );
}

/** Remove the atoms with names in 'atoms' from this residue. This does nothing for 
    the names that are not in this residue. The name match is case-sensitive, e.g.
    remove("ha") would not remove the atom "HA". */
void EditRes::remove(const QStringList &atoms)
{
    foreach( QString atm, atoms )
        this->remove(atm);
}

/** Remove the atom with index 'atm'. This does nothing if this atom is not in this 
    residue. */
void EditRes::remove(const AtomIndex &atm)
{
    if (atm.resNum() == rnum)
        moldata.remove(atm);
}

/** Remove a whole load of atoms - this only removes atoms that are in this residue */
void EditRes::remove(const AtomIndexList &atoms)
{
    foreach( AtomIndex atm, atoms )
        remove(atm);
}

/** Remove a whole load of atoms - this only removes atoms that are in this residue */
void EditRes::remove(const AtomIndexSet &atoms)
{
    foreach( AtomIndex atm, atoms )
        remove(atm);
}

/** Return a short string representation of this EditRes */
QString EditRes::toString() const
{
    return QObject::tr("EditRes (%1-%2) : nAtoms() == %3")
                    .arg(name(), number().toString()).arg(nAtoms());
}

/** Return the set of AtomIndexes of all of the atoms in this residue */
QStringList EditRes::atomNames() const
{
    return moldata.atomNames(rnum);
}

/** Return the set of atoms that are in this residue */
AtomSet EditRes::atoms() const
{
    return moldata.atoms(rnum);
}

/** Return the vector of atoms that are in this residue */
AtomVector EditRes::atomVector() const
{
    return moldata.atomVector(rnum);
}

/** Return the name of this residue */
QString EditRes::name() const
{
    return moldata.residueName(rnum);
}

/** Return this residue's residue number. This is a constant throughout the life of 
    this residue, and is used to uniquely identify this residue in the parent EditMol */
ResNum EditRes::number() const
{
    return rnum;
}

/** Return whether or not this residue contains an Atom with the name 'atomname'
    (exact case-sensitive match) */
bool EditRes::contains(const QString &atomname) const
{
    return moldata.contains(AtomIndex(atomname,rnum));
}

/** Return whether or not this residue contains the atom with index 'atm' */
bool EditRes::contains(const AtomIndex &atm) const
{
    return moldata.contains(atm);
}

/** Return the number of atoms in this residue */
int EditRes::nAtoms() const
{
    return moldata.nAtoms(rnum);
}

/** Return a reference to the ith atom in this residue. Note that the index of an atom
    is quite variable, and will change as atoms are added and deleted from this residue.
    You must use the name of the atom to refer to it, not its index. This will throw
    an exception if this is not a valid index. This is only useful if you are looping
    over the atoms in the residue (and you know that you are not changing the number 
    of atoms)
    
    \throw invalid_index
*/
const Atom& EditRes::at(int i) const
{
    return moldata.at(rnum,i);
}

/** Synonym for EditRes::at(int i) */
const Atom& EditRes::atom(int i) const
{
    return moldata.at(rnum,i);
}

/** Return the ith atom in this residue - this is just an indexing operator.
    See EditRes::at(int i)
     
    \throw invalid_index
*/
const Atom& EditRes::operator[](int i) const
{
    return moldata.at(rnum,i);
}

/** Return the atom with the name 'atomname'. This is just a hash/dictionary indexing operator.
    Set EditRes::atom(const QString &atomname)
    
    \throw SireMol::missing_atom
*/
const Atom& EditRes::atom(const QString &atomname) const
{
    return moldata.atom( AtomIndex(atomname,rnum) );
}

/** Return the atom with the name 'atomname'. This is just a hash/dictionary indexing operator.
    Set EditRes::atom(const QString &atomname)
    
    \throw SireMol::missing_atom
*/
const Atom& EditRes::operator[](const QString &atomname) const
{
    return moldata.atom( AtomIndex(atomname,rnum) );
}

/** Return the number of atoms in this residue */
int EditRes::count() const
{
    return moldata.nAtoms(rnum);
}

/** Return the number of atoms in this residue - this is the array syntax version */
int EditRes::size() const
{
    return moldata.nAtoms(rnum);
}

/** Return a copy of the list of all bonds in this residue, and to other residues */
BondList EditRes::bonds() const
{
    return moldata.connectivity(rnum).bonds();
}

/** Add the bond between the atoms with names 'atom0' and 'atom1'.
    The atoms must exist in this residue or else an exception will be thrown.
    
    Note that the atom name matching rules are the same as for EditRes::atom
    
    \throw SireMol::missing_atom
*/
void EditRes::addBond(const QString &atom0, const QString &atom1)
{
    //add the bond made of the atoms in this residue - we get
    //the actual atoms because then the atom name strings in the bond
    //are implicitly shared with the atom name strings in the atoms.
    moldata.add( Bond(AtomIndex(atom0,rnum), AtomIndex(atom1,rnum)) );
}

/** Add the bond 'bnd' - note that at least one of the atoms in the bond must 
    be in this residue! 
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditRes::add(const Bond &bnd)
{
    //ensure that the residue figures in this bond
    if (bnd.atom0().resNum() != rnum and bnd.atom1().resNum() != rnum)
        throw SireMol::missing_residue(QObject::tr(
            "At least one of the atoms in bond %1 must be in residue %2")
                .arg(bnd.toString(), rnum.toString()), CODELOC);

    moldata.add(bnd);
}

/** Add a whole load of bonds 

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditRes::add(const BondList &bonds)
{
    foreach( Bond bnd, bonds )
        add(bnd);
}

/** Remove the bond between atoms 'atom0' and 'atom1'. This does nothing if these atoms are
    not in this residue, or there is no such bond. Note that the atom names must match
    exactly.
*/
void EditRes::removeBond(const QString &atom0, const QString &atom1)
{
    moldata.remove( Bond(AtomIndex(atom0,rnum), AtomIndex(atom1,rnum)) );
}

/** Remove the specific bond - this only removes the bond if at least 
    one atom in the bond is in this residue.
*/
void EditRes::remove(const Bond &bnd)
{
    if (bnd.atom0().resNum() == rnum or bnd.atom1().resNum() == rnum)
        moldata.remove(bnd);
}

/** Remove a whole load of bonds. This only removes the bonds that have
    at least one atom in this residue.
*/
void EditRes::remove(const BondList &bonds)
{
    foreach( Bond bnd, bonds )
        remove(bnd);
}

/** Remove all bonds from this residue */
void EditRes::removeAllBonds()
{
    moldata.removeAllBonds(rnum);
}

/** Remove all bonds in this residue that are with the atom 'atomname'. This does nothing
    if this atom is not in this residue, or if it has no bonds. 
 */
void EditRes::removeAllBonds(const QString &atomname)
{
    moldata.removeAllBonds( AtomIndex(atomname,rnum) );
}

/** Return the set of bonds that include the atom 'atm'. This will return an empty set
    if there is no such atom in this residue, or if it has no bonds */
BondList EditRes::bonds(const QString &atmname) const
{
    return moldata.connectivity(rnum).bonds(atmname);
}

/** Return the list of residues that this residue is bonded to.
    Note that each residue will be on a shallow copy of the molecule */
EditResSet EditRes::residuesBondedTo()
{
    return moldata.residuesBondedTo(rnum);
}

/** Extract this residue from the molecule. This returns a deep copy of this residue, 
    which is in an otherwise empty molecule. */
EditRes EditRes::extract() const
{
    return moldata.extract(rnum);
}

/** Extract this residue from the molecule, and renumber the result to 'newnum'. 
    This returns a deep copy of this residue, resnumbered to 'newnum', which is 
    in an otherwise empty molecule. */
EditRes EditRes::extract(ResNum newnum) const
{
    EditRes newres = moldata.extract(rnum);
    
    newres.setNumber(newnum);
    
    return newres;
}

/** Renumber this residue to 'newnum'. Note that a residue with this new number 
    must not already exist in the molecule, or else an exception will be thrown.
    This function is not used to merge residues together. If you wish to merge
    two residues then use the 'merge()' function of EditMol. 
    
    \throw SireMol::duplicate_residue 
*/
void EditRes::setNumber(ResNum newnum)
{
    //tell the moldata to renumber this residue
    moldata.renumberResidue(rnum, newnum);
    
    //save our new number
    rnum = newnum;
}

/** Apply a residue template to this residue, using the template function 'tmplfunc' 

    \throw SireMol::template_error
*/
void EditRes::applyTemplate(const EditRes &tmpl, const TemplateFunction &tmplfunc)
{
    tmplfunc.applyTemplate(tmpl, *this);
}

/** Use an algorithm to automatically add the bonds between all atoms of 
    this residue */
void EditRes::addAutoBonds()
{
    //loop over all pairs of atoms in this residue, and if the
    //atoms are within their vdw radius (plus a small margin) then
    //the atoms are considered as being bonded.
    
    AtomSet atms = this->atoms();
    
    int sz = atms.count();
    for (int i=0; i<sz-1; i++)
    {
        const Atom &atom0 = atms[i];
        
        //don't add bonds that involve dummies
        if (atom0.nProtons() > 0)
        {
            for (int j=i+1; j<sz; j++)
            {
                const Atom &atom1 = atms[j];
                
                if (atom1.nProtons() > 0 and Atom::withinBondRadii(atom0,atom1,0.1))
                    moldata.add( Bond(atom0,atom1) );
            }
        }
    }
}

/** Return the geometric line representing the bond from atoms called atom0-atom1.
    Note that there doesn't actually have to be a real bond between these atoms. 
    
    \throw SireMol::missing_atom
*/
SireMaths::Line EditRes::bond(const QString &atom0, const QString &atom1) const
{
    return moldata.bond( Bond(atom0,atom1,rnum) );
}

/** Return the geometric triangle representing the angle from atoms called atom0-atom1-atom2 
    
    \throw SireMol::missing_atom
*/
SireMaths::Triangle EditRes::angle(const QString &atom0, const QString &atom1,
                                   const QString &atom2) const
{
    return moldata.angle( SireMol::Angle(atom0,atom1,atom2,rnum) );
}
  
/** Return the geometric torsion representing the dihedral from atoms called 
    atom0-atom1-atom2-atom3 
    
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditRes::dihedral(const QString &atom0, const QString &atom1,
                                     const QString &atom2, const QString &atom3) const
{
    return moldata.dihedral( Dihedral(atom0,atom1,atom2,atom3,rnum) );
}

/** Return the geometric torsion representing the improper angle from atoms
    called atom0-atom1-atom2-atom3
    
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditRes::improper(const QString &atom0, const QString &atom1,
                                     const QString &atom2, const QString &atom3) const
{
    return moldata.improper( Improper(atom0,atom1,atom2,atom3,rnum) );
}

/** Return the length of the bond between atoms called 'atom0' and 'atom1' 
    
    \throw SireMol::missing_atom
*/
double EditRes::measure(const QString &atom0, const QString &atom1) const
{
    return moldata.measure( Bond(atom0,atom1,rnum) );
}
    
/** Return the size of the angle between atoms called atom0-atom1-atom2 
    
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditRes::measure(const QString &atom0, const QString &atom1,
                                  const QString &atom2) const
{
    return moldata.measure( SireMol::Angle(atom0,atom1,atom2,rnum) );
}

/** Return the size of the dihedral angle for atoms called atom0-atom1-atom2-atom3 
    
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditRes::measure(const QString &atom0, const QString &atom1,
                                  const QString &atom2, const QString &atom3) const
{
    return moldata.measure( Dihedral(atom0,atom1,atom2,atom3,rnum) );
}

/** Return the size of the improper angle for atoms called atom0-atom1-atom2-atom3

    \throw SireMol::missing_atom
*/
SireMaths::Angle EditRes::measureImproper(const QString &atom0, const QString &atom1,
                                          const QString &atom2, const QString &atom3) const
{
    return moldata.measure( Improper(atom0,atom1,atom2,atom3,rnum) );
}

/** Return the number of intra-residue and inter-residue bonds in this residue */
int EditRes::nBonds() const
{
    return moldata.connectivity(rnum).nBonds();
}

/** Return the BondGroup that holds the bonding information for this EditRes */
ResidueBonds EditRes::connectivity() const
{
    return moldata.connectivity(rnum);
}

/** Translate all of the atoms in this residue by 'delta' */
void EditRes::translate(const Vector &delta)
{
    moldata.translate(rnum,delta);
}

/** Translate the atom called 'atm' by 'delta'. This will throw an exception if 
    there is no atom with this name in this residue.
  
    \throw SireMol::missing_atom
*/
void EditRes::translate(const QString &atm, const Vector &delta)
{
    moldata.translate( AtomIndex(atm,rnum), delta );
}

/** Translate the atoms in 'atoms' by 'delta'. An exception is thrown
    if this group contains atoms that are not in this residue.
     
    \throw SireMol::missing_atom
*/
void EditRes::translate(const QStringList &atoms, const Vector &delta)
{
    moldata.translate(rnum, atoms, delta);
}

/** Rotate the entire residue by the quaternion 'quat' around the point 'point'. */
void EditRes::rotate(const Quaternion &quat, const Vector &point)
{
    moldata.rotate(rnum, quat, point);
}

/** Rotate the single atom 'atm' in this residue by the quaternion 'quat' around 
    the point 'point'. An exception will be thrown if there is no such atom in this residue.
    
    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QString &atm, const Quaternion &quat, const Vector &point)
{
    moldata.rotate( AtomIndex(atm,rnum), quat, point );
}

/** Rotate all of the atoms in 'atoms' by the quaternion 'quat'  around the point 'point'. 
    This will throw an exception if 'atoms' contains atoms that are not in this 
    residue.
    
    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QStringList &atoms, const Quaternion &quat, const Vector &point)
{
    moldata.rotate(rnum, atoms, quat, point);
}

/** Change the bond 'bnd', only moving the atoms in this residue. This will not
    move any other atoms in the molecule. Note that the move will ignore any
    inter-residue bonding, so the move may (badly) change interresidue bond 
    lengths and angles. 
    
    If you wish to perform a move that will not change any inter-residue bonds
    or angles, then you need to add the atoms that are involved in inter-residue
    bonds to the list of anchors. The list of atoms in inter-residue bonds
    can be obtained from the editres.connectivity().anchors() function, e.g.
    call
    
    Note that the bond must contain at least one atom that is in this residue
    or else an exception will be thrown
    
    \code
    editres.change(bnd, delta, weightfunc, editres.connectivity().anchors());
    \endcode
      
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
    
*/
void EditRes::change(const Bond &bnd, double delta,
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    if (SireMaths::isZero(delta))
        return;

    //split the residue into two based on the two atoms of the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(bnd, connectivity());
    
    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();
    
    //tell the moldata to move the two groups by delta along the vector of the bond...
    moldata.change(bnd, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function that uses the default RelFromMass weight function 

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Bond &bnd, double delta, const AtomIndexSet &anchors)
{
    change(bnd, delta, RelFromMass(), anchors);
}

/** Change the angle 'ang' by an angle of 'delta'.

    This has the same restrictions as EditRes::change(Bond), i.e. the move will
    only move atoms in this residue
    
    Note that the angle must contain at least one atom that is in this residue
    or an exception will be thrown
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    if (delta.isZero())
        return;

    //split the residue into two parts...
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(ang, connectivity());
    
    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();
    
    //tell the moldata to move the two groups by delta about this angle
    moldata.change(ang, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function that uses the default RelFromMass() weight function 

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                     const AtomIndexSet &anchors)
{
    change(ang, delta, RelFromMass(), anchors);
}

/** Change the dihedral 'dih' by an angle of 'delta'.

    This has the same restrictions as EditRes::change(Bond), i.e. the move 
    will only move atoms in this residue
    
    Note that the dihedral must contain at least one atom that is in this 
    residue or else an exception will be thrown
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Dihedral &dih, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    if (delta.isZero())
        return;

    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(dih, connectivity());
    
    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();
    
    //tell the moldata to move the two groups around the dihedral
    moldata.change(Bond(dih.atom1(),dih.atom2()), delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function 

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Dihedral &dih, const SireMaths::Angle &delta,
                     const AtomIndexSet &anchors)
{
    change(dih, delta, RelFromMass(), anchors);
}

/** Overloaded function that moves all atoms of a dihedral (about the central
    bond of the dihedral)
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Bond &dih, const SireMaths::Angle &delta, 
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    if (delta.isZero())
        return;

    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(dih, connectivity());
    
    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();
    
    //tell the moldata to move the two groups around the dihedral
    moldata.change(dih, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function use to use the default RelFromMass() weight function 

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Bond &bnd, const SireMaths::Angle &delta, const AtomIndexSet &anchors)
{
    change(bnd, delta, RelFromMass(), anchors);
}

/** Change the improper angle by an angle of 'delta'. 
    
    This has the same restrictions as EditRes::change(Bond), i.e. the move 
    will only move atoms in this residue
    
    Note that the improper angle must contain at least one atom that is in this 
    residue or else an exception will be thrown
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Improper &improper, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    if (delta.isZero())
        return;

    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(improper, connectivity());
    
    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();
    
    //tell the moldata to move the two groups around the dihedral
    moldata.change(improper, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() function 

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Improper &improper, const SireMaths::Angle &delta,
                     const AtomIndexSet &anchors)
{
    change(improper, delta, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the length of 
    a bond. Note that this uses EditRes::change(Bond), so has the same restrictions.
    
    Note that at least one of the atoms in the bond must be in this residue
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Bond &bnd, double size, 
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(bnd, size-moldata.measure(bnd), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Bond &bnd, double size, const AtomIndexSet &anchors)
{
    set(bnd, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of 
    an angle. Note that this uses EditRes::change(Angle), so has the same restrictions.
    
    Note that at least one of the atoms in the angle must be in this residue
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const SireMol::Angle &ang, const SireMaths::Angle &size, 
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(ang, size-moldata.measure(ang), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
                  const AtomIndexSet &anchors)
{
    set(ang, size, RelFromMass(), anchors);
}                  

/** This is a convienience function that allows you to directly set the size of 
    a dihedral. Note that this uses EditRes::change(Dihedral), so has the same restrictions.
    
    Note that at least one of the atoms in the dihedral must be in this residue
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Dihedral &dih, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(dih, size-moldata.measure(dih), weightfunc, anchors);
}    

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Dihedral &dih, const SireMaths::Angle &size, const AtomIndexSet &anchors)
{
    set(dih, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of 
    a dihedral. Note that this uses EditRes::change(Bond,SireMaths::Angle), 
    so has the same restrictions.
    
    Note that this moves all of the atoms either side of the central bond of the dihedral
    
    Note that at least one of the atoms in the dihedral must be in this residue
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::setAll(const Dihedral &dih, const SireMaths::Angle &size, 
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(dih, size - moldata.measure(dih), weightfunc, anchors);
}                     

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::setAll(const Dihedral &dih, const SireMaths::Angle &size, 
                     const AtomIndexSet &anchors)
{
    change(dih, size - moldata.measure(dih), anchors);
}

/** This is a convienience function that allows you to directly set the size of 
    an improper angle. Note that this uses EditRes::change(Improper), 
    so has the same restrictions.
    
    Note that at least one of the atoms in the improper angle must be in this residue
    
    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Improper &improper, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(improper, size-moldata.measure(improper), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Improper &improper, const SireMaths::Angle &size,
                  const AtomIndexSet &anchors)
{
    set(improper, size, RelFromMass(), anchors);
}
