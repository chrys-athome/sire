/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "SireBase/sparsematrix.hpp"

#include "softcljpotential.h"
#include "ljparameter.h"
#include "switchingfunction.h"

#include "SireMol/mover.hpp"
#include "SireMol/atomcoords.h"

#include "SireVol/cartesian.h"

#include "SireBase/countflops.h"

#include "SireMaths/maths.h"

#include "SireUnits/units.h"

#include "SireBase/errors.h"
#include "SireError/errors.h"
#include "SireFF/errors.h"

#include "SireStream/datastream.h"

#include <QDebug>

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireMol;
using namespace SireVol;

using namespace SireMaths;

using namespace SireBase;

using namespace SireStream;

///////
/////// Completely instantiate the SoftCLJPotential ancillary classes
///////

template
class FFMolecule3D<InterSoftCLJPotential>;

template
class FFMolecules3D<InterSoftCLJPotential>;

template
class ChangedMolecule<InterSoftCLJPotential::Molecule>;

/////////////
///////////// Implementation of SoftCLJPotential
/////////////

static const RegisterMetaType<SoftCLJPotential> r_cljpot( MAGIC_ONLY,
                                                          "SireMM::SoftCLJPotential" );

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const SoftCLJPotential &cljpot)
{
    writeHeader(ds, r_cljpot, 1);
    
    ds << static_cast<const CLJPotential&>(cljpot);

    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      SoftCLJPotential &cljpot)
{
    VersionID v = readHeader(ds, r_cljpot);
    
    if (v == 1)
    {
        ds >> static_cast<CLJPotential&>(cljpot);
    
        //extract all of the properties
        cljpot.alfa = cljpot.props.property("alpha")
                                  .asA<VariantProperty>().convertTo<double>();
        
        cljpot.coul_power = cljpot.props.property("coulombPower")
                                  .asA<VariantProperty>().convertTo<quint32>();

        cljpot.lj_power = cljpot.props.property("ljPower")
                                  .asA<VariantProperty>().convertTo<quint32>();
    }
    else 
        throw version_error(v, "1", r_cljpot, CODELOC);
    
    return ds;
}

/** Constructor */
SoftCLJPotential::SoftCLJPotential()
                 : CLJPotential(), alfa(0), coul_power(1), lj_power(1)
{
    //record the defaults
    props.setProperty( "alpha", VariantProperty(alfa) );
    props.setProperty( "coulombPower", VariantProperty(coul_power) );
    props.setProperty( "ljPower", VariantProperty(lj_power) );
}

/** Copy constructor */
SoftCLJPotential::SoftCLJPotential(const SoftCLJPotential &other)
                 : CLJPotential(other),
                   alfa(other.alfa), coul_power(other.coul_power),
                   lj_power(other.lj_power)
{}

/** Destructor */
SoftCLJPotential::~SoftCLJPotential()
{}

/** Copy assignment operator */
SoftCLJPotential& SoftCLJPotential::operator=(const SoftCLJPotential &other)
{
    if (this != &other)
    {
        CLJPotential::operator=(other);
        alfa = other.alfa;
        coul_power = other.coul_power;
        lj_power = other.lj_power;
    }
    
    return *this;
}

/** Set the value of alpha which is used to soften the interactions */
bool SoftCLJPotential::setAlpha(double new_alpha)
{
    if (alfa != new_alpha)
    {
        alfa = new_alpha;
        props.setProperty("alpha", VariantProperty(alfa));
        this->changedPotential();
        return true;
    }
    else
        return false;
}

/** Set the coulomb power, which is used to control how strongly
    the coulomb terms are softened */
bool SoftCLJPotential::setCoulombPower(int power)
{
    if (power < 0)
        throw SireError::invalid_arg( QObject::tr(
            "You cannot use a negative Coulomb power (%1)").arg(power),
                CODELOC );
                
    quint32 new_power(power);
    
    if (coul_power != new_power)
    {
        coul_power = new_power;
        props.setProperty("coulombPower", VariantProperty(coul_power));
        this->changedPotential();
        return true;
    }
    else
        return false;
}

/** Set the LJ power, which is used to control how strongly
    the LJ terms are softened */
bool SoftCLJPotential::setLJPower(int power)
{
    if (power < 0)
        throw SireError::invalid_arg( QObject::tr(
            "You cannot use a negative LJ power (%1)").arg(power),
                CODELOC );
                
    quint32 new_power(power);
    
    if (lj_power != new_power)
    {
        lj_power = new_power;
        props.setProperty("ljPower", VariantProperty(lj_power));
        this->changedPotential();
        return true;
    }
    else
        return false;
}

