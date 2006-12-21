/**
  * @file
  *
  * C++ Implementation: MolproFF
  *
  * Description: Implementation of MolproFF
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "molproff.h"
#include "molproprocessor.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/errors.h"

#include "SireMM/chargetable.h"

#include "SireMaths/maths.h"

#include "SireStream/datastream.h"

#include "SireUnits/convert.h"
#include "SireUnits/units.h"

using namespace Squire;
using namespace SireMM;
using namespace SireFF;
using namespace SireMol;
using namespace SireMaths;
using namespace SireUnits;
using namespace SireStream;

namespace SireFF
{

QHash<MoleculeID,int> index(const QVector<Molecule> &mols)
{
    int nmols = mols.count();

    if (nmols > 0)
    {
        QHash<MoleculeID,int> idx;
        idx.reserve(nmols);

        const Molecule *molarray = mols.constData();

        for (int i=0; i<nmols; ++i)
        {
            idx.insert( molarray[i].ID(), i );
        }

        return idx;
    }
    else
        return QHash<MoleculeID,int>();
}

}

///////////
/////////// Implementation of MolproFF::Parameters
///////////

/** Constructor - by default the coulomb properties come from the 'charges'
    property */
MolproFF::Parameters::Parameters()
         : FFBase::Parameters(), coulomb_params("coulomb", "charges")
{}

/** Copy constructor */
MolproFF::Parameters::Parameters(const MolproFF::Parameters &other)
         : FFBase::Parameters(other), coulomb_params(other.coulomb_params)
{}

/** Destructor */
MolproFF::Parameters::~Parameters()
{}

/** Static object returned by MolproFF::parameters() */
MolproFF::Parameters MolproFF::Parameters::default_sources;

///////////
/////////// Implementation of MolproFF::Components
///////////

/** Constructor */
MolproFF::Components::Components() : FFBase::Components()
{}

/** Construct using the supplied basename */
MolproFF::Components::Components(const QString &basename) : FFBase::Components()
{
    MolproFF::Components::setBaseName(basename);
}

/** Copy constructor */
MolproFF::Components::Components(const MolproFF::Components &other)
      : FFBase::Components(other), e_qm(other.e_qm)
{}

/** Destructor */
MolproFF::Components::~Components()
{}

/** Set the names of the functions from the passed base name */
void MolproFF::Components::setBaseName(const QString &basename)
{
    this->unregisterFunction(e_qm);

    e_qm = getFunction(basename, "qm");

    this->registerFunction(e_qm);

    FFBase::Components::setBaseName(basename);
}

/** Describe the coulomb component */
QString MolproFF::Components::describe_qm()
{
    return QObject::tr("The total QM energy (includes the QM part of the QM/MM energy).");
}

///////////
/////////// Implementation of MolproFF
///////////

static const RegisterMetaType<MolproFF> r_molproff("Squire::MolproFF");

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const MolproFF &molproff)
{
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, MolproFF &molproff)
{
    return ds;
}

/** Construct an empty MolproFF */
MolproFF::MolproFF() : FFBase()
{
    this->registerComponents();
}

/** Construct an empty, but named MolproFF */
MolproFF::MolproFF(const QString &name) : FFBase(name)
{
    this->registerComponents();
}

