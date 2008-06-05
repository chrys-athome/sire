
#include "SireMol/qhash_siremol.h"
#include "qhash_siremm.h"

#include "moldihedralinfo.h"
#include "resdihedralinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<MolDihedralInfo> r_moldihinfo("SireMM::MolDihedralInfo");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const MolDihedralInfo &info)
{
    writeHeader(ds, r_moldihinfo, 1) 
          << static_cast<const MolInternalInfo<Dihedral>&>(info);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, MolDihedralInfo &info)
{
    VersionID v = readHeader(ds, r_moldihinfo);
    
    if (v == 1)
    {
        ds >> static_cast<MolInternalInfo<Dihedral>&>(info);
    }
    else
        throw version_error(v, "1", r_moldihinfo, CODELOC);
    
    return ds;
}

/** Empty constructor */
MolDihedralInfo::MolDihedralInfo() : MolInternalInfo<Dihedral>()
{}

/** Construct a MolDihedralInfo that holds the dihedrals for the molecule 'mol'.
    Note that this will not initially contain any dihedrals - these need to be
    added separately. */
MolDihedralInfo::MolDihedralInfo(const Molecule &mol) : MolInternalInfo<Dihedral>(mol)
{}

/** Construct a MolDihedralInfo that holds the dihedrals for the molecule 'mol',
    with the DihedralGenerator 'dihedralgenerator' used to find all of the available
    dihedrals in the molecule. */
MolDihedralInfo::MolDihedralInfo(const Molecule &mol, 
                         const InternalGenerator<MolDihedralInfo> &dihedralgenerator)
            : MolInternalInfo<Dihedral>(mol)
{
    //generate all of the dihedrals using the dihedral generator
    dihedralgenerator.generate(mol, *this);
}

/** Copy constructor */
MolDihedralInfo::MolDihedralInfo(const MolDihedralInfo &other) 
                : MolInternalInfo<Dihedral>(other)
{}

/** Destructor */
MolDihedralInfo::~MolDihedralInfo()
{}

/** Add an dihedral to the molecule - this does nothing if the
    dihedral already exists in the molecule. Returns the GroupIndexID
    of the dihedral */
GroupIndexID MolDihedralInfo::addDihedral(const Dihedral &dihedral)
{
    return addInternal(dihedral);
}

/** Remove an dihedral from the molecule - does nothing if the
    dihedral is not in the molecule. This function may be slow
    as it may trigger a reindexing of the dihedrals */
void MolDihedralInfo::removeDihedral(const Dihedral &dihedral)
{
    removeInternal(dihedral);
}

/** Remove a dihedral from the molecule that has index 'index'.
    This does nothing if there is no such dihedral. */
void MolDihedralInfo::removeDihedral(const GroupIndexID &groupid)
{
    removeInternal(groupid);
}

/** Return the number of dihedrals in the molecule */
int MolDihedralInfo::nDihedrals() const
{
    return nInternals();
}

/** Return the number of dihedrals in the group with GroupID 'group' */
int MolDihedralInfo::nDihedrals(GroupID group) const
{
    return nInternals(group);
}

/** Return the number of dihedrals in the residue with number 'resnum' */
int MolDihedralInfo::nDihedrals(ResNum resnum) const
{
    return nInternals(resnum);
}

/** Return the number of dihedrals that involve both 'res0' and 'res1'.
    This function may be slow as it requires a more complicated
    search than the other functions (as the third residue in the 
    dihedral is not defined)
*/
int MolDihedralInfo::nDihedrals(ResNum res0, ResNum res1) const
{
    if (res0 == res1)
        return nDihedrals(res0);
    else
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        
        return nCommonInternals(resnums);
    }
}

/** Return the number of dihedrals that involve all of res0, res1 and res2 */
int MolDihedralInfo::nDihedrals(ResNum res0, ResNum res1, ResNum res2) const
{
    if (res0 == res1 and res0 == res2)
        return nDihedrals(res0);
    else
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        resnums.insert(res2);
        
        return nCommonInternals(resnums);
    }
}

/** Return the number of dihedrals that involve all of res0, res1, res2 and res3 */
int MolDihedralInfo::nDihedrals(ResNum res0, ResNum res1, 
                                ResNum res2, ResNum res3) const
{
    if (res0 == res1 and res0 == res2 and res0 == res3)
        return nDihedrals(res0);
    else if (res0 == res1 or res0 == res2 or res0 == res3 or
             res1 == res2 or res1 == res3 or res2 == res3)
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        resnums.insert(res2);
        resnums.insert(res3);
        
        return nCommonInternals(resnums);
    }
    else
        return nInternals( DihedralResID(res0,res1,res2,res3) );
}