/** Set the property 'name' to the value 'value'. Returns whether or not
    this changes this forcefield.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool SoftCLJPotential::setProperty(const QString &name, const Property &value)
{
    if (name == QLatin1String("alpha"))
    {
        return this->setAlpha( value.asA<VariantProperty>().convertTo<double>() );
    }
    else if (name == QLatin1String("coulombPower"))
    {
        return this->setCoulombPower( value.asA<VariantProperty>().convertTo<int>() );
    }
    else if (name == QLatin1String("ljPower"))
    {
        return this->setLJPower( value.asA<VariantProperty>().convertTo<int>() );
    }
    else
        return CLJPotential::setProperty(name, value);
}

/** Return the value of alpha */
double SoftCLJPotential::alpha() const
{
    return alfa;
}

/** Return the coulomb power */
int SoftCLJPotential::coulombPower() const
{
    return coul_power;
}

/** Return the LJ power */
int SoftCLJPotential::ljPower() const
{
    return lj_power;
}

/////////////
///////////// Implementation of InterSoftCLJPotential
/////////////

static const RegisterMetaType<InterSoftCLJPotential> r_interclj( MAGIC_ONLY,
                                                   InterSoftCLJPotential::typeName() );

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const InterSoftCLJPotential &interclj)
{
    writeHeader(ds, r_interclj, 1);
    
    ds << static_cast<const SoftCLJPotential&>(interclj);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      InterSoftCLJPotential &interclj)
{
    VersionID v = readHeader(ds, r_interclj);
    
    if (v == 1)
    {
        ds >> static_cast<SoftCLJPotential&>(interclj);
    }
    else
        throw version_error(v, "1", r_interclj, CODELOC);
        
    return ds;
}

/** Constructor */
InterSoftCLJPotential::InterSoftCLJPotential() : SoftCLJPotential()
{}

/** Copy constructor */
InterSoftCLJPotential::InterSoftCLJPotential(const InterSoftCLJPotential &other)
                      : SoftCLJPotential(other)
{}

/** Destructor */
InterSoftCLJPotential::~InterSoftCLJPotential()
{}

/** Copy assignment operator */
InterSoftCLJPotential& 
InterSoftCLJPotential::operator=(const InterSoftCLJPotential &other)
{
    SoftCLJPotential::operator=(other);
    return *this;
}

void InterSoftCLJPotential::throwMissingForceComponent(const Symbol &symbol,
                              const InterSoftCLJPotential::Components &components) const
{
    throw SireFF::missing_component( QObject::tr(
        "There is no force component in potential %1 - available "
        "components are %2, %3 and %4.")
            .arg(this->what())
            .arg(components.total().toString(), components.coulomb().toString(),
                 components.lj().toString()), CODELOC );
}

