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

#include "polarisecharges.h"
#include "delta.h"

#include "SireMaths/nmatrix.h"
#include "SireMaths/nvector.h"

#include "SireMol/connectivity.h"
#include "SireMol/atomcoords.h"
#include "SireMol/atomcharges.h"
#include "SireMol/atompolarisabilities.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculedata.h"
#include "SireMol/atomselection.h"

#include "SireFF/probe.h"
#include "SireFF/potentialtable.h"

#include "SireMM/cljprobe.h"

#include "SireSystem/system.h"

#include "SireUnits/units.h"
#include "SireUnits/convert.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;
using namespace SireMM;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

/////////////
///////////// Implementation of PolariseChargesData
/////////////

namespace SireSystem
{
    namespace detail
    {
        class PolariseChargesData : public QSharedData
        {
        public:
            PolariseChargesData() : QSharedData()
            {}
            
            PolariseChargesData(const MoleculeView &molview,
                                const PropertyName &coords_property,
                                const PropertyName &connectivity_property,
                                const PropertyName &polarise_property);
            
            PolariseChargesData(const PolariseChargesData &other)
                  : QSharedData()
            {}
            
            ~PolariseChargesData()
            {}
            
            PolariseChargesData& operator=(const PolariseChargesData &other)
            {
                if (this != &other)
                {
                    
                }
                
                return *this;
            }
            
            /** The matrix holding alpha * (X X^T)**-1 */
            QVector<NMatrix> inv_xx_matricies;
            
            /** The connectivity of the molecule */
            Connectivity connectivity;
            
            /** The selection of atoms to be polarised - this
                is empty if all of the atoms are selected */
            AtomSelection selected_atoms;
            
            /** The version number of the molecule for which this
                data has been calculated */
            quint64 molversion;
        };
    }
}

using namespace SireSystem::detail;

static void calculateAtomMatrix(AtomIdx atomidx, const AtomCoords &coords,
                                const Volume &polarisability,
                                const Connectivity &connectivity,
                                const MoleculeInfoData &molinfo,
                                QVarLengthArray<Vector,10> &X,
                                NMatrix &inv_xx)
{
    //get the atoms bonded to this atom
    const QSet<AtomIdx> &bonded_atoms = connectivity.connectionsTo(atomidx);

    const int nbonded = bonded_atoms.count();
    
    const double alpha = four_pi_eps0 * polarisability.value();
    
    if (nbonded == 0 or alpha < 1e-6)
        return;

    //construct the X matrix (matrix of vectors from connected 
    //atoms to the this atom
    {
        X.resize(nbonded);

        const Vector &atom_coords = coords[ molinfo.cgAtomIdx(atomidx) ];

        int i = 0;
        foreach (AtomIdx bonded_atom, bonded_atoms)
        {
            X[i] = coords[ molinfo.cgAtomIdx(bonded_atom) ] - atom_coords;
        
            ++i;
        }
    }
    
    //now construct (1/alpha) ( X X^T )                        
    inv_xx = NMatrix(nbonded, nbonded);

    const double one_over_alpha = 1 / alpha;

    for (int i=0; i<nbonded; ++i)
    {
        inv_xx(i,i) = one_over_alpha * Vector::dot(X[i], X[i]);
    
        for (int j=i+1; j<nbonded; ++j)
        {
            const double i_dot_j = Vector::dot(X[i], X[j]);
            inv_xx(i,j) = i_dot_j;
            inv_xx(j,i) = i_dot_j;
        }
    }

    qDebug() << inv_xx.toString();

    //now construct alpha * ( X X^T )**-1
    inv_xx = inv_xx.inverse();
}

PolariseChargesData::PolariseChargesData(const MoleculeView &molview,
                                         const PropertyName &coords_property,
                                         const PropertyName &connectivity_property,
                                         const PropertyName &polarise_property)
                    : QSharedData()
{
    const AtomCoords &coords = molview.data().property(coords_property)
                                             .asA<AtomCoords>();

    const MoleculeInfoData &molinfo = molview.data().info();
                                            
    connectivity = molview.data().property(connectivity_property)
                                 .asA<Connectivity>();
                                 
    const AtomPolarisabilities &polarise = molview.data().property(polarise_property)
                                                         .asA<AtomPolarisabilities>();

    inv_xx_matricies = QVector<NMatrix>(coords.nAtoms());
    inv_xx_matricies.squeeze();

    //this is the matrix of r vectors
    QVarLengthArray<Vector,10> X;
        
    if (molview.selectedAll())
    {
        //loop over all of the atoms
        int nats = coords.nAtoms();
        
        for (AtomIdx i(0); i<nats; ++i)
        {
            calculateAtomMatrix(i, coords, polarise[molinfo.cgAtomIdx(i)],
                                connectivity, molinfo, X, inv_xx_matricies[i]);
        }
    }
    else
    {
        selected_atoms = molview.selection();
        
        int ncgroups = selected_atoms.nCutGroups();
        
        const MoleculeInfoData &molinfo = molview.data().info();
        
        for (CGIdx i(0); i<ncgroups; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                int nats = molinfo.nAtoms(i);
                
                for (Index j(0); j<nats; ++j)
                {
                    CGAtomIdx cgatomidx(i,j);
                
                    AtomIdx atomidx = molinfo.atomIdx(cgatomidx);
                    
                    calculateAtomMatrix(atomidx, coords, polarise[cgatomidx],
                                        connectivity, molinfo, X, 
                                        inv_xx_matricies[atomidx]);
                }
            }
            else
            {
                foreach (Index j, selected_atoms.selectedAtoms(i))
                {
                    CGAtomIdx cgatomidx(i,j);

                    AtomIdx atomidx = molinfo.atomIdx(cgatomidx);

                    calculateAtomMatrix(atomidx, coords, polarise[cgatomidx],
                                        connectivity, molinfo, X, 
                                        inv_xx_matricies[atomidx]);
                }
            }
        }
    }
}

