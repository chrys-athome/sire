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

#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include "SireUnits/units.h"

#include "tip4pff.h"

#include "chargetable.h"
#include "ljtable.h"

#include "detail/molcljinfo.h"

#include "SireVol/coordgroup.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireVol;
using namespace SireDB;

using namespace SireStream;

///////////
/////////// Implementation of Tip4PFF::Components
///////////

/** Constructor */
Tip4PFF::Components::Components() : CLJFF::Components()
{}

/** Constructor that just passes its arguments up to the parent */
Tip4PFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
        : CLJFF::Components(ffbase,symbols)
{}

/** Copy constructor */
Tip4PFF::Components::Components(const Components &other)
           : CLJFF::Components(other)
{}

/** Destructor */
Tip4PFF::Components::~Components()
{}

///////////
/////////// Implementation of Tip4PFF::Parameters
///////////

/** Constructor */
Tip4PFF::Parameters::Parameters() : CLJFF::Parameters()
{}

/** Copy constructor */
Tip4PFF::Parameters::Parameters(const Parameters &other)
           : CLJFF::Parameters(other)
{}

/** Destructor */
Tip4PFF::Parameters::~Parameters()
{}

///////////
/////////// Implementation of Tip4PFF::Groups
///////////

/** Constructor */
Tip4PFF::Groups::Groups() : CLJFF::Groups()
{}

/** Copy constructor */
Tip4PFF::Groups::Groups(const Groups &other)
           : CLJFF::Groups(other)
{}

/** Destructor */
Tip4PFF::Groups::~Groups()
{}

///////////
/////////// Implementation of Tip4PFF
///////////

static const RegisterMetaType<Tip4PFF> r_tip4pff;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const Tip4PFF &tip4pff)
{
    writeHeader(ds, r_tip4pff, 1);

    throw SireError::incomplete_code("Not done!", CODELOC);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, Tip4PFF &tip4pff)
{
    VersionID v = readHeader(ds, r_tip4pff);

    throw SireError::incomplete_code("Not done!", CODELOC);

    if (v == 1)
    {
        //ds >>
    }
    else
        throw version_error(v, "1", r_tip4pff, CODELOC);

    return ds;
}

/** Constructor */
Tip4PFF::Tip4PFF() : CLJFF()
{}

/** Construct a CLJ forcefield using the passed Space and
    switching function (combining rules don't affect TIP4P) */
Tip4PFF::Tip4PFF(const Space &space, const SwitchingFunction &switchfunc)
        : CLJFF(space, switchfunc)
{}

/** Copy constructor */
Tip4PFF::Tip4PFF(const Tip4PFF &other)
        : CLJFF(other), mols(other.mols)
{}

/** Destructor */
Tip4PFF::~Tip4PFF()
{}

/** Calculate the CLJ energy of interaction of group0, with CLJ parameters
    in 'clj0' and group1, with CLJ parameters in 'clj1', using the
    space 'space', and using the provided workspace. The total
    coulomb and LJ energies are returned in the workspace. */
double Tip4PFF::calculateEnergy(const Vector *array0, int nats0,
                                const Vector *array1, int nats1,
                                const CLJParameter *cljarray,
                                const Space &space)
{
    double maxinvdist2(0);

    double inrg = 0;

    //loop over all pairs of atoms
    for (int i=0; i<nats0; ++i)
    {
        const Vector &v0 = array0[i];

        const CLJParameter &cljparam0 = cljarray[i];

        for (int j=0; j<nats1; ++j)
        {
            const Vector &v1 = array1[j];

            const CLJParameter &cljparam1 = cljarray[j];

            double invdist2 = Vector::invDistance2(v0,v1);

            maxinvdist2 = qMax(maxinvdist2, invdist2);

            double sig2_over_dist2 = SireMaths::pow_2(cljparam0.sqrtSigma()*cljparam1.sqrtSigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //coulomb energy
            inrg += SireUnits::one_over_four_pi_eps0 *
                                    cljparam0.charge() *cljparam1.charge() * std::sqrt(invdist2);

            //LJ energy
            inrg += 4 * cljparam0.sqrtEpsilon() * cljparam1.sqrtEpsilon() *
                                   ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    double mindist = sqrt( 1.0 / maxinvdist2 );

    if (mindist < 14.5)
        return inrg;
    else if (mindist < 15.0)
        return inrg * (225.0 - mindist*mindist) * 0.0677966101;
    else
        return 0;
}

/** Recalculate the total energy of this forcefield from scratch */
void Tip4PFF::recalculateEnergy()
{
    int nmols = mols.count();

    const QVector<Vector> *molarray = mols.constData();
    const AABox *boxarray = aaboxes.constData();

    const CLJParameter *cljarray = cljparams.constData();

    double nrg = 0;

    const Space &myspace = space();

    int nats = 4;

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const Vector *array0 = molarray[i].constData();

        const AABox &box0 = boxarray[i];

        for (int j=i+1; j<nmols; ++j)
        {
            const AABox &box1 = boxarray[j];

            if (not ( Vector::distance2(box0.center(),box1.center()) >
                      SireMaths::pow_2(15.0 + box0.radius() + box1.radius())) )
            {
                const Vector *array1 = molarray[j].constData();

                nrg += Tip4PFF::calculateEnergy( array0, nats,
                                                 array1, nats,
                                                 cljarray,
                                                 myspace);
            }
        }
    }

    //this->setComponent( components().coulomb(), cnrg );
    //this->setComponent( components().lj(), ljnrg );
    this->setComponent( components().total(), nrg );
}

/** Temporary function used to add a molecule getting the parameters from
    the properties of the molecule using the passed mapping */
bool Tip4PFF::add(const Molecule &mol, const ParameterMap &map)
{
    if (cljparams.isEmpty())
    {
        AtomicCharges charges = mol.getProperty( map.source(parameters().coulomb()) );
        AtomicLJs ljs = mol.getProperty( map.source(parameters().lj()) );

        int ncg = charges.count();

        BOOST_ASSERT(ncg == 1);

        int nparams = charges[0].count();

        BOOST_ASSERT(nparams == 4);

        const ChargeParameter *chargearray = charges[0].constData();
        const LJParameter *ljarray = ljs[0].constData();

        cljparams.reserve(nparams);

        for (int j=0; j<nparams; ++j)
        {
            cljparams.append( CLJParameter(chargearray[j],ljarray[j]) );
        }
    }

    //add this molecule to the list
    CoordGroup cgroup = mol.coordGroups()[0];

    int nats = cgroup.count();

    QVector<Vector> coords;
    coords.reserve(nats);

    const Vector *array = cgroup.constData();

    for (int i=0; i<nats; ++i)
        coords.append( array[i] );

    mols.append( coords );
    aaboxes.append( cgroup.aaBox() );

    this->incrementMajorVersion();

    return isDirty();
}

/** Move the molecule 'molecule' */
bool Tip4PFF::change(const Molecule &molecule)
{
    return false;
}

bool Tip4PFF::change(const Residue &residue)
{
    return false;
}
