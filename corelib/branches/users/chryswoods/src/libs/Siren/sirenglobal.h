#ifndef SIRENGLOBAL_H
#define SIRENGLOBAL_H

/** This file contains some global macros that are used
    to improve the portability of the code. */

//include qglobal.h - this includes all of the definitions
//that are needed for these macros
#include <qconfig.h>
#include <qglobal.h>

#ifdef __cplusplus

#include <qmetatype.h>

#include <boost/current_function.hpp>
//macro used to return the current file and line as a QString
#ifdef CODELOC
#undef CODELOC
#endif

#define CODELOC \
   QObject::tr("FILE: %1, LINE: %2, FUNCTION: %3") \
           .arg(__FILE__).arg(__LINE__).arg(BOOST_CURRENT_FUNCTION)

//copy the QT_BEGIN_HEADER and QT_END_HEADER
//to SIRE_BEGIN_HEADER and SIRE_END_HEADER. This
//will allow me to change their definition at some
//future point in time without having to mess with Qt
#define SIREN_BEGIN_HEADER QT_BEGIN_HEADER
#define SIREN_END_HEADER QT_END_HEADER

//create keywords that control whether classes or functions are
//exposed to a scripting language
#define SIREN_EXPOSE_FUNCTION(f)  /* Exposing function #1 */
#define SIREN_EXPOSE_CLASS(c)     /* Exposing class #1 */
#define SIREN_EXPOSE_ALIAS(c,a)   /* Exposing class #1 as alias #2 */
#define SIREN_EXPOSE_PROPERTY(c,a)  /* Exposing property #1 of base class #2 */
#define SIREN_EXPOSE_ATOM_PROPERTY(c,a) /* Exposing atom property #1 with alias #2 */
#define SIREN_EXPOSE_CUTGROUP_PROPERTY(c,a) /* Exposing CutGroup property #1 with alias #2 */
#define SIREN_EXPOSE_RESIDUE_PROPERTY(c,a) /* Exposing residue property #1 with alias #2 */
#define SIREN_EXPOSE_CHAIN_PROPERTY(c,a) /* Exposing chain property #1 with alias #2 */
#define SIREN_EXPOSE_SEGMENT_PROPERTY(c,a) /* Exposing segment property #1 with alias #2 */

//create the keyword used to export a symbol - this
//is a copy of Q_DECL_EXPORT, which will definitely
//be set to the correct value
#ifndef SIREN_NO_VISIBILITY_AVAILABLE
#define SIREN_EXPORT Q_DECL_EXPORT

//create the keyword to import a symbol - this is copied
//from Q_DECL_IMPORT, which will definitely be set to
//the correct value
#define SIREN_IMPORT Q_DECL_IMPORT
#else
#define SIREN_EXPORT
#define SIREN_IMPORT
#endif

//create the keyword to fix symbol visibility problems for out-of-line
//template functions
#define SIREN_OUTOFLINE_TEMPLATE Q_OUTOFLINE_TEMPLATE

//do the same for inline template functions
#define SIREN_INLINE_TEMPLATE Q_INLINE_TEMPLATE

//Sire is much more picky about what the compiler can support
//than Qt - use the results of the Qt portability tests
//to check that the C++ compiler is up to scratch

#ifdef Q_BROKEN_TEMPLATE_SPECIALIZATION
#error Siren requires a C++ compiler that can handle templates in \
       a standards compliant manner. Please use a more recent version \
       of your compiler, or try gcc >= 3.0
#endif

#ifdef QT_NO_PARTIAL_TEMPLATE_SPECIALIZATION
#error Siren requires that the C++ compiler supports \
       partial template specialization. Please use a more \
     recent version of your compiler, or try gcc >= 3.0
#endif

#ifdef Q_NO_USING_KEYWORD
#error Siren requires that the C++ compiler supports the 'using' \
       keyword. Please use a more recent version of your compiler \
     or try gcc >= 3.0
#endif

#ifdef QT_NO_MEMBER_TEMPLATES
#error Siren requires that the C++ compiler supports the use of \
       member templates. Please use a more recent version of your \
     compiler or try gcc >= 3.0
#endif

#ifdef QT_NO_THREAD
#error Siren requires that Qt was built with threading enabled. \
       Please recompile Qt with threading enabled.
#endif

#ifdef QT_NO_TEXTSTREAM
#error Siren requires that Qt was built with QTextStream enabled \
       Please recompile Qt with QTextStream enabled.
#endif

//sire requires that at least the QtCore and QtSql modules
//are available
#if  !(QT_EDITION & QT_MODULE_CORE)
#error Sire requires that the QtCore module is enabled.
#endif

