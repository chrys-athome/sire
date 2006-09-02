

class StreamedObject
{
public:
    StreamedObject()
    {}
    
    virtual ~StreamedObject()
    {}
    
    template<class T>
    const T& asA() const
    {
        const StreamedObjectWrapper<T> *wrapper = 
                   dynamic_cast<const StreamedObjectWrapper<T>*>(this);
                   
        if (wrapper)
        {
            return wrapper->contents();
        }
    }
};

template<class T>
class StreamedObjectWrapper : public StreamedObject
{
public:
    StreamedObjectWrapper();
    ~StreamedObjectWrapper();
    
    const T& contents() const;
    
private:
    /** Object contained by this wrapper */
    T obj;
};