/** Copy constructor */
MolproFF::MolproFF(const MolproFF &other)
         : FFBase(other),
           qm_molecules(other.qm_molecules), qm_coords(other.qm_coords),
           qm_molid_to_index(other.qm_molid_to_index),
           mm_molecules(other.mm_molecules), mm_charges(other.mm_charges),
           mm_coords_and_charges(other.mm_coords_and_charges),
           mm_molid_to_index(other.mm_molid_to_index)
{
    //get the pointer from the base class...
    components_ptr = dynamic_cast<const MolproFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Destructor */
MolproFF::~MolproFF()
{}

/** Assignment operator */
MolproFF& MolproFF::operator=(const MolproFF &other)
{
   if (this != &other)
   {
      qm_molecules = other.qm_molecules;
      qm_coords = other.qm_coords;
      qm_molid_to_index = other.qm_molid_to_index;

      mm_molecules = other.mm_molecules;
      mm_charges = other.mm_charges;
      mm_coords_and_charges = other.mm_coords_and_charges;
      mm_molid_to_index = other.mm_molid_to_index;
   }

   return *this;
}

/** Register the components of this forcefield */
void MolproFF::registerComponents()
{
    std::auto_ptr<MolproFF::Components> ptr( new MolproFF::Components(name()) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}

/** Return the array containing the coordinates of the QM atoms.
    This will update the coordinates array to account for any
    changes that may have occured since the QM atoms were last
    moved
*/
const QVector<double>& MolproFF::qmCoordinates()
{
    //
}

/** Return the array containing the coordinates and charges of
    the MM atoms - this will update the coordinates/charges
    array to account for any changes that may have occured since the
    QM atoms were last moved
*/
const QVector<double>& MolproFF::mmCoordsAndCharges()
{
    //
}

/** Return the vector containing the coordinates of the QM atoms,
    arranged into an array of doubles, as
    atom0(x,y,z), atom1(x,y,z) ... atomn(x,y,z), and with the
    coordinates in bohr radii */
inline const QVector<double>& MolproFF::qmCoordinates() const
{
    return qm_coords;
}

/** Return the vector containing the coordinates and charges of the
    MM atoms, arranged into an array of doubles, as
    atom0(x,y,z,q), atom1(x,y,z,q) .... atomn(x,y,z,q)
    with coordinates in bohr radii and charges in atomic charge units */
inline const QVector<double>& MolproFF::mmCoordsAndCharges() const
{
    return mm_coords_and_charges;
}


/** Reconstruct the QM coordinate array */
void MolproFF::reconstructQMArray()
{
    int nmols = qm_molecules.count();
    const Molecule *molecule_array = qm_molecules.constData();

    int nats = 0;

    //get the number of atoms
    for (int i=0; i<nmols; ++i)
    {
        nats += molecule_array[i].nAtoms();
    }

    if (nats == 0)
    {
        //clear the coordinates array
        qm_coords.clear();
        return;
    }

    //reserve sufficient space (3 times number of atoms)
    QVector<double> coords( nats*3 );
    double *coords_array = coords.data();

    int atomid = 0;

    //copy the atoms' coordinates, and convert to bohr radii
    for (int i=0; i<nmols; ++i)
    {
        QVector<Vector> molcoords = molecule_array[i].coordinates();

        int ncoords = molcoords.count();
        const Vector *molcoords_array = molcoords.constData();

        for (int j=0; j<ncoords; ++j)
        {
            const Vector &atmcoords = molcoords_array[j];

            coords_array[atomid] = convertTo(atmcoords.x(), bohr_radii);
            coords_array[atomid+1] = convertTo(atmcoords.y(), bohr_radii);
            coords_array[atomid+2] = convertTo(atmcoords.z(), bohr_radii);

            atomid += 3;
        }
    }

    qm_coords = coords;
}

/** Reconstruct the MM coordinate and charge array */
void MolproFF::reconstructMMArray()
{
    int nmols = mm_molecules.count();
    const Molecule *molecule_array = mm_molecules.constData();
    const QVector<ChargeParameter> *molcharge_array = mm_charges.constData();

    //count the number of atoms that have a non-zero charge
    //(same as the number of non-zero charge parameters...)
    int nats = 0;

    for (int i=0; i<nmols; ++i)
    {
        const QVector<ChargeParameter> &molcharges = molcharge_array[i];

        int ncharges = molcharges.count();
        const ChargeParameter *charge_array = molcharges.constData();

        for (int j=0; j<nmols; ++j)
        {
            if ( not isZero(charge_array[j].charge()) )
               ++nats;
        }
    }

    if (nats == 0)
    {
        //there are no MM charges
        mm_coords_and_charges.clear();
        return;
    }

    //reserve sufficient space for the coordinates and charges (4*nats)
    QVector<double> coords( nats*4 );
    double *coords_array = coords.data();

    int atomid = 0;

    for (int i=0; i<nmols; ++i)
    {
        QVector<Vector> molcoords = molecule_array[i].coordinates();

        int ncoords = molcoords.count();
        const Vector *molcoords_array = molcoords.constData();

        const ChargeParameter *charge_array = molcharge_array[i].constData();

        for (int j=0; j<ncoords; ++j)
        {
            if ( not isZero(charge_array[j].charge()) )
            {
                const Vector &atmcoords = molcoords_array[j];

                //copy (and convert) the coordinates
                coords_array[atomid] = convertTo(atmcoords.x(), bohr_radii);
                coords_array[atomid+1] = convertTo(atmcoords.y(), bohr_radii);
                coords_array[atomid+2] = convertTo(atmcoords.z(), bohr_radii);

                //copy (and convert) the charge
                coords_array[atomid+3] = convertTo(charge_array[j].charge(),
                                                   mod_electrons);

                atomid += 4;
            }
        }
    }

    mm_coords_and_charges = coords;
}

/** Reconstruct all of the info arrays... */
void MolproFF::reconstructIndexAndArrays()
{
    //reconstruct the ID hashes...
    qm_molid_to_index = SireFF::index(qm_molecules);
    mm_molid_to_index = SireFF::index(mm_molecules);

    this->reconstructQMArray();
    this->reconstructMMArray();
}

/** Add a molecule to the QM region

    \throw SireMol::duplicate_molecule
*/
void MolproFF::_pvt_addToQM(const Molecule &molecule)
{
    MoleculeID molid = molecule.ID();

    if ( mm_molid_to_index.contains(molid) )
    {
        const Molecule &existing_mol = mm_molecules.at(mm_molid_to_index.value(molid));

        throw SireMol::duplicate_molecule( QObject::tr(
            "You cannot add the molecule \"%1\" (%2) to the QM region "
            "of the Molpro forcefield \"%3\", as it already exists in "
            "the MM region (\"%4\")")
                .arg(molecule.name()).arg(molid)
                .arg(this->name(), existing_mol.name()), CODELOC );
    }
    else if ( qm_molid_to_index.contains(molid) )
    {
        //replace the existing molecule
        int idx = qm_molid_to_index.value(molid);

        qm_molecules[idx] = molecule;
    }
    else
    {
        qm_molecules.append(molecule);
        qm_molid_to_index.insert(molid, qm_molecules.count() - 1);
    }
}

/** Add a molecule to the MM region.

    \throw SireMol::duplicate_molecule
    \throw SireError::incompatible_error
*/
void MolproFF::_pvt_addToMM(const Molecule &molecule, const ChargeTable &charges)
{
    charges.assertCompatibleWith(molecule);

    MoleculeID molid = molecule.ID();

    if (qm_molid_to_index.contains(molid))
    {
        const Molecule &existing_mol = qm_molecules.at( qm_molid_to_index.value(molid) );

        throw SireMol::duplicate_molecule( QObject::tr(
            "You cannot add the molecule \"%1\" (%2) to the MM region "
            "of the Molpro forcefield \"%3\", as it already exists in "
            "the QM region (\"%4\")")
                .arg(molecule.name()).arg(molid)
                .arg(this->name(), existing_mol.name()), CODELOC );
    }
    else if (mm_molid_to_index.contains(molid))
    {
        //replace the existing molecule
        int idx = mm_molid_to_index.value(molid);

        mm_molecules[idx] = molecule;
        mm_charges[idx] = charges.parameters();
    }
    else
    {
        //add the MM atom
        mm_molecules.append( molecule );
        mm_charges.append( charges.parameters() );
        mm_molid_to_index.insert( molid, mm_molecules.count() - 1 );
    }
}

/** Add a molecule to the QM region

    \throw SireMol::duplicate_molecule
*/
void MolproFF::addToQM(const Molecule &molecule)
{
    this->_pvt_addToQM(molecule);

    this->reconstructQMArray();
    this->incrementVersion();
}

/** Add a load of molecules to the QM region

    \throw SireMol::duplicate_molecule
*/
void MolproFF::addToQM(const QList<Molecule> &molecules)
{
    // Add the molecules to a copy of this forcefield - this
    // is to maintain the invariant

    MolproFF copy(*this);

    for (QList<Molecule>::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        copy._pvt_addToQM(*it);
    }

    copy.reconstructQMArray();
    copy.incrementVersion();

    //everythings ok - copy back to the original
    *this = copy;
}

/** Add a molecule to the MM region.

    \throw SireMol::duplicate_molecule
*/
void MolproFF::addToMM(const Molecule &molecule, const ChargeTable &charges)
{
    this->_pvt_addToMM(molecule, charges);

    this->reconstructMMArray();
    this->incrementVersion();
}

/** Add a load of molecules to the MM region

    \throw SireMol::duplicate_molecule
*/
void MolproFF::addToMM(const QList<Molecule> &molecules,
                       const QList<ChargeTable> &charges)
{
    // Add the molecules to a copy of this forcefield - this
    // is to maintain the invariant

    MolproFF copy(*this);

    int nmols = molecules.count();

    if (nmols != charges.count())
        throw SireError::incompatible_error( QObject::tr(
            "There are not the same number of molecules (%1) as there "
            "are charge tables! (%2)").arg(nmols).arg(charges.count()), CODELOC );

    for (int i=0; i<nmols; ++i)
    {
        copy._pvt_addToMM( molecules[i], charges[i] );
    }

    copy.reconstructMMArray();
    copy.incrementVersion();

    //everythings ok - copy back to the original
    *this = copy;
}

/** Return the molecule in this forcefield with ID == molid

    \throw SireMol::missing_molecule
*/
const Molecule& MolproFF::molecule(MoleculeID molid) const
{
    QHash<MoleculeID,int>::const_iterator it = qm_molid_to_index.find(molid);

    if (it != qm_molid_to_index.end())
        return qm_molecules[ *it ];

    it = mm_molid_to_index.find(molid);

    if (it == mm_molid_to_index.end())
        throw SireMol::missing_molecule( QObject::tr(
                "There is no molecule with ID == %1 in the forcefield \"%2\"")
                    .arg(molid).arg(this->name()), CODELOC );

    return mm_molecules[ *it ];
}

/** Does nothing... */
bool MolproFF::move(const Molecule&)
{
    return false;
}

/** Does nothing... */
bool MolproFF::move(const Residue&)
{
    return false;
}

/** Use the passed MolproSession to recalculate the energy of
    this forcefield - this throws an exception if the session is
    incompatible with this forcefield

    \throw SireError::incompatible_error
*/
Values MolproFF::recalculateEnergy(MolproSession &session)
{
    if (session.incompatibleWith(*this))
        throw SireError::incompatible_error( QObject::tr(
                  "The supplied Molpro session is incompatible with the passed "
                  "Molpro forcefield (%1, %2.%3).")
                      .arg(this->name()).arg(this->ID()).arg(this->version()),
                          CODELOC );

    double total_nrg = ::calculateHFEnergy(session.connection(),
                                           qmCoordinates().constData(),
                                           mmCoordinatesAndCharges().constData(), 0);

    this->setComponent(total(), total_nrg);
    this->setClean();

    return currentEnergies();
}

/** The MolproFF forcefield is unable to recalculate its own
    energy - it has to be placed on a MolproProcessor to be
    able to do this. */
void MolproFF::recalculateEnergy()
{
    throw SireError::program_bug( QObject::tr(
            "It is not possible to calculate the energy of a MolproFF "
            "directly. You must place the MolproFF forcefield onto a "
            "MolproProcessor and ask the MolproProcessor to calculate "
            "the energy."), CODELOC );
}
