/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "class.h"
#include "objref.h"
#include "hanref.h"
#include "tester.h"
#include "logger.h"
#include "stream.h"

#include "Siren/errors.h"

#include <QDebug>

using namespace Siren;

static const RegisterObject<Class> r_class;

typedef QHash<QString,const RegisterMetaType*> MetaTypeRegistry;

Q_GLOBAL_STATIC( MetaTypeRegistry, metaTypeRegistry );

void SIREN_EXPORT RegisterMetaType::registerClassName(const QString &class_name,
                                                      const RegisterMetaType *metatype)
{
    MetaTypeRegistry *reg = metaTypeRegistry();
    
    if (not reg)
        qDebug() << "CANNOT REGISTER" << class_name;
    else
        reg->insert(class_name, metatype->clone());
}

const RegisterMetaType SIREN_EXPORT *RegisterMetaType::getRegistration(
                                                    const QString &class_name)
{
    MetaTypeRegistry *reg = metaTypeRegistry();
    
    if (reg)
        return reg->value(class_name);
    else
        return 0;
}

/** Return the complete (sorted) list of all of the class types
    that have been registered using RegisterMetaType<T> */
QStringList Class::registeredTypes()
{
    MetaTypeRegistry *reg = metaTypeRegistry();
    
    if (reg)
    {
        QStringList l = reg->keys();
        qSort(l);
        return l;
    }
    else
        return QStringList();
}

void Class::buildInheritedTypes()
{
    if (not metatype)
        return;

    if (super_class)
        inherited_types = super_class->inherited_types;

    inherited_types.insert( metatype->typeName() );
    
    foreach (const QString &interface, interface_types )
    {
        inherited_types.insert( interface );
    }
}

typedef QHash< QString,boost::shared_ptr<Class> > ClassRegistryType;

Q_GLOBAL_STATIC( ClassRegistryType, registered_types );

/** Null constructor */
Class::Class() 
      : Implements<Class, Object>(), metatype(0), super_class(0)
{}

/** Internal constructor used by Object, Handle and 
    primitive types to create their class types */
Class::Class(const RegisterMetaType *r)
      : Implements<Class, Object>(), metatype(r), super_class(0)
{
    buildInheritedTypes();
    registered_types()->insert( this->name(), boost::shared_ptr<Class>(new Class(*this)));
}

/** Internal constructor used by Object to create a derived class type */
Class::Class(const RegisterMetaType *r, 
             const Class &base_class, const QStringList &interfaces)
      : Implements<Class, Object>(), metatype(r), super_class(&base_class),
        interface_types(interfaces)
{
    buildInheritedTypes();
    registered_types()->insert( this->name(), boost::shared_ptr<Class>(new Class(*this)));
}

/** Construct the class type object for the passed object */
Class::Class(const Object &object) : Implements<Class, Object>()
{
    this->operator=( object.getClass() );
}

/** Construct the Class object for the type with name 'type_name' 

    \throw Siren::unknown_type
*/
Class::Class(const QString &type_name) 
      : Implements<Class, Object>(), metatype(0), super_class(0)
{
    if (type_name.isEmpty())
    {
        this->operator=( None().getClass() );
    }
    else if (registered_types()->contains(type_name))
    {
        this->operator=( *(registered_types()->value(type_name)) );
    }
    else
    {
        MetaTypeRegistry *reg = metaTypeRegistry();
        
        if (reg)
        {
            if (reg->contains(type_name))
            {
                const RegisterMetaType *mt = reg->value(type_name);
                
                if (not mt->isConcrete())
                {
                    //this is a hack - until we create an object
                    //of this type we can't know it's inheritance
                    //hierarchy, so here we just create a basic Class
                    //object that just knows its RegisterMetaType object
                    metatype = mt; 
                }
                else if (mt->isObject())
                {
                    //create an object of this type so that we get
                    //the full class hierarchy
                    int typ = QMetaType::type(type_name.toLatin1().constData());

                    std::auto_ptr<Object> ptr( static_cast<Object*>(
                                                        QMetaType::construct(typ)) );
                                
                    if (ptr.get() == 0)
                        throw Siren::program_bug( QObject::tr(
                            "Unable to create an Object of type %1, ID = %2.")
                                .arg(type_name).arg(typ), CODELOC );
                                
                    this->operator=( ptr->getClass() );
                }
                else if (mt->isHandle())
                {
                    //create a Handle of this type so that we get
                    //the full class hierarchy
                    int typ = QMetaType::type(type_name.toLatin1().constData());

                    std::auto_ptr<Handle> ptr( static_cast<Handle*>(
                                                        QMetaType::construct(typ)) );
                                
                    if (ptr.get() == 0)
                        throw Siren::program_bug( QObject::tr(
                            "Unable to create a Handle of type %1, ID = %2.")
                                .arg(type_name).arg(typ), CODELOC );
                                
                    this->operator=( ptr->getClass() );
                }
                else if (mt->isPrimitive())
                {
                    this->operator=( Class(mt) );
                }
            }
        }
        else 
            throw Siren::unknown_type( QObject::tr(
              "Cannot create the Class for type \"%1\". "
              "This could be because either this is not a concrete type, "
              "or that the library or module containing "
              "this type has been loaded, or that the programmer "
              "has not registered the type with RegisterObject, "
              "RegisterHandle or RegisterPrimitive.").arg(type_name), CODELOC );
    }
}

