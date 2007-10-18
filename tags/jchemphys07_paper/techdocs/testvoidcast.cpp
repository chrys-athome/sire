
/**
  This file contains my ramblings that led to the creation of the second incarnation of the
  NetObjects system (NetPtr, NetPtrProxy, NetPtrWriter, NetWeakPtr etc.)
  
  Simply add 'Net' to the class names, and you have the basis of the system :-)
  
  
*/

#include <stdio.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

template<class T>
T* clone(const T *original)
{
    if (original)
        return new T(*original);
    else
        return 0;
}

template<class T>
T* create(const T *original)
{
    if (original)
        return new T();
    else
        return 0;
}

class PtrProxyBase;    
typedef boost::shared_ptr<PtrProxyBase> PtrProxyPtr;
typedef boost::weak_ptr<PtrProxyBase> PtrProxyWeakPtr;

class PtrProxyBase
{
public:
    PtrProxyBase(void *p = 0) : ptr(p)
    {
            printf("Creating the object (%d)\n",(int)ptr);
    }
    
    virtual ~PtrProxyBase()
    {}
 
    void* voidPointer()
    {
        return ptr;
    }
    
    const void* voidPointer() const
    {
        return ptr;
    }

    virtual PtrProxyPtr clone() const=0;
    virtual PtrProxyPtr create() const=0;        

protected:

    /** Actual pointer to the object */
    void *ptr;
};

template<class T>
PtrProxyPtr registerPtr(T *ptr);

template<class T>
class PtrProxy : public PtrProxyBase
{
public:
    PtrProxy(T *p = 0) : PtrProxyBase(p)
    {}
    
    ~PtrProxy()
    {
        reset();
    }
    
    void reset()
    {
        printf("Deleting the object (%d)\n",(int)ptr);
        delete pointer();
        ptr = 0;
    }
    
    PtrProxyPtr clone() const
    {
        return ::registerPtr<T>( ::clone<T>(pointer()) );
    }
    
    PtrProxyPtr create() const
    {
        return ::registerPtr<T>( ::create<T>(pointer()) );
    }

    const T* pointer() const
    {
        return static_cast<const T*>(voidPointer());
    }

protected:
    
    T* pointer()
    {
        return static_cast<T*>(voidPointer());
    }

};

template<class T>
PtrProxyPtr createProxy(T *p)
{
    if (p)
       return PtrProxyPtr(new PtrProxy<T>(p));
    else
       return PtrProxyPtr();
}

template<class T>
PtrProxyPtr registerPtr(T *p)
{
    return createProxy<T>(p);
}

class WeakPtrBase;

class PtrBase : private PtrProxyPtr
{

friend class WeakPtrBase;

public:

    PtrBase() : PtrProxyPtr()
    {}
    
    ~PtrBase()
    {}
    
    void reset()
    {
        PtrProxyPtr::reset();
    }
    
    bool isNull() const
    {
        return voidPointer() == 0;
    }
    
    bool isValid() const
    {
        return voidPointer() != 0;
    }
    
protected:
    PtrBase(const PtrProxyPtr &other) : PtrProxyPtr(other)
    {}

    PtrBase(const PtrBase &other) : PtrProxyPtr(other)
    {}

    const PtrProxyBase* proxyBase() const
    {
        return PtrProxyPtr::get();
    }

    PtrProxyPtr clone() const
    {
        const PtrProxyBase *proxy = proxyBase();
        if (proxy)
            return proxy->clone();
        else
            return PtrProxyPtr();
    }
    
    PtrProxyPtr create() const
    {
        const PtrProxyBase *proxy = proxyBase();
        if (proxy)
            return proxy->create();
        else
            return PtrProxyPtr();
    }

    void* voidPointer()
    {
        PtrProxyBase *proxy = PtrProxyPtr::get();
        if (proxy)
            return proxy->voidPointer();
        else
            return 0;
    }

    const void* voidPointer() const
    {
        PtrProxyBase *proxy = PtrProxyPtr::get();
        if (proxy)
            return proxy->voidPointer();
        else
            return 0;
    }

};

class invalid_cast
{
public:
    invalid_cast()
    {}
    
    ~invalid_cast()
    {}
};

class WeakPtrPtr;

class PtrPtr : public PtrBase
{

friend class WeakPtrPtr;

public:
    PtrPtr() : PtrBase()
    {}
    
    PtrPtr(const PtrBase &other) : PtrBase(other)
    {}
    
    PtrPtr(const PtrProxyPtr &other) : PtrBase(other)
    {}

    ~PtrPtr()
    {}

