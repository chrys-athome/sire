/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "fastintercljff.h"

#include "atomljs.h"

#include "SireMol/molecule.h"
#include "SireMol/viewsofmol.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/atomcoords.h"
#include "SireMol/atomcharges.h"

#include "SireBase/propertymap.h"
#include "SireBase/properties.h"

#include "SireError/errors.h"
#include "SireBase/errors.h"

#include "tostring.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireMM;
using namespace SireFF;
using namespace SireMaths;
using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<FastInterCLJFF> r_fastintercljff;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const FastInterCLJFF &fastcljff)
{
    throw SireError::incomplete_code( "TODO", CODELOC );

    writeHeader(ds, r_fastintercljff, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const G1FF&>(fastcljff);
    
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       FastInterCLJFF &fastcljff)
{
    throw SireError::incomplete_code( "TODO", CODELOC );

    VersionID v = readHeader(ds, r_fastintercljff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> static_cast<G1FF&>(fastcljff);
    }
    else
        throw version_error(v, "1", r_fastintercljff, CODELOC);
        
    return ds;
}

/** Rebuild the properties object from the current properties */
void FastInterCLJFF::rebuildProperties()
{
    props.clear();
    props.setProperty("patching", ptchs.patching());
    props.setProperty("space", ptchs.space());
    props.setProperty("switchingFunction", switchfunc);
    props.setProperty("shiftElectrostatics", VariantProperty(use_electrostatic_shifting));
    props.setProperty("combiningRules", 
                        VariantProperty(LJParameterDB::toString(combining_rules)));
}

/** Construct an un-named, empty FastInterCLJFF */
FastInterCLJFF::FastInterCLJFF() 
               : ConcreteProperty<FastInterCLJFF,G1FF>(), FF3D(),
                 switchfunc( SwitchingFunction::null() ),
                 combining_rules( LJParameterDB::interpret("arithmetic") ),
                 need_update_ljpairs(true), use_electrostatic_shifting(false),
                 recording_changes(false)
{
    this->_pvt_updateName();
    this->rebuildProperties();
}

/** Construct a named, empty FastInterCLJFF */
FastInterCLJFF::FastInterCLJFF(const QString &name)
               : ConcreteProperty<FastInterCLJFF,G1FF>(), FF3D(),
                 switchfunc( SwitchingFunction::null() ),
                 combining_rules( LJParameterDB::interpret("arithmetic") ),
                 need_update_ljpairs(true), use_electrostatic_shifting(false),
                 recording_changes(false)
{
    FF::setName(name);
    this->_pvt_updateName();
    this->rebuildProperties();
}

/** Copy constructor */
FastInterCLJFF::FastInterCLJFF(const FastInterCLJFF &other)
               : ConcreteProperty<FastInterCLJFF,G1FF>(other), FF3D(other),
                 ffcomponents(other.ffcomponents),
                 ljpairs(other.ljpairs),
                 props(other.props),
                 mol_to_beadid(other.mol_to_beadid),
                 beads_by_molnum(other.beads_by_molnum),
                 switchfunc(other.switchfunc),
                 combining_rules(other.combining_rules),
                 ptchs(other.ptchs),
                 added_beads(other.added_beads),
                 removed_beads(other.removed_beads),
                 changed_beads(other.changed_beads),
                 need_update_ljpairs(other.need_update_ljpairs),
                 use_electrostatic_shifting(other.use_electrostatic_shifting),
                 recording_changes(other.recording_changes)
{}

/** Destructor */
FastInterCLJFF::~FastInterCLJFF()
{}

const char* FastInterCLJFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<FastInterCLJFF>() );
}

/** Copy assignment operator */
FastInterCLJFF& FastInterCLJFF::operator=(const FastInterCLJFF &other)
{
    if (this != &other)
    {
        G1FF::operator=(other);
        ffcomponents = other.ffcomponents;
        ljpairs = other.ljpairs;
        props = other.props;
        mol_to_beadid = other.mol_to_beadid;
        beads_by_molnum = other.beads_by_molnum;
        switchfunc = other.switchfunc;
        combining_rules = other.combining_rules;
        ptchs = other.ptchs;
        added_beads = other.added_beads;
        removed_beads = other.removed_beads;
        changed_beads = other.changed_beads;
        need_update_ljpairs = other.need_update_ljpairs;
        use_electrostatic_shifting = other.use_electrostatic_shifting;
        recording_changes = other.recording_changes;
    }
    
    return *this;
}

