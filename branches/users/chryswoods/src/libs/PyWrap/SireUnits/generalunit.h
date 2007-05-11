#ifndef PYWRAP_SIREUNITS_GENERALUNIT_H
#define PYWRAP_SIREUNITS_GENERALUNIT_H

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{

class GeneralUnit
{
public:
    GeneralUnit();

    template<int M, int L, int T, int C, int t, int Q>
    GeneralUnit(const PhysUnit<M,L,T,C,t,Q> &unit)
    {
        value = unit;
        Mass = M;
        Length = L;
        Time = T;
        Charge = C;
        temperature = t;
        Quantity = Q;
    }
    
    GeneralUnit(const GeneralUnit &other);
    
    ~GeneralUnit();

    int M() const;
    int L() const;
    int T() const;
    int C() const;
    int t() const;
    int Q() const;

    template<int M, int L, int T, int C, int t, int Q>
    PhysUnit<M,L,T,C,t,Q> convert() const
    {
        BOOST_ASSERT( M == Mass );
        BOOST_ASSERT( L == Length );
        BOOST_ASSERT( T == Time );
        BOOST_ASSERT( C == Charge );
        BOOST_ASSERT( t == temperature );
        BOOST_ASSERT( Q == Quantity );
    
        return PhysUnit<M,L,T,C,t,Q>( value );
    }

    GeneralUnit& operator=(const GeneralUnit &other);
    
    bool operator==(const GeneralUnit &other) const;
    
    bool operator!=(const GeneralUnit &other) const;
    
    GeneralUnit operator-() const;
    
    GeneralUnit& operator+=(const GeneralUnit &other);
    
    GeneralUnit& operator-=(const GeneralUnit &other);
    
    GeneralUnit operator+(const GeneralUnit &other) const;
    
    GeneralUnit operator-(const GeneralUnit &other) const;

    GeneralUnit operator*=(const GeneralUnit &other);

    GeneralUnit operator/=(const GeneralUnit &other);
    
    GeneralUnit operator*(const GeneralUnit &other) const;
    
    GeneralUnit operator/(const GeneralUnit &other) const;
    
    GeneralUnit& operator*=(double val);
    
    GeneralUnit& operator/=(double val);
    
    GeneralUnit operator*(double val) const;
    
    GeneralUnit operator/(double val) const;
    
    operator double() const;

private:
    void assertCompatible(const GeneralUnit &other) const;

    double value;
    int Mass, Length, Time, Charge, temperature, Quantity;
};

template<class D>
struct from_general_unit
{
    /** Constructor - register the conversion functions
        for this type */
    from_general_unit()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            type_id< D >());
    }

    /** Test whether or not it is possible to convert the PyObject
        holding a GeneralUnit to this specific PhysUnit*/
    static void* convertible(PyObject* obj_ptr)
    {
        object obj( handle<>(borrowed(obj_ptr)) );
    
        extract<GeneralUnit> x(obj); 
    
        //is this a GeneralUnit?
        if ( x.check() )
        {
            //it is ;-)  Get a reference to it and make sure
            //that it is of the right dimension
            GeneralUnit &gen_unit = x();
            
            if ( gen_unit.M() == D::M() and
                 gen_unit.L() == D::L() and
                 gen_unit.T() == D::T() and
                 gen_unit.C() == D::C() and
                 gen_unit.t() == D::t() and
                 gen_unit.Q() == D::Q() )
            {
                //this has the right dimension :-)
                return obj_ptr;
            }
        }
  
        //could not recognise the type or the dimension was wrong
        return 0;
    }

    /** Construct a PhysUnit from the passed GeneralUnit */
    static void construct(
        PyObject* obj_ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        object obj( handle<>(borrowed(obj_ptr)) );
    
        extract<GeneralUnit> x(obj); 
    
        //is this a GeneralUnit?
        if ( x.check() )
        {
            //it is ;-)  Get a reference to it and make sure
            //that it is of the right dimension
            GeneralUnit &gen_unit = x();
            
            if ( gen_unit.M() == D::M() and
                 gen_unit.L() == D::L() and
                 gen_unit.T() == D::T() and
                 gen_unit.C() == D::C() and
                 gen_unit.t() == D::t() and
                 gen_unit.Q() == D::Q() )
            {
                //locate the storage space for the result
                void* storage =
                    ( (converter::rvalue_from_python_storage<D>*)data )->storage.bytes;

                //create the T container
                new (storage) D( double(gen_unit) );
                
                data->convertible = storage;
            }
        }
    }
};    

template<class D>
struct to_general_unit
{
    static PyObject* convert(const D &unit)
    {
        return incref( object(GeneralUnit(unit)).ptr() );
    }
};

template<class D>
void register_unit()
{
    typedef typename PhysUnit<M,L,T,C,t,Q> D;

    to_python_converter< D, to_general_unit<D> >();

    converter::registry::push_back( 
          &from_general_unit<D>::convertible,
          &from_general_unit<D>::construct,
          type_id<D>() );
}


}

SIRE_END_HEADER

#endif
