
#include "property.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

namespace SireMol
{

/** This is a null property */
class SIREMOL_EXPORT NullProperty : public PropertyData
{
public:
    NullProperty() : PropertyData()
    {}
    
    NullProperty(const NullProperty &other) : PropertyData(other)
    {}
    
    ~NullProperty()
    {}
    
    NullProperty* clone() const
    {
        return new NullProperty(*this);
    }
    
    static const char* typeName()
    {
        return "SireMol::NullProperty";
    }
    
    const char* what() const
    {
        return NullProperty::typeName();
    }
    
    void assertCompatibleWith(const Molecule&) const
    {
        throw SireError::incompatible_error( QObject::tr(
                    "Null data is not compatible with anything!"), CODELOC );
    }
};

}

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

/** Constructor */
PropertyData::PropertyData()
{}
    
/** Copy constructor */
PropertyData::PropertyData(const PropertyData&)
{}
  
/** Destructor */
PropertyData::~PropertyData()
{}

static const RegisterMetaType<PropertyData> r_propbase("SireMol::PropertyData", 
                                                       MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const PropertyData&)
{
    writeHeader(ds, r_propbase, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, PropertyData&)
{
    VersionID v = readHeader(ds, r_propbase);
    
    if (v != 0)
        throw version_error(v, "0", r_propbase, CODELOC);
    
    return ds;
}

static SharedPolyPointer<PropertyData> shared_null( new NullProperty() );

/** Return a shared null property */
Property PropertyData::null_property()
{
    return Property(shared_null);
}

/** Null constructor - construct a null property */
Property::Property() : SharedPolyPointer<PropertyData>( PropertyData::null_property() )
{}
    
/** Construct from the passed pointer - this must not be null! This
    takes over ownership of the passed pointer and may delete it 
    whenever it wishes. 
        
    \throw SireError::nullptr_error
*/
Property::Property(PropertyData *ptr)
         : SharedPolyPointer<PropertyData>(ptr)
{
    if (not ptr)
        throw SireError::nullptr_error( QObject::tr(
                  "Cannot construct a Property from a null pointer!"), CODELOC );
}
    
/** Construct from the passed pointer - this must not be null!

    \throw SireError::nullptr_error
*/
Property::Property(const SireBase::SharedPolyPointer<PropertyData> &ptr)
         : SharedPolyPointer<PropertyData>(ptr)
{
    if (not ptr)
        throw SireError::nullptr_error( QObject::tr(
                  "Cannot construct a Property from a null pointer!"), CODELOC );
}
    
/** Construct from the passed property */
Property::Property(const PropertyData &property)
         : SharedPolyPointer<PropertyData>( 
                      SharedPolyPointerHelper<PropertyData>::clone(property) )
{}
    
/** Copy constructor */
Property::Property(const Property &other)
         : SharedPolyPointer<PropertyData>(other)
{}
  
/** Destructor */
Property::~Property()
{}
