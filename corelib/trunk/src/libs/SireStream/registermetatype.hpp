#ifndef SIRESTREAM_REGISTERMETATYPE_HPP
#define SIRESTREAM_REGISTERMETATYPE_HPP

#include <QMetaType>

#include "sireglobal.h"

#include "getmagic.h"

SIRE_BEGIN_HEADER

namespace SireStream
{

/** This is the base class of the type that is used to register 
    classes for QVariant and also for the streaming operators 
    (i.e. to get magic numbers for each class)
    
    @author Christopher Woods
*/
class RegisterMetaTypeBase
{
public:
    /** Constructor */
    RegisterMetaTypeBase(const char *type_name) 
               : typnam(type_name)
    {
    	magicid = getMagic(type_name);
    }

    /** Destructor */
    ~RegisterMetaTypeBase()
    {}

    /** Return the MagicID of this class */
    MagicID magicID() const
    {
        return magicid;
    }

    /** Return the ID number of this class - this will only
        be non-zero for registered concrete classes */
    int ID() const
    {
        return id;
    }

    /** Return the type name of this class (as supplied to the 
        constructor) */
    const char* name() const
    {
        return typnam;
    }

protected:
    int id;
    MagicID magicid;
    const char *typnam;
};

/** Enum used to register with only the magic ID */
enum MagicOnlyEnum{ MAGIC_ONLY = 1 };
/** Enum used to register without using the streaming operators */
enum NoStreamEnum{ NO_STREAM = 2 };

/** This is the template-specific part of RegisterMetaType

    @author Christopher Woods
*/
template<class T>
class RegisterMetaType : public RegisterMetaTypeBase
{
public:
    
    /** Use this constructor to register a concrete class */
    RegisterMetaType(const char *type_name)
	         : RegisterMetaTypeBase(type_name)
    {
        id = qRegisterMetaType<T>(type_name);
        qRegisterMetaTypeStreamOperators<T>(type_name);
    }

    /** Use this construct to register a pure-virtual class */
    RegisterMetaType(const char *type_name, MagicOnlyEnum )
                : RegisterMetaTypeBase(type_name)
    {}
    
    /** Use this constructor to register a class that cannot be streamed */
    RegisterMetaType(const char *type_name, NoStreamEnum )
                : RegisterMetaTypeBase(type_name)
    {
        id = qRegisterMetaType<T>(type_name);
    }
    
    /** Destructor */
    ~RegisterMetaType()
    {}
};

}

SIRE_END_HEADER

#endif
