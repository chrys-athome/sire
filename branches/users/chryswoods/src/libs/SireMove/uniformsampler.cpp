
#include "uniformsampler.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"

#include "SireMol/moleculeid.h"
#include "SireMol/resid.h"
#include "SireMol/atomid.h"
#include "SireMol/idmolatom.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireMove;
using namespace SireStream;

static const RegisterMetaType<UniformSampler> r_sampler;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const UniformSampler &sampler)
{
    writeHeader(ds, r_sampler, 1)
          << static_cast<const SamplerBase&>(sampler);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, UniformSampler &sampler)
{
    VersionID v = readHeader(ds, r_sampler);

    if (v == 1)
    {
        ds >> static_cast<SamplerBase&>(sampler);
    }
    else
        throw version_error(v, "1", r_sampler, CODELOC);

    return ds;
}

/** Constructor - uses the global random number generator */
UniformSampler::UniformSampler() : SamplerBase()
{}

/** Construct using a specified random number generator */
UniformSampler::UniformSampler(const RanGenerator &generator)
               : SamplerBase(generator)
{}

/** Copy constructor */
UniformSampler::UniformSampler(const UniformSampler &other)
               : SamplerBase(other)
{}

/** Destructor */
UniformSampler::~UniformSampler()
{}

/** Copy assignment */
UniformSampler& UniformSampler::operator=(const UniformSampler &other)
{
    SamplerBase::operator=(other);

    return *this;
}

/** Internal function used to return a random molecule
    from a MoleculeGroup that is known not to be empty */
tuple<Molecule,double>
UniformSampler::_pvt_randomMolecule(const MoleculeGroup &group, uint nmols)
{
    return tuple<Molecule,double>(
                      group.molecules().constData()[generator().randInt(nmols-1)],
                      1.0 / nmols);
}

/** Return a random molecule that is in the group 'group'. This
    throws an exception if there are no molecules in the group

    \throw SireMol::missing_molecule
*/
tuple<Molecule,double>
UniformSampler::randomMolecule(const MoleculeGroup &group)
{
    uint nmols = group.count();

    if (nmols == 0)
        throw SireMol::missing_molecule( QObject::tr(
              "Cannot select a random molecule from an empty MoleculeGroup!"),
                  CODELOC );

    return _pvt_randomMolecule(group, nmols);
}

/** Return a random residue that is in the group 'group'. This
    throws an exception if there are no residues in the group

    \throw SireMol::missing_residue
*/
tuple<Residue,double>
UniformSampler::randomResidue(const MoleculeGroup &group)
{
    uint nmols = group.count();

    if (nmols == 0)
        throw SireMol::missing_residue( QObject::tr(
            "Cannot select a random residue from an empty MoleculeGroup!"),
                CODELOC );

    //get a random molecule
    tuple<Molecule,double> randmol = this->_pvt_randomMolecule(group, nmols);

    //choose a random residue within that molecule
    const Molecule &molecule = randmol.get<0>();

    uint nres = molecule.nResidues();

    //there should never be any empty molecules in
    //the group!
    BOOST_ASSERT(nres != 0);

    return tuple<Residue,double>(
                  molecule.residue( ResID(generator().randInt(nres-1)) ),
                  randmol.get<1>() / nres );
}

/** Return a random atom that is in the group 'group'. This
    throws an exception if there are no atoms in the group.

    \throw SireMol::missing_atom
*/
tuple<NewAtom,double> UniformSampler::randomAtom(const MoleculeGroup &group)
{
    uint nmols = group.count();

    if (nmols == 0)
        throw SireMol::missing_atom( QObject::tr(
            "Cannot select a random atom from an empty MoleculeGroup!"),
                CODELOC );

    //get a random molecule
    tuple<Molecule,double> randmol = this->_pvt_randomMolecule(group, nmols);

    //choose a random atom within that molecule
    const Molecule &molecule = randmol.get<0>();

    uint natms = molecule.nAtoms();

    //there should never be any empty molecules in the group!
    BOOST_ASSERT(natms != 0);

    return tuple<NewAtom,double>(
                NewAtom( AtomID(generator().randInt(natms-1)), molecule ),
                randmol.get<1>() / natms );
}

/** Return the probability of selecting the molecule 'molecule' from
    the group 'group'. A probability of zero is returned if the
    molecule is not in the group!
*/
double UniformSampler::probability(const MoleculeGroup &group,
                                   const Molecule &molecule)
{
    if (not group.contains(molecule))
        return 0;

    return 1.0 / group.count();
}

/** Return the probability of selecting the residue 'residue' from
    the group 'group'. A probability of zero is returned if the
    residue is not in the group.
*/
double UniformSampler::probability(const MoleculeGroup &group,
                                   const Residue &residue)
{
    Molecule molecule = residue.molecule();

    if (not group.contains(molecule))
        return 0;
    else
    {
        //this is safe as the group cannot contain empty molecules
        BOOST_ASSERT( molecule.nResidues() != 0 );

        return 1.0 / (group.count() * molecule.nResidues());
    }
}

/** Return the probability of selecting the atom 'atom' from
    the group 'group'. A probability of zero is returned if
    the atom is not in the group.
*/
double UniformSampler::probability(const MoleculeGroup &group,
                                   const NewAtom &atom)
{
    Molecule molecule = atom.molecule();

    if (not group.contains(molecule))
        return 0;
    else
    {
        //this is safe as the group cannot contain empty molecules
        BOOST_ASSERT( molecule.nAtoms() != 0 );

        return 1.0 / (group.count() * molecule.nAtoms());
    }
}