/////////////
///////////// Implementation of PolariseCharges
/////////////

static const RegisterMetaType<PolariseCharges> r_polarise_charges;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const PolariseCharges &polchgs)
{
    writeHeader(ds, r_polarise_charges, 1);
    
    SharedDataStream sds(ds);
    
    sds << polchgs.field_component << polchgs.field_probe
        << static_cast<const ChargeConstraint&>(polchgs);

    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, 
                                          PolariseCharges &polchgs)
{
    VersionID v = readHeader(ds, r_polarise_charges);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        polchgs = PolariseCharges();
        
        sds >> polchgs.field_component >> polchgs.field_probe 
            >> static_cast<ChargeConstraint&>(polchgs);
    }
    else
        throw version_error( v, "1", r_polarise_charges, CODELOC );

    return ds;
}

void PolariseCharges::setProbe(const Probe &probe)
{
    if (probe.isA<CoulombProbe>())
    {
        field_probe = probe.asA<CoulombProbe>();
    }
    else if (probe.isA<CLJProbe>())
    {
        field_probe = CoulombProbe( probe.asA<CLJProbe>() );
    }
    else
        throw SireError::incompatible_error( QObject::tr(
                "You can only use a CoulombProbe or CLJProbe with "
                "the PolariseCharges constraint - you cannot use %1.")
                    .arg(probe.toString()), CODELOC );
}

/** Null constructor */
PolariseCharges::PolariseCharges() 
                : ConcreteProperty<PolariseCharges,ChargeConstraint>()
{}

/** Construct a constraint that uses the total energy field and a 
    single unit charge probe to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(ForceFields::totalComponent())
{
    this->setProbe( CoulombProbe( 1*mod_electron ) );
}

/** Construct a constraint that uses the total energy field and 
    the passed probe to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const Probe &probe, const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(ForceFields::totalComponent())
{
    this->setProbe(probe);
}

/** Construct a constraint that uses the field represented by 'field_component'
    and a single unit charge to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const Symbol &fieldcomp, const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(fieldcomp)
{
    this->setProbe( CoulombProbe( 1*mod_electron ) );
}

/** Construct a constraint that uses the field represented by 'field_component'
    and the passed probe to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const Symbol &fieldcomp, const Probe &probe,
                                 const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(fieldcomp)
{
    this->setProbe(probe);
}

/** Copy constructor */
PolariseCharges::PolariseCharges(const PolariseCharges &other)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(other),
                  field_component(other.field_component),
                  field_probe(other.field_probe),
                  moldata(other.moldata)
{}

/** Destructor */
PolariseCharges::~PolariseCharges()
{}

/** Copy assignment operator */
PolariseCharges& PolariseCharges::operator=(const PolariseCharges &other)
{
    if (this != &other)
    {
        field_component = other.field_component;
        field_probe = other.field_probe;
        moldata = other.moldata;
    
        ChargeConstraint::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool PolariseCharges::operator==(const PolariseCharges &other) const
{
    return (this == &other) or
           (field_component == other.field_component and
            field_probe == other.field_probe and ChargeConstraint::operator==(other));
}

/** Comparison operator */
bool PolariseCharges::operator!=(const PolariseCharges &other) const
{
    return not PolariseCharges::operator==(other);
}

const char* PolariseCharges::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PolariseCharges>() );
}

PolariseCharges* PolariseCharges::clone() const
{
    return new PolariseCharges(*this);
}

/** Return a string representation of this constraint */
QString PolariseCharges::toString() const
{
    return "SireSystem::PolariseCharges";
}

/** Return the component of the forcefield that is used to 
    calculate the electrostatic field on the atoms to be
    polarised */
const Symbol& PolariseCharges::fieldComponent() const
{
    return field_component;
}

/** Return the probe that is used to calculate the electrostatic
    field on the atoms to be polarised */
const CoulombProbe& PolariseCharges::probe() const
{
    return field_probe;
}