/** Return all of the parameters needed by this potential for 
    the molecule 'molecule', using the supplied property map to
    find the properties that contain those parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterSoftCLJPotential::Parameters 
InterSoftCLJPotential::getParameters(const PartialMolecule &molecule,
                                     const PropertyMap &map)
{
    return Parameters( molecule, map[parameters().coordinates()],
                       CLJPotential::getCLJParameters(molecule, 
                                                      map[parameters().charge()],
                                                      map[parameters().lj()]) );
}

/** Update the parameters for the molecule going from 'old_molecule' to 
    'new_molecule', with the parameters found using the property map 'map'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterSoftCLJPotential::Parameters
InterSoftCLJPotential::updateParameters(
                                    const InterSoftCLJPotential::Parameters &old_params,
                                    const PartialMolecule &old_molecule,
                                    const PartialMolecule &new_molecule,
                                    const PropertyMap &map)
{
    if (old_molecule.selection() != new_molecule.selection())
        //the selection has changed - just get completely new parameters
        return this->getParameters(new_molecule, map);

    Parameters new_params = old_params;

    //get the property names
    const PropertyName &coords_property = map[parameters().coordinates()];
    const PropertyName &chg_property = map[parameters().charge()];
    const PropertyName &lj_property = map[parameters().lj()];
    
    //get what has changed
    bool new_coords = old_molecule.version(coords_property) !=
                         new_molecule.version(coords_property);
                             
    bool new_clj = ( old_molecule.version(chg_property) !=
                         new_molecule.version(chg_property) ) or
                   ( old_molecule.version(lj_property) !=
                         new_molecule.version(lj_property) );

    if (new_coords)
    {
        new_params.setAtomicCoordinates( AtomicCoords3D(new_molecule, 
                                                        coords_property) );
    }

    if (new_clj)
    {
        new_params.setAtomicParameters( CLJPotential::getCLJParameters(new_molecule,
                                                         chg_property, lj_property) );
    }

    return new_params;
}
                 
/** Update the parameters for the molecule going from 'old_molecule' to 
    'new_molecule', also while the parameters of 'old_molecule'
    where found in 'old_map', now get the parameters using 'new_map'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterSoftCLJPotential::Parameters
InterSoftCLJPotential::updateParameters(
                                    const InterSoftCLJPotential::Parameters &old_params,
                                    const PartialMolecule &old_molecule,
                                    const PartialMolecule &new_molecule,
                                    const PropertyMap &old_map, 
                                    const PropertyMap &new_map)
{
    if (old_molecule.selection() != new_molecule.selection())
        //the selection has changed - just get completely new parameters
        return this->getParameters(new_molecule, new_map);

    Parameters new_params = old_params;

    //get the property names
    const PropertyName &old_coords = old_map[parameters().coordinates()];
    const PropertyName &old_chg = old_map[parameters().charge()];
    const PropertyName &old_lj = old_map[parameters().lj()];
    
    const PropertyName &new_coords = new_map[parameters().coordinates()];
    const PropertyName &new_chg = new_map[parameters().charge()];
    const PropertyName &new_lj = new_map[parameters().lj()];
    
    //get what has changed
    bool changed_coords = (new_coords != old_coords) or
                           old_molecule.version(old_coords) !=
                           new_molecule.version(old_coords);
                             
    bool changed_clj = (new_chg != old_chg or new_lj != old_lj) or
                       ( old_molecule.version(old_chg) !=
                         new_molecule.version(old_chg) ) or
                       ( old_molecule.version(old_lj) !=
                         new_molecule.version(old_lj) );

    if (changed_coords)
        new_params.setAtomicCoordinates( AtomicCoords3D(new_molecule, 
                                                        new_coords) );

    if (changed_clj)
        new_params.setAtomicParameters( CLJPotential::getCLJParameters(new_molecule,
                                                                       new_chg, new_lj) );

    return new_params;
}

/** Return the InterSoftCLJPotential::Molecule representation of 'molecule',
    using the supplied PropertyMap to find the properties that contain
    the necessary forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterSoftCLJPotential::Molecule
InterSoftCLJPotential::parameterise(const PartialMolecule &molecule,
                                    const PropertyMap &map)
{
    return InterSoftCLJPotential::Molecule(molecule, *this, map);
}

/** Convert the passed group of molecules into InterSoftCLJPotential::Molecules,
    using the supplied PropertyMap to find the properties that contain
    the necessary forcefield parameters in each molecule
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterSoftCLJPotential::Molecules 
InterSoftCLJPotential::parameterise(const MoleculeGroup &molecules,
                                    const PropertyMap &map)
{
    return InterSoftCLJPotential::Molecules(molecules, *this, map);
}

/** Return the total charge of the parameters for the group in 'params' */
double InterSoftCLJPotential::totalCharge(
                            const InterSoftCLJPotential::Parameters::Array &params) const
{
    int n = params.count();
    const Parameter *params_array = params.constData();
    
    double chg = 0;
    
    for (int i=0; i<n; ++i)
    {
        chg += params_array[i].reduced_charge;
    }
    
    return chg;
}

/** Calculate the coulomb and LJ energy between the passed pair
    of molecules and add these energies onto 'energy'. This uses
    the passed workspace to perform the calculation */
