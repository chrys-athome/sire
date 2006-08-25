
#include "qhash_siremol.h"

#include <QDataStream>
#include <QSharedData>
#include <QMutex>

#include "moleculedata.h"
#include "residuebonds.h"
#include "cutgroup.h"
#include "cutgroupset.h"
#include "molecule.h"
#include "residue.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "atomidgroup.h"
#include "weightfunction.h"
#include "moleculeinfo.hpp"
#include "moleculesignature.h"

#include "SireBase/mutablesetiterator.hpp"
#include "SireMol/errors.h"

#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/rotate.h"

#include "SireStream/datastream.h"

using namespace SireStream;

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeDataPvt&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeDataPvt&);

namespace SireMol
{

typedef SireBase::MutableSetIterator<CutGroupID,CutGroup> MCGSetIterator;

using SireMaths::Matrix;
using SireMaths::Quaternion;

class SIREMOL_EXPORT MoleculeDataPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeDataPvt&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeDataPvt&);

public:
    MoleculeDataPvt();
    
    MoleculeDataPvt(const QString &molname, 
                    const ResidueIDSet &resnames,
                    const CutGroupVector &cutgroups, 
                    const MoleculeBonds &molbonds);
    
    MoleculeDataPvt(const MoleculeDataPvt &other);
    
    ~MoleculeDataPvt();
    
   ////// Operators ////////////////////////////////////////
    bool operator==(const MoleculeDataPvt &other) const;
    bool operator!=(const MoleculeDataPvt &other) const;
   /////////////////////////////////////////////////////////
   
   
   ////// Dealing with the ID number ///////////////////////
     MoleculeID ID() const;
     void setNewID();
   
     const MoleculeVersion& version() const;
     void incrementMajorVersion();
     void incrementMinorVersion();
   /////////////////////////////////////////////////////////
   
   
   ///// Querying the molecule /////////////////////////////
    MoleculeBonds connectivity() const;
    ResidueBonds connectivity(ResNum resnum) const;
    
    const MoleculeCGInfo& info() const;
    
    MoleculeSignature signature() const;
    
    AtomSet atoms() const;
    AtomSet atoms(ResNum resnum) const;
    
    AtomVector atomVector() const;
    AtomVector atomVector(ResNum resnum) const;
    
    CutGroupSet cutGroups() const;
    CutGroupSet cutGroups(ResNum resnum) const;
    
    const CutGroup& cutGroup(CutGroupID id) const;
    const Atom& atom(const CGAtomID &id) const;
    
    const QString& name() const;
    QString residueName(ResNum resnum) const;
    
    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;
    
    ResNumList residueNumbers() const;
    QStringList residueNames() const;
    
    ResNumList residueNumbers(const QString &resnam) const;
    
    const Atom& atom(const AtomIndex &atm) const;
    
    bool contains(ResNum resnum) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(const Bond &bond) const;
    
    const Atom& at(ResNum resnum, int i) const;
    
    int nAtoms() const;
    int nAtoms(ResNum resnum) const;
    int nAtoms(CutGroupID id) const;
    
    int nResidues() const;
    
    int nBonds() const;
    int nCutGroups() const;
    
    QStringList atomNames(ResNum resnum) const;
    
    SireMaths::Line bond(const Bond &bnd) const;
    SireMaths::Triangle angle(const SireMol::Angle &ang) const;
    SireMaths::Torsion dihedral(const Dihedral &dih) const;
    SireMaths::Torsion improper(const Improper &improper) const;
    
    double measure(const Bond &bnd) const;
    SireMaths::Angle measure(const SireMol::Angle &ang) const;
    SireMaths::Angle measure(const Dihedral &dih) const;
    SireMaths::Angle measure(const Improper &improper) const;
   
    double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1, 
                     const WeightFunction &weightfunc) const;
   
    void checkResidue(ResNum resnum) const;
    void checkCutGroup(CutGroupID id) const;
   /////////////////////////////////////////////////////////


   //////// Moving the molecule ////////////////////
    void translate(const Vector &delta);
    void translate(const AtomIDGroup &group, const Vector &delta);
    void translate(const AtomIndexSet &atoms, const Vector &delta);
    void translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
    void translate(ResNum resnum, const Vector &delta);
    
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndexSet &atoms, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
   /////////////////////////////////////////////////

   
   //////// Internal geometry moves ////////////////
    void change(const Bond &bnd, double delta, 
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
    
    void change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
    
    void change(const Bond &bnd, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
                          
    void change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
   /////////////////////////////////////////////////

private:

    static MoleculeID getNewID();
    static QMutex idmutex;
    static MoleculeID lastid;
   
    void translate(const AtomIndex &atm, const Vector &delta);
   
    void rotate(const AtomIndex &atm, const Matrix &rotmat, const Vector &point);
    void rotate(ResNum resnum, const Matrix &rotmat, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, 
                const Matrix &rotmat, const Vector &point);

    /** The molecule info - this contains a hash of ResidueInfo objects, indexed
        by residue number, each of which contains the names of the atoms in each 
        residue. This also provides the information necessary to map from an
        AtomIndex to a CGAtomID, so that we can locate the actual atom data. */
    MoleculeCGInfo molinfo;
    
    /** All of the atoms in this molecule, arranged into CutGroups. The index
        of the CutGroup in the vector is its CutGroupID */
    CutGroupVector cgroups;
    
    /** The CutGroupIDs of the CutGroups that contain atoms of each residue,
        indexed by residue number */
    QHash<ResNum, CutGroupIDSet> rescgroups;
    
    /** The connectivity of this molecule */
    MoleculeBonds molbonds;

    /** ID number used to identify the molecule */
    MoleculeID id;
    
    /** The version number of the molecule */
    MoleculeVersion molversion;
};

} // End of namespace SireMol

using namespace SireMol;

////////////
//////////// Implementation of MoleculeDataPvt
////////////

static const RegisterMetaType<MoleculeData> r_moldata("SireMol::MoleculeData", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const MoleculeDataPvt &moldata)
{
    writeHeader(ds, r_moldata, 1) << moldata.molinfo << moldata.cgroups
                                  << moldata.rescgroups << moldata.molbonds
                                  << moldata.id << moldata.molversion;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, MoleculeDataPvt &moldata)
{
    VersionID v = readHeader(ds, r_moldata);
    
    if (v == 1)
    {
        ds >> moldata.molinfo >> moldata.cgroups >> moldata.rescgroups
           >> moldata.molbonds >> moldata.id >> moldata.molversion;
    }
    else
        throw version_error(v, "1", r_moldata, CODELOC);
    
    return ds;
}

/** Mutex to serialise the aquisition of new ID numbers */
QMutex MoleculeDataPvt::idmutex;

/** The last allocated ID number */
MoleculeID MoleculeDataPvt::lastid(0);

