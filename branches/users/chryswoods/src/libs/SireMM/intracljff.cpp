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

#include "intracljff.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireStream;

/////////////
///////////// Implementation of IntraCLJFF::Parameters
/////////////

/** Constructor */
IntraCLJFF::Parameters::Parameters()
           : CLJFF::Parameters(), nbscl_params("nbscale", "nbscale")
{}

/** Copy constructor */
IntraCLJFF::Parameters::Parameters(const IntraCLJFF::Parameters &other)
           : CLJFF::Parameters(other),
             nbscl_params(other.nbscl_params)
{}

/** Destructor */
IntraCLJFF::Parameters::~Parameters()
{}

/** Static object returned by IntraCLJFF::parameters() */
IntraCLJFF::Parameters IntraCLJFF::Parameters::default_sources;


/////////////
///////////// Implementation of IntraCLJFF
/////////////


/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds, const )
{
    writeHeader(ds, , 1)
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds, )
{
    VersionID v = readHeader(ds, );
    if (v == 1)
    {
        ds >>
    }
    else
        throw version_error(v, "1", , CODELOC);
    return ds;
}

IntraCLJFF::IntraCLJFF();

IntraCLJFF::IntraCLJFF(const Space &space, const SwitchingFunction &switchfunc);

IntraCLJFF::IntraCLJFF(const IntraCLJFF &other);

IntraCLJFF::~IntraCLJFF();

void IntraCLJFF::mustNowRecalculateFromScratch();

bool IntraCLJFF::change(const PartialMolecule &molecule);
bool IntraCLJFF::change(const Molecules &molecules);

bool IntraCLJFF::add(const PartialMolecule &molecule,
           const ParameterMap &map = ParameterMap());

bool IntraCLJFF::add(const Molecules &molecules,
           const ParameterMap &map = ParameterMap());

bool IntraCLJFF::remove(const PartialMolecule &molecule);
bool IntraCLJFF::remove(const Molecules &molecules);

bool IntraCLJFF::contains(const PartialMolecule &molecule) const;

bool IntraCLJFF::refersTo(MoleculeID molid) const;

QSet<FFBase::Group> IntraCLJFF::groupsReferringTo(MoleculeID molid) const;

QSet<MoleculeID> IntraCLJFF::moleculeIDs() const;

PartialMolecule IntraCLJFF::molecule(MoleculeID molid) const;

Molecules IntraCLJFF::contents() const;

void IntraCLJFF::_pvt_copy(const FFBase &other);

void IntraCLJFF::recalculateViaDelta();

/** Recalculate the total energy of this forcefield from scratch */
void IntraCLJFF::recalculateTotalEnergy()
{
    //calculate the CLJ energy of each molecule
    CLJEnergy cljnrg(0,0);

    int nmols = mols.count();

    const CLJMolecule *mols_array = mols.constData();

    DistMatrix distmat(30,30);
    CLJPairMatrix cljmat(30,30);

    //loop over all molecules
    for (int i=0; i<nmols; ++i)
    {
        const CLJMolecule &mol = mols_array[i];
        const CLJNBPairs &nbpair = *(mol_nbpairs.constFind(mol.ID()));


        //add the energy of this molecule

    }
}

void IntraCLJFF::recalculateEnergy();
