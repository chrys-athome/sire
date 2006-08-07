
#include "qhash_siremol.h"

#include "moleculesignature.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<MoleculeSignature> r_molsig("SireMol::MoleculeSignature");

/** Serialise a signature to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeSignature &sig)
{
    writeHeader(ds, r_molsig, 1) << sig.molname << sig.atominfo << sig.bondinfo;
       
    return ds;
}

/** Deserialise a signature from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeSignature &sig)
{
    VersionID v = readHeader(ds, r_molsig);
    
    if (v == 1)
    {
        ds >> sig.molname >> sig.atominfo >> sig.bondinfo;
    }
    else
        throw version_error(v, "1", r_molsig, CODELOC);
    
    return ds;
}

/** Construct an empty signature */
MoleculeSignature::MoleculeSignature()
{}

/** Construct a signature for the molecule called 'name', with
    atom info in 'atminfo' and bonding data in 'bonding' */
MoleculeSignature::MoleculeSignature(const QString &name,
                                     const QHash<ResidueID, QStringSet> &atminfo,
                                     const MoleculeBonds &bonding)
                  : molname(name), atominfo(atminfo), bondinfo(bonding)
{}

/** Copy constructor */
MoleculeSignature::MoleculeSignature(const MoleculeSignature &other)
        : molname(other.molname), atominfo(other.atominfo), bondinfo(other.bondinfo)
{}

/** Copy constructor, except override the bonds with those contained in 'bonding' */
MoleculeSignature::MoleculeSignature(const MoleculeSignature &other, 
                                     const MoleculeBonds &bonding)
                  : molname(other.molname), atominfo(other.atominfo), bondinfo(bonding)
{}

/** Destructor */
MoleculeSignature::~MoleculeSignature()
{}

/** Comparison operator */
bool MoleculeSignature::operator==(const MoleculeSignature &other) const
{
    return molname == other.molname and atominfo == other.atominfo
            and bondinfo == other.bondinfo;
}

/** Comparison operator */
bool MoleculeSignature::operator!=(const MoleculeSignature &other) const
{
    return molname != other.molname or atominfo != other.atominfo
            or bondinfo != other.bondinfo;
}

/** Return a string representation of this signature */
QString MoleculeSignature::toString() const
{
    int nats = 0;
    QHashIterator<ResidueID,QStringSet> it(atominfo);
    while( it.hasNext() )
    {
        it.next();
        nats += it.value().count();
    }
        
    int nbnds = bondinfo.nBonds();
    int nres = atominfo.count();

    return QObject::tr("Signature for \"%1\". nResidues() == %2, nAtoms() == %3, nBonds() == %4")
                                .arg(molname).arg(nres).arg(nats).arg(nbnds);
}
