/**
  * @file
  *
  * C++ Interface: SwitchingFunction, SwitchFuncBase, HarmonicSwitchingFunction
  *
  * Description: 
  * Interface for SwitchingFunction, SwitchFuncBase and HarmonicSwitchingFunction
  * 
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMM_SWITCHINGFUNCTION_H
#define SIREMM_SWITCHINGFUNCTION_H

#include "SireBase/dynamicsharedptr.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class SwitchingFunction;
class SwitchFuncBase;
class HarmonicSwitchingFunction;
}

QDataStream& operator<<(QDataStream&, const SireMM::SwitchingFunction&);
QDataStream& operator>>(QDataStream&, SireMM::SwitchingFunction&);

QDataStream& operator<<(QDataStream&, const SireMM::SwitchFuncBase&);
QDataStream& operator>>(QDataStream&, SireMM::SwitchFuncBase&);

QDataStream& operator<<(QDataStream&, const SireMM::HarmonicSwitchingFunction&);
QDataStream& operator>>(QDataStream&, SireMM::HarmonicSwitchingFunction&);

namespace SireMM 
{

/** 
This is the virtual base class of all switching functions. These return scale factors
for based on the supplied distance

@author Christopher Woods
*/
class SIREMM_EXPORT SwitchFuncBase
{

friend QDataStream& ::operator<<(QDataStream&, const &);
friend QDataStream& ::operator>>(QDataStream&, &);

public:
    SwitchFuncBase();
    SwitchFuncBase(double cutdistance);
    
    SwitchFuncBase(const SwitchFuncBase &other);
    
    virtual ~SwitchFuncBase();
    
    virtual SwitchFuncBase* clone() const;
    
    virtual const char* what() const;
    
    virtual double electrostaticScaleFactor(double dist) const=0;
    virtual double vdwScaleFactor(double dist) const=0;

    double cutoffDistance() const;

protected:
    /** The maximum cutoff distance - both the electrostatic  
        and vdw energies are scaled to zero beyond this distance */
    double cutdist;
};

/** Return the cutoff distance beyond which both the electrostatic 
    and vdw energies are scaled to zero */
inline double SwitchFuncBase::cutoffDistance() const
{
    return cutdist;
}

/**
This class implements harmonic switching functions - these scale the energy
harmonically down to zero.

@author Christopher Woods
*/
class SIREMM_EXPORT HarmonicSwitchingFunction : public SwitchFuncBase
{

friend QDataStream& ::operator<<(QDataStream&, const HarmonicSwitchingFunction&);
friend QDataStream& ::operator>>(QDataStream&, HarmonicSwitchingFunction&);

public:
    HarmonicSwitchingFunction();
    
    HarmonicSwitchingFunction(double cutoffdist);
    HarmonicSwitchingFunction(double cutoffdist, double featherdist);
    HarmonicSwitchingFunction(double cutoffdist, 
                              double elecfeather, double vdwfeather);
    HarmonicSwitchingFunction(double eleccutoff, double elecfeather,
                              double vdwcutoff, double vdwfeather);
    
    HarmonicSwitchingFunction(const HarmonicSwitchingFunction &other);
    
    ~HarmonicSwitchingFunction();
    
    static const char* typeName()
    {
        return "SireMM::HarmonicSwitchingFunction";
    }
    
    const char* what() const
    {
        return HarmonicSwitchingFunction::typeName();
    }
    
    const SwitchFuncBase* clone() const
    {
        return new HarmonicSwitchingFunction(*this);
    }

protected:
    
    /** The electrostatic cutoff distance */
    double cut_elec;
    /** The electrostatic feather distance */
    double feather_elec;
    
    /** The vdw cutoff distance */
    double cut_vdw;
    /** The vdw feather distance */
    double feather_vdw;
};

/**
This class provides an abstraction of a switching function - these are used to cutoff a non-bonded interaction potential smoothly. These functions provide a scaling factor that can be used to scale the electrostatic or vdw energies.

@author Christopher Woods
*/
class SIREMM_EXPORT SwitchingFunction
{

friend QDataStream& ::operator<<(QDataStream&, const SwitchingFunction&);
friend QDataStream& ::operator>>(QDataStream&, SwitchingFunction&);

public:
    SwitchingFunction();
    SwitchingFunction(const SwitchFuncBase &switchingfunction);

    SwitchingFunction(const SwitchingFunction &other);

    ~SwitchingFunction();

    const char* what() const;

    double electrostaticScaleFactor(double dist) const;
    double vdwScaleFactor(double dist) const;

    double cutoffDistance() const;

private:
    /** Dynamic shared pointer to the object implementing
        the switching function */
    DynamicSharedPtr<SwitchFunc> d;
};

/** Return the scaling factor for the electrostatic energy for the 
    distance 'dist' */
inline double SwitchingFunction::electrostaticScaleFactor(double dist) const
{
    return d->electrostaticScaleFactor(dist);
}

/** Return the scaling factor for the vdw energy for the 
    distance 'dist' */
inline double SwitchingFunction::vdwScaleFactor(double dist) const
{
    return d->vdwScaleFactor(dist);
}

/** Return the cutoff distance beyond which both the electrostatic 
    and vdw energies are scaled to zero */
inline double SwitchingFunction::cutoffDistance() const
{
    return d->cutoffDistance();
}

}

Q_DECLARE_METATYPE(SireMM::SwitchingFunction)
Q_DECLARE_METATYPE(SireMM::HarmonicSwitchingFunction)

SIRE_END_HEADER

#endif
