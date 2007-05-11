#ifndef PYWRAP_SIREUNITS_GENERALUNIT_H
#define PYWRAP_SIREUNITS_GENERALUNIT_H

#ifndef SKIP_TEMPLATE_DEFINITIONS
#include <Python.h>
#include <boost/python.hpp>
#endif

#include <QString>

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

#ifndef SKIP_TEMPLATE_DEFINITIONS
namespace bp = boost::python;
#endif

namespace SireUnits
{

namespace Dimension
{

class GeneralUnit
{
public:
    GeneralUnit();

    template<class D>
    explicit GeneralUnit(const D &unit)
    {
        value = unit;
        
        Mass = D::MASS();
        Length = D::LENGTH();
        Time = D::TIME();
        Charge = D::CHARGE();
        temperature = D::TEMPERATURE();
        Quantity = D::QUANTITY();
        Angle = D::ANGLE();
    }
    
    GeneralUnit(const GeneralUnit &other);
    
    ~GeneralUnit();

    int MASS() const;
    int LENGTH() const;
    int TIME() const;
    int CHARGE() const;
    int TEMPERATURE() const;
    int QUANTITY() const;
    int ANGLE() const;

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
    
    GeneralUnit& operator*=(int val);
    GeneralUnit& operator/=(int val);
    
    GeneralUnit operator*(double val) const;
    GeneralUnit operator/(double val) const;
    
    GeneralUnit operator*(int val) const;
    GeneralUnit operator/(int val) const;
    
    GeneralUnit invert() const;
    
    double to(const GeneralUnit &other) const;
    
    double scaleFactor() const;
    
    double convertToInternal(double value) const;
    double convertFromInternal(double value) const;
    
    QString toString() const;

private:
    void assertCompatible(const GeneralUnit &other) const;

    double value;
    int Mass, Length, Time, Charge, temperature, Quantity, Angle;
};

inline GeneralUnit operator*(double val, const GeneralUnit &unit)
{
    return unit * val;
}

inline GeneralUnit operator*(int val, const GeneralUnit &unit)
{
    return unit * val;
}

inline GeneralUnit operator/(double val, const GeneralUnit &unit)
{
    return unit.invert() * val;
}

inline GeneralUnit operator/(int val, const GeneralUnit &unit)
{
    return unit.invert() * val;
}

} // end of namespace Dimension

#ifndef SKIP_TEMPLATE_DEFINITIONS

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
            bp::type_id< D >());
    }

    /** Test whether or not it is possible to convert the PyObject
        holding a GeneralUnit to this specific PhysUnit*/
    static void* convertible(PyObject* obj_ptr)
    {
        bp::object obj( bp::handle<>(bp::borrowed(obj_ptr)) );
    
        bp::extract<const Dimension::GeneralUnit&> x(obj); 
    
        //is this a GeneralUnit?
        if ( x.check() )
        {
            //it is ;-)  Get a reference to it and make sure
            //that it is of the right dimension
            const Dimension::GeneralUnit &gen_unit = x();
            
            if ( gen_unit.MASS() == D::MASS() and
                 gen_unit.LENGTH() == D::LENGTH() and
                 gen_unit.TIME() == D::TIME() and
                 gen_unit.CHARGE() == D::CHARGE() and
                 gen_unit.TEMPERATURE() == D::TEMPERATURE() and
                 gen_unit.QUANTITY() == D::QUANTITY() and
                 gen_unit.ANGLE() == D::ANGLE() )
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
        bp::object obj( bp::handle<>(bp::borrowed(obj_ptr)) );
    
        bp::extract<const Dimension::GeneralUnit&> x(obj); 
    
        //is this a GeneralUnit?
        if ( x.check() )
        {
            //it is ;-)  Get a reference to it and make sure
            //that it is of the right dimension
            const Dimension::GeneralUnit &gen_unit = x();
            
            if ( gen_unit.MASS() == D::MASS() and
                 gen_unit.LENGTH() == D::LENGTH() and
                 gen_unit.TIME() == D::TIME() and
                 gen_unit.CHARGE() == D::CHARGE() and
                 gen_unit.TEMPERATURE() == D::TEMPERATURE() and
                 gen_unit.QUANTITY() == D::QUANTITY() and
                 gen_unit.ANGLE() == D::ANGLE() )
            {
                //locate the storage space for the result
                void* storage =
                    ( (bp::converter::rvalue_from_python_storage<D>*)data )->storage.bytes;

                //create the T container
                new (storage) D( gen_unit.scaleFactor() );
                
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
        return bp::incref( bp::object(Dimension::GeneralUnit(unit)).ptr() );
    }
};

template<class D>
void register_dimension()
{
    bp::to_python_converter< D, to_general_unit<D> >();

    bp::converter::registry::push_back( 
          &from_general_unit<D>::convertible,
          &from_general_unit<D>::construct,
          bp::type_id<D>() );
}

#endif // 'SKIP_TEMPLATE_DEFINITIONS'

}

SIRE_END_HEADER

#endif