/** Copy constructor */
Class::Class(const Class &other)
      : Implements<Class, Object>(other),
        metatype(other.metatype), super_class(other.super_class),
        interface_types(other.interface_types),
        inherited_types(other.inherited_types)
{}

/** Destructor */
Class::~Class()
{}

/** Copy assignment operator */
Class& Class::operator=(const Class &other)
{
    if (this != &other)
    {
        super::operator=(other);
        
        metatype = other.metatype;
        super_class = other.super_class;
        interface_types = other.interface_types;
        inherited_types = other.inherited_types;
    }
    
    return *this;
}

/** Comparison operator */
bool Class::operator==(const Class &other) const
{
    return metatype == other.metatype;
}

/** Comparison operator */
bool Class::operator!=(const Class &other) const
{
    return not Class::operator==(other);
}

/** Return the name of the class */
QString Class::name() const
{
    if (metatype)
        return metatype->typeName();
    else
        return QString::null;
}

QString Class::toString() const
{
    QStringList t;
    
    if (isHandle())
        t.append( QObject::tr("handle") );
    
    if (isObject())
        t.append( QObject::tr("object") );
        
    if (isPrimitive())
        t.append( QObject::tr("primitive") );

    return QObject::tr("Class( %1, %2 )").arg(this->name(), t.join(":"));
}

/** Return whether or not this class has a superclass */
bool Class::hasSuper() const
{
    return super_class != 0;
}

/** Return the superclass of this type - this returns the 
    same type if it doesn't have a super class */
const Class& Class::superClass() const
{
    if (super_class)
        return *super_class;
    else
        return *this;
}

/** Return whether or not this class implements the type 'type_name' */
bool Class::implements(const QString &type_name) const
{
    return inherited_types.contains(type_name);
}

/** Return all of the direct interfaces for this type */
QStringList Class::interfaces() const
{
    return interface_types;
}

/** Create a default-constructed object of this class.
    This is only possible if this is a concrete class
    (isConcrete() returns true)
    
    \throw Siren::invalid_operation
*/
ObjRef Class::createObject() const
{
    if (metatype == 0)
        return ObjRef();

    else if (this->isHandle())
        throw Siren::invalid_operation( QObject::tr(
                "You cannot create an object of type %1 as this class "
                "is derived from Siren::Handle - you must use "
                "Class::createHandle() to create objects of this type.")
                    .arg(this->name()), CODELOC );

    else if (not isConcrete())
        throw Siren::invalid_operation( QObject::tr(
                "You cannot create an object of type %1 as it is not "
                "concrete (it is virtual).")
                    .arg(this->name()), CODELOC );

    
    if (isPrimitive())
    {
        //we'll create a PrimitiveObject instead so that we have the
        //benefits of a virtual class
        const RegisterPrimitiveBase *prim_mt 
                        = static_cast<const RegisterPrimitiveBase*>(metatype);
        
        const RegisterMetaType *prim_obj_mt = prim_mt->getObjectRegistration();
        
        if (not prim_obj_mt)
            throw Siren::program_bug( QObject::tr(
                "Why does the Primitive registration for %1 not have a "
                "PrimitiveObject registration?").arg(this->name()), CODELOC );
                
        int type_id = QMetaType::type( prim_obj_mt->typeName().toAscii().constData() );
        
        if (not type_id)
            throw Siren::program_bug( QObject::tr(
                "Why hasn't the PrimitiveObject<T> for %1 not been "
                "registered with QMetaType?").arg(prim_obj_mt->typeName()), CODELOC );

        Object *obj = static_cast<Object*>( QMetaType::construct(type_id) );
        
        return ObjRef(obj);
    }
    else
    {
        int type_id = QMetaType::type( this->name().toAscii().constData() );
    
        if (not type_id)
            throw Siren::program_bug( QObject::tr(
                    "You cannot create an object of type %1 as it does not "
                    "appear to be registered with QMetaType - has the programmer "
                    "registered this type using RegisterObject<%1>()?")
                        .arg(this->name()), CODELOC );

        Object *obj = static_cast<Object*>( QMetaType::construct(type_id) );
    
        return ObjRef(obj);
    }
}