/** Return the total number of inter-residue dihedrals in the molecule */
int MolDihedralInfo::nInterDihedrals() const
{
    return nInterInternals();
}

/** Return the total number if intra-residue dihedrals in the molecule. */
int MolDihedralInfo::nIntraDihedrals() const
{
    return nIntraInternals();
}

/** Return the total number of intra-residue dihedrals in the 
    residue with number 'resnum' */
int MolDihedralInfo::nIntraDihedrals(ResNum resnum) const
{
    return nIntraDihedrals(resnum);
}

/** Return the total number of inter-residue dihedrals in the 
    residue with number 'resnum' */
int MolDihedralInfo::nInterDihedrals(ResNum resnum) const
{
    return nInterInternals(resnum);
}

/** Return whether or not there is an dihedral that connects residues 
    res0 and res1. This may be slow as it requires a more complete
    search as the dihedral is not totally defined. */
bool MolDihedralInfo::residuesDihedraled(ResNum res0, ResNum res1) const
{
    QSet<ResNum> resnums;
    resnums.insert(res0);
    resnums.insert(res1);
    
    return hasCommonInternal(resnums);
}

/** Return whether or not there is an dihedral that connects residues 
    res0, res1 and res2. This may be slow as it requires a more complete
    search as the dihedral is not totally defined. */
bool MolDihedralInfo::residuesDihedraled(ResNum res0, ResNum res1, ResNum res2) const
{
    QSet<ResNum> resnums;
    resnums.insert(res0);
    resnums.insert(res1);
    resnums.insert(res2);
    
    return hasCommonInternal(resnums);
}

/** Return whether or not there is a single dihedral that encompasses 
    residues res0, res1, res2 and res3 */
bool MolDihedralInfo::residuesDihedraled(ResNum res0, ResNum res1, 
                                         ResNum res2, ResNum res3) const
{
    return contains( DihedralResID(res0,res1,res2,res3) );
}

/** Return an iterator over all of the dihedrals in the molecule.
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::dihedrals() const
{
    return internals();
}

/** Return an iterator over all of the dihedrals in the residue 'resnum'.
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::dihedrals(ResNum resnum) const
{
    return internals(resnum);
}

/** Return an iterator over all of the dihedrals that contain 
    both of the residues res0 and res1. 
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::dihedrals(ResNum res0, ResNum res1) const
{
    if (res0 == res1)
        return dihedrals(res0);
    else
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
    
        return commonInternals(resnums);
    }
}

/** Return an iterator over all of the dihedrals that contain 
    all of the residues res0, res1 and res2. 
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::dihedrals(ResNum res0, ResNum res1,
                                                           ResNum res2) const
{
    if (res0 == res1 and res0 == res2)
        return dihedrals(res0);
    else
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        resnums.insert(res2);
        
        return commonInternals(resnums);
    }
}

/** Return an iterator over all of the dihedrals that contain 
    all of the residues res0, res1, res2 and res3. 
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::dihedrals(ResNum res0, ResNum res1, 
                                                           ResNum res2, ResNum res3) const
{
    if (res0 == res1 and res0 == res2 and res0 == res3)
        return dihedrals(res0);
    else if (res0 == res1 or res0 == res2 or res0 == res3 or 
             res1 == res2 or res1 == res3 or res2 == res3)
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        resnums.insert(res2);
        resnums.insert(res3);
        
        return commonInternals(resnums);
    }
    else
        return internals( DihedralResID(res0,res1,res2,res3) );
}

/** Return an iterator over all of the intra-residue dihedrals in the 
    molecule.
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::intraDihedrals() const
{
    return intraInternals();
}

/** Return an iterator over all of the inter-residue dihedrals in the 
    molecule.
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::interDihedrals() const
{
    return interInternals();
}

/** Return an iterator over all of the intra-residue dihedrals in the 
    residue 'resnum'.
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::intraDihedrals(ResNum resnum) const
{
    return intraInternals(resnum);
}

/** Return an iterator over all of the inter-residue dihedrals in the 
    residue 'resnum'.
    
    This returns an iterator pointing to the first dihedral, or an 
    invalid iterator if there are no dihedrals to iterate over.
*/
MolDihedralInfo::const_iterator MolDihedralInfo::interDihedrals(ResNum resnum) const
{
    return interInternals(resnum);
}

/** Return the metainfo about the dihedrals in residue 'resnum'. This 
    returns an empty object if there are no dihedrals recorded for 
    this residue. */
ResDihedralInfo MolDihedralInfo::residue(ResNum resnum) const
{
    return extractResidue(resnum);
}