/** Get a new, unique, ID number */
MoleculeID MoleculeDataPvt::getNewID()
{
    QMutexLocker lkr(&idmutex);
    ++lastid;
    return lastid;
}

/** Constructor */
MoleculeDataPvt::MoleculeDataPvt() : QSharedData()
{}

/** Construct the MoleculeData for a molecule called 'molname', with identified residues
    in 'resids', atoms arranged into the CutGroups in 'cutgroups', and molecular bonding
    described in 'bonds'. Exceptions will be thrown if either the same atom appears in
    two different CutGroups, or if a bond is passed that refers to an atom that isn't
    in the CutGroups, or if an atom refers to a residue that is not in 'resids'. 
    
    \throw SireMol::duplicate_atom
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
MoleculeDataPvt::MoleculeDataPvt(const QString &molname, const ResidueIDSet &resids,
                                 const CutGroupVector &cutgroups, const MoleculeBonds &bonds)
                : QSharedData(), id(getNewID())
{
    //ok - lets build everything.
    
    //create the ResidueInfos for this molecule...
    QHash<ResNum,ResidueCGInfo> residues;
    
    int nres = resids.count();
    for (int i=0; i<nres; ++i)
    {
        const ResidueID &resid = resids.at(i);
        residues.insert(resid.number(), ResidueCGInfo(resid.name(),resid.number()));
    }
    
    //copy the CutGroupVector
    cgroups = cutgroups;
    cgroups.squeeze();
    
    //now run through each CutGroup and set its ID (same as index in vector). Then
    //add the ID of each atom to the ResidueInfo of the residue to which it belongs
    int ncg = cgroups.count();
    for (int i=0; i<ncg; ++i)
    {
        CutGroup &cgroup = cgroups[i];
        cgroup.setID( MolCutGroupID(id,i) );
        
        int nats = cgroup.nAtoms();
        const Atom *atomarray = cgroup.atoms().constData();
        
        for (int j=0; j<nats; ++j)
        {
            const Atom &atom = atomarray[j];
            if (not residues.contains(atom.resNum()))
                throw SireMol::missing_residue(QObject::tr(
                    "Error while building the MoleculeData for molecule \"%1\"! "
                    "No residue with number %2. (needed by atom \"%3\")")
                        .arg(molname,atom.resNum().toString(),atom.toString()), CODELOC);

            residues[atom.resNum()].add(atom.name(), CGAtomID(i,j));
            
            //now add this CutGroup to the list of CutGroups that contain atoms 
            //from this residue
            rescgroups[atom.resNum()].insert(i);
        }
    }
    
    //set the name of this molecule
    molinfo = MoleculeCGInfo(molname);
    
    //squeeze each residueinfo, and add them to our moleculeinfo
    for (int i=0; i<nres; ++i)
    {
        ResidueCGInfo resinfo = residues[resids[i].number()];
        resinfo.squeeze();
        
        molinfo.add(resinfo);
    }
    
    //now that the atom, residue and molecule info is complete, we now
    //just have to do the bonds.
    
    //copy the bonds (fast, as implicitly shared)
    molbonds = bonds;
    
    //finalise them (they won't be edited any more, so we can minimise
    //their memory usage)
    molbonds.finalise();
    
    //run through all of the atoms referred to by these bonds and ensure
    //that they exist in this atom. 
    ResNumList resnums = molbonds.resNums();
    
    foreach( ResNum resnum, resnums )
    {
        if (not molinfo.contains(resnum))
            throw SireMol::missing_atom(QObject::tr(
                "Error while building the MoleculeData for molecule \"%1\". "
                "Bonds refer to missing atom - indeed a whole missing residue - %2")
                    .arg(molinfo.name(),resnum.toString()), CODELOC);
                    
        ResidueBonds resbonds = molbonds.residue(resnum);
        AtomIndexSet atms = resbonds.atoms();
        
        foreach( AtomIndex atm, atms )
        {
            if (not molinfo.contains(atm))
                throw SireMol::missing_atom(QObject::tr(
                    "Error while building the MoleculeData for molecule \"%1\". "
                    "Bonds refer to missing atom \"%2\"")
                        .arg(molinfo.name(),atm.toString()), CODELOC);
        }
    }
    
    //increment the version number of the molecule
    molversion.incrementMajor();
    
    //all done!
}

/** Copy constructor */
MoleculeDataPvt::MoleculeDataPvt(const MoleculeDataPvt &other) 
                : QSharedData(), molinfo(other.molinfo),
                  cgroups(other.cgroups), rescgroups(other.rescgroups),
                  molbonds(other.molbonds), id(other.id), molversion(other.molversion)
{}

/** Destructor */
MoleculeDataPvt::~MoleculeDataPvt()
{}

/** Comparison operator */
bool MoleculeDataPvt::operator==(const MoleculeDataPvt &other) const
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);
    return false;
}

/** Comparison operator */
bool MoleculeDataPvt::operator!=(const MoleculeDataPvt &other) const
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);
    return true;
}
    
/** Return the name of this molecule */
inline const QString& MoleculeDataPvt::name() const
{
    return molinfo.name();
}
    
/** Return the number of atoms in this molecule */
inline int MoleculeDataPvt::nAtoms() const
{
    return molinfo.nAtoms();
}

/** Return the number of atoms in residue 'resnum'

    \throw SireMol::missing_residue
*/
inline int MoleculeDataPvt::nAtoms(ResNum resnum) const
{
    return molinfo.residue(resnum).nAtoms();
}
    
/** Return the number of residues in the molecule. */
inline int MoleculeDataPvt::nResidues() const
{
    return molinfo.nResidues();
}
    
/** Return the number of bonds in the molecule */
inline int MoleculeDataPvt::nBonds() const
{
    return molbonds.nBonds();
}
    
/** Check that the residue 'resnum' exists - else throw an exception */
inline void MoleculeDataPvt::checkResidue(ResNum resnum) const
{
    if (not molinfo.contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "Molecule \"%1\" does not contain a residue with number \"%2\"")
                .arg(name(),resnum.toString()), CODELOC);
}

/** Return the ID number of the molecule */
inline MoleculeID MoleculeDataPvt::ID() const
{
    return id;
}

/** Give this molecule a new ID number. This will also give a new 
    ID number to all contained (identified) objects, e.g. CutGroups */
void MoleculeDataPvt::setNewID()
{
    id = getNewID();
    
    //tell all of the CutGroups about the change of ID number
    int sz = cgroups.count();
    CutGroup *array = cgroups.data();
    for (int i=0; i<sz; ++i)
    {
        array[i].setID( MolCutGroupID(id,i) );
    }
}
 
/** Return the version number of the molecule */
inline const MoleculeVersion& MoleculeDataPvt::version() const
{
    return molversion;
}

/** Increment the major version of the molecule */
inline void MoleculeDataPvt::incrementMajorVersion()
{
    molversion.incrementMajor();
}

