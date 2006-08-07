
#include "qhash_siremm.h"
#include "qhash_siremol.h"

#include "molbondinfo.h"
#include "resbondinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<MolBondInfo> r_molbondinfo("SireMM::MolBondInfo");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const MolBondInfo &info)
{
    writeHeader(ds, r_molbondinfo, 1) 
            << static_cast<const MolInternalInfo<Bond>&>(info);
            
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, MolBondInfo &info)
{
    VersionID v = readHeader(ds, r_molbondinfo);
    
    if (v == 1)
    {
        ds >> static_cast<MolInternalInfo<Bond>&>(info);
    }
    else
        throw version_error(v, "1", r_molbondinfo, CODELOC);
    
    return ds;
}

/** Empty constructor */
MolBondInfo::MolBondInfo() : MolInternalInfo<Bond>()
{}

/** Construct a MolBondInfo that holds the bonds for the molecule 'mol'.
    Note that this will not initially contain any bonds - these need to be
    added separately. */
MolBondInfo::MolBondInfo(const Molecule &mol) : MolInternalInfo<Bond>(mol)
{}

/** Construct a MolBondInfo that holds the bonds for the molecule 'mol',
    with the BondGenerator 'bondgenerator' used to find all of the available
    bonds in the molecule. */
MolBondInfo::MolBondInfo(const Molecule &mol, 
                         const InternalGenerator<MolBondInfo> &bondgenerator)
            : MolInternalInfo<Bond>(mol)
{
    //generate all of the bonds using the bond generator
    bondgenerator.generate(mol, *this);
}

/** Copy constructor */
MolBondInfo::MolBondInfo(const MolBondInfo &other) : MolInternalInfo<Bond>(other)
{}

/** Destructor */
MolBondInfo::~MolBondInfo()
{}

/** Return the bonds involving the residue 'resnum', or an 
    empty ResBondInfo if there are no bonds listed that involve 
    that residue. */
ResBondInfo MolBondInfo::residue(ResNum resnum) const
{
    return extractResidue(resnum);
}

/** Add the bond 'bond' to the molecule - does nothing if 
    the bond is already in this molecule. Returns the 
    GroupIndexID of the added bond. */
GroupIndexID MolBondInfo::addBond(const Bond &bond)
{
    return addInternal(bond);
}

/** Remove the bond 'bond' from the molecule - does nothing if
    the bond is not in the molecule. */
void MolBondInfo::removeBond(const Bond &bond)
{
    removeInternal(bond);
}

/** Remove a bond from the molecule that has index 'index'.
    This does nothing if there is no such bond. */
void MolBondInfo::removeBond(const GroupIndexID &groupid)
{
    removeInternal(groupid);
}

/** Return the number of bonds in the molecule */
int MolBondInfo::nBonds() const
{
    return nInternals();
}

/** Return the number of bonds in the group with GroupID 'group' */
int MolBondInfo::nBonds(GroupID group) const
{
    return nInternals(group);
}

/** Return the number of bonds in the residue with residue number 'resnum'. */
int MolBondInfo::nBonds(ResNum resnum) const
{
    return nInternals(resnum);
}

/** Return the number of bonds between residues res0 and res1 */
int MolBondInfo::nBonds(ResNum res0, ResNum res1) const
{
    return nInternals( BondResID(res0,res1) );
}

/** Return the total number of inter-residue bonds in the molecule */
int MolBondInfo::nInterBonds() const
{
    return nInterInternals();
}

/** Return the total number of intra-residue bonds in the molecule */
int MolBondInfo::nIntraBonds() const
{
    return nIntraInternals();
}

/** Return the number of intra-residue bonds in residue with number 'resnum' */
int MolBondInfo::nIntraBonds(ResNum resnum) const
{
    return nIntraInternals(resnum);
}

/** Return the number of inter-residue bonds in the residue with number 'resnum' */
int MolBondInfo::nInterBonds(ResNum resnum) const
{
    return nInterInternals(resnum);
}

/** Return whether or not there are any bonds between the residues 'res0' and 'res1' */
bool MolBondInfo::residuesBonded(ResNum res0, ResNum res1) const
{
    return contains( BondResID(res0,res1) );
}

/** Return an iterator over all of the bonds in the molecule. 

    The iterator is initially position on the first bond. 
    Note that an invalid iterator is returned if there are no 
    bonds in the molecule. 
*/
MolBondInfo::const_iterator MolBondInfo::bonds() const
{
    return internals();
}

/** Return an iterator over all of the bonds in the
    residue with number 'resnum'.

    The iterator is initially position on the first bond. 
    Note that an invalid iterator is returned if there are no 
    bonds in the residue. 
*/
MolBondInfo::const_iterator MolBondInfo::bonds(ResNum resnum) const
{
    return internals(resnum);
}

/** Return an iterator over all of the bonds between
    residues 'res0' and 'res1'.

    The iterator is initially position on the first bond. 
    Note that an invalid iterator is returned if there are no 
    bonds between these residues.
*/
MolBondInfo::const_iterator MolBondInfo::bonds(ResNum res0, ResNum res1) const
{
    return internals( BondResID(res0,res1) );
}

/** Return an iterator over all of the intra-residue bonds in the molecule.

    The iterator is initially position on the first bond. 
    Note that an invalid iterator is returned if there are no 
    intra-residue bonds in the molecule. 
*/
MolBondInfo::const_iterator MolBondInfo::intraBonds() const
{
    return intraInternals();
}

/** Return an iterator over all of the inter-residue bonds in the molecule.

    The iterator is initially position on the first bond. 
    Note that an invalid iterator is returned if there are no 
    inter-residue bonds in the molecule. 
*/
MolBondInfo::const_iterator MolBondInfo::interBonds() const
{
    return interInternals();
}

/** Return an iterator over all of the intra-residue bonds 
    in residue 'resnum'.

    The iterator is initially position on the first bond. 
    Note that an invalid iterator is returned if there are no 
    intra-residue bonds in the residue. 
*/
MolBondInfo::const_iterator MolBondInfo::intraBonds(ResNum resnum) const
{
    return intraInternals(resnum);
}

/** Return an iterator over all of the inter-residue bonds 
    involving the residue 'resnum'.

    The iterator is initially position on the first bond. 
    Note that an invalid iterator is returned if there are no 
    inter-residue bonds involving the residue. 
*/
MolBondInfo::const_iterator MolBondInfo::interBonds(ResNum resnum) const
{
    return interInternals(resnum);
}
