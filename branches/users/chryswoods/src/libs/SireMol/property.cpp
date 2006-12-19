
#include "property.h"

#include "molecule.h"
#include "moleculeid.h"
#include "moleculeinfo.h"
#include "moleculeversion.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

///////////////
/////////////// Implementation of NullProperty
///////////////

namespace SireMol
{

/** This is a null property */
class SIREMOL_EXPORT NullProperty : public PropertyBase
{
public:
    NullProperty() : PropertyBase()
    {}

    NullProperty(const NullProperty &other) : PropertyBase(other)
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

    bool isCompatibleWith(const Molecule&) const
    {
        return false;
    }
};

}

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

///////////////
/////////////// Implementation of PropertyBase
///////////////

/** Constructor */
PropertyBase::PropertyBase()
{}

/** Copy constructor */
PropertyBase::PropertyBase(const PropertyBase&)
{}

/** Destructor */
PropertyBase::~PropertyBase()
{}

/** Assert that this property is compatible with the molecule 'molecule'

    \throw SireError::incompatible_error
*/
void PropertyBase::assertCompatibleWith(const Molecule &molecule) const
{
    if (not this->isCompatibleWith(molecule))
        throw SireError::incompatible_error( QObject::tr(
                "This property is incompatible with the molecule \"%1\" (%2:%3)")
                    .arg(molecule.info().name()).arg(molecule.ID())
                    .arg(molecule.version().toString()), CODELOC );
}

static const RegisterMetaType<PropertyBase> r_propbase("SireMol::PropertyBase",
                                                       MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const PropertyBase&)
{
    writeHeader(ds, r_propbase, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, PropertyBase&)
{
    VersionID v = readHeader(ds, r_propbase);

    if (v != 0)
        throw version_error(v, "0", r_propbase, CODELOC);

    return ds;
}

static SharedPolyPointer<PropertyBase> shared_null( new NullProperty() );

/** Return a shared null property */
Property PropertyBase::null_property()
{
    return Property(shared_null);
}

///////////////
/////////////// Implementation of VariantProperty
///////////////

static const RegisterMetaType<VariantProperty> r_varprop("SireMol::VariantProperty");

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const VariantProperty &varprop)
{
    writeHeader(ds, r_varprop, 1)
          << static_cast<const PropertyBase&>(varprop)
          << static_cast<const QVariant&>(varprop);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, VariantProperty &varprop)
{
    VersionID v = readHeader(ds, r_varprop);

    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(varprop)
           >> static_cast<QVariant&>(varprop);
    }
    else
        throw version_error(v, "1", r_varprop, CODELOC);

    return ds;
}

/** Null constructor */
VariantProperty::VariantProperty()
                : PropertyBase(), QVariant()
{}

/** Construct a property equal to 'value' */
VariantProperty::VariantProperty(const QVariant &value)
                : PropertyBase(), QVariant(value)
{}

/** Copy constructor */
VariantProperty::VariantProperty(const VariantProperty &other)
                : PropertyBase(other), QVariant(other)
{}

/** Destructor */
VariantProperty::~VariantProperty()
{}

///////////////
/////////////// Implementation of Property
///////////////

static const RegisterMetaType<Property> r_prop("SireMol::Property");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Property &property)
{
    writeHeader(ds, r_prop, 1)
          << static_cast<const SharedPolyPointer<PropertyBase>&>(property);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Property &property)
{
    VersionID v = readHeader(ds, r_prop);

    if (v == 1)
    {
        ds >> static_cast<SharedPolyPointer<PropertyBase>&>(property);
    }
    else
        throw version_error(v, "1", r_prop, CODELOC);

    return ds;
}

/** Null constructor - construct a null property */
Property::Property() : SharedPolyPointer<PropertyBase>( PropertyBase::null_property() )
{}

/** Construct from the passed pointer - this must not be null! This
    takes over ownership of the passed pointer and may delete it
    whenever it wishes.

    \throw SireError::nullptr_error
*/
Property::Property(PropertyBase *ptr)
         : SharedPolyPointer<PropertyBase>(ptr)
{
    if (not ptr)
        throw SireError::nullptr_error( QObject::tr(
                  "Cannot construct a Property from a null pointer!"), CODELOC );
}

/** Construct from the passed pointer - this must not be null!

    \throw SireError::nullptr_error
*/
Property::Property(const SireBase::SharedPolyPointer<PropertyBase> &ptr)
         : SharedPolyPointer<PropertyBase>(ptr)
{
    if (not ptr)
        throw SireError::nullptr_error( QObject::tr(
                  "Cannot construct a Property from a null pointer!"), CODELOC );
}

/** Construct from the passed property */
Property::Property(const PropertyBase &property)
         : SharedPolyPointer<PropertyBase>(
                      SharedPolyPointerHelper<PropertyBase>::clone(property) )
{}

/** Copy constructor */
Property::Property(const Property &other)
         : SharedPolyPointer<PropertyBase>(other)
{}

/** Destructor */
Property::~Property()
{}
