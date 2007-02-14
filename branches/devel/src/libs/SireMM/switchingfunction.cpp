/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include <numeric>
#include <cmath>

#include "switchingfunction.h"

#include "SireMaths/maths.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireStream;

/////////////
///////////// Implementation of SwitchFuncBase
/////////////

static const RegisterMetaType<SwitchFuncBase> r_switchbase(MAGIC_ONLY,
                                                           "SireMM::SwitchFuncBase");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const SwitchFuncBase &switchbase)
{
    writeHeader(ds, r_switchbase, 1) << switchbase.cutdist;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, SwitchFuncBase &switchbase)
{
    VersionID v = readHeader(ds, r_switchbase);

    if (v == 1)
    {
        ds >> switchbase.cutdist;
    }
    else
        throw version_error(v, "1", r_switchbase, CODELOC);

    return ds;
}

/** Null constructor - this places the cutoff distance at
    the maximum value of a double (e.g. on cutoff) */
SwitchFuncBase::SwitchFuncBase() : QSharedData(), cutdist( std::numeric_limits<double>::max() )
{}

/** Construct, placing the ultimate cutoff distance at 'cutdistance' */
SwitchFuncBase::SwitchFuncBase(double cutdistance)
               : QSharedData(), cutdist( std::abs(cutdistance) )
{}

/** Copy constructor */
SwitchFuncBase::SwitchFuncBase(const SwitchFuncBase &other)
               : QSharedData(), cutdist(other.cutdist)
{}

/** Destructor */
SwitchFuncBase::~SwitchFuncBase()
{}

/////////////
///////////// Implementation of NoCutoff
/////////////

static const RegisterMetaType<NoCutoff> r_nocutoff;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const NoCutoff &nocutoff)
{
    writeHeader(ds, r_nocutoff, 1)
            << static_cast<const SwitchFuncBase&>(nocutoff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, NoCutoff &nocutoff)
{
    VersionID v = readHeader(ds, r_nocutoff);

    if (v == 1)
    {
        ds >> static_cast<NoCutoff&>(nocutoff);
    }
    else
        throw version_error(v, "1", r_nocutoff, CODELOC);

    return ds;
}

/** Constructor */
NoCutoff::NoCutoff() : SwitchFuncBase(std::numeric_limits<double>::max())
{}

/** Copy constructor */
NoCutoff::NoCutoff(const NoCutoff &other)
         : SwitchFuncBase(other)
{}

/** Destructor */
NoCutoff::~NoCutoff()
{}

/** Return the scale factor for the electrostatic energies - this
    will always be 1.0, as there are no cutoffs */
double NoCutoff::electrostaticScaleFactor(double) const
{
    return 1;
}

/** Return the scale factor for the vdw energies - this
    will always be 1.0, as there are no cutoffs */
double NoCutoff::vdwScaleFactor(double) const
{
    return 1;
}

/////////////
///////////// Implementation of HarmonicSwitchingFunction
/////////////

static const RegisterMetaType<HarmonicSwitchingFunction> r_harm;

/** Internal function used to set the parameters of the harmonic cutoff */
void HarmonicSwitchingFunction::set(double cutelec, double featherelec,
                                    double cutvdw, double feathervdw)
{
    cut_elec = cutelec;
    cut_elec2 = SireMaths::pow_2(cutelec);
    feather_elec = featherelec;

    if (cut_elec != feather_elec)
        norm_elec = 1.0 / (cut_elec2 - SireMaths::pow_2(featherelec));
    else
        norm_elec = 0;

    cut_vdw = cutvdw;
    cut_vdw2 = SireMaths::pow_2(cutvdw);
    feather_vdw = feathervdw;

    if (cut_vdw != feather_vdw)
        norm_vdw = 1.0 / (cut_vdw2 - SireMaths::pow_2(feathervdw));
    else
        norm_vdw = 0;
}

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const HarmonicSwitchingFunction &harm)
{
    writeHeader(ds, r_harm, 1)
          << harm.cut_elec << harm.feather_elec
          << harm.cut_vdw << harm.feather_vdw
          << static_cast<const SwitchFuncBase&>(harm);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      HarmonicSwitchingFunction &harm)
{
    VersionID v = readHeader(ds, r_harm);

    if (v == 1)
    {
        ds >> harm.cut_elec >> harm.feather_elec
           >> harm.cut_vdw >> harm.feather_vdw
           >> static_cast<SwitchFuncBase&>(harm);

        harm.set( harm.cut_elec, harm.feather_elec,
                  harm.cut_vdw, harm.feather_vdw );
    }
    else
        throw version_error(v, "1", r_harm, CODELOC);

    return ds;
}

/** Construct a null harmonic switching function (no cutoff) */
HarmonicSwitchingFunction::HarmonicSwitchingFunction()
                          : SwitchFuncBase(),
                            cut_elec(cutdist), feather_elec(cutdist),
                            cut_elec2(cutdist), norm_elec(0),
                            cut_vdw(cutdist), feather_vdw(cutdist),
                            cut_vdw2(cutdist), norm_vdw(0)
{}

/** Construct an harmonic switching function which represents a hard
    cutoff of both the electrostatic and vdw interactions at a distance
    of 'cutoffdist' */
HarmonicSwitchingFunction::HarmonicSwitchingFunction(double cutoffdist)
                          : SwitchFuncBase(cutoffdist)
{
    this->set(cutdist,cutdist,cutdist,cutdist);
}

/** Construct an harmonic switching function which represents the smoothed
    cutoff, with the cutoff at 'cutoffdist', but smoothed down using an
    harmonic function from 'featherdist'. If featherdist >= cutoffdist
    then this represents a hard cutoff */