    PtrPtr clone() const
    {
        return PtrBase::clone();
    }
    
    PtrPtr create() const
    {
        return PtrBase::create();
    }

    template<class T>
    const T* cast() const
    {
        const PtrProxyBase *proxybase = proxyBase();
        
        if (proxybase)
        {
            const PtrProxy<T> *tproxy = dynamic_cast<const PtrProxy<T>*>(proxybase);
        
            if (tproxy)
                return tproxy->pointer();
            else
                throw invalid_cast();
        }
        else
            return 0;
    }
    
    template<class D, class T>
    const D* cast() const
    {
        const PtrProxyBase *proxybase = proxyBase();
        
        if (proxybase)
        {
            const PtrProxy<T> *tproxy = dynamic_cast<const PtrProxy<T>*>(proxybase);
            
            if (tproxy)
            {
                const T *tptr = tproxy->pointer();
                
                if (tptr)
                {
                    const D *dptr = dynamic_cast<const D*>(tptr);
                    
                    if (dptr)
                        return dptr;
                    else
                        throw invalid_cast();
                }
                else
                    return 0;
            }
            else
                throw invalid_cast();
        }
        else
            return 0;
    }
};

class WeakPtrBase : private PtrProxyWeakPtr
{
public:
    WeakPtrBase() : PtrProxyWeakPtr()
    {}
    
    ~WeakPtrBase()
    {}
    
    void reset()
    {
        PtrProxyWeakPtr::reset();
    }

    bool expired() const
    {
        return PtrProxyWeakPtr::expired();
    }
    
    int use_count() const
    {
        return PtrProxyWeakPtr::use_count();
    }
    
protected:
    WeakPtrBase(const PtrProxyPtr &other) : PtrProxyWeakPtr(other)
    {}

    WeakPtrBase(const PtrBase &other) : PtrProxyWeakPtr(other)
    {}

    PtrProxyPtr proxyBase() const
    {
        return PtrProxyWeakPtr::lock();
    }
        
};

class WeakPtrPtr : public WeakPtrBase
{
public:
    WeakPtrPtr() : WeakPtrBase()
    {}
    
    WeakPtrPtr(const PtrBase &ptr) : WeakPtrBase(ptr)
    {}
    
    WeakPtrPtr(const WeakPtrBase &other) : WeakPtrBase(other)
    {}
    
    ~WeakPtrPtr()
    {}
    
    PtrPtr lock() const
    {
        return PtrPtr( proxyBase() );
    }
};

template<class T>
class WeakPtr;

class WeakPtrPtr;

template<class T>
class Ptr : public PtrBase
{

friend class WeakPtr<T>;

public:
    Ptr(T *p = 0) : PtrBase( ::registerPtr(p) )
    {}

    Ptr(const Ptr<T> &other) : PtrBase(other)
    {}
    
    Ptr(const PtrBase &ptr) : PtrBase(ptr)
    {
        checkSanity();
    }
    
    Ptr(const WeakPtr<T> &ptr) : PtrBase(ptr.lock())
    {}
    
    Ptr(const WeakPtrPtr &ptr) : PtrBase(ptr.lock())
    {
        checkSanity();
    }
    
    ~Ptr()
    {}
        
    const T* pointer() const
    {
        return static_cast<const T*>(voidPointer());
    }
    
    const T* get() const
    {
        return pointer();
    }
    
    const T& operator*() const
    {
        return *(pointer());
    }
    
    const T* operator->() const
    {
        return pointer();
    }

    Ptr<T> clone() const
    {
        return PtrBase::clone();
    }
    
    Ptr<T> create() const
    {
        return PtrBase::clone();
    }

protected:
    
    //constructor used by WeakPtr<T>
    Ptr(const PtrProxyPtr &other) : PtrBase(other)
    {}

    void checkSanity()
    {
        //ensure that PtrBase is pointing to a PtrProxy<T>
        const PtrProxyBase *proxybase = proxyBase();
        
        if (proxybase)
        {
            const PtrProxy<T> *proxy = dynamic_cast<const PtrProxy<T>*>(proxybase);
            
            if (not proxy)
                throw invalid_cast();
        }
    }
};

template<class T>
class PtrWriter : public Ptr<T>
{
public:
    PtrWriter() : Ptr<T>()
    {}
    
    PtrWriter(const Ptr<T> &other) : Ptr<T>(other)
    {}
    
    ~PtrWriter()
    {}
        
    T* pointer()
    {
        return static_cast<T*>(Ptr<T>::voidPointer());
    }
    
