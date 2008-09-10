#ifndef PYTHON2_HELPERS_OBJECTREGISTRY_HPP
#define PYTHON2_HELPERS_OBJECTREGISTRY_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireStream/streamdata.hpp"

#include <QHash>
#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

class ObjectRegistry
{
public:
    virtual ~ObjectRegistry();

    static boost::python::object load(const QByteArray &data);
    static QByteArray save(const boost::python::object &object);
    static QString getTypeName(const QByteArray &data);

    template<class T>
    static void registerConverterFor();

protected:
    virtual boost::python::object convertFromVoid(const void * const ptr) const=0;

    virtual QByteArray saveObject(const boost::python::object &object) const=0;

    static void registerConverter(const char *type_name,
                                  ObjectRegistry *converter);

    static const ObjectRegistry& getConverter(const QString &type_name);

    void throwExtractionError(const boost::python::object &obj, 
                              const QString &type_name) const;

    ObjectRegistry();
};

namespace detail
{

template<class T>
class ObjectRegistryT : public ObjectRegistry
{

friend class ObjectRegistry;

public:
    ~ObjectRegistryT()
    {}

protected:
    boost::python::object convertFromVoid(const void * const ptr) const
    {
        if (ptr == 0)
            return boost::python::object();

        const T * const t_ptr = static_cast<const T * const>(ptr);

        return boost::python::object( *t_ptr );
    }

    QByteArray saveObject(const boost::python::object &obj) const
    {
        boost::python::extract<const T&> t_object(obj);

        if (not t_object.check())
            this->throwExtractionError(obj, T::typeName());

        return SireStream::save<T>( t_object() );
    }

private:
    ObjectRegistryT() : ObjectRegistry()
    {}
};

}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ObjectRegistry::registerConverterFor()
{
    ObjectRegistry::registerConverter( T::typeName(), new detail::ObjectRegistryT<T>() );
}

SIRE_END_HEADER

#endif
