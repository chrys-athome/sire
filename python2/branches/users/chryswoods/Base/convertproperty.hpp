#ifndef CONVERTPROPERTY_HPP
#define CONVERTPROPERTY_HPP

#include "SireBase/sharedpolypointer.hpp"
#include "SireBase/property.h"

template<class P, class Base>
struct property_to_base_object
{
    static PyObject* convert(const P &property_container)
    {
        return incref( object( SireBase::SharedPolyPointer<Base>(property_container.read()) ).ptr() );
    }
};

template<class P, class Base>
void register_property_container()
{
    to_python_converter< P, property_to_base_object<P,Base> >();

    implicitly_convertible< Base, P >();

    register_ptr_to_python< SireBase::SharedPolyPointer<Base> >();
}

#endif

