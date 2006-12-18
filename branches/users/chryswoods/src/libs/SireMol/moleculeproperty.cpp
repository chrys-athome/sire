
#include "moleculeproperty.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculePropertyData> 
                    r_molprop("SireMol::MoleculePropertyData");

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const MoleculePropertyData &molprop)
{
    writeHeader(ds, r_molprop, 1) 
          << static_cast<const PropertyData&>(molprop)
          << static_cast<const QVariant&>(molprop);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculePropertyData &molprop)
{
    VersionID v = readHeader(ds, r_molprop);
    
    if (v == 1)
    {
        ds >> static_cast<PropertyData&>(molprop)
           >> static_cast<QVariant&>(molprop);
    }
    else
        throw version_error(v, "1", r_molprop, CODELOC);
    
    return ds;
}
    
/** Null constructor */
MoleculePropertyData::MoleculePropertyData()
                     : PropertyData(), QVariant()
{}
    
/** Construct from the passed variant value */
MoleculePropertyData::MoleculePropertyData(const QVariant &value)
                     : PropertyData(), QVariant(value)
{}
    
/** Copy constructor */
MoleculePropertyData::MoleculePropertyData(const MoleculePropertyData &other)
                     : PropertyData(other), QVariant(other)
{}
    
/** Destructor */
MoleculePropertyData::~MoleculePropertyData()
{}

/** Assert that this is compatible with the passed molecule - it always is! */
void MoleculePropertyData::assertCompatibleWith(const Molecule&) const
{}