/** Comparison operator */
bool FastInterCLJFF::operator==(const FastInterCLJFF &other) const
{
    throw SireError::incomplete_code( "TODO", CODELOC );

    return G1FF::operator==(other) and
           props == other.props;
}

/** Comparison operator */
bool FastInterCLJFF::operator!=(const FastInterCLJFF &other) const
{
    return not FastInterCLJFF::operator==(other);
}

FastInterCLJFF* FastInterCLJFF::clone() const
{
    return new FastInterCLJFF(*this);
}

const CLJComponent& FastInterCLJFF::_pvt_components() const
{
    return ffcomponents;
}

/** Internal function used to return whether or not this
    forcefield is recording changes (so that an energy delta
    is calculated, rather than calculating the energy from scratch) */
bool FastInterCLJFF::recordingChanges() const
{
    return recording_changes;
}

/** Call to rebuild the entire internal state from the current
    molecules in this forcefield */
void FastInterCLJFF::rebuildAll()
{
    this->mustNowRecalculateFromScratch();
    need_update_ljpairs = true;
    
    switchfunc = props.property("switchingFunction").asA<SwitchingFunction>();
    ptchs = Patches( props.property("patching").asA<Patching>() );
    combining_rules = LJParameterDB::interpret( props.property("combiningRules")
                                        .asA<VariantProperty>().convertTo<QString>() );
    use_electrostatic_shifting = props.property("shiftElectrostatics")
                                      .asA<VariantProperty>().convertTo<bool>();
    
    //now rebuild all of the bead groups from the list of molecules
    const Molecules &mols = this->operator[](MGIdx(0)).molecules();
    
    ChunkedHash< MolNum,QPair<Beads,PropertyMap> > old_beads = beads_by_molnum;
    
    mol_to_beadid.clear();
    beads_by_molnum.clear();
    added_beads.clear();
    removed_beads.clear();
    changed_beads.clear();
    
    for (Molecules::const_iterator it = mols.constBegin();
         it != mols.constEnd();
         ++it)
    {
        this->_pvt_added( *it, old_beads.value(it.key()).second );
    }

    //the energy almost certainly will need recalculating
    FF::setDirty();
}

typedef PairMatrix<double> DistMatrix;

