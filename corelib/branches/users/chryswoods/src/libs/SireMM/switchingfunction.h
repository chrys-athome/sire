/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#ifndef SIREMM_SWITCHINGFUNCTION_H
#define SIREMM_SWITCHINGFUNCTION_H

#include "SireBase/sharedpolypointer.hpp"
#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class SwitchingFunction;
class SwitchFunc;

class NoCutoff;
class HarmonicSwitchingFunction;
class CHARMMSwitchingFunction;
}

QDataStream& operator<<(QDataStream&, const SireMM::SwitchingFunction&);
QDataStream& operator>>(QDataStream&, SireMM::SwitchingFunction&);

QDataStream& operator<<(QDataStream&, const SireMM::SwitchFunc&);
QDataStream& operator>>(QDataStream&, SireMM::SwitchFunc&);

QDataStream& operator<<(QDataStream&, const SireMM::NoCutoff&);
QDataStream& operator>>(QDataStream&, SireMM::NoCutoff&);

QDataStream& operator<<(QDataStream&, const SireMM::HarmonicSwitchingFunction&);
QDataStream& operator>>(QDataStream&, SireMM::HarmonicSwitchingFunction&);

QDataStream& operator<<(QDataStream&, const SireMM::CHARMMSwitchingFunction&);
QDataStream& operator>>(QDataStream&, SireMM::CHARMMSwitchingFunction&);

namespace SireMM
{

using SireBase::SharedPolyPointer;

/** This is the virtual base class of all switching functions. These 
    return scale factors based on the supplied distance

    @author Christopher Woods
*/
class SIREMM_EXPORT SwitchFunc : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const SwitchFunc&);
friend QDataStream& ::operator>>(QDataStream&, SwitchFunc&);

public:
    ~SwitchFunc();

    virtual SwitchFunc* clone() const=0;

    /** Return the electrostatic scale factor for the distance 'dist' */
    virtual double electrostaticScaleFactor(double dist) const=0;

    /** Return the VDW scale factor for the distance 'dist' */
    virtual double vdwScaleFactor(double dist) const=0;

    /** Return the derivative (gradient) of the electrostatic
        scale factor at the distance 'dist' */
    virtual double dElectrostaticScaleFactor(double dist) const=0;

    /** Return the derivative (gradient) of the VDW
        scale factor at the distance 'dist' */
    virtual double dVDWScaleFactor(double dist) const=0;

    double cutoffDistance() const;
    double featherDistance() const;

    double electrostaticCutoffDistance() const;
    double electrostaticFeatherDistance() const;
    
    double vdwCutoffDistance() const;
    double vdwFeatherDistance() const;

protected:
    SwitchFunc();
    SwitchFunc(double cutdistance);
    SwitchFunc(double cutdistance, double featherdistance);
    SwitchFunc(double eleccut, double elecfeather,
               double vdwcut, double vdwfeather);

    SwitchFunc(const SwitchFunc &other);

    SwitchFunc& operator=(const SwitchFunc &other);

    bool operator==(const SwitchFunc &other) const;
    bool operator!=(const SwitchFunc &other) const;

    /** The maximum cutoff distance - both the electrostatic
        and vdw energies are scaled to zero beyond this distance */
    double cutdist;

    /** The maximum feather distance - feathering of the electrostatic
        and vdw interaction is *not* performed below this distance */
    double featherdist;

    /** The electrostatic cutoff distance */
    double cut_elec;
    /** The electrostatic feather distance */
    double feather_elec;

    /** The vdw cutoff distance */
    double cut_vdw;
    /** The vdw feather distance */
    double feather_vdw;
};

/** Return the cutoff distance beyond which both the electrostatic
    and vdw energies are scaled to zero */
inline double SwitchFunc::cutoffDistance() const
{
    return cutdist;
}

/** Return the feather distance, below which feathering of the 
    electrostatic and vdw interactions is *not* performed */
inline double SwitchFunc::featherDistance() const
{
    return featherdist;
}

/** Return the distance beyond which the electrostatic interaction
    is not evaluated */
inline double SwitchFunc::electrostaticCutoffDistance() const
{
    return cut_elec;
}

/** Return the distance below which the electrostatic interaction
    is *not* feathered */
inline double SwitchFunc::electrostaticFeatherDistance() const
{
    return feather_elec;
}

/** Return the distance beyond which the VDW interaction is not evaluated */
inline double SwitchFunc::vdwCutoffDistance() const
{
    return cut_vdw;
}

/** Return the distance below which the VDW interaction is 
    *not* feathered */
inline double SwitchFunc::vdwFeatherDistance() const
{
    return feather_vdw;
}

/** This class implements no cutoffs (e.g. there is no cutoff, 
    and no switching function!).

    @author Christopher Woods
*/
class SIREMM_EXPORT NoCutoff 
        : public SireBase::ConcreteProperty<NoCutoff,SwitchFunc>
{

friend QDataStream& ::operator<<(QDataStream&, const NoCutoff&);
friend QDataStream& ::operator>>(QDataStream&, NoCutoff&);

public:
    NoCutoff();

    NoCutoff(const NoCutoff &other);

    ~NoCutoff();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NoCutoff>() );
    }
    
    NoCutoff& operator=(const NoCutoff &other);
    
    bool operator==(const NoCutoff &other) const;
    bool operator!=(const NoCutoff &other) const;
    
    double electrostaticScaleFactor(double dist) const;
    double vdwScaleFactor(double dist) const;
    
    double dElectrostaticScaleFactor(double dist) const;
    double dVDWScaleFactor(double dist) const;
};