    T* get()
    {
        return pointer();
    }
    
    T& operator*()
    {
        return *(pointer());
    }
    
    T* operator->()
    {
        return pointer();
    }

    void commit()
    {
        Ptr<T>::reset();
    }
};    

template<class D, class T>
class DerivedPtr : public Ptr<T>
{
public:
    DerivedPtr(D *ptr=0) : Ptr<T>(ptr)
    {}
    
    DerivedPtr(const DerivedPtr<D,T> &other) : Ptr<T>(other)
    {}

    DerivedPtr(const Ptr<T> &other) : Ptr<T>(other)
    {
        checkSanity();
    }

    DerivedPtr(const PtrPtr &other) : Ptr<T>(other)
    {
        checkSanity();
    }
    
    DerivedPtr(const WeakPtr<T> &other) : Ptr<T>(other.lock())
    {
        checkSanity();
    }
    
    DerivedPtr(const WeakPtrPtr &other) : Ptr<T>(other.lock())
    {
        checkSanity();
    }
    
    const D* pointer() const
    {
        return static_cast<const D*>(Ptr<T>::voidPointer());
    }
    
    const D* get() const
    {
        return pointer();
    }
    
    const D& operator*() const
    {
        return *(pointer());
    }
    
    const D* operator->() const
    {
        return pointer();
    }

protected:
    void checkSanity()
    {
        //ensure that we are pointing to a D*
        const T *tptr = Ptr<T>::pointer();
        
        if (tptr)
        {
            const D *dptr = dynamic_cast<const D*>(tptr);
            if (!dptr)
                throw invalid_cast();
        }
    }
};

template<class D, class T>
class DerivedPtrWriter : public DerivedPtr<D,T>
{
public:
    DerivedPtrWriter() : DerivedPtr<D,T>()
    {}
    
    DerivedPtrWriter(const DerivedPtr<D,T> &ptr) : DerivedPtr<D,T>(ptr)
    {}
    
    ~DerivedPtrWriter()
    {}
    
    D* pointer()
    {
        return static_cast<D*>(DerivedPtr<D,T>::voidPointer());
    }
    
    D* get()
    {
        return pointer();
    }
    
    D& operator*()
    {
        return *(pointer());
    }
    
    D* operator->()
    {
        return pointer();
    }

    void commit()
    {
        DerivedPtr<D,T>::reset();
    }
};    
    
template<class T>
class WeakPtr : public WeakPtrBase
{
public:
    WeakPtr() : WeakPtrBase()
    {}
    
    WeakPtr(const Ptr<T> &ptr) : WeakPtrBase(ptr)
    {}
    
    WeakPtr(const PtrPtr &ptr) : WeakPtrBase(Ptr<T>(ptr))
    {}
    
    WeakPtr(const WeakPtrPtr &ptr) : WeakPtrBase(Ptr<T>(ptr.lock()))
    {}
    
    WeakPtr(const PtrProxyPtr &ptr) : WeakPtrBase(Ptr<T>(ptr))
    {}
    
    ~WeakPtr()
    {}
    
    Ptr<T> lock() const
    {
        return Ptr<T>( proxyBase() );
    }
};   

class Cat
{

friend PtrProxyPtr registerPtr<Cat>(Cat*);

public:
    Cat() : nme("Sylvestor")
    {}
    
    Cat(const char *name) : nme(name)
    {}
    
    virtual ~Cat()
    {}
    
    void setName(const char *newname)
    {
        nme = newname;
    }
    
    virtual void hello() const
    {
        printf("%s says Mieow!\n", nme);
    }

    virtual Cat* clone() const
    {
        return new Cat(*this);
    }

    virtual Cat* createNew() const
    {
        return new Cat();
    }

    const char* name() const
    {
        return nme;
    }

    Ptr<Cat> self() const
    {
        return selfptr.lock();
    }

private:
    const char *nme;
    WeakPtr<Cat> selfptr;

};

class Tiger : public Cat
{
public:
    Tiger() : Cat()
    {}

    Tiger(const char *name) : Cat(name)
    {}
    
    ~Tiger()
    {}
    
    Cat* clone() const
    {
        return new Tiger(*this);
    }
    
    Cat* createNew() const
    {
        return new Tiger();
    }
    
    void hello() const
    {
        printf("%s growls!\n", name());
    }
};

class Lion : public Cat
{
public:
    Lion() : Cat()
    {}
    
    Lion(const char *name) : Cat(name)
    {}
    
    ~Lion()
    {}
    
    
    Cat* clone() const
    {
        return new Lion(*this);
    }
    