void InterSoftCLJPotential::_pvt_calculateEnergy(
                                          const InterSoftCLJPotential::Molecule &mol0,
                                          const InterSoftCLJPotential::Molecule &mol1,
                                          InterSoftCLJPotential::Energy &energy,
                                          InterSoftCLJPotential::EnergyWorkspace &distmat,
                                          double scale_energy) const
{
    const quint32 ngroups0 = mol0.coordinates().count();
    const quint32 ngroups1 = mol1.coordinates().count();
    
    const CoordGroup *groups0_array = mol0.coordinates().constData();
    const CoordGroup *groups1_array = mol1.coordinates().constData();
    
    BOOST_ASSERT( mol0.parameters().atomicParameters().count() == int(ngroups0) );
    BOOST_ASSERT( mol1.parameters().atomicParameters().count() == int(ngroups1) );
    
    const Parameters::Array *molparams0_array 
                                = mol0.parameters().atomicParameters().constData();
    const Parameters::Array *molparams1_array 
                                = mol1.parameters().atomicParameters().constData();
    
    double cnrg = 0;
    double ljnrg = 0;
    
    //this uses the following potentials (equations 4 and 5 from Taylor et. al.)
    //
    //  V_{LJ}(r) = 4 epsilon [ (sigma^12 / (alpha^m sigma^6 + r^6)^2) - 
    //                          (sigma^6  / (alpha^m sigma^6 + r^6) ) ]
    //
    //  V_{coul}(r) = (1-alpha)^n q_i q_j / 4 pi eps_0 (alpha+r^2)^(1/2)
    
    const double one_minus_alfa_to_n = SireMaths::pow(1 - alfa, int(coul_power));
    const double alfa_to_m = SireMaths::pow(alfa, int(lj_power));
    
    #ifdef SIRE_TIME_ROUTINES
    int nflops = 0;
    #endif

    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        const Parameters::Array &params0 = molparams0_array[igroup];

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const Parameters::Array &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (not within_cutoff)
                //this CutGroup is either the cutoff distance
                continue;
            
            //calculate all of the interatomic distances^2
            const double mindist = spce->calcDist2(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
            {
                //all of the atoms are definitely beyond cutoff
                continue;
            }
               
            double icnrg = 0;
            double iljnrg = 0;
            
            //loop over all interatomic pairs and calculate the energies
            const quint32 nats1 = group1.count();
            const Parameter *params1_array = params1.constData();

            #ifdef SIRE_USE_SSE
            {
                const int remainder = nats1 % 2;
                
                __m128d sse_cnrg = { 0, 0 };
                __m128d sse_ljnrg = { 0, 0 };

                const __m128d sse_alpha = { alfa, alfa };
                const __m128d sse_alpha_to_m = { alfa_to_m, alfa_to_m };
                
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                    
                    __m128d sse_chg0 = { param0.reduced_charge, 
                                         param0.reduced_charge };
                                         
                    //process atoms in pairs (so can then use SSE)
                    for (quint32 j=0; j<nats1; j += 2)
                    {
                        const Parameter &param10 = params1_array[j];
                        const Parameter &param11 = params1_array[j+1];
                        
                        __m128d sse_dist2 = { distmat[j], distmat[j+1] };
                        __m128d sse_chg1 = { param10.reduced_charge,
                                             param11.reduced_charge };
                                           
                        const LJPair &ljpair0 = ljpairs.constData()[
                                                ljpairs.map(param0.ljid,
                                                            param10.ljid)];
                    
                        const LJPair &ljpair1 = ljpairs.constData()[
                                                ljpairs.map(param0.ljid,
                                                            param11.ljid)];
                    
                        __m128d sse_sig = { ljpair0.sigma(), ljpair1.sigma() };
                        __m128d sse_eps = { ljpair0.epsilon(), ljpair1.epsilon() };
                        
                        //calculate the coulomb energy
                        __m128d coul_denom = _mm_sqrt_pd( sse_dist2 + sse_alpha );
                        
                        sse_cnrg += sse_chg0 * sse_chg1 / coul_denom;
                        
                        #ifdef SIRE_TIME_ROUTINES
                        nflops += 10;
                        #endif
                        
                        //calculate alpha_m sig^6 + r^6
                        const __m128d sse_sig3 = sse_sig * sse_sig * sse_sig;
                        const __m128d sse_sig6 = sse_sig3 * sse_sig3;

                        const __m128d lj_denom = (sse_alpha_to_m * sse_sig6) + 
                                                 (sse_dist2 * sse_dist2 * sse_dist2);
                        
                        const __m128d sig6_over_denom = sse_sig6 / lj_denom;
                        const __m128d sig12_over_denom2 = sig6_over_denom * 
                                                          sig6_over_denom;
                                              
                        //calculate LJ energy (the factor of 4 is added later)
                        sse_ljnrg += sse_eps * (sig12_over_denom2 - sig6_over_denom);
                                                
                        #ifdef SIRE_TIME_ROUTINES
                        nflops += 24;
                        #endif
                    }
                          
                    if (remainder == 1)
                    {
                        const Parameter &param1 = params1_array[nats1-1];

                        const double dist2 = distmat[nats1-1];
                        
                        icnrg += param0.reduced_charge * param1.reduced_charge 
                                    / std::sqrt( alfa + dist2 );
                    
                        #ifdef SIRE_TIME_ROUTINES
                        nflops += 5;
                        #endif

                        const LJPair &ljpair = ljpairs.constData()[
                                                ljpairs.map(param0.ljid,
                                                            param1.ljid)];
                        
                        const double sig2 = ljpair.sigma() * ljpair.sigma();
                        const double sig6 = sig2 * sig2 * sig2;
                        
                        const double lj_denom = (alfa_to_m * sig6) + 
                                                (dist2 * dist2 * dist2);
                        
                        const double sig6_over_denom = sig6 / lj_denom;
                        const double sig12_over_denom2 = sig6_over_denom * 
                                                         sig6_over_denom;
                        
                        iljnrg += ljpair.epsilon() * (sig12_over_denom2 - 
                                                      sig6_over_denom);
                                                          
                        #ifdef SIRE_TIME_ROUTINES
                        nflops += 12;
                        #endif
                    }
                }
                
                icnrg += *((const double*)&sse_cnrg) +
                         *( ((const double*)&sse_cnrg) + 1 );
                         
                iljnrg += *((const double*)&sse_ljnrg) +
                          *( ((const double*)&sse_ljnrg) + 1 );
            }
            #else
            {
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    for (quint32 j=0; j<nats1; ++j)
                    {
                        const Parameter &param1 = params1_array[j];

                        const double dist2 = distmat[j];
                        
                        icnrg += param0.reduced_charge * param1.reduced_charge 
                                    / std::sqrt(alfa + dist2);
                    
                        #ifdef SIRE_TIME_ROUTINES
                        nflops += 4;
                        #endif

                        const LJPair &ljpair = ljpairs.constData()[
                                                ljpairs.map(param0.ljid,
                                                            param1.ljid)];
                        
                        const double sig2 = ljpair.sigma() * ljpair.sigma();
                        const double sig6 = sig2 * sig2 * sig2;
                        
                        const double lj_denom = (alfa_to_m * sig6) + 
                                                (dist2 * dist2 * dist2);
                        
                        const double sig6_over_denom = sig6 / lj_denom;
                        const double sig12_over_denom2 = sig6_over_denom *
                                                         sig6_over_denom;
    
                        iljnrg += ljpair.epsilon() * (sig12_over_denom2 - 
                                                      sig6_over_denom);
                                                          
                        #ifdef SIRE_TIME_ROUTINES
                        nflops += 12;
                        #endif
                    }
                }
            }
            #endif
            
            //are we shifting the electrostatic potential?
            if (use_electrostatic_shifting)
            {
                icnrg -= this->totalCharge(params0) * this->totalCharge(params1)
                              / switchfunc->electrostaticCutoffDistance();
                        
                #ifdef SIRE_TIME_ROUTINES      
                nflops += 3;
                #endif
            }
            
            //now add these energies onto the total for the molecule,
            //scaled by any non-bonded feather factor
            if (mindist > switchfunc->featherDistance())
            {
                cnrg += switchfunc->electrostaticScaleFactor( Length(mindist) ) * icnrg;
                ljnrg += switchfunc->vdwScaleFactor( Length(mindist) ) * iljnrg;
                
                #ifdef SIRE_TIME_ROUTINES
                nflops += 4;
                #endif
            }
            else
            {
                cnrg += icnrg;
                ljnrg += iljnrg;
                
                #ifdef SIRE_TIME_ROUTINES
                nflops += 2;
                #endif
            }
        }
    }
    
    //add this molecule pair's energy onto the total
    //(also multiply LJ by 4 as it is 4 * epsilon ((sig/r)^12 - (sig/r)^6))
    //(also multiply COUL by (1-alpha)^n)
    energy += Energy(scale_energy * one_minus_alfa_to_n * cnrg, 
                     4 * scale_energy * ljnrg);
    
    #ifdef SIRE_TIME_ROUTINES
    nflops += 6;
    ADD_FLOPS(nflops);
    #endif
}