/** Internal function used to calculate the energy of the passed two CoordGroups */
static void addEnergy(const CoordGroup &icoords, const CLJParams &iparams,
                      const CoordGroup &jcoords, const CLJParams &jparams,
                      const SwitchingFunction &switchfunc, double mindist,
                      DistMatrix &distmat, const LJPairMatrix &ljpairs,
                      double &cnrg, double &ljnrg, double scale)
{
    static const Cartesian cart_space;

    const int inats = icoords.count();
    const int jnats = jcoords.count();

    if (inats == 0 or jnats == 0 or scale == 0)
        return;

    double icnrg = 0;
    double iljnrg = 0;
    
    //loop over all interatomic pairs and calculate the energies
    const CLJParam *iparams_array = iparams.constData();
    const CLJParam *jparams_array = jparams.constData();

    #ifdef SIRE_USE_SSE
    {
        const int remainder = jnats % 2;
        
        __m128d sse_cnrg = { 0, 0 };
        __m128d sse_ljnrg = { 0, 0 };

        const __m128d sse_one = { 1.0, 1.0 };
        
        for (int i=0; i<inats; ++i)
        {
            distmat.setOuterIndex(i);
            const CLJParam &iparam = iparams_array[i];
            
            __m128d sse_chg0 = _mm_set_pd( iparam.q(), iparam.q() );
                                 
            //process atoms in pairs (so can then use SSE)
            for (int j=0; j<jnats-1; j += 2)
            {
                const CLJParam &jparam0 = jparams_array[j];
                const CLJParam &jparam1 = jparams_array[j+1];
                
                __m128d sse_dist = _mm_set_pd( distmat[j], distmat[j+1] );
                __m128d sse_chg1 = _mm_set_pd( jparam0.q(), jparam1.q() );
                                   
                const LJPair &ljpair0 = ljpairs.constData()[
                                        ljpairs.map(iparam.ljID(), jparam0.ljID()) ];
            
                const LJPair &ljpair1 = ljpairs.constData()[
                                        ljpairs.map(iparam.ljID(), jparam1.ljID()) ];
            
                __m128d sse_sig = _mm_set_pd( ljpair0.sigma(), ljpair1.sigma() );
                __m128d sse_eps = _mm_set_pd( ljpair0.epsilon(), 
                                              ljpair1.epsilon() );
                
                sse_dist = _mm_div_pd(sse_one, sse_dist);
                
                //calculate the coulomb energy
                __m128d tmp = _mm_mul_pd(sse_chg0, sse_chg1);
                tmp = _mm_mul_pd(tmp, sse_dist);
                sse_cnrg = _mm_add_pd(sse_cnrg, tmp);
                
                #ifdef SIRE_TIME_ROUTINES
                nflops += 8;
                #endif
                
                //calculate (sigma/r)^6 and (sigma/r)^12
                __m128d sse_sig_over_dist2 = _mm_mul_pd(sse_sig, sse_dist);
                sse_sig_over_dist2 = _mm_mul_pd( sse_sig_over_dist2,  
                                                 sse_sig_over_dist2 );
                                             
                __m128d sse_sig_over_dist6 = _mm_mul_pd(sse_sig_over_dist2,
                                                        sse_sig_over_dist2);
                                                
                sse_sig_over_dist6 = _mm_mul_pd(sse_sig_over_dist6,
                                                sse_sig_over_dist2);
                                             
                __m128d sse_sig_over_dist12 = _mm_mul_pd(sse_sig_over_dist6,
                                                         sse_sig_over_dist6);
                                      
                //calculate LJ energy (the factor of 4 is added later)
                tmp = _mm_sub_pd(sse_sig_over_dist12, 
                                 sse_sig_over_dist6);
                                         
                tmp = _mm_mul_pd(tmp, sse_eps);
                sse_ljnrg = _mm_add_pd(sse_ljnrg, tmp);
                                        
                #ifdef SIRE_TIME_ROUTINES
                nflops += 16;
                #endif
            }
                  
            if (remainder == 1)
            {
                const CLJParam &jparam = jparams_array[jnats-1];

                const double invdist = double(1) / distmat[jnats-1];
                
                icnrg += iparam.q() * jparam.q() * invdist;
            
                #ifdef SIRE_TIME_ROUTINES
                nflops += 4;
                #endif

                const LJPair &ljpair = ljpairs.constData()[
                                        ljpairs.map(iparam.ljID(), jparam.ljID()) ];
                
                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                double sig_over_dist12 = pow_2(sig_over_dist6);

                iljnrg += ljpair.epsilon() * (sig_over_dist12 - 
                                              sig_over_dist6);
                                                  
                #ifdef SIRE_TIME_ROUTINES
                nflops += 8;
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

                const double invdist = double(1) / distmat[j];
                
                icnrg += param0.reduced_charge * param1.reduced_charge 
                            * invdist;
            
                #ifdef SIRE_TIME_ROUTINES
                nflops += 4;
                #endif

                const LJPair &ljpair = ljpairs.constData()[
                                        ljpairs.map(param0.ljid,
                                                    param1.ljid)];

                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                double sig_over_dist12 = pow_2(sig_over_dist6);

                iljnrg += ljpair.epsilon() * (sig_over_dist12 - 
                                              sig_over_dist6);
                                                  
                #ifdef SIRE_TIME_ROUTINES
                nflops += 8;
                #endif
            }
        }
    }
    #endif
    
    //now add these energies onto the total for the molecule,
    //scaled by any non-bonded feather factor
    if (mindist > switchfunc.featherDistance())
    {
        cnrg += scale * switchfunc.electrostaticScaleFactor( Length(mindist) ) * icnrg;
        ljnrg += scale * switchfunc.vdwScaleFactor( Length(mindist) ) * 4 * iljnrg;
        
        #ifdef SIRE_TIME_ROUTINES
        nflops += 4;
        #endif
    }
    else
    {
        cnrg += scale * icnrg;
        ljnrg += scale * 4 * iljnrg;
        
        #ifdef SIRE_TIME_ROUTINES
        nflops += 2;
        #endif
    }
}

/** Recalculate the energy - this will either increment the energy
    or will recaculate it from scratch */
