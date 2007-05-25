
#include <iostream>
#include <cmath>

#include <boost/random.hpp>

using namespace std;


typedef boost::variate_generator<boost::mt19937, 
                                 boost::uniform_real<double> > UniformRand;

UniformRand rng( boost::mt19937(), boost::uniform_real<double>(0.0,1.0) );

class Oscillator
{
public:
    Oscillator(double k=10) : _k(k), _r(0)
    {}
    
    ~Oscillator()
    {}
    
    double energy() const
    {
        return _k * _r * _r;
    }

    Oscillator change(double delta) const
    {
        Oscillator o = *this;
        o._r += delta;
        return o;
    }

private:
    double _k, _r;
};

class MonteCarlo
{
private:
    void setTemperature(double t)
    {
        beta = 1.0 / (0.0019872065368068833 * t);
    }

public:
    MonteCarlo(double t=298.15) : avgnrg(0), naccept(0), nreject(0), 
                                  ntotal(0)
    {
        setTemperature(t);
    }
    
    ~MonteCarlo()
    {}
    
    double average() const
    {
        return avgnrg;
    }
    
    Oscillator move(Oscillator &o_old, int nmoves = 1)
    {
        for (int i=0; i<nmoves; ++i)
        {
            double old_energy = o_old.energy();
        
            Oscillator o_new = o_old.change( 0.3 * rng() );
        
            double new_energy = o_new.energy();
        
            cout << new_energy << " " << old_energy << endl;
        
            if (test(new_energy,old_energy))
                o_old = o_new;
            
            ++ntotal;
            
            double bigratio = double(ntotal-1)/double(ntotal);
            double smallratio = 1.0 - bigratio;
            
            avgnrg = (bigratio*avgnrg) + (smallratio*o_old.energy());
        }
        
        return o_old;
    }
    
    bool test(double new_energy, double old_energy)
    {
        if (new_energy <= old_energy)
        {
            ++naccept;
            return true;
        }

        double x = std::exp( -beta*(new_energy - old_energy) );

        if (x > 1 or x > rng())
        {
            ++naccept;
            return true;
        }
        else
        {
            ++nreject;
            return false;
        }
    }

private:
    double beta, avgnrg;
    int naccept, nreject, ntotal;
};

int main(int argc, const char **argv)
{
    Oscillator o;
    
    MonteCarlo mc;
    o = mc.move(o, 100);
    cout << mc.average() << endl;

    return 0;
}