    Cat* createNew() const
    {
        return new Lion();
    }
    
    void hello() const
    {
        printf("%s is the king! Hear me roar!\n", name());
    }
};

template<>
Cat* clone(const Cat *original)
{
    if (original)
        return original->clone();
    else
        return 0;
}

template<>
Cat* create(const Cat *original)
{
    if (original)
        return original->createNew();
    else
        return 0;
}

template<>
PtrProxyPtr registerPtr(Cat *cat)
{
    if (cat)
    {
        //get the proxy
        PtrProxyPtr proxy = createProxy<Cat>(cat);
    
        //give a weak pointer to the proxy to the cat
        cat->selfptr = WeakPtr<Cat>(proxy);
        
        return proxy;
    }
    else
        return PtrProxyPtr();
}

int main(void)
{
    float *f = new float(3.4);
    printf("float = (%d)\n", (int)f);

    Ptr<float> fptr( f );
    
    Ptr<Cat> catptr( new Cat("Snowy") );  
    
    printf("%f\n", *fptr);
    
    catptr->hello();

    PtrPtr ptrptr = catptr;

    catptr.reset();
    
    DerivedPtr<Tiger,Cat> tig( new Tiger("Stripy") );
    
    tig->hello();
    
    catptr = tig;
    
    catptr->hello();
    
    WeakPtrPtr weakptr = catptr;

    printf("expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());

    catptr = 0;

    printf("expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());
    
    catptr = weakptr;
    catptr->hello();
    
    tig = 0;
    printf("expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());
    
    tig = weakptr;
    printf("expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());
    tig->hello();

    weakptr = tig;
    printf("expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());

    DerivedPtr<Tiger,Cat> tig2 = tig.clone();
    tig2->hello();
    
    catptr = tig2.clone();
    catptr->hello();
    
    tig2 = catptr.create();
    tig2->hello();

    DerivedPtrWriter<Tiger,Cat> tigwriter = tig2;
    tigwriter->setName("Tigger");
    tigwriter.commit();
    tig2->hello();
    
    catptr = catptr.clone();
    catptr->hello();
    
    catptr = 0;
    tig.reset();
    
    printf("expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());

    catptr = weakptr;
    printf("Cat is expired? %d\n", catptr.isNull());    

    catptr = ptrptr;
    catptr->hello();
    
    weakptr = ptrptr;
    printf("expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());

    WeakPtr<Cat> weakcatptr = weakptr;
    printf("expired = %d, use_count = %d\n", weakcatptr.expired(), weakcatptr.use_count());

    catptr.reset();
    printf("expired = %d, use_count = %d\n", weakcatptr.expired(), weakcatptr.use_count());

    catptr = weakcatptr;
    catptr->hello();

    Ptr<Cat> cat2 = catptr->self();
    cat2->hello();

    Ptr<Cat> cat3 = cat2.create();
    cat3->hello();

    ptrptr = fptr;

    PtrPtr ptrptr2 = ptrptr.clone();
    printf("f = %f\n", *(ptrptr2.cast<float>()));
    ptrptr2 = ptrptr2.create();
    printf("f = %f\n", *(ptrptr2.cast<float>()));
    
    weakptr = fptr;
    fptr.reset();

    PtrWriter<Cat> catwriter = cat3;
    catwriter->setName("Snooky");
    cat3->hello();

    PtrPtr catptrptr = cat3.clone();
    catptrptr.cast<Cat>()->hello();

    Ptr<Cat> cat4 = new Tiger("Michael");
    catptrptr = cat4;
    catptrptr.cast<Cat>()->hello();
    
    DerivedPtr<Tiger,Cat> tigerptr = catptrptr;
    tigerptr->hello();
    
    catptrptr.cast<Tiger,Cat>()->hello();

    printf("Cast to cat = \"%s\"\n", typeid(catptrptr.cast<Cat>()).name());
    printf("Cast to tiger = \"%s\"\n", typeid(catptrptr.cast<Tiger,Cat>()).name());
    
    DerivedPtr<Lion,Cat> lionptr = new Lion("Leo");
    catptrptr = lionptr;
    
    catptrptr.cast<Lion,Cat>()->hello();

    cat3 = lionptr;
    cat3->hello();
    
    catptr.reset();
    printf("expired = %d, use_count = %d\n", weakcatptr.expired(), weakcatptr.use_count());
    
    printf("fptr expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());        
    ptrptr.reset();
    printf("fptr expired = %d, use_count = %d\n", weakptr.expired(), weakptr.use_count());        

    return 0;
}