void FastInterCLJFF::recalculateEnergy()
{
    if (need_update_ljpairs)
    {
        //rebuild the LJ pair parameter database
        ljpairs = LJParameterDB::getLJPairs(combining_rules);
        need_update_ljpairs = false;
        this->mustNowRecalculateFromScratch();
    }

    if ( this->recordingChanges() )
    {
        if (added_beads.isEmpty() and removed_beads.isEmpty() and
            changed_beads.isEmpty())
        {
            //nothing has changed
            return;
        }
    
        //calculate an energy delta
        
    }
    else
    {
        //calculate the energy from scratch - loop over each patch,
        //then pair of patches
        
        const Patch *patches_array = ptchs.constData();
        const int npatches = ptchs.nPatches();
        
        qDebug() << npatches;
        
        double cnrg = 0;
        double ljnrg = 0;
        const double cutoff = switchfunc.read().cutoffDistance();
        
        PairMatrix<double> distmatrix;
        
        //Cartesian cart_space;
        const Space &spce = this->space();
        
        for (int ip=0; ip<npatches; ++ip)
        {
            const Patch &patch = patches_array[ip];
            const int nbeads = patch.nBeads();
            
            if (nbeads < 2)
                continue;
            
            const CoordGroup *coords_array = patch.coordinates().constData();
            const CLJParamsArray::Array *params_array = patch.parameters()
                                            .asA<CLJParamsArray>().constData();
                                               
            for (int i=0; i<nbeads-1; ++i)
            {
                const CoordGroup &ibead_coords = coords_array[i];
                const CLJParamsArray::Array &ibead_params = params_array[i];
                
                for (int j=i+1; j<nbeads; ++j)
                {
                    const CoordGroup &jbead_coords = coords_array[j];
                    const CLJParamsArray::Array &jbead_params = params_array[j];

                    if (spce.beyond(cutoff, ibead_coords, jbead_coords))
                        continue;
                        
                    double mindist = spce.calcDist(ibead_coords, jbead_coords,
                                                   distmatrix);
                    
                    if (mindist > cutoff)
                        continue;
                    
                    addEnergy(ibead_coords, ibead_params,
                              jbead_coords, jbead_params,
                              switchfunc.read(), mindist,
                              distmatrix, ljpairs,
                              cnrg, ljnrg, 1);
                }
            }
        }
        
        for (int ip=0; ip<npatches-1; ++ip)
        {
            const Patch &ipatch = patches_array[ip];
            
            const int inbeads = ipatch.nBeads();
            
            if (inbeads == 0)
                continue;

            const CoordGroup *icoords_array = ipatch.coordinates().constData();
            const CLJParamsArray::Array *iparams_array = ipatch.parameters()
                                                   .asA<CLJParamsArray>().constData();
                
            for (int jp=ip+1; jp<npatches; ++jp)
            {
                const Patch &jpatch = patches_array[jp];
                
                const int jnbeads = jpatch.nBeads();
                
                if (jnbeads == 0)
                    continue;
                    
                //calculate the minimum distance between these
                //two patches
                if (spce.beyond(cutoff, ipatch.aaBox(), jpatch.aaBox()))
                {
                    //the patches are beyond cutoff, so the contents must also
                    //be beyond cutoff
                    continue;
                }
                
                const CoordGroup *jcoords_array = jpatch.coordinates().constData();
                const CLJParamsArray::Array *jparams_array = jpatch.parameters()
                                                       .asA<CLJParamsArray>().constData();
                                                       
                for (int i=0; i<inbeads; ++i)
                {
                    const CoordGroup &ibead_coords = icoords_array[i];
                    const CLJParamsArray::Array &ibead_params = iparams_array[i];
                    
                    if (spce.beyond(cutoff, ibead_coords.aaBox(), jpatch.aaBox()))
                        continue;
                    
                    for (int j=0; j<jnbeads; ++j)
                    {
                        const CoordGroup &jbead_coords = jcoords_array[j];
                        const CLJParamsArray::Array &jbead_params = jparams_array[j];
                        
                        if (spce.beyond(cutoff, ibead_coords.aaBox(), 
                                                      jbead_coords.aaBox()))
                        {
                            continue;
                        }
                        
                        const double mindist = spce.calcDist(ibead_coords,
                                                                   jbead_coords,
                                                                   distmatrix);
                        
                        if (mindist > cutoff)
                            continue;
                        
                        addEnergy(ibead_coords, ibead_params,
                                  jbead_coords, jbead_params,
                                  switchfunc.read(), mindist,
                                  distmatrix, ljpairs,
                                  cnrg, ljnrg, 1);
                    }
                }
            }
        }
        
        qDebug() << "CNRG" << cnrg << "LJNRG" << ljnrg
                 << "TOTAL" << (cnrg+ljnrg);
        
        this->components().setEnergy(*this, CLJEnergy(cnrg, ljnrg));
    }
    
    recording_changes = true;
}