/** Increment the minor version number of the molecule */
void MoleculeDataPvt::incrementMinorVersion()
{
    molversion.incrementMinor();
}

/** Return the connectivity of this molecule */
inline MoleculeBonds MoleculeDataPvt::connectivity() const
{
    return molbonds;
}

/** Return the connectivity of residue 'resnum' 

    \throw SireMol::missing_residue
*/
inline ResidueBonds MoleculeDataPvt::connectivity(ResNum resnum) const
{
    checkResidue(resnum);
    return molbonds.residue(resnum);
}
    
/** Return a const reference to the molecule info */
inline const MoleculeCGInfo& MoleculeDataPvt::info() const
{
    return molinfo;
}

/** Return a signature for the molecule. This can be used to compare
    this molecule with another (or with an EditMol) to see if the
    two objects describe the same molecules (i.e. contain the same
    residues, atoms and bonds) */
MoleculeSignature MoleculeDataPvt::signature() const
{
    return MoleculeSignature(molinfo.signature(), molbonds);
}

/** Return the number of CutGroups in the molecule */
inline int MoleculeDataPvt::nCutGroups() const
{
    return cgroups.count();
}
    
/** Check the CutGroupID is valid */
inline void MoleculeDataPvt::checkCutGroup(CutGroupID id) const
{
    if ( id >= cgroups.count() )
        throw SireMol::missing_cutgroup(QObject::tr(
            "There is no CutGroup with ID == %1 in Molecule \"%2\" (max ID == %3)")
                .arg(id.toString(),name()).arg(cgroups.count()-1), CODELOC);
}
    
/** Return the CutGroup with CutGroupID == id 

    \throw SireMol::missing_cutgroup
*/
inline const CutGroup& MoleculeDataPvt::cutGroup(CutGroupID id) const
{
    checkCutGroup(id);
    return cgroups.constData()[id.index()];
}
    
/** Return the Atom with CGAtomID 'id' 

    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
*/
inline const Atom& MoleculeDataPvt::atom(const CGAtomID &id) const
{
    return cutGroup(id.cutGroupID()).at(id.atomID());
}
    
/** Return a vector of all of the atoms in residue 'resnum', in the same
    order as they were added to the residue.

    \throw SireMol::missing_residue
*/
inline AtomVector MoleculeDataPvt::atomVector(ResNum resnum) const
{
    //get the residue info for the residue
    const ResidueCGInfo &resinfo = molinfo.residue(resnum);
    
    //copy all of the atoms into a vector 
    AtomVector resatms;
    
    int nats = resinfo.nAtoms();
    resatms.reserve(nats);
    
    for( int i=0; i<nats; ++i)
    {
        resatms += atom( resinfo[i] );
    }
    
    return resatms;
}
    
/** Return a vector of all of the atoms in this molecule. This 
    returns the atoms in the order of the residues (ordered in
    the order the residues were added to the molecule) then 
    the atoms ordered in the order that they were added to the 
    residue. This function can be slow as it copies all of the 
    atoms that are in the molecule. If you want fast access to
    all of the atoms in the molecule then I recommend looping over
    all of the CutGroups in the molecule, then over all of the atoms
    in each CutGroup. */
inline AtomVector MoleculeDataPvt::atomVector() const
{
    AtomVector allatms;
    allatms.reserve( nAtoms() );
    
    foreach( ResNum resnum, molinfo.residueNumbers() )
        allatms += atomVector(resnum);
        
    return allatms;
}

/** Return a set of all atoms in residue 'resnum', in the same order
    as they were added to the residue.
    
    \throw SireMol::missing_residue
*/
inline AtomSet MoleculeDataPvt::atoms(ResNum resnum) const
{
    return AtomSet(atomVector(resnum));
}

/** Return a set of all atoms in the molecule */
inline AtomSet MoleculeDataPvt::atoms() const
{
    return AtomSet(atomVector());
}

/** Return the set of all CutGroups in this molecule */    
inline CutGroupSet MoleculeDataPvt::cutGroups() const
{
    CutGroupSet cgset;
    
    int sz = cgroups.count();
    cgset.reserve(sz);
    
    for (int i=0; i<sz; ++i)
    {
        cgset.insert( cgroups.constData()[i] );
    }
    
    return cgset;
}

/** Return the set of all CutGroups in the residue 'resnum' */
inline CutGroupSet MoleculeDataPvt::cutGroups(ResNum resnum) const
{
    checkResidue(resnum);
    
    //get the set of CutGroupIDs of CutGroups that have atoms
    //that are in the residue
    CutGroupIDSet cgroupids = rescgroups[resnum];
    
    //get the set of CutGroups that correspond to these ID numbers
    CutGroupSet groups;
    const CutGroup *array = cgroups.constData();
    
    foreach( CutGroupID id, cgroupids )
    {
        groups.insert( array[id.index()] );
    }
    
    return groups;
}

/** Return the name of residue 'resnum' 

    \throw SireMol::missing_residue
*/
inline QString MoleculeDataPvt::residueName(ResNum resnum) const
{
    return molinfo.residue(resnum).name();
}
    
/** Return whether or not this is an empty molecule (no residues or atoms) */
inline bool MoleculeDataPvt::isEmpty() const
{
    return nResidues() == 0;
}

/** Return whether residue 'resnum' is empty (no atoms) 

    \throw SireMol::missing_residue
*/
inline bool MoleculeDataPvt::isEmpty(ResNum resnum) const
{
    return nAtoms(resnum) == 0;
}
    
/** Return the list of residue numbers in this molecule */
inline ResNumList MoleculeDataPvt::residueNumbers() const
{
    return molinfo.residueNumbers();
}

/** Return the list of residue names in this molecule */
inline QStringList MoleculeDataPvt::residueNames() const
{
    return molinfo.residueNames();
}
    
/** Return the list of residue numbers of residues called 'resnam'.
    Returns an empty list if there are no residues called 'resnam' */
inline ResNumList MoleculeDataPvt::residueNumbers(const QString &resnam) const
{
    return molinfo.residueNumbers(resnam);
}
    
/** Return the atom with index 'atm' 

    \throw SireMol::missing_atom
*/
inline const Atom& MoleculeDataPvt::atom(const AtomIndex &atm) const
{
    return atom( molinfo.index(atm) );
}
    
/** Return whether this molecule contains a residue with number 'resnum' */
inline bool MoleculeDataPvt::contains(ResNum resnum) const
{
    return molinfo.contains(resnum);
}

/** Return whether this molecule contains an atom with index 'atm' */
inline bool MoleculeDataPvt::contains(const AtomIndex &atm) const
{
    return molinfo.contains(atm);
}

/** Return whether this molecule contains the bond 'bnd' */
inline bool MoleculeDataPvt::contains(const Bond &bond) const
{
    return molbonds.contains(bond);
}

