
#include "qhash_siremm.h"
#include "SireMol/qhash_siremol.h"

#include "resbondinfo.h"
#include "molbondinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireMol;

static const RegisterMetaType<ResBondInfo> r_resbondinfo("SireMM::ResBondInfo");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const ResBondInfo &info)
{
    writeHeader(ds, r_resbondinfo, 1) 
          << static_cast<const ResInternalInfo<Bond>&>(info);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, ResBondInfo &info)
{
    VersionID v = readHeader(ds, r_resbondinfo);
    
    if (v == 1)
    {
        ds >> static_cast<ResInternalInfo<Bond>&>(info);
    }
    else
        throw version_error(v, "1", r_resbondinfo, CODELOC);
    
    return ds;
}

/** Empty constructor */
ResBondInfo::ResBondInfo() : ResInternalInfo<Bond>()
{}

/** Construct from residue 'resnum' from 'molinfo' - this creates
    an empty ResBondInfo if there are no bonds for this residue
    in 'molinfo' */
ResBondInfo::ResBondInfo(const MolBondInfo &molinfo, ResNum resnum)
            : ResInternalInfo<Bond>( molinfo.residue(resnum) )
{}

/** Copy constructor */
ResBondInfo::ResBondInfo(const ResInternalInfo<Bond> &other)
            : ResInternalInfo<Bond>(other)
{}

/** Destructor */
ResBondInfo::~ResBondInfo()
{}

/** Return the number of bonds involving this residue */
int ResBondInfo::nBonds() const
{
    return nInternals();
}
    
/** Return the number of intra-residue bonds in this residue */
int ResBondInfo::nIntraBonds() const
{
    return nIntraInternals();
}

/** Return the number of inter-residue bonds to this residue */
int ResBondInfo::nInterBonds() const
{
    return nInterInternals();
}
    
/** Return an iterator over all of the bonds in this residue */
ResBondInfo::const_iterator ResBondInfo::bonds() const
{
    return internals();
}

/** Return an iterator over all of the intra-residue bond in
    this residue */
ResBondInfo::const_iterator ResBondInfo::intraBonds() const
{
    return intraInternals();
}

/** Return an iterator over all of the inter-residue bonds 
    to this residue */
ResBondInfo::const_iterator ResBondInfo::interBonds() const
{
    return interInternals();
}
