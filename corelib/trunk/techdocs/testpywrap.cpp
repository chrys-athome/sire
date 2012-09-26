
#include <Python.h>

#include <boost/python.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/mpl/if.hpp>

#include <iostream>

using namespace std;
using namespace boost::python;

class A
{
public:
    A() : val(0.0)
    {}
    
    ~A()
    {}
    
    float& value()
    {
        return val;
    }
    
    void print() const
    {
        cout << "A == " << val << endl;
    }
    
private:
    float val;
};

template<class T>
struct pod_wrapper
{
    float value(T &obj)
    {
        return obj.value();
    }

    template<class WrapperType>
    static void wrap_funcs( WrapperType &wrapper )
    {
        wrapper.def( "value", &pod_wrapper<T>::value );
    }
};

template<class T>
struct nonpod_wrapper
{
    template<class WrapperType>
    static void wrap_funcs( WrapperType &wrapper )
    {
        wrapper.def( "value", &T::value,
           return_internal_reference<1,with_custodian_and_ward<1,2> >() );
    }
};

void export_A()
{
    class_<A> wrapper("A", init<>());

    wrapper
        .def( "print", &A::print )
    ;

    boost::mpl::if_< boost::is_pod<float>,
                     pod_wrapper<A>,
                     nonpod_wrapper<A> >::type::wrap_funcs(wrapper);
}

template<class T>
struct get_reference_return_policy
{
    typedef typename boost::mpl::if_< boost::is_pod<T>,
                             return_value_policy<copy_non_const_reference>,
                             return_internal_reference<1,
                                with_custodian_and_ward<1,2> > >::type type;
};

void export_A2()
{
    class_<A> wrapper("A", init<>());
    
    wrapper.def("value", &A::value, 
                    get_reference_return_policy<double>::type() );
}

int main(void)
{
    export_A();
    return 0;
}
