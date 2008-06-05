
#include <iostream>

using namespace std;

namespace Dimension
{

class Unit
{
public:
    ~Unit()
    {}
    
    operator double() const
    {
        return sclfac;
    }
    
    double scaleFactor() const
    {
        return sclfac;
    }

protected:
    Unit(double scale_factor) : sclfac(scale_factor)
    {}
    
    void setScale(double scale_factor)
    {
        sclfac = scale_factor;
    }
    
private:
    double sclfac;
};

template<int M, int L, int T,
         int C, int t>
class ConvertedUnit;

/** Construct a physical unit with the specified
    Mass, Length, Time, Charge and temperature 
    dimensions 
    
    @author Christopher Woods
*/
template<int M, int L, int T,
         int C, int t>
class PhysUnit : public Unit
{
public:
    PhysUnit() : Unit(1)
    {}
    
    explicit PhysUnit(double scale_factor)
               : Unit(scale_factor)
    {}
    
    PhysUnit(const PhysUnit<M,L,T,C,t> &other)
               : Unit(other)
    {}
    
    ~PhysUnit()
    {}
    
    PhysUnit<M,L,T,C,t>
    operator=(const PhysUnit<M,L,T,C,t> &other)
    {
        Unit::setScale(other.scaleFactor());
        return *this;
    }

    bool operator==(const PhysUnit<M,L,T,C,t> &other) const
    {
        return scaleFactor() == other.scaleFactor();
    }
    
    bool operator!=(const PhysUnit<M,L,T,C,t> &other) const
    {
        return scaleFactor() != other.scaleFactor();
    }

    PhysUnit<M,L,T,C,t>
    operator+(const PhysUnit<M,L,T,C,t> &other) const
    {
        return PhysUnit<M,L,T,C,t>(
                  scaleFactor() + other.scaleFactor());
    }
    
    PhysUnit<M,L,T,C,t>
    operator-(const PhysUnit<M,L,T,C,t> &other) const
    {
        return PhysUnit<M,L,T,C,t>(
                  scaleFactor() - other.scaleFactor());
    }

    PhysUnit<M,L,T,C,t>&
    operator+=(const PhysUnit<M,L,T,C,t> &other)
    {
        Unit::setScale( scaleFactor() + other.scaleFactor() );
    }
    
    PhysUnit<M,L,T,C,t>&
    operator-=(const PhysUnit<M,L,T,C,t> &other)
    {
        Unit::setScale( scaleFactor() - other.scaleFactor() );
    }
    
    PhysUnit<M,L,T,C,t> operator*(double val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() * val);
    }
    
    PhysUnit<M,L,T,C,t> operator/(double val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() / val);
    }
    
    PhysUnit<M,L,T,C,t> operator*(int val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() * val);
    }
    
    PhysUnit<M,L,T,C,t> operator/(int val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() / val);
    }

    template<int _M, int _L, int _T,
             int _C, int _t>
    PhysUnit<M+_M, L+_L, T+_T, C+_C, t+_t>
    operator*(const PhysUnit<_M,_L,_T,_C,_t> &other) const
    {
        return PhysUnit<M+_M,L+_L,T+_T,C+_C,t+_t>(
                  scaleFactor() * other.scaleFactor());
    }
    
    template<int _M, int _L, int _T,
             int _C, int _t>
    PhysUnit<M-_M, L-_L, T-_T, C-_C, t-_t>
    operator/(const PhysUnit<_M,_L,_T,_C,_t> &other) const
    {
        return PhysUnit<M-_M,L-_L,T-_T,C-_C,t-_t>(
                  scaleFactor() / other.scaleFactor());
    }

    ConvertedUnit<M,L,T,C,t> 
    in(const PhysUnit<M,L,T,C,t> &units) const
    {
        return ConvertedUnit<M,L,T,C,t>(*this, units);
    }
    
    template<class U>
    U in(const U &units) const;
};

