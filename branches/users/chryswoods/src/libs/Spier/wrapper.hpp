#ifndef SPIER_WRAPPER_HPP
#define SPIER_WRAPPER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

void throwNullPtrError();

template<class T>
class Wrapper;

template<class T>
Wrapper<T> unlockWrapper( const boost::weak_ptr<T> &weakptr );

/** This template class is used to wrap up the GLWidgetBase (and derived) classes,
    so as to provide a simple to use explicit sharing scheme.
    
    @author Christopher Woods
*/
template<class T>
class Wrapper : private boost::shared_ptr<T>
{

// Copied from boost/shared_ptr
private:
    // Borland 5.5.1 specific workaround
    typedef boost::shared_ptr<T> this_type;
// Copied from boost/shared_ptr


template<class D>
friend class Wrapper;

friend Wrapper<T> unlockWrapper<T>(const boost::weak_ptr<T> &weakptr);

public:
    Wrapper() : boost::shared_ptr<T>()
    {}
    
    Wrapper(T *ptr) : boost::shared_ptr<T>( boost::dynamic_pointer_cast<T>(ptr->self()) )
    {}
    
    Wrapper(const Wrapper<T> &other) : boost::shared_ptr<T>(other)
    {}
    
    template<class D>
    Wrapper(const Wrapper<D> &other) : boost::shared_ptr<T>(other)
    {}
    
    ~Wrapper()
    {}
    
    Wrapper<T>& operator=(T *ptr)
    {
        boost::shared_ptr<T>::operator=( boost::dynamic_pointer_cast<T>(ptr->self()) );
        return *this;
    }
    
    static Wrapper<T> create()
    {
        return Wrapper<T>(new T());
    }
    
    static Wrapper<T> create(const Wrapper<T> &parent)
    {
        return Wrapper<T>(new T(parent));
    }
    
    bool isValid() const
    {
        return boost::shared_ptr<T>::get() != 0;
    }
    
    boost::weak_ptr<T> weakPointer() const
    {
        return boost::weak_ptr<T>(*this);
    }
    
    template<class D>
    bool operator==(const Wrapper<D> &other) const
    {
        return this->get() == other.get();
    }
    
    template<class D>
    bool operator!=(const Wrapper<D> &other) const
    {
        return this->get() != other.get();
    }
    
    T* operator->()
    {
        if (not this->isValid())
            throwNullPtrError();
          
        return boost::shared_ptr<T>::operator->();
    }
    
    template<class D>
    Wrapper<D> dynamicCast() const
    {
        return Wrapper<D>(boost::dynamic_pointer_cast<D>(*this));
    }
    
    operator T*()
    {
        return this->get();
    }
    
    //These conversions to 'bool' are strongly based on those in boost/shared_ptr.hpp
    // implicit conversion to "bool"
#if defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x530)

    operator bool () const
    {
        this->get() != 0;
    }

#elif \
    ( defined(__MWERKS__) && BOOST_WORKAROUND(__MWERKS__, < 0x3200) ) || \
    ( defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ < 304) )

    typedef T * (this_type::*unspecified_bool_type)() const;
    
    operator unspecified_bool_type() const // never throws
    {
        return this->get() == 0? 0: &this_type::get;
    }

#else 

    typedef T * this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const // never throws
    {
        return this->get() == 0? 0: &this_type::px;
    }

#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const // never throws
    {
        return this->get() == 0;
    }
// end of copying from boost/shared_ptr
    
    T* get()
    {
        return boost::shared_ptr<T>::get();
    }
    
    const T* get() const
    {
        return boost::shared_ptr<T>::get();
    }
    
private:

    Wrapper(const boost::shared_ptr<T> &other) : boost::shared_ptr<T>(other)
    {}
};

template<class T>
Wrapper<T> unlockWrapper(const boost::weak_ptr<T> &weakptr)
{
    return Wrapper<T>(weakptr.lock());
}

}

SIRE_END_HEADER

#endif