HarmonicSwitchingFunction::HarmonicSwitchingFunction(double cutoffdist,
                                                     double featherdist)
                          : SwitchFuncBase(cutoffdist)
{
    featherdist = qMin(cutdist, std::abs(featherdist));

    this->set(cutdist,featherdist,cutdist,featherdist);
}

/** Construct an harmonic switching function which represents the smoothed
    cutoff, with the cutoff at 'cutoffdist', but with the electrostatic
    interactions smoothed down using a harmonic function from
    'elecfeather' and the vdw interactions smoothed down using
    an harmonic function from 'vdwfeather'. If either feather distance
    is greater than the cutoff, then a hard cutoff for that interaction
    will be used. */
HarmonicSwitchingFunction::HarmonicSwitchingFunction(double cutoffdist,
                                                     double elecfeather,
                                                     double vdwfeather)
                          : SwitchFuncBase(cutoffdist)
{
    elecfeather = qMin(cutdist, std::abs(elecfeather));
    vdwfeather = qMin(cutdist, std::abs(vdwfeather));

    this->set(cutdist,elecfeather,cutdist,vdwfeather);
}

/** Construct an harmonic switching function which represents the smoothed
    cutoff, with the electrostatic interactions cutoff at 'eleccutoff', and
    smoothed down using an harmonic function from 'elecfeather', and the
    vdw interactions cutoff at 'vdwcutoff' and smoothed down using an
    harmonic function from 'vdwfeather'. If either feather distance is
    greater than the corresponding cutoff, then a hard cutoff will be
    used for that interaction. */
HarmonicSwitchingFunction::HarmonicSwitchingFunction(double eleccutoff,
                                                     double elecfeather,
                                                     double vdwcutoff,
                                                     double vdwfeather)
                          : SwitchFuncBase()
{
    eleccutoff = std::abs(eleccutoff);
    vdwcutoff = std::abs(vdwcutoff);

    cutdist = qMax(eleccutoff, vdwcutoff);

    elecfeather = qMin(eleccutoff, std::abs(elecfeather));
    vdwfeather = qMin(vdwcutoff, std::abs(vdwfeather));

    this->set(eleccutoff, elecfeather, vdwcutoff, vdwfeather);
}

/** Copy constructor */
HarmonicSwitchingFunction::HarmonicSwitchingFunction(
                                const HarmonicSwitchingFunction &other)
                          : SwitchFuncBase(other),
                            cut_elec(other.cut_elec), feather_elec(other.feather_elec),
                            cut_elec2(other.cut_elec2), norm_elec(other.norm_elec),
                            cut_vdw(other.cut_vdw), feather_vdw(other.feather_vdw),
                            cut_vdw2(other.cut_vdw2), norm_vdw(other.norm_vdw)
{}

/** Destructor */
HarmonicSwitchingFunction::~HarmonicSwitchingFunction()
{}

/** Return the scale factor for the electrostatic interaction for the
    distance 'dist'. This returns;

    dist <= feather_elec           : 1
    feather_elec < dist < cut_elec : (cut_elec^2 - dist^2) / (cut_elec^2 - feather_elec^2)
    dist >= cut_elec               : 0
*/
double HarmonicSwitchingFunction::electrostaticScaleFactor(double dist) const
{
    if (dist <= feather_elec)
        return 1;
    else if (dist >= cut_elec)
        return 0;
    else
    {
        return norm_elec * (cut_elec2 - SireMaths::pow_2(dist));
    }
}

/** Return the scale factor for the vdw interaction for the
    distance 'dist' This returns;

    dist <= feather_vdw           : 1
    feather_vdw < dist < cut_vdw  : (cut_vdw^2 - dist^2) / (cut_vdw^2 - feather_vdw^2)
    dist >= cut_vdw               : 0
*/
double HarmonicSwitchingFunction::vdwScaleFactor(double dist) const
{
    if (dist <= feather_vdw)
        return 1;
    else if (dist >= cut_vdw)
        return 0;
    else
    {
        return norm_vdw * (cut_vdw2 - SireMaths::pow_2(dist));
    }
}

/////////////
///////////// Implementation of SwitchingFunction
/////////////

static const RegisterMetaType<SwitchingFunction> r_switchfunc;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const SwitchingFunction &switchfunc)
{
    writeHeader(ds, r_switchfunc, 1);

    SharedDataStream(ds) << switchfunc.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      SwitchingFunction &switchfunc)
{
    VersionID v = readHeader(ds, r_switchfunc);

    if (v == 1)
    {
        SharedDataStream(ds) >> switchfunc.d;
    }
    else
        throw version_error(v, "1", r_switchfunc, CODELOC);

    return ds;
}

static const SharedPolyPointer<SwitchFuncBase> shared_null(new NoCutoff());

/** Constructor - default is NoCutoff */
SwitchingFunction::SwitchingFunction() : d(shared_null)
{}

/** Construct from the passed switching function */
SwitchingFunction::SwitchingFunction(const SwitchFuncBase &switchingfunction)
                  : d( switchingfunction.clone() )
{}

/** Copy constructor */
SwitchingFunction::SwitchingFunction(const SwitchingFunction &other)
                  : d(other.d)
{}

/** Destructor */
SwitchingFunction::~SwitchingFunction()
{}

/** Assignment operator */
SwitchingFunction& SwitchingFunction::operator=(const SwitchingFunction &other)
{
    d = other.d;
    return *this;
}

/** Return the type name of this switching function */
const char* SwitchingFunction::what() const
{
    return d->what();
}