/** Add to the forces in 'forces0' the forces acting on 'mol0' caused
    by 'mol1' */
void InterSoftCLJPotential::_pvt_calculateForce(
                                         const InterSoftCLJPotential::Molecule &mol0, 
                                         const InterSoftCLJPotential::Molecule &mol1,
                                         MolForceTable &forces0, 
                                         InterSoftCLJPotential::ForceWorkspace &distmat,
                                         double scale_force) const
{
    throw SireError::incomplete_code( QObject::tr(
            "Soft-core forces are not yet implemented!"), CODELOC );

    BOOST_ASSERT( mol0.molecule().data().info().nCutGroups() == forces0.nCutGroups() );
    BOOST_ASSERT( mol0.molecule().data().number() == forces0.molNum() );

    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordinates().constData();
    const CoordGroup *groups1_array = mol1.coordinates().constData();
    
    BOOST_ASSERT(mol0.parameters().atomicParameters().count() == int(ngroups0));
    BOOST_ASSERT(mol1.parameters().atomicParameters().count() == int(ngroups1));
    
    const Parameters::Array *molparams0_array 
                                    = mol0.parameters().atomicParameters().constData();
    const Parameters::Array *molparams1_array
                                    = mol1.parameters().atomicParameters().constData();
    
    const MolForceTable::Array *forces0_array = forces0.constData();
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        //get the CGIdx of this group
        CGIdx cgidx_igroup = mol0.cgIdx(igroup);

        //get the index of this CutGroup in the forces array
        int force0_idx = forces0.map(cgidx_igroup);
        
        if (force0_idx == -1)
            //there is no space for the forces on this CutGroup in 
            //the forcetable - were are therefore not interested in
            //this CutGroup
            continue;

        const Parameters::Array &params0 = molparams0_array[igroup];

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        //get the table that holds the forces acting on all of the
        //atoms of this CutGroup (tables are indexed by CGIdx)
        BOOST_ASSERT(forces0_array[force0_idx].count() == int(nats0));
    
        Vector *group_forces0_array = forces0.data(force0_idx);

        //ok, we are interested in the forces acting on this CutGroup
        // - calculate all of the forces on this group interacting
        //   with all of the CutGroups in mol1 
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const Parameters::Array &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (not within_cutoff)
                //this CutGroup is beyond the cutoff distance
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = spce->calcDistVectors(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;

            const quint32 nats1 = group1.count();
            
            //loop over all interatomic pairs and calculate the energies
            const Parameter *params1_array = params1.constData();

            if (mindist > switchfunc->featherDistance())
            {
                //we need to calculate the forces taking into account
                //the derivative of the switching function!
                
                //calculate the switching scale factors and their 
                //derivatives
                const double scl_coul = switchfunc->electrostaticScaleFactor( 
                                                                        Length(mindist) );
                const double scl_lj = switchfunc->vdwScaleFactor( Length(mindist) );
                
                Vector group_sep = (group1.aaBox().center() - aabox0.center())
                                        .normalise();
                
                Vector dscl_coul = switchfunc->dElectrostaticScaleFactor( 
                                                                    Length(mindist) ) 
                                     * group_sep;
                                     
                Vector dscl_lj = switchfunc->dVDWScaleFactor( Length(mindist) )
                                     * group_sep;
                
                double shift_coul = 0;

                if (use_electrostatic_shifting)
                    shift_coul = this->totalCharge(params0) * this->totalCharge(params1)
                                    / switchfunc->electrostaticCutoffDistance();
                
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    Vector total_force;
                
                    if (param0.ljid == 0)
                    {
                        //null LJ parameter - only add on the coulomb energy
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            const double q2 = param0.reduced_charge *
                                              params1_array[j].reduced_charge;
                            
                            if (q2 != 0)
                            {
                                //calculate the coulomb energy
                                const double cnrg = q2 / distmat[j].length();
                                               
                                //calculate the coulomb force
                                Vector cforce = (scl_coul * -cnrg / distmat[j].length() *
                                                 distmat[j].direction()) +
                                             
                                                ((cnrg-shift_coul) * dscl_coul);
                        
                                total_force -= cforce;
                            }
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j].length();
                        
                            Vector force;
                            
                            const double q2 = param0.reduced_charge *
                                              param1.reduced_charge;
                        
                            if (q2 != 0)
                            {
                                //calculate the energy
                                const double cnrg = q2 * invdist;
                        
                                //calculate the force
                                force = (scl_coul * -cnrg / distmat[j].length() *
                                         distmat[j].direction()) +
                                             
                                         ((cnrg-shift_coul) * dscl_coul);
                            }
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                //calculate the energy
                                const double ljnrg = 4 * ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                force += ((scl_lj * ljpair.epsilon() * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                            * distmat[j].direction())
                                            
                                          + (ljnrg * dscl_lj);
                            }

                            total_force -= force;
                        }
                    }
                    
                    //update the forces array
                    group_forces0_array[i] += scale_force * total_force;
                }
            }
            else
            {
                //not in the feather region, so can calculate the forces
                //directly (also, no need to calculate shift, as 
                //the shifting function is constant, so does not
                //affect the gradient)
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];

                    Vector total_force;
                
                    if (param0.ljid == 0)
                    {
                        //null LJ parameter - only add on the coulomb energy
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            const double q2 = param0.reduced_charge * 
                                              params1_array[j].reduced_charge;
                        
                            //calculate the coulomb force
                            if (q2 != 0)
                            {
                                Vector cforce = -(q2 / distmat[j].length2()) *
                                                    distmat[j].direction();
                        
                                total_force -= cforce;
                            }
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j].length();
                            const double invdist2 = pow_2(invdist);
                        
                            //calculate the force
                            Vector force = -(param0.reduced_charge * 
                                             param1.reduced_charge * invdist2) 
                                            
                                            * distmat[j].direction();
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                force += (4 * ljpair.epsilon() * (6.0*sig_over_dist6 - 
                                                              12.0*sig_over_dist12))
                                        * distmat[j].direction();
                            }
                        
                            total_force -= force;
                        }
                    }
                    
                    group_forces0_array[i] += scale_force * total_force;

                } // end of loop over i atoms

            } // end of if within feather

        } // end of loop over jgroup CutGroups

    } // end of loop over igroup CutGroups
}