/** Create a default-constructed object of this class.
    This is only possible if this is a concrete class
    (isConcrete() returns true)
    
    \throw Siren::invalid_operation
*/
HanRef Class::createHandle() const
{
    if (not this->isHandle())
        throw Siren::invalid_operation( QObject::tr(
                "You cannot create a Handle of type %1 as this class "
                "is not derived from Siren::Handle.")
                    .arg(this->name()), CODELOC );

    else if (not isConcrete())
        throw Siren::invalid_operation( QObject::tr(
                "You cannot create an object of type %1 as it is not "
                "concrete (it is virtual).")
                    .arg(this->name()), CODELOC );

    int type_id = QMetaType::type( this->name().toAscii().constData() );
    
    if (not type_id)
        throw Siren::program_bug( QObject::tr(
                "You cannot create an object of type %1 as it does not "
                "appear to be registered with QMetaType - has the programmer "
                "registered this type using RegisterMetaType<%1>()?")
                    .arg(this->name()), CODELOC );

    Handle *handle = static_cast<Handle*>( QMetaType::construct(type_id) );
    
    return HanRef(handle);
}

void Class::stream(Stream &s)
{
    s.assertVersion<Class>(1);
    
    Schema schema = s.item<Class>();
    
    QString class_name = this->name();
    
    schema.data("class_name") & class_name;
    
    super::stream( schema.base() );
    
    if (s.isLoading())
    {
        Class::operator=( Class(class_name) );
    }
}

/** Return whether or not we can create an object of this type */
bool Class::canCreate() const
{
    return this->isConcrete();
}

/** Return whether or not this is a concrete type */
bool Class::isConcrete() const
{
    if (metatype)
        return metatype->isConcrete();
    else
        return true;
}

/** Return whether or not this is a type derived from Siren::Object */
bool Class::isObject() const
{
    if (metatype)
        return metatype->isObject();
    else
        return true;
}

/** Return whether or not this is a type derived from Siren::Handle */
bool Class::isHandle() const
{
    if (metatype)
        return metatype->isHandle();
    else
        return false;
}

/** Return whether or not this is a Primitive type */
bool Class::isPrimitive() const
{
    if (metatype)
        return metatype->isPrimitive();
    else
        return false;
}

/** Return whether or not this class can be cast to 'classname' */
bool Class::canCast(const QString &classname) const
{
    return inherited_types.contains(classname);
}

/** Assert that it is possible to cast from this class to 'classname'

    \throw Siren::invalid_cast
*/
void Class::assertCanCast(const QString &classname) const
{
    if (not this->canCast(classname))
        throw Siren::invalid_cast( QObject::tr(
                "It is not possible to cast from type %1 to type %2.")
                    .arg(this->name(), classname), CODELOC );
}

bool Class::test(Logger &logger) const
{
    Tester tester(*this, logger);
    
    #ifndef SIREN_DISABLE_TESTS
    
    try
    {
        Class c = this->getClass();
    
        // Test 1
        {
            tester.nextTest();
            
            tester.expect_equal( 
                  QObject::tr("Test a c->getClass() is Class(\"Siren::Class\""),
                  CODELOC,
                  c, Class(Class::typeName()) );
        }
        
        // Test 2
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("c->getClass().name() is correct."),
                                 CODELOC,
                                 c.name(), Class::typeName());
        }
        
        // Test 3
        {
            tester.nextTest();
            
            tester.expect_true( QObject::tr("Class::getClass() has a super type."),
                                CODELOC, c.hasSuper() );
            
            Class o = c.superClass();
            
            tester.expect_equal( QObject::tr("Super class is object."),
                                 CODELOC,
                                 o, Class(Object::typeName()) );
        }
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    catch(const std::exception &e)
    {
        tester.unexpected_error( std_exception(e) );
    }
    catch(...)
    {
        tester.unexpected_error( unknown_error(CODELOC) );
    }
    
    #endif // SIREN_DISABLE_TESTS
    
    return tester.allPassed();
}

uint Class::hashCode() const
{
    return qHash( this->name() ); 
}
