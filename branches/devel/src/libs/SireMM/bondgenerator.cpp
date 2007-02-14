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

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"

#include "SireMol/molecule.h"

#include "SireMol/moleculeinfo.h"
#include "SireMol/residueinfo.h"

#include "SireMol/atom.h"

#include "bondgenerator.h"
#include "molbondinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireMM;

static const RegisterMetaType<BondGenerator> r_bondgenerator;
static const RegisterMetaType<UsePassedBonds> r_usepassedbonds;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const BondGenerator &generator)
{
    writeHeader(ds, r_bondgenerator, 0)
          << static_cast<const BondGeneratorBase&>(generator);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, BondGenerator &generator)
{
    VersionID v = readHeader(ds, r_bondgenerator);

    if (v == 0)
    {
        ds >> static_cast<BondGeneratorBase&>(generator);
    }
    else
        throw version_error(v, "0", r_bondgenerator, CODELOC);

    return ds;
}

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const UsePassedBonds &generator)
{
    writeHeader(ds, r_usepassedbonds, 0)
          << static_cast<const UsePassedBondsBase&>(generator);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, UsePassedBonds &generator)
{
    VersionID v = readHeader(ds, r_usepassedbonds);

    if (v == 0)
    {
        ds >> static_cast<UsePassedBonds&>(generator);
    }
    else
        throw version_error(v, "0", r_usepassedbonds, CODELOC);

    return ds;
}

/** Constructor */
BondGenerator::BondGenerator() : BondGeneratorBase()
{}

/** Copy constructor */
BondGenerator::BondGenerator(const BondGenerator &other)
              : BondGeneratorBase(other)
{}

/** Destructor */
BondGenerator::~BondGenerator()
{}

/** Find all of the bonds in the molecule 'mol' that don't involve dummy atoms
    and add them to 'bondinfo'. This uses the connectivity of the molecule
    to find the bonds. */
void BondGenerator::generate(const Molecule &mol, MolBondInfo &bondinfo) const
{
    //check that we are working with the same molecule!
    checkMolecule(mol, bondinfo);

    const MoleculeBonds &molbonds = mol.connectivity();
    const MoleculeInfo &molinfo = mol.info();

    //loop over every residue in the molecule
    foreach( ResNum resnum, molinfo.residueNumbers() )
    {
        //get all of the bonds in the residue
        const ResidueBonds &resbonds = molbonds.residue(resnum);

        //get all of the asymmetric bonds in this residue (prevents duplication)
        QList<Bond> asymm_bonds = resbonds.asymmetricBonds();

        //loop over each of the asymmetric bonds
        for (QList<Bond>::const_iterator it = asymm_bonds.constBegin();
             it != asymm_bonds.constEnd();
             ++it)
        {
            const Bond &bond = *it;

            //only add the bond if it doesn't involve dummy atoms
            if ( molinfo.atom(bond[0]).nProtons() > 0 and
                 molinfo.atom(bond[1]).nProtons() > 0 )
            {
                bondinfo.addBond(bond);
            }
        }
    }
}

//////////////////////
////////////////////// Implementation of UsePassedBonds
//////////////////////

/** Constructor */
UsePassedBonds::UsePassedBonds() : UsePassedBondsBase()
{}

/** Construct to use the passed bonds */
UsePassedBonds::UsePassedBonds(const QSet<Bond> &bonds)
               : UsePassedBondsBase(bonds)
{}

/** Copy constructor */
UsePassedBonds::UsePassedBonds(const UsePassedBonds &other)
               : UsePassedBondsBase(other)
{}

/** Destructor */
UsePassedBonds::~UsePassedBonds()
{}

/** Generate the bonds - instantiated here to reduce over-template instantiation */
void UsePassedBonds::generate(const Molecule &mol, MolBondInfo &bondinfo) const
{
    UsePassedBondsBase::generate(mol, bondinfo);
}
