
#include "objectregistry.hpp"

#include <QMutex>

#include "SireStream/streamdata.hpp"

#include "SireError/errors.h"

using boost::python::object;
using boost::python::extract;

using boost::tuples::tuple;
using boost::shared_ptr;

ObjectRegistry::ObjectRegistry()
{}

ObjectRegistry::~ObjectRegistry()
{}

void ObjectRegistry::throwExtractionError(const object &obj, 
                                          const QString &type_name) const
{
    throw SireError::program_bug( QObject::tr(
       "There was an error trying to extract a Python object of type %1 using "
       "a C++ type of %2.").arg( extract<const char*>(obj.attr("__class__").attr("__name__"))() )
                           .arg(type_name), CODELOC );
}

object ObjectRegistry::load(const QByteArray &data)
{
    tuple<shared_ptr<void>,QString> obj = SireStream::load(data);

    if (obj.get<0>().get() == 0 or obj.get<1>().isEmpty())
    {
       //nothing of interest here!
       return object();
    }

    return getConverter(obj.get<1>()).convertFromVoid(obj.get<0>().get());
}

object ObjectRegistry::load(const QString &filename)
{
    tuple<shared_ptr<void>,QString> obj = SireStream::load(filename);

    if (obj.get<0>().get() == 0 or obj.get<1>().isEmpty())
    {
        //nothing of interest here
        return object();
    }

    return getConverter(obj.get<1>()).convertFromVoid(obj.get<0>().get());
}

QByteArray ObjectRegistry::save(const object &obj)
{
    /*if (not obj.has_attr("what"))
    {
        throw SireError::invalid_arg( QObject::tr(
           "You cannot save this object to binary, as it does not have a \".what()\" "
           "member function. Ask the programmer of this class to provide that "
           "functionality."), CODELOC );
    }*/

    object result = obj.attr("what")();

    extract<QString> test_result(result);

    if (not test_result.check())
    {
        throw SireError::invalid_arg( QObject::tr(
           "You cannot save this object to binary, as while it has a \".what()\" "
           "member function, it does not return a string, as expected. "
           "Ask the programmer of this class to provide this functionality."),
                CODELOC );
    }

    QString type_name = test_result();

    return getConverter(type_name).saveObject(obj);
}

void ObjectRegistry::save(const object &obj, const QString &filename)
{
    /*if (not obj.has_attr("what"))
    {
        throw SireError::invalid_arg( QObject::tr(
           "You cannot save this object to binary, as it does not have a \".what()\" "
           "member function. Ask the programmer of this class to provide that "
           "functionality."), CODELOC );
    }*/

    object result = obj.attr("what")();

    extract<QString> test_result(result);

    if (not test_result.check())
    {
        throw SireError::invalid_arg( QObject::tr(
           "You cannot save this object to binary, as while it has a \".what()\" "
           "member function, it does not return a string, as expected. "
           "Ask the programmer of this class to provide this functionality."),
                CODELOC );
    }

    QString type_name = test_result();

    getConverter(type_name).saveObject(obj, filename);
}

static QMutex registry_mutex; static QHash< QString, shared_ptr<ObjectRegistry> > *registry(0);

static QHash< QString,shared_ptr<ObjectRegistry> >& getRegistry()
{ 
    if (not registry)
    {
        registry = new QHash< QString,shared_ptr<ObjectRegistry> >();
    }

    return *registry;
}

void ObjectRegistry::registerConverter(const char *type_name,
                                       ObjectRegistry *converter)
{
    QMutexLocker lkr(&registry_mutex);

    getRegistry().insert( type_name, shared_ptr<ObjectRegistry>(converter) );   
}

const ObjectRegistry& ObjectRegistry::getConverter(const QString &type_name)
{
    QMutexLocker lkr(&registry_mutex);

    if (not getRegistry().contains(type_name))
    {
        throw SireError::unknown_type( QObject::tr(
           "The type %1 is not known in the registry. Make sure that you load the "
           "Python module that contains this type, and, if that doesn't work, then "
           "ask a programmer to see if they could add support.").arg(type_name),
               CODELOC );
    }

    return *(getRegistry().constFind(type_name)->get());
}