template<int M, int L, int T, int C, int t>
PhysUnit<M,L,T,C,t> 
operator*(double val, const PhysUnit<M,L,T,C,t> &unit)
{
    return PhysUnit<M,L,T,C,t>( val * unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t>
PhysUnit<M,L,T,C,t> 
operator*(int val, const PhysUnit<M,L,T,C,t> &unit)
{
    return PhysUnit<M,L,T,C,t>( val * unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t>
class ConvertedUnit : public PhysUnit<M,L,T,C,t>
{
public:
    ConvertedUnit() : PhysUnit<M,L,T,C,t>(), val(1)
    {}
    
    ConvertedUnit(const PhysUnit<M,L,T,C,t> &value, 
                  const PhysUnit<M,L,T,C,t> &units)
           : PhysUnit<M,L,T,C,t>(value)
    {
        val = value.scaleFactor() / units.scaleFactor();
    }

    ConvertedUnit(const ConvertedUnit<M,L,T,C,t> &other)
           : PhysUnit<M,L,T,C,t>(other), val(other.val)
    {}
    
    ~ConvertedUnit()
    {}
    
    ConvertedUnit<M,L,T,C,t>&
    operator=(const ConvertedUnit<M,L,T,C,t> &other)
    {
        val = other.val;
        PhysUnit<M,L,T,C,t>::operator=(other);
        return *this;
    }

    ConvertedUnit<M,L,T,C,t>&
    operator=(const PhysUnit<M,L,T,C,t> &other)
    {
        val = 1;
        PhysUnit<M,L,T,C,t>::operator=(other);
        return *this;
    }
    
    operator double() const
    {
        return val;
    }
    
private:
    double val;
};

typedef PhysUnit<0,0,0,0,0> Quantity;
typedef PhysUnit<1,0,0,0,0> Mass;
typedef PhysUnit<0,1,0,0,0> Length;
typedef PhysUnit<0,0,1,0,0> Time;
typedef PhysUnit<0,0,0,1,0> Charge;
typedef PhysUnit<0,0,0,0,1> Temperature;

typedef PhysUnit<0,2,0,0,0> Area;
typedef PhysUnit<0,3,0,0,0> Volume;

typedef PhysUnit<0,1,-1,0,0> Velocity;

typedef PhysUnit<1,2,-2,0,0> Energy;

typedef PhysUnit<1,-3,0,0,0> Density;

typedef PhysUnit<1,1,-2,0,0> Force;

typedef PhysUnit<1,-1,-2,0,0> Pressure;

typedef PhysUnit<0,0,-1,1,0> ElecCurrent;
typedef PhysUnit<1,2,-2,-1,0> ElecPotential;

/** Specialisation of the temperature units */
class Celsius;
class Fahrenheit;

class TempBase
{
friend class Celsius;
friend class Fahrenheit;

public:
    TempBase(double value = 0) : val(value)
    {}
    
    TempBase(const TempBase &other) : val(other.val)
    {}
    
    virtual ~TempBase()
    {}

    operator Temperature() const
    {
        return Temperature( convertToInternal() );
    }
    
    operator double() const
    {
        return val;
    }

protected:
    TempBase& operator=(const TempBase &other)
    {
        val = other.val;
        return *this;
    }

    virtual double convertToInternal() const=0;
    virtual void convertFromInternal(double value)=0;

    double val;
};

class Celsius : public TempBase
{
protected:
    double convertToInternal() const
    {
        return val + 273.15;
    }
    
    void convertFromInternal(double value)
    {
        val = value - 273.15;
    }

public:
    Celsius() : TempBase(1)
    {}

    explicit Celsius(double value) : TempBase(value)
    {}

    Celsius(const Temperature &temp) : TempBase()
    {
        convertFromInternal(temp);
    }

    Celsius(const TempBase &other) : TempBase()
    {
        convertFromInternal( other.convertToInternal() );
    }

    Celsius(const Celsius &other)
          : TempBase(other)
    {}

    ~Celsius()
    {}
    
    Celsius& operator=(const Celsius &other)
    {
        TempBase::operator=(other);
        return *this;
    }

    Celsius& operator=(const Temperature &temp)
    {
        convertFromInternal(temp);
        return *this;
    }
    
    bool operator==(const Celsius &other) const
    {
        return val == other.val;
    }
    
    bool operator!=(const Celsius &other) const
    {
        return val != other.val;
    }

    Celsius operator+(const Celsius &other) const
    {
        return Celsius(val + other.val);
    }
    
    Celsius operator-(const Celsius &other) const
    {
        return Celsius(val - other.val);
    }
    
    Celsius& operator+=(const Celsius &other)
    {
        val += other.val;
        return *this;
    }
    
    Celsius& operator-=(const Celsius &other)
    {
        val -= other.val;
        return *this;
    }
    
    Celsius operator+(const Temperature &other) const
    {
        return *this + Celsius(other);
    }
    
    Celsius operator-(const Temperature &other) const
    {
        return *this - Celsius(other);
    }

    Celsius& operator+=(const Temperature &other)
    {
        val += Celsius(other);
        return *this;
    }

    Celsius& operator-=(const Temperature &other)
    {
        val -= Celsius(other);
        return *this;
    }

    Celsius operator*(double value) const
    {
        return Celsius(val * value);
    }
    
    Celsius operator/(double value) const
    {
        return Celsius(val / value);
    }

    Celsius operator*(int value) const
    {
        return Celsius(val * value);
    }
    
    Celsius operator/(int value) const
    {
        return Celsius(val / value);
    }
};

Celsius operator*(double value, const Celsius &temp)
{
    return Celsius(value * double(temp));
}

PhysUnit<0,0,0,0,-1> operator/(double value, const Celsius &temp)
{
    return PhysUnit<0,0,0,0,-1>(value / double(temp));
}

Celsius operator*(int value, const Celsius &temp)
{
    return double(value) * temp;
}

PhysUnit<0,0,0,0,-1> operator/(int value, const Celsius &temp)
{
    return double(value) / temp;
}

class Fahrenheit : public TempBase
{
protected:
    double convertToInternal() const
    {
        return (val + 459.67) / 1.8;
    }
    
    void convertFromInternal(double value)
    {
        val = (value * 1.8) - 459.67;
    }

public:
    Fahrenheit() : TempBase(1)
    {}

    explicit Fahrenheit(double value) : TempBase(value)
    {}

    Fahrenheit(const Temperature &temp) : TempBase()
    {
        convertFromInternal(temp);
    }

    Fahrenheit(const TempBase &other) : TempBase()
    {
        convertFromInternal( other.convertToInternal() );
    }

    Fahrenheit(const Fahrenheit &other)
          : TempBase(other)
    {}

    ~Fahrenheit()
    {}
    
    Fahrenheit& operator=(const Fahrenheit &other)
    {
        TempBase::operator=(other);
        return *this;
    }

    Fahrenheit& operator=(const Temperature &temp)
    {
        convertFromInternal(temp);
        return *this;
    }
    
    bool operator==(const Fahrenheit &other) const
    {
        return val == other.val;
    }
    
    bool operator!=(const Fahrenheit &other) const
    {
        return val != other.val;
    }

    Fahrenheit operator+(const Fahrenheit &other) const
    {
        return Fahrenheit(val + other.val);
    }
    
    Fahrenheit operator-(const Fahrenheit &other) const
    {
        return Fahrenheit(val - other.val);
    }
    
    Fahrenheit& operator+=(const Fahrenheit &other)
    {
        val += other.val;
        return *this;
    }
    
    Fahrenheit& operator-=(const Fahrenheit &other)
    {
        val -= other.val;
        return *this;
    }
    
    Fahrenheit operator+(const Temperature &other) const
    {
        return *this + Fahrenheit(other);
    }
    
    Fahrenheit operator-(const Temperature &other) const
    {
        return *this - Fahrenheit(other);
    }

    Fahrenheit& operator+=(const Temperature &other)
    {
        val += Fahrenheit(other);
        return *this;
    }

    Fahrenheit& operator-=(const Temperature &other)
    {
        val -= Fahrenheit(other);
        return *this;
    }

    Fahrenheit operator*(double value) const
    {
        return Fahrenheit(val * value);
    }
    
    Fahrenheit operator/(double value) const
    {
        return Fahrenheit(val / value);
    }

    Fahrenheit operator*(int value) const
    {
        return Fahrenheit(val * value);
    }
    
    Fahrenheit operator/(int value) const
    {
        return Fahrenheit(val / value);
    }
    
    template<class T>
    T in() const
    {
        return T(*this);
    }
};

Fahrenheit operator*(double value, const Fahrenheit &temp)
{
    return Fahrenheit(value * double(temp));
}

PhysUnit<0,0,0,0,-1> operator/(double value, const Fahrenheit &temp)
{
    return PhysUnit<0,0,0,0,-1>( value / (1.8*double(temp)) );
}

Fahrenheit operator*(int value, const Fahrenheit &temp)
{
    return double(value) * temp;
}

PhysUnit<0,0,0,0,-1> operator/(int value, const Fahrenheit &temp)
{
    return double(value) / temp;
}

}

const Dimension::Quantity mole( 6.0221419947e23 );

const Dimension::Length angstrom( 1 );
const Dimension::Length nanometer = 10 * angstrom;
const Dimension::Length picometer = 1000 * nanometer;
const Dimension::Length meter = 1.0e9 * nanometer;
const Dimension::Length bohr_radii = 0.529177249 * angstrom;
const Dimension::Length centimeter = 0.01 * meter;
const Dimension::Length kilometer = 1000 * meter;
const Dimension::Length inch = 2.54 * centimeter;
const Dimension::Length foot = 12 * inch;
const Dimension::Length yard = 3 * foot;
const Dimension::Length mile = 1760 * yard;

const Dimension::Time femtosecond(1);
const Dimension::Time picosecond(1000 * femtosecond);
const Dimension::Time nanosecond(1000 * picosecond);
const Dimension::Time microsecond(1000 * nanosecond);
const Dimension::Time millisecond(1000 * microsecond);
const Dimension::Time second(1000 * millisecond);
const Dimension::Time minute( 60 * second );
const Dimension::Time hour( 60 * minute );
const Dimension::Time day( 24 * hour );

const Dimension::Mass g_per_mol(1);
const Dimension::Mass gram( mole * g_per_mol );
const Dimension::Mass kilogram( 1000 * gram );
const Dimension::Mass kg_per_mol( 1000 * g_per_mol );

const Dimension::Charge mod_electrons(1);
const Dimension::Charge coulomb( mod_electrons / 1.60217646263e-19 );
const Dimension::Charge e_charge(1);

const Dimension::Energy kcal_per_mol(1);
const Dimension::Energy kcal( mole * kcal_per_mol );
const Dimension::Energy cal_per_mol( 0.001 * kcal_per_mol );
const Dimension::Energy cal( 0.001 * kcal );
const Dimension::Energy kJ_per_mol( kcal_per_mol / 4.184 );
const Dimension::Energy kilojoule( mole * kJ_per_mol );
const Dimension::Energy J_per_mol( 0.001 * kJ_per_mol );
const Dimension::Energy joule( 0.001 * kilojoule );
const Dimension::Energy int_kcal_per_mol( 4.1868 * kJ_per_mol );
const Dimension::Energy int_cal_per_mol( 0.001 * int_kcal_per_mol );
const Dimension::Energy int_kcal( mole * int_kcal_per_mol );
const Dimension::Energy int_cal( 0.001 * int_kcal );
const Dimension::Energy hartree(4.3597438134e-18 * joule);

const Dimension::Force newton_per_mol( J_per_mol / meter );
const Dimension::Force newton( joule / meter );

const Dimension::Pressure pascal = newton / (meter*meter);
const Dimension::Pressure atm = 101325.0 * pascal;

const Dimension::ElecCurrent amp = coulomb / second;

const Dimension::ElecPotential volt =  joule / coulomb;
const Dimension::PhysUnit<-1,-2,2,2,0> farad = coulomb / volt;

const Dimension::Temperature kelvin( 1 );
const Dimension::Celsius celsius( 1 );
const Dimension::Fahrenheit fahrenheit( 1 );

const double epsilon0 = 8.854187817e-12 * farad / meter;
const double gasr = 8.31447215 * J_per_mol / kelvin;
const double k_boltz = 1.380650524e-23 * joule / kelvin;

void v(const Dimension::Velocity &val)
{
    cout << val.in( mile / minute ) << " mile per minute\n";
}

void e(const Dimension::Energy &val)
{
    cout << val.in( joule ) << " J\n";
}

int main(void)
{
    cout << epsilon0 << " " << gasr << " " << k_boltz << endl;
    cout << atm << endl;

    cout << (5 * mile / hour).in( kilometer / hour ) << endl;

    cout << (10 * hour).in(second) << endl;

    Dimension::Fahrenheit temp = 37 * celsius;

    cout << 37 * celsius << " " << temp << endl;
    cout << (100 * fahrenheit).in<Dimension::Celsius>() << endl;
    cout << 100 * fahrenheit << endl;

    v( 60 * mile / (2*hour) );
    e( kilogram * meter * meter / (second * second) );

    cout << (joule / meter).in(newton) << endl;

    return 0;
}
