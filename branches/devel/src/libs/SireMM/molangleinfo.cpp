
#include "SireMol/qhash_siremol.h"
#include "qhash_siremm.h"

#include "molangleinfo.h"
#include "resangleinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<MolAngleInfo> r_molanginfo("SireMM::MolAngleInfo");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const MolAngleInfo &info)
{
    writeHeader(ds, r_molanginfo, 1) 
          << static_cast<const MolInternalInfo<Angle>&>(info);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, MolAngleInfo &info)
{
    VersionID v = readHeader(ds, r_molanginfo);
    
    if (v == 1)
    {
        ds >> static_cast<MolInternalInfo<Angle>&>(info);
    }
    else
        throw version_error(v, "1", r_molanginfo, CODELOC);
    
    return ds;
}

/** Empty constructor */
MolAngleInfo::MolAngleInfo() : MolInternalInfo<Angle>()
{}

/** Construct a MolAngleInfo that holds the angles for the molecule 'mol'.
    Note that this will not initially contain any angles - these need to be
    added separately. */
MolAngleInfo::MolAngleInfo(const MoleculeInfo &molinfo) : MolInternalInfo<Angle>(molinfo)
{}

/** Construct a MolAngleInfo that holds the angles for the molecule 'mol',
    with the AngleGenerator 'anglegenerator' used to find all of the available
    angles in the molecule. */
MolAngleInfo::MolAngleInfo(const Molecule &mol, 
                           const InternalGenerator<MolAngleInfo> &anglegenerator)
            : MolInternalInfo<Angle>(mol.info())
{
    //generate all of the angles using the angle generator
    anglegenerator.generate(mol, *this);
}

/** Copy constructor */
MolAngleInfo::MolAngleInfo(const MolAngleInfo &other) : MolInternalInfo<Angle>(other)
{}

/** Destructor */
MolAngleInfo::~MolAngleInfo()
{}

/** Add an angle to the molecule - this does nothing if the
    angle already exists in the molecule. Returns the GroupIndexID
    of the angle */
GroupIndexID MolAngleInfo::addAngle(const Angle &angle)
{
    return addInternal(angle);
}

/** Remove an angle from the molecule - does nothing if the
    angle is not in the molecule. This function may be slow
    as it may trigger a reindexing of the angles */
void MolAngleInfo::removeAngle(const Angle &angle)
{
    removeInternal(angle);
}

/** Remove an angle from the molecule that has index 'index'.
    This does nothing if there is no such angle. */
void MolAngleInfo::removeAngle(const GroupIndexID &groupid)
{
    removeInternal(groupid);
}

/** Return the number of angles in the molecule */
int MolAngleInfo::nAngles() const
{
    return nInternals();
}

/** Return the number of angles in the group with GroupID 'group' */
int MolAngleInfo::nAngles(GroupID group) const
{
    return nInternals(group);
}

/** Return the number of angles in the residue with number 'resnum' */
int MolAngleInfo::nAngles(ResNum resnum) const
{
    return nInternals(resnum);
}

/** Return the number of angles that involve both 'res0' and 'res1'.
    This function may be slow as it requires a more complicated
    search than the other functions (as the third residue in the 
    angle is not defined)
*/
int MolAngleInfo::nAngles(ResNum res0, ResNum res1) const
{
    if (res0 == res1)
        return nAngles(res0);
    else
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        
        return nCommonInternals(resnums);
    }
}

/** Return the number of angles that involve all of res0, res1 and res2 */
int MolAngleInfo::nAngles(ResNum res0, ResNum res1, ResNum res2) const
{
    if (res0 == res1 and res0 == res2)
        return nAngles(res0);
    else if (res0 == res1 or res0 == res1 or res1 == res2)
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        resnums.insert(res2);
        
        return nCommonInternals(resnums);
    }
    else
        return nInternals( AngleResID(res0,res1,res2) );
}

/** Return the total number of inter-residue angles in the molecule */
int MolAngleInfo::nInterAngles() const
{
    return nInterInternals();
}

/** Return the total number if intra-residue angles in the molecule. */
int MolAngleInfo::nIntraAngles() const
{
    return nIntraInternals();
}

/** Return the total number of intra-residue angles in the 
    residue with number 'resnum' */
int MolAngleInfo::nIntraAngles(ResNum resnum) const
{
    return nIntraAngles(resnum);
}

/** Return the total number of inter-residue angles in the 
    residue with number 'resnum' */
int MolAngleInfo::nInterAngles(ResNum resnum) const
{
    return nInterInternals(resnum);
}

/** Return whether or not there is an angle that connects residues 
    res0 and res1. This may be slow as it requires a more complete
    search as the angle is not totally defined. */
bool MolAngleInfo::residuesAngled(ResNum res0, ResNum res1) const
{
    QSet<ResNum> resnums;
    resnums.insert(res0);
    resnums.insert(res1);
    
    return hasCommonInternal(resnums);
}

/** Return whether or not there is a single angle that encompasses 
    residues res0, res1 and res2 */
bool MolAngleInfo::residuesAngled(ResNum res0, ResNum res1, ResNum res2) const
{
    return contains( AngleResID(res0,res1,res2) );
}

/** Return an iterator over all of the angles in the molecule.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::angles() const
{
    return internals();
}

/** Return an iterator over all of the angles in the residue 'resnum'.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::angles(ResNum resnum) const
{
    return internals(resnum);
}

/** Return an iterator over all of the angles that contain 
    all of the residues res0 and res2. 
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::angles(ResNum res0, ResNum res1) const
{
    if (res0 == res1)
        return angles(res0);
    else
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
    
        return commonInternals(resnums);
    }
}

/** Return an iterator over all of the angles that contain 
    all of the residues res0, res1 and res2. 
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::angles(ResNum res0, ResNum res1, 
                                                  ResNum res2) const
{
    if (res0 == res1 and res0 == res2)
        return angles(res0);
    else if (res0 == res1 or res0 == res2 or res1 == res2)
    {
        QSet<ResNum> resnums;
        resnums.insert(res0);
        resnums.insert(res1);
        resnums.insert(res2);
        
        return commonInternals(resnums);
    }
    else
        return internals( AngleResID(res0,res1,res2) );
}

/** Return an iterator over all of the intra-residue angles in the 
    molecule.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::intraAngles() const
{
    return intraInternals();
}

/** Return an iterator over all of the inter-residue angles in the 
    molecule.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::interAngles() const
{
    return interInternals();
}

/** Return an iterator over all of the intra-residue angles in the 
    residue 'resnum'.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::intraAngles(ResNum resnum) const
{
    return intraInternals(resnum);
}

/** Return an iterator over all of the inter-residue angles in the 
    residue 'resnum'.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
MolAngleInfo::const_iterator MolAngleInfo::interAngles(ResNum resnum) const
{
    return interInternals(resnum);
}

/** Return the metainfo about the angles in residue 'resnum'. This 
    returns an empty object if there are no angles recorded for 
    this residue. */
ResAngleInfo MolAngleInfo::residue(ResNum resnum) const
{
    return extractResidue(resnum);
}