/** Add to the forces in 'forces0' the forces acting on 'mol0' caused
    by 'mol1' */
void InterSoftCLJPotential::_pvt_calculateCoulombForce(
                                         const InterSoftCLJPotential::Molecule &mol0, 
                                         const InterSoftCLJPotential::Molecule &mol1,
                                         MolForceTable &forces0, 
                                         InterSoftCLJPotential::ForceWorkspace &distmat,
                                         double scale_force) const
{
    throw SireError::incomplete_code( QObject::tr(
            "Soft-core forces are not yet implemented!"), CODELOC );

    BOOST_ASSERT( mol0.molecule().data().info().nCutGroups() == forces0.nCutGroups() );
    BOOST_ASSERT( mol0.molecule().data().number() == forces0.molNum() );

    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordinates().constData();
    const CoordGroup *groups1_array = mol1.coordinates().constData();
    
    BOOST_ASSERT(mol0.parameters().atomicParameters().count() == int(ngroups0));
    BOOST_ASSERT(mol1.parameters().atomicParameters().count() == int(ngroups1));
    
    const Parameters::Array *molparams0_array 
                                    = mol0.parameters().atomicParameters().constData();
    const Parameters::Array *molparams1_array
                                    = mol1.parameters().atomicParameters().constData();
    
    const MolForceTable::Array *forces0_array = forces0.constData();
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        //get the CGIdx of this group
        CGIdx cgidx_igroup = mol0.cgIdx(igroup);

        //get the index of this CutGroup in the forces array
        int force0_idx = forces0.map(cgidx_igroup);
        
        if (force0_idx == -1)
            //there is no space for the forces on this CutGroup in 
            //the forcetable - were are therefore not interested in
            //this CutGroup
            continue;

        const Parameters::Array &params0 = molparams0_array[igroup];

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        //get the table that holds the forces acting on all of the
        //atoms of this CutGroup (tables are indexed by CGIdx)
        BOOST_ASSERT(forces0_array[force0_idx].count() == int(nats0));
    
        Vector *group_forces0_array = forces0.data(force0_idx);

        //ok, we are interested in the forces acting on this CutGroup
        // - calculate all of the forces on this group interacting
        //   with all of the CutGroups in mol1 
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const Parameters::Array &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (not within_cutoff)
                //this CutGroup is beyond the cutoff distance
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = spce->calcDistVectors(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;

            const quint32 nats1 = group1.count();
            
            //loop over all interatomic pairs and calculate the energies
            const Parameter *params1_array = params1.constData();

            if (mindist > switchfunc->featherDistance())
            {
                //we need to calculate the forces taking into account
                //the derivative of the switching function!
                
                //calculate the switching scale factors and their 
                //derivatives
                const double scl_coul = switchfunc->electrostaticScaleFactor(
                                                                    Length(mindist) );
                
                Vector group_sep = (group1.aaBox().center() - aabox0.center())
                                        .normalise();
                
                Vector dscl_coul = switchfunc->dElectrostaticScaleFactor(
                                                                    Length(mindist) ) 
                                     * group_sep;
                
                double shift_coul = 0;

                if (use_electrostatic_shifting)
                    shift_coul = this->totalCharge(params0) * this->totalCharge(params1)
                                    / switchfunc->electrostaticCutoffDistance();
                
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    Vector total_force;
                
                    for (quint32 j=0; j<nats1; ++j)
                    {
                        const double q2 = param0.reduced_charge *
                                          params1_array[j].reduced_charge;
                          
                        if (q2 != 0)
                        {
                            //calculate the coulomb energy
                            const double cnrg = q2 / distmat[j].length();
                                               
                            //calculate the coulomb force
                            Vector cforce = (scl_coul * -cnrg / distmat[j].length() *
                                             distmat[j].direction()) +
                                             
                                            ((cnrg-shift_coul) * dscl_coul);
                        
                            total_force -= cforce;
                        }
                    }
                    
                    //update the forces array
                    group_forces0_array[i] += scale_force * total_force;
                }
            }
            else
            {
                //not in the feather region, so can calculate the forces
                //directly (also, no need to calculate shift, as 
                //the shifting function is constant, so does not
                //affect the gradient)
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];

                    Vector total_force;
                
                    //null LJ parameter - only add on the coulomb energy
                    for (quint32 j=0; j<nats1; ++j)
                    {
                        const double q2 = param0.reduced_charge * 
                                          params1_array[j].reduced_charge;
                        
                        //calculate the coulomb force
                        if (q2 != 0)
                        {
                            Vector cforce = -(q2 / distmat[j].length2()) *
                                                distmat[j].direction();
                        
                            total_force -= cforce;
                        }
                    }
                    
                    group_forces0_array[i] += scale_force * total_force;

                } // end of loop over i atoms

            } // end of if within feather

        } // end of loop over jgroup CutGroups

    } // end of loop over igroup CutGroups
}

