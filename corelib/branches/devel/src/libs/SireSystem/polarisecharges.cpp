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

#include "SireMol/connectivity.h"
#include "SireMol/atomcoords.h"
#include "SireMol/atomcharges.h"
#include "SireMol/atompolarisabilities.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculedata.h"

#include "SireFF/probe.h"

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
            
            void update(const MoleculeView &molview,
                        const PropertyName &coords_property,
                        const PropertyName &connectivity_property,
                        const PropertyName &polarise_property);
            
            /** The matrix holding alpha * (X X^T)**-1 */
            QVector<NMatrix> inv_xx_matricies;
            
            /** The connectivity of the molecule */
            Connectivity connectivity;
            
            /** The selection of atoms to be polarised - this
                is empty if all of the atoms are selected */
            AtomSelection selected_atoms;
            
            /** The polarised charges */
            AtomCharges polarised_charges;
            
            /** The version number of the molecule for which this
                data has been calculated */
            quint64 molversion;
        };
    }
}

using namespace SireSystem::detail;

PolariseChargesData::PolariseChargesData(const MoleculeView &molview,
                                         const PropertyName &coords_property,
                                         const PropertyName &connectivity_property,
                                         const PropertyName &polarise_property)
                    : QSharedData()
{
    const AtomCoords &coords = molview.data().property(coords_property)
                                             .asA<AtomCoords>();
                                            
    connectivity = molview.data().property(connectivity_property)
                                 .asA<Connectivity>();
                                 
    const AtomPolarisabilities &polarise = molview.data().property(polarise_property)
                                                         .asA<AtomPolarisabilities>();

    polarised_charges = AtomCharges(molview.data().info());
    inv_xx_matricies = QVector<NMatrix>(coords.nAtoms());
    inv_xx_matricies.squeeze();

    //this is the matrix of r vectors
    QVarLengthArray<Vector,10> X;

    if (molview.selectedAll())
    {
        //loop over all of the atoms
        int nats = coords.nAtoms();
        
        const Vector *coords_array = coords.array().coordsData();
        const Volume *polarise_array = polarise.array().valueData();
        
        for (AtomIdx i(0); i<nats; ++i)
        {
            //get the atoms bonded to this atom
            const QSet<AtomIdx> &bonded_atoms = connectivity.connectionsTo(i);
            
            if (bonded_atoms.isEmpty())
                continue;

            const int nbonded = bonded_atoms.count();

            //construct the X matrix (matrix of vectors from connected 
            //atoms to the this atom
            X.resize(nbonded);
            
            if (polarise_array[i].value() == 0)
                continue;
            
            const Vector &i_coords = coords_array[i];
            
            int j = 0;
            foreach (AtomIdx bonded_atom, bonded_atoms)
            {
                X[j] = coords_array[bonded_atom] - i_coords;
                
                X[j] = Vector( SireUnits::convertTo( X[j][0], bohr_radii ),
                               SireUnits::convertTo( X[j][1], bohr_radii ),
                               SireUnits::convertTo( X[j][2], bohr_radii ) );
                
                X[j] /= polarise_array[i].value();
                
                ++j;
            }
            
            //now construct ( X X^T )                        
            NMatrix &inv_xx = inv_xx_matricies[i];
            inv_xx = NMatrix(nbonded, nbonded);

            for (int j=0; j<nbonded; ++j)
            {
                inv_xx(j,j) = Vector::dot(X[j], X[j]);
            
                for (int k=j+1; k<nbonded; ++k)
                {
                    const double j_dot_k = Vector::dot(X[j], X[k]);
                    inv_xx(j,k) = j_dot_k;
                    inv_xx(k,j) = j_dot_k;
                }
            }

            //now construct alpha * ( X X^T )**-1
            inv_xx = inv_xx.inverse();
        }
    }
    else
    {
        selected_atoms = molview.selection();
    }
}

void PolariseChargesData::update(const MoleculeView &molview,
                                 const PropertyName &coords_property,
                                 const PropertyName &connectivity_property,
                                 const PropertyName &polarise_property)
{
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
        
        ProbePtr probe;
    
        polchgs = PolariseCharges();
        
        sds >> polchgs.field_component >> probe 
            >> static_cast<ChargeConstraint&>(polchgs);
            
        polchgs.setProbe(probe);
    }
    else
        throw version_error( v, "1", r_polarise_charges, CODELOC );

    return ds;
}

void PolariseCharges::setProbe(const Probe &probe)
{}

/** Null constructor */
PolariseCharges::PolariseCharges() 
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(),
                  probe_charge(0)
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
                  field_probe(other.field_probe), probe_charge(other.probe_charge),
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
        probe_charge = other.probe_charge;
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
const Probe& PolariseCharges::probe() const
{
    return field_probe.read();
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
                moldata[it.key()]->update(it.value(), coords_property,
                                          connectivity_property, polarise_property);
            }
        }
    }

    //loop over each molecule
    //{
        //if this molecule has moves
        //{
            //loop over each selected atom
    
            //if the connectivity has changed
                //find the atoms bonded to this atom
    
            //calculate the distance vector matrix to each atom
    
            //invert this matrix
        //}

        //calculate the potential on the atoms
        
        //calculate the potential difference
        
        //calculate the polarised charges
    //}

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
