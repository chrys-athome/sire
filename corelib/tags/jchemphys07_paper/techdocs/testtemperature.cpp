
#include <iostream>

using namespace std;

class Unit
{
public:
      virtual ~Unit()
      {}
      
      virtual double convertFromInternal(double val) const
      {
          return val / relativeScale();
      }
      
      virtual double convertToInternal(double val) const
      {
          return val * relativeScale();
      }

      virtual double relativeScale() const
      {
          return sclfac;
      }

protected:
      Unit(double scale_factor) : sclfac(scale_factor)
      {}
      
      Unit(const Unit &other) : sclfac(other.sclfac)
      {}
      
private:
      double sclfac;
};

class DerivedUnit : public Unit
{
public:
      DerivedUnit() : Unit(1)
      {}
      
      explicit DerivedUnit(const Unit &unit)  : Unit(unit)
      {}
      
      DerivedUnit(const DerivedUnit &other)
              : Unit(other)
      {}
      
      ~DerivedUnit()
      {}
      
      DerivedUnit multiply(const Unit &unit) const
      {
          return DerivedUnit( relativeScale() * unit.relativeScale() );
      }
      
      DerivedUnit divide(const Unit &unit) const
      {
          return DerivedUnit( relativeScale() / unit.relativeScale() );
      }
protected:
      explicit DerivedUnit(double scale) : Unit(scale)
      {}
};

double operator*(double val, const DerivedUnit &unit)
{
    return unit.convertToInternal(val);
}

double operator/(double val, const DerivedUnit &unit)
{
    return unit.convertFromInternal(val);
}

DerivedUnit operator*(const Unit &unit0, const Unit &unit1)
{
    return DerivedUnit(unit0).multiply(unit1);
}

DerivedUnit operator/(const Unit &unit0, const Unit &unit1)
{
    return DerivedUnit(unit0).divide(unit1);
}

double convertFrom(double val, const DerivedUnit from_units)
{
    return from_units.convertToInternal(val);
}

double convertTo(double val, const DerivedUnit &to_units)
{
    return to_units.convertFromInternal(val);
}

double convert(double val, const DerivedUnit &from_units,
                           const DerivedUnit &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

class Temperature : public Unit
{
public:
      ~Temperature()
      {}

protected:
      Temperature(double scale_factor) : Unit(scale_factor)
      {}
};

double operator*(double val, const Temperature &temp_unit)
{
    return temp_unit.convertToInternal(val);
}

double operator/(double val, const Temperature &temp_unit)
{
    return temp_unit.convertFromInternal(val);
}

double convertFrom(double val, const Temperature &from_units)
{
    return from_units.convertToInternal(val);
}

double convertTo(double val, const Temperature &to_units)
{
    return to_units.convertFromInternal(val);
}

double convert(double val, const Temperature &from_units,
                           const Temperature &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

class Kelvin : public Temperature
{
public:
      Kelvin() : Temperature(1)
      {}
      
      ~Kelvin()
      {}
};

class Celsius : public Temperature
{
public:
      Celsius() : Temperature(1)
      {}
      
      ~Celsius()
      {}
      
      double convertFromInternal(double val) const
      {
            //internal units are kelvin
            return val - 273.15;
      }
      
      double convertToInternal(double val) const
      {
            //internal units are kelvin
            return val + 273.15;
      }
};

class Fahrenheit : public Temperature
{
public:
      Fahrenheit() : Temperature(1.0 / 1.8)
      {}
      
      ~Fahrenheit()
      {}
      
      double convertFromInternal(double val) const
      {
          //internal units are kelvin
          return (val*1.8) - 459.67;
      }
      
      double convertToInternal(double val) const
      {
          //internal units are kelvin
          return (val + 459.67) / 1.8;
      }
};

static Kelvin kelvin;
static Celsius celsius;
static Fahrenheit fahrenheit;

class Energy : public Unit
{
public:
      Energy(double scale_factor) : Unit(scale_factor)
      {}

      explicit Energy(const DerivedUnit &unit) : Unit(unit)
      {}

      ~Energy()
      {}
};

double operator*(double val, const Energy &nrg_unit)
{
    return nrg_unit.convertToInternal(val);
}

double operator/(double val, const Energy &nrg_unit)
{
    return nrg_unit.convertFromInternal(val);
}

double convertFrom(double val, const Energy &from_units)
{
    return from_units.convertToInternal(val);
}

double convertTo(double val, const Energy &to_units)
{
    return to_units.convertFromInternal(val);
}

double convert(double val, const Energy &from_units,
                           const Energy &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

class Mole : public Unit
{
public:
    Mole() : Unit(6.0221419947e23)
    {}
    
    ~Mole()
    {}
};

static Mole mole;

static Energy kcal_per_mol( 1 );
static Energy cal_per_mol( 0.001 * kcal_per_mol );

static Energy kcal( kcal_per_mol * mole );
static Energy cal( 0.001 * kcal );

static Energy kJ_per_mol( (1.0/4.184) * kcal_per_mol );
static Energy J_per_mol( 0.001 * kJ_per_mol );

static Energy kilojoule( kJ_per_mol * mole );
static Energy joule( 0.001 * kilojoule );

static double k_boltz = 1.380650524e-23 * (joule / kelvin);

int main(int argc, char **argv)
{
    cout << "100 C == " << 100 * celsius << " K\n";
    cout << "0 F == " << 0 * fahrenheit << " K\n";

    cout << "k_boltz == " << k_boltz << endl;

    for (int i=0; i<argc; ++i)
    {
        double val = atof(argv[i]);
        
        cout << val << " C == " << convert(val,celsius,kelvin) << " K\n";
        cout << val << " C == " << convert(val,celsius,fahrenheit) << " F\n";
        cout << val << " F == " << convert(val,fahrenheit,kelvin) << " K\n";
        cout << val << " F == " << convert(val,fahrenheit,celsius) << " C\n";
        cout << val << " K == " << convert(val,kelvin,celsius) << " C\n";
        cout << val << " K == " << convert(val,kelvin,fahrenheit) << " F\n";
    }
    
    return 0;
}