/** Add to the forces in 'forces0' the forces acting on 'mol0' caused
    by 'mol1' */
void InterSoftCLJPotential::_pvt_calculateLJForce(
                                         const InterSoftCLJPotential::Molecule &mol0, 
                                         const InterSoftCLJPotential::Molecule &mol1,
                                         MolForceTable &forces0, 
                                         InterSoftCLJPotential::ForceWorkspace &distmat,
                                         double scale_force) const
{
    throw SireError::incomplete_code( QObject::tr(
            "Soft-core forces are not yet implemented!"), CODELOC );

    BOOST_ASSERT( mol0.molecule().data().info().nCutGroups() == forces0.nCutGroups() );
    BOOST_ASSERT( mol0.molecule().data().number() == forces0.molNum() );

    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordinates().constData();
    const CoordGroup *groups1_array = mol1.coordinates().constData();
    
    BOOST_ASSERT(mol0.parameters().atomicParameters().count() == int(ngroups0));
    BOOST_ASSERT(mol1.parameters().atomicParameters().count() == int(ngroups1));
    
    const Parameters::Array *molparams0_array 
                                    = mol0.parameters().atomicParameters().constData();
    const Parameters::Array *molparams1_array
                                    = mol1.parameters().atomicParameters().constData();
    
    const MolForceTable::Array *forces0_array = forces0.constData();
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        //get the CGIdx of this group
        CGIdx cgidx_igroup = mol0.cgIdx(igroup);

        //get the index of this CutGroup in the forces array
        int force0_idx = forces0.map(cgidx_igroup);
        
        if (force0_idx == -1)
            //there is no space for the forces on this CutGroup in 
            //the forcetable - were are therefore not interested in
            //this CutGroup
            continue;

        const Parameters::Array &params0 = molparams0_array[igroup];

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        //get the table that holds the forces acting on all of the
        //atoms of this CutGroup (tables are indexed by CGIdx)
        BOOST_ASSERT(forces0_array[force0_idx].count() == int(nats0));
    
        Vector *group_forces0_array = forces0.data(force0_idx);

        //ok, we are interested in the forces acting on this CutGroup
        // - calculate all of the forces on this group interacting
        //   with all of the CutGroups in mol1 
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const Parameters::Array &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (not within_cutoff)
                //this CutGroup is beyond the cutoff distance
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = spce->calcDistVectors(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;

            const quint32 nats1 = group1.count();
            
            //loop over all interatomic pairs and calculate the energies
            const Parameter *params1_array = params1.constData();

            if (mindist > switchfunc->featherDistance())
            {
                //we need to calculate the forces taking into account
                //the derivative of the switching function!
                
                //calculate the switching scale factors and their 
                //derivatives
                const double scl_lj = switchfunc->vdwScaleFactor( Length(mindist) );
                
                Vector group_sep = (group1.aaBox().center() - aabox0.center())
                                        .normalise();
                
                Vector dscl_lj = switchfunc->dVDWScaleFactor( Length(mindist) )
                                     * group_sep;
                
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    Vector total_force;
                
                    if (param0.ljid != 0)
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            const Parameter &param1 = params1_array[j];

                            if (param1.ljid != 0)
                            {
                                const double invdist = double(1) / distmat[j].length();

                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                //calculate the energy
                                const double ljnrg = 4 * ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                Vector force = ((scl_lj * 4 * ljpair.epsilon() * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                               * distmat[j].direction())
                                            
                                             + (ljnrg * dscl_lj);

                                total_force -= force;
                            }
                        }
                    }
                    
                    //update the forces array
                    group_forces0_array[i] += scale_force * total_force;
                }
            }
            else
            {
                //not in the feather region, so can calculate the forces
                //directly
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];

                    Vector total_force;
                
                    if (param0.ljid != 0)
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                              
                            if (param1.ljid != 0)
                            {
                                const double invdist = double(1) / distmat[j].length();

                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                Vector force = (4 * ljpair.epsilon() * 
                                                               (6.0*sig_over_dist6 - 
                                                                12.0*sig_over_dist12))
                                                * distmat[j].direction();

                                total_force -= force;
                            }
                        }
                    }
                    
                    group_forces0_array[i] += scale_force * total_force;

                } // end of loop over i atoms

            } // end of if within feather

        } // end of loop over jgroup CutGroups

    } // end of loop over igroup CutGroups
}