/** This class implements harmonic switching functions - these scale the energy
    harmonically down to zero.

    @author Christopher Woods
*/
class SIREMM_EXPORT HarmonicSwitchingFunction 
         : public SireBase::ConcreteProperty<HarmonicSwitchingFunction,
                                             SwitchFunc>
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
        return QMetaType::typeName( qMetaTypeId<HarmonicSwitchingFunction>() );
    }

    HarmonicSwitchingFunction& operator=(const HarmonicSwitchingFunction &other);
    
    bool operator==(const HarmonicSwitchingFunction &other) const;
    bool operator!=(const HarmonicSwitchingFunction &other) const;
    
    double electrostaticScaleFactor(double dist) const;
    double vdwScaleFactor(double dist) const;

    double dElectrostaticScaleFactor(double dist) const;
    double dVDWScaleFactor(double dist) const;
    
protected:
    void set(double cutelec, double featherelec,
             double cutvdw, double feathervdw);

    /** Square of the cutoff distance */
    double cut_elec2;
    /** Normalisation factor for the electrostatic cutoff */
    double norm_elec;

    /** Square of the cutoff distance */
    double cut_vdw2;
    /** Normalisation factor for the vdw cutoff */
    double norm_vdw;
};

/** This class implements the CHARMMM switching function - these scale the energy
    to zero is such a way that the first derivative of the switching
    function is continuous (and therefore this function can be used
    in a dynamics simulation)

    This is the switching function reported in;
    
    Steinbach and Brooks, "New spherical cutoff methods for long-range
                           forces in macromolecular simulaton"
    
    J. Comp. Chem., 15, 7, pp667-683, 1994

    @author Christopher Woods
*/
class SIREMM_EXPORT CHARMMSwitchingFunction 
         : public SireBase::ConcreteProperty<CHARMMSwitchingFunction,
                                             SwitchFunc>
{

friend QDataStream& ::operator<<(QDataStream&, const CHARMMSwitchingFunction&);
friend QDataStream& ::operator>>(QDataStream&, CHARMMSwitchingFunction&);

public:
    CHARMMSwitchingFunction();

    CHARMMSwitchingFunction(double cutoffdist);
    CHARMMSwitchingFunction(double cutoffdist, double featherdist);
    CHARMMSwitchingFunction(double cutoffdist,
                            double elecfeather, double vdwfeather);
    CHARMMSwitchingFunction(double eleccutoff, double elecfeather,
                            double vdwcutoff, double vdwfeather);

    CHARMMSwitchingFunction(const CHARMMSwitchingFunction &other);

    ~CHARMMSwitchingFunction();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CHARMMSwitchingFunction>() );
    }

    CHARMMSwitchingFunction& operator=(const CHARMMSwitchingFunction &other);
    
    bool operator==(const CHARMMSwitchingFunction &other) const;
    bool operator!=(const CHARMMSwitchingFunction &other) const;
    
    double electrostaticScaleFactor(double dist) const;
    double vdwScaleFactor(double dist) const;

    double dElectrostaticScaleFactor(double dist) const;
    double dVDWScaleFactor(double dist) const;
    
protected:
    void set(double cutelec, double featherelec,
             double cutvdw, double feathervdw);

    /** Square of the cutoff distance */
    double cut_elec2;
    
    /** Square of the feather distance */
    double feather_elec2;
    
    /** Normalisation factor for the electrostatic cutoff */
    double norm_elec;

    /** Square of the cutoff distance */
    double cut_vdw2;
    
    /** Square of the feather distance */
    double feather_vdw2;
    
    /** Normalisation factor for the vdw cutoff */
    double norm_vdw;
};

/** This is the user-handle class that is used to hold 
    the dynamic SwitchingFunction classes.

    @author Christopher Woods
*/
class SIREVOL_EXPORT SwitchingFunction : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const SwitchingFunction&);
friend QDataStream& ::operator>>(QDataStream&, SwitchingFunction&);

public:
    SwitchingFunction();
    SwitchingFunction(const SwitchFunc &other);
    SwitchingFunction(const SireBase::PropertyBase &property);

    SwitchingFunction(const SwitchingFunction &other);

    ~SwitchingFunction();

    virtual SwitchingFunction& operator=(const SwitchFunc &other);
    virtual SwitchingFunction& operator=(const SireBase::PropertyBase &other);

    const SwitchFunc* operator->() const;
    const SwitchFunc& operator*() const;
    
    const SwitchFunc& read() const;
    SwitchFunc& edit();
    
    const SwitchFunc* data() const;
    const SwitchFunc* constData() const;
    
    SwitchFunc* data();
    
    operator const SwitchFunc&() const;

    static const SwitchingFunction& shared_null();
};

}

Q_DECLARE_METATYPE(SireMM::SwitchingFunction)
Q_DECLARE_METATYPE(SireMM::NoCutoff)
Q_DECLARE_METATYPE(SireMM::HarmonicSwitchingFunction)
Q_DECLARE_METATYPE(SireMM::CHARMMSwitchingFunction)

SIRE_END_HEADER

#endif