void FastInterCLJFF::_pvt_updateName()
{
    ffcomponents = CLJComponent(this->name());
    G1FF::_pvt_updateName();
}

/** Internal function used to merge the passed charge and LJ parameters
    into a single array (and also convert the LJ parameters into a
    lookup ID into the LJ parameter database, and multiplies the charges
    by sqrt(1 / 4 pi epsilon_0)) */
static CLJParamsArray mergeCLJ(const AtomCharges &charges, const AtomLJs &ljs)
{
    try
    {
        const int nbeads = charges.array().nArrays();
        BOOST_ASSERT( ljs.array().nArrays() == nbeads );
        
        const AtomCharges::Array *atom_charges_array = charges.constData();
        const AtomLJs::Array *atom_ljs_array = ljs.constData();

        QVector< QVector<CLJParam> > cljparams( nbeads );
        QVector<CLJParam> *cljparams_array = cljparams.data();
        
        LJParameterDB::lock();
    
        for (int i=0; i<nbeads; ++i)
        {
            const AtomCharges::Array &atom_charges = atom_charges_array[i];
            const AtomLJs::Array &atom_ljs = atom_ljs_array[i];
        
            const int nats = atom_charges.count();
            BOOST_ASSERT( atom_ljs.count() == nats );
            
            const SireUnits::Dimension::Charge *chgs_array = atom_charges.constData();
            const LJParameter *ljs_array = atom_ljs.constData();
            
            QVector<CLJParam> bead_cljparams(nats);
            CLJParam *bead_cljparams_array = bead_cljparams.data();
            
            for (int j=0; j<nats; ++j)
            {
                bead_cljparams_array[j] = CLJParam( chgs_array[j], 
                                                    ljs_array[j], false );
            }
            
            cljparams_array[i] = bead_cljparams;
        }
        
        LJParameterDB::unlock();
        
        return CLJParamsArray(cljparams);
    }
    catch(...)
    {
        LJParameterDB::unlock();
        throw;
    }
}

/** Internal function used to create the parameters used to
    represent the beads in this forcefield */
static QPair<CoordGroupArray,CLJParamsArray> 
createParameters(const Beads &beads, const PropertyMap &map)
{
    AtomCoords coords = beads.atomProperty( map["coordinates"] )
                             .read().asA<AtomCoords>();

    AtomCharges charges = beads.atomProperty( map["charge"] )
                               .read().asA<AtomCharges>();
                                
    AtomLJs ljs = beads.atomProperty( map["LJ"] )
                       .read().asA<AtomLJs>();
                       
    return QPair<CoordGroupArray,CLJParamsArray>(coords.array(),
                                                 mergeCLJ(charges, ljs));
}

template<class C, class T>
static void remove(C &container, const T &values)
{
    for (typename T::const_iterator it = values.constBegin();
         it != values.constEnd();
         ++it)
    {
        container.remove(*it);
    }
}

template<class C, class T>
static void insert(C &container, const T &values)
{
    for (typename T::const_iterator it = values.constBegin();
         it != values.constEnd();
         ++it)
    {
        container.insert(*it);
    }
}