/** Return the 'ith' atom in residue 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
inline const Atom& MoleculeDataPvt::at(ResNum resnum, int i) const
{
    return atom( molinfo.residue(resnum).index(i) );
}
    
/** Return the number of atoms in the CutGroup with ID = id 

    \throw SireMol::missing_cutgroup
*/
inline int MoleculeDataPvt::nAtoms(CutGroupID id) const
{
    checkCutGroup(id);
    return cgroups.at(id.index()).count();
}

/** Return the name of all of the atoms in residue 'resnum' */
inline QStringList MoleculeDataPvt::atomNames(ResNum resnum) const
{
    return molinfo.residue(resnum).atomNames();
}
    
/** Return the geometric line representing the bond 'bnd' 

    \throw SireMol::missing_atom
*/
inline SireMaths::Line MoleculeDataPvt::bond(const Bond &bnd) const
{
    return SireMaths::Line( atom(bnd.atom0()), atom(bnd.atom1()) );
}

/** Return the geometric triangle representing the angle 'ang' 

    \throw SireMol::missing_atom
*/
inline SireMaths::Triangle MoleculeDataPvt::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( atom(ang.atom0()), atom(ang.atom1()), 
                                atom(ang.atom2()) );
}

/** Return the geometric torsion representing the dihedral 'dih'

    \throw SireMol::missing_atom
*/
inline SireMaths::Torsion MoleculeDataPvt::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( atom(dih.atom0()), atom(dih.atom1()),
                               atom(dih.atom2()), atom(dih.atom3()) );
}

/** Return the geometric line representing the improper angle 'imp'

    \throw SireMol::missing_atom
*/
inline SireMaths::Torsion MoleculeDataPvt::improper(const Improper &imp) const
{
    return SireMaths::Torsion( atom(imp.atom0()), atom(imp.atom1()),
                               atom(imp.atom2()), atom(imp.atom3()) );
}
    