static AtomCharges calculateCharges(const MoleculeView &molview,
                                    const PolariseChargesData &poldata,
                                    const MolPotentialTable &moltable)
{
    const AtomSelection &selected_atoms = poldata.selected_atoms;
    const NMatrix *inv_xx_mat_array = poldata.inv_xx_matricies.constData();
    const MoleculeInfoData &molinfo = molview.data().info();
    const Connectivity &connectivity = poldata.connectivity;
    
    AtomCharges induced_charges(molinfo);
    
    if (selected_atoms.isEmpty())
    {
        //all of the atoms have been selected
        int nats = moltable.nValues();
        BOOST_ASSERT( nats == poldata.inv_xx_matricies.count() );
        
        for (AtomIdx atomidx(0); atomidx<nats; ++atomidx)
        {
            const NMatrix &alpha_inv_XX = inv_xx_mat_array[atomidx];

            if (alpha_inv_XX.nRows() == 0)
                //this atom has a zero polarisability, or no bonded atoms
                continue;

            CGAtomIdx cgatomidx = molinfo.cgAtomIdx(atomidx);
            
            const QSet<AtomIdx> &bonded_atoms = connectivity.connectionsTo(atomidx);
            
            int nbonded = bonded_atoms.count();
            
            BOOST_ASSERT(nbonded == alpha_inv_XX.nRows());
            
            if (nbonded <= 1 or alpha_inv_XX.nRows() <= 1)
                //not enough bonded atoms to take the charge
                continue;
                
            double phi_a = moltable[cgatomidx.cutGroup()][cgatomidx.atom()].value();
            
            NVector delta_phi(nbonded);
            {
                int i = 0;
                foreach (AtomIdx bonded_atom, bonded_atoms)
                {
                    CGAtomIdx bonded_cgatom = molinfo.cgAtomIdx(bonded_atom);
                
                    delta_phi[i] = phi_a - moltable[bonded_cgatom.cutGroup()]
                                                   [bonded_cgatom.atom()].value();
                    ++i;
                }
            }
            
            NVector delta_q = alpha_inv_XX * delta_phi;
            
            induced_charges[cgatomidx] = Charge( -(delta_q.sum()) );
            //
        }
    }
    else
    {
    }

    return AtomCharges();
}

/** Set the baseline system for the constraint - this is 
    used to pre-calculate everything for the system
    and to check if the constraint is satisfied */
void PolariseCharges::setSystem(const System &system)
{
    if (Constraint::wasLastSystem(system) and Constraint::wasLastSubVersion(system))
        return;
    
    Constraint::clearLastSystem();

    //update the molecule group - clear the current list of molecules
    //if molecules have been added or removed from the group]
    {
        Version old_version = this->moleculeGroup().version();

        this->updateGroup(system);
    
        if (old_version.majorVersion() != this->moleculeGroup().version().majorVersion())
            moldata.clear();
    }

    const Molecules &molecules = this->moleculeGroup().molecules();

    const PropertyMap &map = this->propertyMap();
    const PropertyName coords_property = map["coordinates"];
    const PropertyName connectivity_property = map["connectivity"];
    const PropertyName polarise_property = map["polarisability"];

    //now calculate the potential on each molecule
    PotentialTable potentials(this->moleculeGroup());
    System new_system(system);
    new_system.potential(potentials, field_probe);

    //now calculate the induced charges
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        QHash< MolNum,QSharedDataPointer<PolariseChargesData> >::const_iterator
                                        it2 = moldata.constFind(it.key());
                                        
        if (it2 == moldata.constEnd())
        {
            moldata.insert( it.key(), QSharedDataPointer<PolariseChargesData>(
                                            new PolariseChargesData(
                                                    it.value(),
                                                    coords_property, 
                                                    connectivity_property,
                                                    polarise_property) ) );
        }
        else
        {
            if (it2.value()->molversion != it.value().version())
            {
                //this molecule has changed and needs updating
                *(moldata[it.key()]) = PolariseChargesData(it.value(), coords_property,
                                                           connectivity_property,
                                                           polarise_property);
            }
        }
        
        it2 = moldata.constFind(it.key());
        
        const MolPotentialTable &moltable = potentials.getTable(it.key());
        
        AtomCharges new_charges = calculateCharges(it.value(), *(it2.value().constData()),
                                                   moltable);
    }

    Constraint::setSatisfied(system, false);
}

/** Return whether or not the changes in the passed
    delta *may* have changed the system since the last
    subversion 'subversion' */
bool PolariseCharges::mayChange(const Delta &delta, quint32 last_subversion) const
{
    return true;
}

/** Fully apply this constraint on the passed delta - this returns
    whether or not this constraint affects the delta */
bool PolariseCharges::fullApply(Delta &delta)
{
    this->setSystem(delta.deltaSystem());
    return false;
}

/** Apply this constraint based on the delta, knowing that the 
    last application of this constraint was on this system, 
    at subversion number last_subversion */
bool PolariseCharges::deltaApply(Delta &delta, quint32 last_subversion)
{
    this->setSystem(delta.deltaSystem());
    return false;
}