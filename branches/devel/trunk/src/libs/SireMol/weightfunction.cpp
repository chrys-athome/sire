
#include "weightfunction.h"

using namespace SireMol;

///////////
/////////// Implementation of WeightCalculator
///////////

/** Construct a new, empty WeightCalculator that will calculate the 
    relative weight using 'weightfunc'. This will hold the workspace and 
    results of a weight calculation using an arbitrary WeightFunction. */
WeightCalculator::WeightCalculator(const WeightFunction &weightfunction)
                 : weightfunc(weightfunction), natms_a(0), natms_b(0)
{}

/** Destructor */
WeightCalculator::~WeightCalculator()
{}

/** Add an atom to group 'A' */
void WeightCalculator::addToA(const Atom &atom)
{
    group_a.append(atom);
    
    if (group_a.count() >= MaxWeightFunctionAtoms)
        processBuffers();
}

/** Add an atom to group 'B' */
void WeightCalculator::addToB(const Atom &atom)
{
    group_b.append(atom);
    
    if (group_b.count() >= MaxWeightFunctionAtoms)
        processBuffers();
}
    
/** Add a whole list of atoms to group 'A' */
void WeightCalculator::addToA(const AtomVector &atoms)
{
    if (group_a.count() + atoms.count() >= MaxWeightFunctionAtoms)
        processBuffers();
        
    group_a.reserve(atoms.count());
    
    for (int i=0; i<atoms.count(); ++i)
        group_a.append( atoms.at(i) );
        
    if (group_a.count() >= MaxWeightFunctionAtoms)
        processBuffers();
}

/** Add a whole list of atoms to group 'B' */
void WeightCalculator::addToB(const AtomVector &atoms)
{
    if (group_b.count() + atoms.count() >= MaxWeightFunctionAtoms)
        processBuffers();
        
    group_b.reserve(atoms.count());
    
    for (int i=0; i<atoms.count(); ++i)
        group_b.append( atoms.at(i) );
       
    if (group_b.count() >= MaxWeightFunctionAtoms)
        processBuffers();
}

/** Return the calculated weight based on the atoms added so far... */
double WeightCalculator::weight() const
{
    //ensure that all of the atoms have been processed...
    if (group_a.count() > 0 or group_b.count() > 0)
        const_cast<WeightCalculator*>(this)->processBuffers();

    double weight = weightfunc.calculateWeight(*this);
    
    //clamp the result to lie within the range 0->1 inclusive
    if (weight < 0.0)
        return 0.0;
    else if (weight > 1.0)
        return 1.0;
    else
        return weight;
}

/** Process the atoms that are in the group_a and group_b buffers, then
    clear the buffers after processing. Using a buffer means that we don't
    call the virtual WeightFunction function for every added atom. This
    will hopefully result in major performance advantages. */
void WeightCalculator::processBuffers()
{
    if (group_a.count() == 0 and group_b.count() == 0)
        return;
        
    //tell the weight function to process the buffers...
    weightfunc.processBuffers(*this);
    
    //finally, clear the buffers
    group_a.clear();
    group_b.clear();
}

///////////
/////////// Implementation of WeightFunction 'processBuffer' functions...
///////////

//////////// AbsFromNumber ///////////////

/** AbsFromNumber doesn't need to do any runtime processing */
void AbsFromNumber::processBuffers(WeightCalculator&) const
{}

/** AbsFromNumber calculates the weight purely based on which of the two groups 
    was the largest. */
double AbsFromNumber::calculateWeight(const WeightCalculator &calc) const
{
    qint32 natsa = nAtomsA(calc);
    qint32 natsb = nAtomsB(calc);
    
    if (natsa > natsb)
        return 0.0;
    else if (natsa < natsb)
        return 1.0;
    else
        return 0.5;
}

//////////// RelFromNumber ///////////////

/** RelFromNumber calculates the weight purely based on the ratio of the number 
    of atoms of the two groups. */
double RelFromNumber::calculateWeight(const WeightCalculator &calc) const
{
    qint32 natsa = nAtomsA(calc);
    qint32 natsb = nAtomsB(calc);
    
    qint32 total = natsa + natsb;
    
    if (total == 0)
        return 0.5;
    else
        return double(natsb) / double(total);
}

//////////// AbsFromMass ///////////////

/** Need to accumalate the total mass of the two groups... */
void AbsFromMass::processBuffers(WeightCalculator &calc) const
{
    //get the current value of the mass of the two groups (default 0.0)
    double mass_a = boost::any_cast<double>(getValue(calc, "mass_a", 0.0));
    double mass_b = boost::any_cast<double>(getValue(calc, "mass_b", 0.0));

    //go through all of the atoms in groupA and calculate their mass
    const WeightFunctionAtoms &atoms_a = groupA(calc);
    const WeightFunctionAtoms &atoms_b = groupB(calc);
    
    for (int i=0; i<atoms_a.count(); ++i)
        mass_a += atoms_a[i].mass();
        
    for (int i=0; i<atoms_b.count(); ++i)
        mass_b += atoms_b[i].mass();

    //save the calculated values of the mass
    setValue(calc, "mass_a", mass_a);
    setValue(calc, "mass_b", mass_b);
}

/** The weight is given entirely to the group with the largest mass */
double AbsFromMass::calculateWeight(const WeightCalculator &calc) const
{
    //get the current value of the mass of the two groups (default 0.0)
    double mass_a = boost::any_cast<double>(getValue(calc, "mass_a", 0.0));
    double mass_b = boost::any_cast<double>(getValue(calc, "mass_b", 0.0));

    if (mass_a > mass_b)
        return 0.0;
    else if (mass_a < mass_b)
        return 1.0;
    else
        return 0.5;
}

//////////// RelFromMass ///////////////

/** The weight is based on the ratio of the masses of the two groups */
double RelFromMass::calculateWeight(const WeightCalculator &calc) const
{
    //get the current value of the mass of the two groups (default 0.0)
    double mass_a = boost::any_cast<double>(getValue(calc, "mass_a", 0.0));
    double mass_b = boost::any_cast<double>(getValue(calc, "mass_b", 0.0));

    double totalmass = mass_a + mass_b;
    
    if ( SireMaths::isZero(totalmass) )
        return 0.5;
    else
        return mass_b / totalmass;
}