/** Return the length of the bond 'bnd'
    
    \throw SireMol::missing_atom
*/
inline double MoleculeDataPvt::measure(const Bond &bnd) const
{
    return bond(bnd).length();
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_atom
*/
inline SireMaths::Angle MoleculeDataPvt::measure(const SireMol::Angle &ang) const
{
    return angle(ang).angle1();
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_atom
*/
inline SireMaths::Angle MoleculeDataPvt::measure(const Dihedral &dih) const
{
    return dihedral(dih).angle();
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_atom
*/
inline SireMaths::Angle MoleculeDataPvt::measure(const Improper &improper) const
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);
        
    return 0.0;
}

/** Return the relative weights of 'group0' and 'group1' using the weight function
    'weightfunc'.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
double MoleculeDataPvt::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1, 
                                  const WeightFunction &weightfunc) const
{
    WeightCalculator calc(weightfunc);
    
    //add all of atoms in the groups to the calculator
    foreach( AtomIndex atm, group0.atoms() )
    {
        calc.addToA( atom(atm) );
    }
    
    foreach( AtomIndex atm, group1.atoms() )
    {
        calc.addToB( atom(atm) );
    }
    
    //now add all of the residues
    foreach( ResNum resnum, group0.residues() )
    {
        calc.addToA( atomVector(resnum) );
    }
    
    foreach( ResNum resnum, group1.residues() )
    {
        calc.addToB( atomVector(resnum) );
    }
    
    return calc.weight();
}

/** Translate the whole molecule by 'delta' */
void MoleculeDataPvt::translate(const Vector &delta)
{
    if (delta.isZero())
        return;
    
    int sz = cgroups.count();
    CutGroup *array = cgroups.data();

    for (int i=0; i<sz; ++i)
    {
        CutGroup &cgroup = array[i];
    
        VectorVector coords = cgroup.coordinates();
        
        int nats = coords.count();
        Vector *coordarray = coords.data();
        for (int j=0; j<nats; ++j)
            coordarray[j] += delta;
            
        cgroup.setCoordinates(coords);
    }
    
    //increment the version number
    incrementMinorVersion();
}

/** Translate the set of atoms in 'atoms' by 'delta' 

    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(const AtomIndexSet &atoms, const Vector &delta)
{
    if (delta.isZero())
        return;
    
    CutGroup *cgarray = cgroups.data();
    
    foreach( AtomIndex atm, atoms )
    {
        CGAtomID id  = molinfo.index(atm);
    
        CutGroup &cgroup = cgarray[id.cutGroupID().index()];
    
        Vector coords = cgroup.coordinates(id.atomID());
        coords += delta;
    
        cgroup.setCoordinates(id.atomID(),coords);
    }
    
    //increment the version number
    incrementMinorVersion();
}

/** Translate all of the atoms in residue 'resnum' whose names are in 
    the string list 'atoms' by 'delta' 
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(ResNum resnum, const QStringList &atoms, 
                                const Vector &delta)
{
    if (delta.isZero())
        return;
    
    ResidueCGInfo resinfo = molinfo.residue(resnum);
    
    CutGroup *cgarray = cgroups.data();
    
    foreach( QString atmname, atoms )
    {
        CGAtomID id = resinfo[atmname];
    
        CutGroup &cgroup = cgarray[id.cutGroupID().index()];
    
        Vector coords = cgroup.coordinates(id.atomID());
        coords += delta;
    
        cgroup.setCoordinates(id.atomID(),coords);
    }
    
    //increment the version number
    incrementMinorVersion();
}

/** Translate all of the atoms in residue 'resnum' by 'delta' 

    \throw SireMol::missing_residue
*/
void MoleculeDataPvt::translate(ResNum resnum, const Vector &delta)
{
    if (delta.isZero())
        return;
        
    ResidueCGInfo resinfo = molinfo.residue(resnum);
    const CGAtomID *indicies = resinfo.indicies().constData();
    CutGroup *cgarray = cgroups.data();
    
    int nats = resinfo.nAtoms();
    for (int i=0; i<nats; ++i)
    {
        CGAtomID id = indicies[i];
        
        CutGroup &cgroup = cgarray[id.cutGroupID().index()];
    
        Vector coords = cgroup.coordinates(id.atomID());
        coords += delta;
    
        cgroup.setCoordinates(id.atomID(),coords);
    }
    
    //increment the version number
    incrementMinorVersion();
}

/** Internal function used to translate an individual atom by 'delta' 

    \throw SireMol::missing_atom
*/
inline void MoleculeDataPvt::translate(const AtomIndex &atm, const Vector &delta)
{
    CGAtomID id = molinfo.index(atm);
    
    CutGroup &cgroup = cgroups[id.cutGroupID().index()];
    
    Vector coords = cgroup.coordinates(id.atomID());
    coords += delta;
    
    cgroup.setCoordinates(id.atomID(),coords);
    
    //increment the version number
    incrementMinorVersion();
}

/** Translate the AtomIDGroup 'group' by 'delta' 

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(const AtomIDGroup &group, const Vector &delta)
{
    if (delta.isZero())
        return;

    //translate all of the residues
    for (ResNumSet::const_iterator it = group.residues().begin(); 
         it != group.residues().end(); ++it)
    {
        this->translate(*it, delta);
    }
    
    //now translate all of the atoms...
    for (AtomIndexSet::const_iterator it = group.atoms().begin();
         it != group.atoms().end(); ++it)
    {
        this->translate(*it, delta);
    }
}
    
/** Rotate the entire molecule by the quaternion 'quat' about the point 'point' */
void MoleculeDataPvt::rotate(const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;
    
    Matrix rotmat = quat.toMatrix();
    
    int sz = cgroups.count();
    CutGroup *cgarray = cgroups.data();
    
    for (int i=0; i<sz; ++i)
    {
        CutGroup &cgroup = cgarray[i];
        
        VectorVector coords = cgroup.coordinates();
        
        int nats = coords.count();
        Vector *coordsarray = coords.data();
        
        for (int j=0; j<nats; ++j)
        {
            Vector &coord = coordsarray[j];
            coord = SireMaths::rotate(coord,rotmat,point);
        }
        
        cgroup.setCoordinates(coords);
    }
    
    //increment the version number
    incrementMinorVersion();
}

/** Rotate the atom 'atm' by the rotation matrix 'rotmat' about the point 'point'

    \throw SireMol::missing_atom
*/
inline void MoleculeDataPvt::rotate(const AtomIndex &atm, 
                                    const Matrix &rotmat, const Vector &point)
{
    CGAtomID id = molinfo.index(atm);
    
    CutGroup &cgroup = cgroups[id.cutGroupID().index()];
    
    Vector coords = cgroup.coordinates(id.atomID());
    
    coords = SireMaths::rotate(coords,rotmat,point);
    
    cgroup.setCoordinates(id.atomID(),coords);
    
    //increment the version number
    incrementMinorVersion();
}

/** Rotate the atoms in residue 'resnum' whose names are in 'atoms' by the 
    rotation matrix 'rotmat' about the point 'point' 
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
inline void MoleculeDataPvt::rotate(ResNum resnum, const QStringList &atoms, 
                                    const Matrix &rotmat, const Vector &point)
{
    ResidueCGInfo resinfo = molinfo.residue(resnum);
    
    CutGroup *cgarray = cgroups.data();
    
    foreach( QString atmname, atoms )
    {
        CGAtomID id = resinfo[atmname];
    
        CutGroup &cgroup = cgarray[id.cutGroupID().index()];
    
        Vector coords = cgroup.coordinates(id.atomID());
        coords = SireMaths::rotate(coords,rotmat,point);
    
        cgroup.setCoordinates(id.atomID(),coords);
    }
    
    //increment the version number
    incrementMinorVersion();
}

/** Rotate the residue 'resnum' by the rotation matrix 'rotmat' about the 
    point 'point'
    
    \throw SireMol::missing_residue
*/
inline void MoleculeDataPvt::rotate(ResNum resnum, 
                                    const Matrix &rotmat, const Vector &point)
{
    ResidueCGInfo resinfo = molinfo.residue(resnum);
    const CGAtomID *indicies = resinfo.indicies().constData();
    CutGroup *cgarray = cgroups.data();
    
    int nats = resinfo.nAtoms();
    for (int i=0; i<nats; ++i)
    {
        CGAtomID id = indicies[i];
        
        CutGroup &cgroup = cgarray[id.cutGroupID().index()];
    
        Vector coords = cgroup.coordinates(id.atomID());
        coords = SireMaths::rotate(coords,rotmat,point);
    
        cgroup.setCoordinates(id.atomID(),coords);
    }
    
    //increment the version number
    incrementMinorVersion();
}

/** Rotate the atoms in residue 'resnum' whose names are in 'atoms'
    by the quaternion 'quat' about the point 'point' 

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::rotate(ResNum resnum, const QStringList &atoms, 
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;
        
    Matrix rotmat = quat.toMatrix();
    rotate(resnum, atoms, rotmat, point);
}

/** Rotate the atoms in residue 'resnum' by the quaternion 'quat' about the point 'point' 

    \throw SireMol::missing_residue
*/
void MoleculeDataPvt::rotate(ResNum resnum, 
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;
        
    Matrix rotmat = quat.toMatrix();
    rotate(resnum, rotmat, point);
}

/** Rotate the group 'group' by the quaternion 'quat' about the point 'point' 

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::rotate(const AtomIDGroup &group, 
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    Matrix rotmat = quat.toMatrix();

    //translate all of the residues
    for (ResNumSet::const_iterator it = group.residues().begin(); 
         it != group.residues().end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }
    
    //now translate all of the atoms...
    for (AtomIndexSet::const_iterator it = group.atoms().begin();
         it != group.atoms().end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }
}

/** Rotate the atoms in 'atoms' by the quaternion 'quat' about the point 'point' 

    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::rotate(const AtomIndexSet &atoms, 
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;
        
    Matrix rotmat = quat.toMatrix();
    
    for( AtomIndexSet::const_iterator it = atoms.begin();
         it != atoms.end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }
}

/** Translate the atoms in group0 and group1 along the vector of the bond 'bnd', ensuring
    weighting the translation of the two sides by 'weight' and ensuring that the 
    atoms in 'anchors' are not moved. 
    
    This will throw an exception if either the atoms in 'bnd' or any of the atoms in 
    group0 or group1 are not in this molecule.
    
    An exception will also be thrown if there are anchor atoms in both group0 and group1.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeDataPvt::change(const Bond &bnd, double delta, 
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //get the anchor status
    bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
    bool anchor1 = group1.isEmpty() or group1.intersects(anchors);
        
    double weight;
        
    if (anchor0 and anchor1)
        //this bond is immovable
        throw SireMol::anchor_error(QObject::tr(
            "Cannot change bond \"%1\" as both groups are either anchored or empty.\n%2\n%3")
                .arg(bnd.toString(),group0.inventory(),group1.inventory()), CODELOC);
    else if (anchor0)
        weight = 0.0;
    else if (anchor1)
        weight = 1.0;
    else
        weight = getWeight(group0, group1, weightfunc);
            
    //get the vector of the bond...
    SireMaths::Line line = this->bond(bnd);
    Vector bondvec = line.vector();
    
    if (bondvec.isZero())
        //if we have a zero bondvector, then translate along the x axis
        bondvec = Vector(1.0,0.0,0.0);
    else
        bondvec = bondvec.normalise();
            
    //translate group0 by (weight-1) * delta * bondvec
    this->translate(group0, (weight-1.0)*delta*bondvec);
    
    //translate group1 by weight * delta * bondvec
    this->translate(group1, weight*delta*bondvec);    
}
    
/** Rotate the atoms in group0 and group1 around the angle 'ang', ensuring
    weighting the rotation of the two sides by 'weight' and ensuring that the 
    atoms in 'anchors' are not moved. 
    
    This will throw an exception if any of the atoms in 'ang' or any of the atoms in 
    group0 or group1 are not in this molecule.
    
    An exception will also be thrown if there are anchor atoms in both group0 and group1.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeDataPvt::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //get the anchor status - this will override the passed weight
    bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
    bool anchor1 = group1.isEmpty() or group1.intersects(anchors);
        
    double weight;
        
    if (anchor0 and anchor1)
        //this bond is immovable
        throw SireMol::anchor_error(QObject::tr(
            "Cannot change angle \"%1\" as both groups are either anchored or empty.\n%2\n%3")
                .arg(ang.toString(),group0.inventory(),group1.inventory()), CODELOC);
    else if (anchor0)
        weight = 0.0;
    else if (anchor1)
        weight = 1.0;
    else
        weight = getWeight(group0, group1, weightfunc);
            
    //get the geometry of the angle...
    SireMaths::Triangle trig = angle(ang);
    Vector angvec = trig.vector();
    
    if (angvec.isZero())
        //if we have a zero angle vector, then rotate about the z axis
        angvec = Vector(0.0,0.0,1.0);
    else
        angvec = angvec.normalise();
            
    //rotate group0 by (weight-1)*delta around angvec, about the central atom
    //of the angle
    this->rotate( group0, Quaternion((weight-1.0)*delta,angvec), trig[1] );
    
    //rotate group1 by weight*delta around angvec, about the central atom of the angle
    this->rotate( group1, Quaternion(weight*delta,angvec), trig[1] );
}
    
/** Rotate the atoms in group0 and group1 around the bond 'dih', ensuring
    weighting the rotation of the two sides by 'weight' and ensuring that the 
    atoms in 'anchors' are not moved. 
    
    This will throw an exception if any of the atoms in 'dih' or any of the atoms in 
    group0 or group1 are not in this molecule.
    
    An exception will also be thrown if there are anchor atoms in both group0 and group1.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeDataPvt::change(const Bond &dih, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //get the anchor status - this will override the passed weight
    bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
    bool anchor1 = group1.isEmpty() or group1.intersects(anchors);
        
    double weight;
        
    if (anchor0 and anchor1)
        //this bond is immovable
        throw SireMol::anchor_error(QObject::tr(
            "Cannot change dihedral centered about \"%1\" as both groups are "
            "either anchored or empty.\n%2\n%3")
                .arg(dih.toString(),group0.inventory(),group1.inventory()), CODELOC);
    else if (anchor0)
        weight = 0.0;
    else if (anchor1)
        weight = 1.0;
    else
        weight = getWeight(group0, group1, weightfunc);
            
    //get the geometry of the dihedral...
    SireMaths::Line line = bond(dih);
    Vector dihvec = line.vector();
    
    if (dihvec.isZero())
        //if we have a zero dihedral vector, then rotate about the x axis
        dihvec = Vector(1.0,0.0,0.0);
    else
        dihvec = dihvec.normalise();
            
    //rotate group0 by (weight-1)*delta around dihvec, about the first atom of the bond
    this->rotate( group0, Quaternion((weight-1.0)*delta,dihvec), line[0] );
    
    //rotate group1 by weight*delta around dihvec, about the second atom of the bond
    this->rotate( group1, Quaternion(weight*delta,dihvec), line[1] );
}
                          
/** Rotate the atoms in group0 and group1 around the improper angle 'improper', ensuring
    weighting the rotation of the two sides by 'weight' and ensuring that the 
    atoms in 'anchors' are not moved. 
    
    This will throw an exception if any of the atoms in 'improper' or any of the atoms in 
    group0 or group1 are not in this molecule.
    
    An exception will also be thrown if there are anchor atoms in both group0 and group1.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeDataPvt::change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);
}

////////////
//////////// Implementation of MoleculeData
////////////

/** Serialise the molecule data to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const MoleculeData &mol)
{
    ds << mol.d();
    return ds;
}

/** Deserialise the molecule data from a binary datastream - this will detach
    this data from any other shared copies before reading in the data. */
QDataStream& operator>>(QDataStream &ds, MoleculeData &mol)
{
    mol.detach();
    ds >> mol.d();
    return ds;
}

/** Construct a new, empty molecule. */
MoleculeData::MoleculeData() 
             : ptr( new QSharedDataPointer<MoleculeDataPvt>(new MoleculeDataPvt()) )
{}

/** Construct the MoleculeData for a molecule called 'molname', with identified residues
    in 'resids', atoms arranged into the CutGroups in 'cutgroups', and molecular bonding
    described in 'bonds'. Exceptions will be thrown if either the same atom appears in
    two different CutGroups, or if a bond is passed that refers to an atom that isn't
    in the CutGroups, or if an atom refers to a residue that is not in 'resids'. 
    
    \throw SireMol::duplicate_atom
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
MoleculeData::MoleculeData(const QString &molname, const ResidueIDSet &resids,
                           const CutGroupVector &cutgroups, const MoleculeBonds &bonds)
             : ptr( new QSharedDataPointer<MoleculeDataPvt>(
                          new MoleculeDataPvt(molname,resids,cutgroups,bonds)) )
{}

/** Shallow copy constructor. This makes a shallow copy of the molecule data. This may
    not be what you expect so beware. If you wish to get your own personal copy, then
    either use this constructor with 'deepCopy()' or use the 'detach()' function. */
MoleculeData::MoleculeData(const MoleculeData &other) : ptr(other.ptr)
{}

/** Destructor */
MoleculeData::~MoleculeData()
{}

/** Shallow copy operator */
MoleculeData& MoleculeData::operator=(const MoleculeData &other)
{
    ptr = other.ptr;
    return *this;
}

/** Private function used to easily retrieve a const reference to the 
    underlying MoleculeDataPvt object */
const MoleculeDataPvt& MoleculeData::d() const
{
    return *(*ptr);
}

/** Private function used to easily retrieve a reference to the 
    underlying MoleculeDataPvt object */
MoleculeDataPvt& MoleculeData::d()
{
    return *(*ptr);
}

/** Comparison operator - two molecules are equal if their molecule IDs are 
    the same (even if the actual data is different) */
bool MoleculeData::operator==(const MoleculeData &other) const
{
    return d() == other.d();
}

/** Comparison operator - two molecules are different if they have different
    molecule ID numbers. */
bool MoleculeData::operator!=(const MoleculeData &other) const
{
    return d() != other.d();
}

/** Detach this MoleculeData from the shared data storage. Now any changes
    to this data will not affect any other copies */
void MoleculeData::detach()
{
    ptr.reset( new QSharedDataPointer<MoleculeDataPvt>(*ptr) );
}
  
/** Return a deep copy of this MoleculeData. This is an exact copy
    of this MoleculeData, but changing it will not change this copy. */
MoleculeData MoleculeData::deepCopy() const
{
    MoleculeData mol(*this);
    mol.detach();
    return mol;
}

/** Return a shallow copy of this MoleculeData - modifying the returned
    copy will also modify this original */
MoleculeData MoleculeData::shallowCopy() const
{
    return MoleculeData(*this);
}
    
/** Return a Molecule view of this data - this is a shallow copy. Modifying 
    this molecule will change this data. */
Molecule MoleculeData::molecule()
{
    return Molecule(*this);
}

/** Return a Residue view of this data - this is a shallow copy. Modifying
    this residue will change this data. 
    
    \throw SireMol::missing_residue
*/
Residue MoleculeData::residue(ResNum resnum)
{
    d().checkResidue(resnum);
    
    return Residue(*this, resnum);
}

/** Return a Residue view of the first residue called 'resname'.

    \throw SireMol::missing_residue
*/
Residue MoleculeData::residue(const QString &resname)
{
    ResNumList resnums = d().residueNumbers(resname);
    
    if (resnums.count() == 0)
        throw SireMol::missing_residue(QObject::tr(
            "There is no residue called \"%1\" in molecule \"%2\"")
                .arg(resname, name()), CODELOC);

    return residue(resnums[0]);
}
    
/** Return the set of all Residues in the molecule. The residues are 
    all shallow copies on this MoleculeData. Thus changing the residues
    can change this data, and may change the other residues in the set. */
ResidueSet MoleculeData::residues()
{
    ResNumList resnums = d().residueNumbers();
    
    ResidueSet residus;
    foreach( ResNum resnum, resnums )
        residus.insert( Residue(*this, resnum) );

    return residus;
}

/** Return a Residue view of the ith residue in the molecule.

    \throw SireError::invalid_index
*/
Residue MoleculeData::at(int i)
{
    ResNumList resnums = d().residueNumbers();
    
    if (i < 0 or i >= resnums.count())
        throw SireError::invalid_index(QObject::tr(
            "Index error accessing index %1 for molecule \"%2\" (nResidues == %3)")
                .arg(i).arg(name()).arg(resnums.count()), CODELOC);

    return residue(resnums[0]);
}

/** Return the ID number of this molecule */
MoleculeID MoleculeData::ID() const
{
    return d().ID();
}
    
/** Give this Molecule a new ID number. This detachs the molecule from shared
    storage. */
void MoleculeData::setNewID()
{
    this->detach();
    d().setNewID();
}

/** Return the version number of this molecule */
const MoleculeVersion& MoleculeData::version() const
{
    return d().version();
}

/** Increment the major version number of this molecule - this does not
    detach the molecule from shared storage (e.g. it increases the version
    number for the molecule and any residue views) */
void MoleculeData::incrementMajorVersion()
{
    d().incrementMajorVersion();
}

/** Increment the minor version number of this molecule - this does not
    detach the molecule from shared storage (e.g. it increases the version
    number for the molecule and any residue views) */
void MoleculeData::incrementMinorVersion()
{
    d().incrementMinorVersion();
}

/** Return a clone of this Molecule. A clone is a Molecule that has the same data,
    but has a different ID number, and different ID numbers for any contained
    objects, e.g. CutGroups */
MoleculeData MoleculeData::clone() const
{
    MoleculeData mol(*this);
    mol.setNewID();
    
    return mol;
}
    
/** Return the connectivity of the molecule */
MoleculeBonds MoleculeData::connectivity() const
{
    return d().connectivity();
}

/** Return the connectivity of the residue 'resnum' */
ResidueBonds MoleculeData::connectivity(ResNum resnum) const
{
    return d().connectivity(resnum);
}
    
/** Return a const reference to the moleculeinfo for this molecule */
const MoleculeCGInfo& MoleculeData::info() const
{
    return d().info();
}

/** Return a signature for the molecule. This can be used to compare
    this molecule with another (or with an EditMol) to see if the
    two objects describe the same molecules (i.e. contain the same
    residues, atoms and bonds) */
MoleculeSignature MoleculeData::signature() const
{
    return d().signature();
}

/** Return a set of all of the atoms in residue 'resnum'
    
    \throw SireMol::missing_residue
*/
AtomSet MoleculeData::atoms(ResNum resnum) const
{
    return d().atoms(resnum);
}

/** Return a set of all atoms in the molecule */
AtomSet MoleculeData::atoms() const
{
    return d().atoms();
}
    
/** Return a vector of all of the atoms in residue 'resnum'
    
    \throw SireMol::missing_residue
*/
AtomVector MoleculeData::atomVector(ResNum resnum) const
{
    return d().atomVector(resnum);
}

/** Return a vector of all atoms in the molecule */
AtomVector MoleculeData::atomVector() const
{
    return d().atomVector();
}

/** Return the set of all CutGroups in the molecule */
CutGroupSet MoleculeData::cutGroups() const
{
    return d().cutGroups();
}
    
/** Return the set of all CutGroups that contain atoms that are in
    residue 'resnum'. If there are no CutGroups with atoms in this
    residue, or this residue does not exist, then an empty
    set is returned */
CutGroupSet MoleculeData::cutGroups(ResNum resnum) const
{
    return d().cutGroups(resnum);
}
    
/** Return the CutGroup with ID == id

    \throw SireMol::missing_cutgroup
*/
const CutGroup& MoleculeData::cutGroup(CutGroupID id) const
{
    return d().cutGroup(id);
}
 
/** Return the atom in CutGroup with ID == id

    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
*/
const Atom& MoleculeData::atom(const CGAtomID &id) const
{
    return d().atom(id);
}
    
/** Return the name of the molecule */
const QString& MoleculeData::name() const
{
    return d().name();
}

/** Return the name of residue 'resnum'

    \throw SireMol::missing_residue
*/
QString MoleculeData::residueName(ResNum resnum) const
{
    return d().residueName(resnum);
}
    
/** Return whether or not this is an empty molecule (contains no atoms or bonds) */
bool MoleculeData::isEmpty() const
{
    return d().isEmpty();
}

/** Return whether the residue 'resnum' is empty
    
    \throw SireMol::missing_residue
*/
bool MoleculeData::isEmpty(ResNum resnum) const
{
    return d().isEmpty(resnum);
}
    
/** Return the list of residue numbers in this molecule */
ResNumList MoleculeData::residueNumbers() const
{
    return d().residueNumbers();
}

/** Return the list of residue names in this molecule */
QStringList MoleculeData::residueNames() const
{
    return d().residueNames();
}
    
/** Return the list of residue numbers of residues called 'resname'. This 
    returns an empty list if there are no residues called 'resname'. */    
ResNumList MoleculeData::residueNumbers(const QString &resnam) const
{
    return d().residueNumbers(resnam);
}

/** Return the list of residues that are bonded to residue 'resnum'. This returns
    an empty list if there are no other residues bonded to residue 'resnum'
    
    \throw SireMol::missing_residue
*/
ResidueSet MoleculeData::residuesBondedTo(ResNum resnum)
{
    d().checkResidue(resnum);
    ResNumList resnums = connectivity().resNumsBondedTo(resnum);
    
    ResidueSet residus;
    foreach( ResNum rnum,  resnums )
        residus.insert( Residue(*this, rnum) );
        
    return residus;
}
    
/** Return a const reference to the atom with index 'atm'
    
    \throw SireMol::missing_atom
*/
const Atom& MoleculeData::atom(const AtomIndex &atm) const
{
    return d().atom(atm);
}

/** Return whether or not the molecule contains a residue with number 'resnum' */
bool MoleculeData::contains(ResNum resnum) const
{
    return d().contains(resnum);
}

/** Return whether or not the molecule contains an atom with index 'atm' */
bool MoleculeData::contains(const AtomIndex &atm) const
{
    return d().contains(atm);
}

/** Return whether or not the molecule contains the bond 'bond' */
bool MoleculeData::contains(const Bond &bond) const
{
    return d().contains(bond);
}
    
/** Return the ith atom of residue 'resnum' 

    \throw SireMol::missing_residue
    \throw SireMol::invalid_index
*/
const Atom& MoleculeData::at(ResNum resnum, int i) const
{
    return d().at(resnum,i);
}
    
/** Return the total number of atoms in the molecule */
int MoleculeData::nAtoms() const
{
    return d().nAtoms();
}

/** Return the total number of atoms in residue 'resnum' 

    \throw SireMol::missing_residue
*/
int MoleculeData::nAtoms(ResNum resnum) const
{
    return d().nAtoms(resnum);
}
    
/** Return the total number of atoms in CutGroup 'id' */
int MoleculeData::nAtoms(CutGroupID id) const
{
    return d().nAtoms(id);
}
    
/** Return the total number of residues in the molecule */
int MoleculeData::nResidues() const
{
    return d().nResidues();
}

/** Return the total number of CutGroups in the molecule */
int MoleculeData::nCutGroups() const
{
    return d().nCutGroups();
}
    
/** Return the list of atom names in residue 'resnum' 

    \throw SireMol::missing_residue
*/
QStringList MoleculeData::atomNames(ResNum resnum) const
{
    return d().atomNames(resnum);
}
    
/** Check that the residue with number 'resnum' exists - if not, then 
    throw an exception.
    
    \throw SireMol::missing_residue
*/
void MoleculeData::checkResidue(ResNum resnum) const
{
    d().checkResidue(resnum);
}
    
/** Return the geometric line representing the bond 'bnd'

    \throw SireMol::missing_atom
*/
SireMaths::Line MoleculeData::bond(const Bond &bnd) const
{
    return d().bond(bnd);
}

/** Return the geometric triangle representing the angle 'ang' 

    \throw SireMol::missing_atom
*/
SireMaths::Triangle MoleculeData::angle(const SireMol::Angle &ang) const
{
    return d().angle(ang);
}

/** Return the geometric torsion representing the dihedral 'dih' 

    \throw SireMol::missing_atom
*/
SireMaths::Torsion MoleculeData::dihedral(const Dihedral &dih) const
{
    return d().dihedral(dih);
}

/** Return the geometric torsion representing the improper angle 'imp' 

    \throw SireMol::missing_atom
*/
SireMaths::Torsion MoleculeData::improper(const Improper &imp) const
{
    return d().improper(imp);
}

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_atom
*/
double MoleculeData::measure(const Bond &bnd) const
{
    return d().measure(bnd);
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const SireMol::Angle &ang) const
{
    return d().measure(ang);
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const Dihedral &dih) const
{
    return d().measure(dih);
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const Improper &imp) const
{
    return d().measure(imp);
}
   
/** Return the relative weight of the two atom groups 'group0' and 'group1' using 
    the supplied weight function
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
double MoleculeData::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1, 
                               const WeightFunction &weightfunc) const
{
    return d().getWeight(group0,group1,weightfunc);
}
   
/** Translate the whole molecule by 'delta' */
void MoleculeData::translate(const Vector &delta)
{
    d().translate(delta);
}

/** Translate the atom group 'group' by 'delta'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::translate(const AtomIDGroup &group, const Vector &delta)
{
    d().translate(group,delta);
}

/** Translate the atoms whose indexes are in 'atoms' by 'delta'

    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const AtomIndexSet &atoms, const Vector &delta)
{
    d().translate(atoms,delta);
}

/** Translate atoms whose names are in 'atoms' in residue 'resnum' by 'delta'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
{
    d().translate(resnum,atoms,delta);
}

/** Translate the whole of the residue 'resnum' by 'delta' 

    \throw SireMol::missing_residue
*/
void MoleculeData::translate(ResNum resnum, const Vector &delta)
{
    d().translate(resnum,delta);
}

/** Rotate the whole molecule by the quaternion 'quat' about the point 'point' */
void MoleculeData::rotate(const Quaternion &quat, const Vector &point)
{
    d().rotate(quat,point);
}

/** Rotate the atoms in 'group' by 'quat' about 'point'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
{
    d().rotate(group,quat,point);
}

/** Rotate the atoms whose indexes are in 'atoms' by 'quat' about 'point' 

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIndexSet &atoms, const Quaternion &quat, const Vector &point)
{
    d().rotate(atoms,quat,point);
}

/** Rotate the atoms whose names are in 'atoms' and that are in residue 'resnum' by 
    'quat' about 'point' 
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                          const Vector &point)
{
    d().rotate(resnum,atoms,quat,point);
}

/** Rotate the whole of residue 'resnum' by 'quat' about 'point'

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
{
    d().rotate(resnum,quat,point);
}
   
/** Translate 'group0' and 'group1' by 'delta' along the vector of bond 'bnd', weighting the motion
    using the weight function 'weightfunc' and ensuring that the atoms in 'anchors' are
    anchored.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const Bond &bnd, double delta, 
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(bnd,delta,group0,group1,weightfunc,anchors);
}
    
/** Rotate 'group0' and 'group1' by 'delta' about the vector perpendicular to 'ang' and 
    about the point of atom1 in the angle, with the motion weighted using 'weightfunc' and 
    ensuring that the atoms in 'anchors' are anchored.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(ang,delta,group0,group1,weightfunc,anchors);
}
    
/** Rotate 'group0' and 'group1' by delta about the vector of the bond 'bnd' and 
    the point in the centre of the vector, with the motion weighted using 'weightfunc'
    and ensuring that the atoms in 'anchors' are anchored.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const Bond &bnd, const SireMaths::Angle &delta,
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(bnd,delta,group0,group1,weightfunc,anchors);
}

/** Rotate 'group0' and 'group1' about the improper 'imp' by 'delta', weighting the
    motion of the two groups by 'weightfunc' and ensuring that the atoms in 
    'anchors' are anchored.
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const Improper &imp, const SireMaths::Angle &delta,
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(imp,delta,group0,group1,weightfunc,anchors);
}