/** Called when the passed molecule view has been added to the forcefield */
void FastInterCLJFF::_pvt_added(const PartialMolecule &mol, const PropertyMap &map)
{
    if (not mol.selectedAll())
        throw SireError::unsupported( QObject::tr(
                "The FastInterCLJFF currently only supports whole molecules!"),
                    CODELOC );

    if (mol_to_beadid.contains(mol.number()))
    {
        //remove the current version of the molecule
        QVector<quint32> beadids = mol_to_beadid.value(mol.number());
        
        ptchs.remove(beadids);
        ::remove(added_beads, beadids);
        ::remove(removed_beads, beadids);
        ::remove(changed_beads, beadids);

        mol_to_beadid.remove(mol.number());
        beads_by_molnum.remove(mol.number());
    }
    
    //break the molecule into beads
    Beads beads = Beads(mol.data(), map);

    QPair<CoordGroupArray,CLJParamsArray> param_beads = createParameters(beads, map);
    need_update_ljpairs = true;

    QVector<quint32> bead_ids = ptchs.add(param_beads.first, param_beads.second);

    mol_to_beadid.insert(mol.number(), bead_ids);
    beads_by_molnum.insert(mol.number(), QPair<Beads,PropertyMap>(beads,map));
    
    if (this->recordingChanges())
        ::insert(added_beads, bead_ids);

    return;
}
                
/** Called when the passed molecule view has been removed from the forcefield */
void FastInterCLJFF::_pvt_removed(const PartialMolecule &mol)
{
    return;
}

/** Called when the passed molecule view has been changed */
void FastInterCLJFF::_pvt_changed(const Molecule &mol)
{
    return;
}

/** Called when the passed list of molecules has been changed */
void FastInterCLJFF::_pvt_changed(const QList<Molecule> &mols)
{
    return;
}

/** Called when all molecules have been removed */
void FastInterCLJFF::_pvt_removedAll()
{
    return;
}

/** Called to see if the properties for molecule 'molnum' would change
    if the passed property map was used to get properties */
bool FastInterCLJFF::_pvt_wouldChangeProperties(MolNum molnum, 
                                                const PropertyMap &map) const
{
    return false;
}

/** Return the symbols for the energy components of this forcefield */
const CLJComponent& FastInterCLJFF::components() const
{
    return ffcomponents;
}

/** Set the space used by this forcefield - this returns whether or
    not this changed the space */
bool FastInterCLJFF::setSpace(const Space &space)
{
    if (not ptchs.space().equals(space))
    {
        ptchs.repatch(space);
        props.setProperty("space", space);        
        this->mustNowRecalculateFromScratch();
        return true;
    }
    else
        return false;
}

/** Set the patching scheme (together with enclosed space) to use
    to break the molecules into patches */
bool FastInterCLJFF::setPatching(const Patching &patching)
{
    qDebug() << ptchs.patching().toString() << patching.toString();

    if (not ptchs.patching().equals(patching))
    {
        FastInterCLJFF new_cljff(*this);
        
        new_cljff.ptchs = Patches(patching);
        new_cljff.props.setProperty("space", patching.space());
        new_cljff.props.setProperty("patching", patching);
        new_cljff.rebuildAll();
        new_cljff.mustNowRecalculateFromScratch();
        FastInterCLJFF::operator=(new_cljff);
        
        qDebug() << new_cljff.ptchs.patching().toString()
                 << ptchs.patching().toString();
        
        return true;
    }
    else
        return false;
}

/** Set the switching function used to scale the interactions between
    CutGroups to zero at the cutoff */
bool FastInterCLJFF::setSwitchingFunction(const SwitchingFunction &new_switchfunc)
{
    if ( not switchfunc->equals(new_switchfunc) )
    {
        switchfunc = new_switchfunc;
        props.setProperty( "switchingFunction", new_switchfunc );
        this->mustNowRecalculateFromScratch();
        return true;
    }
    else
        return false;
}

/** Set whether or not to shift the electrostatics between CutGroups so that
    the group-group net electrostatic interaction energy between CutGroups
    is zero at the cutoff */
bool FastInterCLJFF::setShiftElectrostatics(bool switchelectro)
{
    if (switchelectro != use_electrostatic_shifting)
    {
        use_electrostatic_shifting = switchelectro;
        props.setProperty( "shiftElectrostatics", VariantProperty(switchelectro) );
        this->mustNowRecalculateFromScratch();
        
        return true;
    }
    else
        return false;
}