/** These functions convert a pointer into an integer.

    toInt(void*) : Returns an integer from the pointer. Returns
                   an integer of the same size as the pointer.

    toUInt(void*) : Returns an unsigned integer of the same size as
                    the pointer.

    toInt32(void*)   : Returns a 32bit integer from a pointer. This
                       will truncate if the pointer is larger than 32bits.

    toUInt32(void*)  : Same as toInt32, but returns the pointer as an
                       unsigned 32bit integer.

    toInt64(void*) : Returns a 64bit integer

    toUInt64(void*) : Returns a 64bit unsigned integer.

    @author Christopher Woods
*/
#ifdef QT_POINTER_SIZE
  #if QT_POINTER_SIZE == 4

    // Functions used if we have 32bit pointers
    inline qint32 toInt(const void *ptr)
    {
        return qint32(ptr);
    }

    inline quint32 toUInt(const void *ptr)
    {
        return quint32(ptr);
    }

    inline qint32 toInt32(const void *ptr)
    {
        return toInt(ptr);
    }

    inline quint32 toUInt32(const void *ptr)
    {
        return toUInt(ptr);
    }

    inline qint64 toInt64(const void *ptr)
    {
        return qint64( toInt(ptr) );
    }

    inline quint64 toUInt64(const void *ptr)
    {
        return quint64( toUInt(ptr) );
    }

  #elif QT_POINTER_SIZE == 8

    // Functions used if we have 64bit pointers
    inline qint64 toInt(const void *ptr)
    {
        return qint64(ptr);
    }

    inline quint64 toUInt(const void *ptr)
    {
        return quint64(ptr);
    }

    inline qint32 toInt32(const void *ptr)
    {
        return qint32( toInt(ptr) );
    }

    inline quint32 toUInt32(const void *ptr)
    {
        return qint32( toUInt(ptr) );
    }

    inline qint64 toInt64(const void *ptr)
    {
        return toInt(ptr);
    }

    inline quint64 toUInt64(const void *ptr)
    {
        return toUInt(ptr);
    }

  #else
    #fatal Invalid QT_POINTER_SIZE value stored!
  #endif
#else
  #fatal No QT_POINTER_SIZE macro defined!
#endif

//Add functions that are used to register all public
//types (this allows them to be streamed to a binary file and
//created dynamically)
#include <QMetaType>

namespace Siren
{

typedef quint64 CLASS_UID;
typedef quint32 VERSION_ID;
typedef quint32 HASH_CODE;

/** Enum used to register without using the streaming operators */
enum VirtualClassEnum{ VIRTUAL_CLASS = 1 };

namespace detail
{

class RegisterMetaTypeBase
{
public:
    RegisterMetaTypeBase(CLASS_UID c_uid, VERSION_ID v_id, const QString &name)
          : class_uid(c_uid), version_id(v_id), type_name(name)
    {}

    ~RegisterMetaTypeBase()
    {}

    CLASS_UID UID() const
    {
        return class_uid;
    }

    HASH_CODE hashBase() const
    {
        return (class_uid / 0xF0000000);
    }

    VERSION_ID version() const
    {
        return version_id;
    }

    QString typeName() const
    {
        return type_name;
    }

private:
    CLASS_UID class_uid;
    VERSION_ID version_id;
    QString type_name;
};

} // end of namespace detail

/** This is used to register the type 'T' - this
    needs to be called once for each public type.

    @author Christopher Woods
*/
template<class T>
class RegisterMetaType : public detail::RegisterMetaTypeBase
{
public:
    /** Use this constructor to register a class */
    RegisterMetaType( CLASS_UID class_uid, VERSION_ID version_id )
        : detail::RegisterMetaTypeBase( class_uid, version_id,
                                        QMetaType::typeName( qMetaTypeId<T>() ) )
    {
        qRegisterMetaType<T>(this->typeName());
        qRegisterMetaTypeStreamOperators<T>(this->typeName());
     
        //build the full inheritance and interface hierarchy of this class
        T::registerInheritance();
        
        singleton = this;
    }

    /** Use this constructor to register a virtual class */
    RegisterMetaType( VirtualClassEnum, CLASS_UID class_uid, VERSION_ID version_id )
        : detail::RegisterMetaTypeBase( class_uid, version_id,
                                        QMetaType::typeName( qMetaTypeId<T>() ) )
    {
        singleton = this;
    }

    /** Destructor */
    ~RegisterMetaType()
    {}

    static const detail::RegisterMetaTypeBase* getRegistration()
    {
        return singleton;
    }

private:
    /** Global shared pointer to this singleton object */
    static const detail::RegisterMetaTypeBase *singleton;
};

template<class T>
const detail::RegisterMetaTypeBase* RegisterMetaType<T>::singleton = 0;

} // end of namespace Siren

#else  // else #ifdef __cplusplus

//copied directly from qglobal.h /////////////////
#ifndef Q_DECL_EXPORT
#  ifdef Q_OS_WIN
#    define Q_DECL_EXPORT __declspec(dllexport)
#  elif defined(QT_VISIBILITY_AVAILABLE)
#    define Q_DECL_EXPORT __attribute__((visibility("default")))
#  endif
#  ifndef Q_DECL_EXPORT
#    define Q_DECL_EXPORT
#  endif
#endif
#ifndef Q_DECL_IMPORT
#  ifdef Q_OS_WIN
#    define Q_DECL_IMPORT __declspec(dllimport)
#  else
#    define Q_DECL_IMPORT
#  endif
#endif
///////////////////////////////////////////////////

#ifndef SIREN_NO_VISIBILITY_AVAILABLE
#define SIREN_EXPORT Q_DECL_EXPORT
#define SIREN_IMPORT Q_DECL_IMPORT
#else
#define SIREN_EXPORT
#define SIREN_IMPORT
#endif

#endif // #ifdef __cplusplus

#endif // SIREGLOBAL_H
