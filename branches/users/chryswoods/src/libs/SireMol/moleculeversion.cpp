
#include "moleculeversion.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeVersion> r_molversion("SireMol::MoleculeVersion");

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeVersion &molver)
{
    writeHeader(ds, r_molversion, 1) << molver.majversion << molver.minversion;
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeVersion &molver)
{
    VersionID v = readHeader(ds, r_molversion);
    
    if (v == 1)
    {
        ds >> molver.majversion >> molver.minversion;
    }
    else
        throw version_error(v, "1", r_molversion, CODELOC);
    
    return ds;
}

/** Empty constructor - version = 0:0 */
MoleculeVersion::MoleculeVersion() : majversion(0), minversion(0)
{}

/** Construct a specific version */
MoleculeVersion::MoleculeVersion(quint32 major, quint32 minor)
                : majversion(major), minversion(minor)
{}
    
/** Copy constructor */
MoleculeVersion::MoleculeVersion(const MoleculeVersion &other)
                : majversion(other.majversion), minversion(other.minversion)
{}

/** Destructor */
MoleculeVersion::~MoleculeVersion()
{}

/** Assignment operator */
MoleculeVersion& MoleculeVersion::operator=(const MoleculeVersion &other)
{
    majversion = other.majversion;
    minversion = other.minversion;
    
    return *this;
}

/** Return a string representation of this version */
QString MoleculeVersion::toString() const
{
    return QString("%1.%2").arg(majversion).arg(minversion);
}
