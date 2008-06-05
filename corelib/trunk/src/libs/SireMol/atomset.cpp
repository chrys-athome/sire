
inline unsigned int qHash(const double &val)
{
    //reinterpret the bits of the double as an unsigned int
    union { double d; unsigned int i; } converter;
    
    //clear the memory of the union - this should
    //prevent any surprises
    converter.i = 0;
    
    //assign the memory of the union to hold the double
    converter.d = val;
    
    //now return the union as an integer - this forces
    //the bit pattern of the double to be interpreted
    //as an integer
    return converter.i;
}

#include "qhash_siremol.h"

#include <QMultiHash>
#include <QList>
#include <QtAlgorithms>

#include "SireMaths/angle.h"
#include "SireUnits/units.h"

#include "atomset.h"
#include "atom.h"

using namespace SireMol;
using namespace SireMaths;
using namespace SireStream;

/** Indexing function used by SireBase::Set<AtomIndex,Atom> to index the atoms */
template<>
SireMol::AtomIndex SIREMOL_EXPORT set_indexer(const SireMol::Atom &atm)
{
    return atm.index();
}

AtomSet SIREMOL_EXPORT SireMol::getNearest(const Vector &point, const AtomSet &atoms, int n)
{
    //just return the original set if it contains <= n atoms   
    if (n >= atoms.count())
        return atoms;
        
    //loop over all of the atoms and store the distances to the point in a hash...
    QMultiHash<double,int> disthash;
    
    int sz = atoms.count();
    for (int i=0; i<sz; i++)
    {
        //insert the inverse distance
        double invdist2 = Vector::invDistance2(point,atoms[i]);
        disthash.insert(invdist2, i);
    }
    
    //now get the keys of the hash...
    QList<double> keys = disthash.keys();
    
    //sort the keys from the highest (thus closest) to the lowest
#warning - need to sort qGreater - must get Qt docs...!
    qSort( keys.begin(),keys.end() );
    
    AtomSet nearatms;    
    //now get the first 'n' keys...
    for (int i=0; i<n; i++)
    {
        //get the 'ith' distance
        double dist = keys[i];
        
        //now add the atom at this distance to the returned atomset
        nearatms.insert( atoms[ disthash.take(dist) ] );
    }
    
    return nearatms;
}

void SIREMOL_EXPORT SireMol::removeNearAtoms(AtomSet &atoms, double dist)
{
    //get the dist^2
    dist = dist*dist;
    
    //start from the last atom and loop to the second (no need to test the first atom!)
    int sz = atoms.count();
    for (int i=sz-1; i>0; i--)
    {
        const Atom &atm = atoms[i];
    
        //loop from the first atom to the atom before this...
        for (int j=0; j<i; j++)
        {
            if (Vector::distance2(atm,atoms[j]) < dist)
            {
                //the atoms are too close - remove this atom!
                atoms.removeAt(i);
                //break out of the loop
                break;
            }
        }
    }
}

void SIREMOL_EXPORT SireMol::removeLinearAtoms(AtomSet &atoms, const SireMaths::Angle &delta)
{
    //start from the last atom and loop to the third (no need to test the first two atoms!)
    int sz = atoms.count();
    for (int i=sz-1; i>1; i--)
    {
        const Atom &atm0 = atoms[i];
        
        //loop over all atom pairs the don't include this atom..
        for (int j=0; j<i-1; j++)
        {
            const Atom &atm1 = atoms[j];
            //get the vector from atm0 to atm1
            Vector v1 = atm1.vector() - atm0.vector();
            
            bool breakout = false;
            
            for (int k=j+1; k<i; k++)
            {
                const Atom &atm2 = atoms[k];
                
                //get the vector from atm0 to atm2
                Vector v2 = atm2.vector() - atm0.vector();
                
                //calculate the angle between the two vectors
                SireMaths::Angle ang = Vector::angle(v1,v2);
                
                if ( ang.isWithin(SireUnits::pi,delta) or ang.isWithin(0.0,delta) )
                {
                    //remove atm0 and break out of the loop
                    atoms.removeAt(i);
                    breakout = true;
                    break;
                }
            }
            
            if (breakout)
                break;
        }
    }
}