/** Set the combining rules to use to obtain mixed LJ parameters */
bool FastInterCLJFF::setCombiningRules(const QString &combiningrules)
{
    LJParameterDB::CombiningRules new_rules = LJParameterDB::interpret(combiningrules);
    
    if (new_rules != combining_rules)
    {
        combining_rules = new_rules;
        need_update_ljpairs = true;
        props.setProperty( "combiningRules", VariantProperty(combiningrules) );
        this->mustNowRecalculateFromScratch();

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
bool FastInterCLJFF::setProperty(const QString &name, const Property &value)
{
    if (name == QLatin1String("space"))
    {
        return this->setSpace( value.asA<Space>() );
    }
    else if (name == QLatin1String("patching"))
    {
        return this->setPatching( value.asA<Patching>() );
    }
    else if (name == QLatin1String("switchingFunction"))
    {
        return this->setSwitchingFunction( value.asA<SwitchingFunction>() );
    }
    else if (name == QLatin1String("shiftElectrostatics"))
    {
        return this->setShiftElectrostatics( value.asA<VariantProperty>()
                                                     .convertTo<bool>() );
    }
    else if (name == QLatin1String("combiningRules"))
    {
        return this->setCombiningRules( value.asA<VariantProperty>()
                                                     .convertTo<QString>() );
    }
    else
        throw SireBase::missing_property( QObject::tr(
            "The CLJ potentials do not have a property called \"%1\" that "
            "can be changed. Available properties are [ %2 ].")
                .arg(name, QStringList(props.propertyKeys()).join(", ")), CODELOC );
                
    return false;
}

/** Return the 3D space in which this potential is evaluated */
const Space& FastInterCLJFF::space() const
{
    return ptchs.space();
}

/** Return the patching function used to divide space into patches */
const Patching& FastInterCLJFF::patching() const
{
    return ptchs.patching();
}

/** Return the switching function used to scale the group-group
    interactions to zero at the cutoff */
const SwitchingFunction& FastInterCLJFF::switchingFunction() const
{
    return switchfunc.read();
}

/** Return whether or not the net group-group electrostatic interaction
    energy is shifted so that it is zero at the cutoff */
bool FastInterCLJFF::shiftElectrostatics() const
{
    return use_electrostatic_shifting;
}

/** Return the string identifying the combining rules used to 
    obtain the mixed LJ parameters */
const QString& FastInterCLJFF::combiningRules() const
{
    return LJParameterDB::toString(combining_rules);
}

/** Return the property 'name' from this forcefield

    \throw SireBase::missing_property
*/
const Property& FastInterCLJFF::property(const QString &name) const
{
    if (props.hasProperty(name))
    {
        return props.property(name);
    }
    else
    {
        throw SireBase::missing_property( QObject::tr(
                "There is no property called \"%1\" in the forcefield %2. "
                "Available properties are [ %3 ].")
                    .arg(name, this->toString())
                    .arg( Sire::toString(props.propertyKeys()) ), CODELOC );
    }
}

/** Return whether or not this forcefield contains a property called 'name' */
bool FastInterCLJFF::containsProperty(const QString &name) const
{
    return props.hasProperty(name);
}

/** Return all of the properties of this forcefield */
const Properties& FastInterCLJFF::properties() const
{
    return props;
}

/** Set that this forcefield must recalculate the energy from scratch */
void FastInterCLJFF::mustNowRecalculateFromScratch()
{
    recording_changes = false;
    removed_beads.clear();
    changed_beads.clear();
    added_beads.clear();
}

void FastInterCLJFF::force(ForceTable &forcetable, double scale_force)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support force calculations!"), CODELOC );
}

void FastInterCLJFF::force(ForceTable &forcetable, const Symbol &symbol,
                           double scale_force)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support force calculations!"), CODELOC );
}
           
void FastInterCLJFF::field(FieldTable &fieldtable, double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::field(FieldTable &fieldtable, const Symbol &component,
                           double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::potential(PotentialTable &potentialtable, double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}

void FastInterCLJFF::potential(PotentialTable &potentialtable, const Symbol &component,
                               double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}

void FastInterCLJFF::field(FieldTable &fieldtable, const Probe &probe, double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::field(FieldTable &fieldtable, const Symbol &component,
                           const Probe &probe, double scale_field)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support field calculations!"), CODELOC );
}

void FastInterCLJFF::potential(PotentialTable &potentialtable, const Probe &probe,
                               double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}
                               
void FastInterCLJFF::potential(PotentialTable &potentialtable, const Symbol &component,
                               const Probe &probe, double scale_potential)
{
    throw SireError::incomplete_code( QObject::tr(
            "FastInterCLJFF does not yet support potential calculations!"), CODELOC );
}
