
#include "SireCAS/qhash_sirecas.h"

#include "ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireFF/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireBase;
using namespace SireCAS;
using namespace SireMol;
using namespace SireStream;

///////////
/////////// Implementation of FFBase::Parameters
///////////

/** Constructor */
FFBase::Parameters::Parameters()
{}

/** Copy constructor */
FFBase::Parameters::Parameters( const FFBase::Parameters& )
{}

/** Destructor */
FFBase::Parameters::~Parameters()
{}

///////////
/////////// Implementation of FFBase::Components
///////////

/** Constructor */
FFBase::Components::Components()
{}

/** Construct based on the passed FFBase */
FFBase::Components::Components(const FFBase &ffbase)
       : e_total(ffbase)
{
    this->registerComponent(ffbase.ID());
}

/** Copy constructor */
FFBase::Components::Components(const FFBase::Components &other)
       : e_total(other.e_total), symbolids(other.symbolids)
{}

/** Destructor */
FFBase::Components::~Components()
{}

/** Assignment operator */
FFBase::Components& FFBase::Components::operator=(const FFBase::Components &other)
{
    e_total = other.e_total;
    symbolids = other.symbolids;
    
    return *this;
}

/** Describe the 'total' component */
QString FFBase::Components::describe_total()
{
    return QObject::tr("The total energy of the forcefield.");
}

/** Set the names of the functions from the passed base name */
void FFBase::Components::setForceField(const FFBase &ffbase)
{
    *this = Components(ffbase);
}

/** Register the component 'component' */
void FFBase::Components::registerComponent(const FFComponent &component)
{
    symbolids.insert( component.ID() );
}

/** Assert that this contains the function 'function' */
void FFBase::Components::assertContains(const Function &function) const
{
    if (not this->contains(function))
        throw SireFF::missing_component( QObject::tr(
                "The forcefield does not contain a component represented by the "
                "function \"%1\"").arg(function.toString()), CODELOC );
}

///////////
/////////// Implementation of FFBase
///////////

static const RegisterMetaType<FFBase> r_ffbase("SireFF::FFBase", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const FFBase &ffbase)
{
    writeHeader(ds, r_ffbase, 1)
          << ffbase.ffname
          << ffbase.id_and_version
          << ffbase.nrg_components
          << ffbase.isdirty;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, FFBase &ffbase)
{
    VersionID v = readHeader(ds, r_ffbase);

    if (v == 1)
    {
        QString newname;

        ds >> newname
           >> ffbase.id_and_version
           >> ffbase.nrg_components
           >> ffbase.isdirty;

        //tell the forcefield about the change in name
        ffbase.setName(newname);
    }
    else
        throw version_error(v, "1", r_ffbase, CODELOC);

    return ds;
}

/** This is the Incremint that is used to increment
    forcefield ID numbers */
static Incremint forcefield_incremint;

/** Null constructor */
FFBase::FFBase() : QSharedData(),
                   ffname(QObject::tr("Unnamed")),
                   id_and_version(&forcefield_incremint),
                   isdirty(true)
{}

/** Construct a forcefield called 'name' */
FFBase::FFBase(const QString &name)
       : QSharedData(),
         ffname(name),
         id_and_version(&forcefield_incremint),
         isdirty(true)
{}

/** Copy constructor */
FFBase::FFBase(const FFBase &other)
       : QSharedData(),
         ffname(other.ffname),
         id_and_version(other.id_and_version),
         nrg_components(other.nrg_components),
         components_ptr( other.components_ptr->clone() ),
         isdirty(other.isdirty)
{}

/** Destructor */
FFBase::~FFBase()
{}

/** Register the passed components */
void FFBase::registerComponents(FFBase::Components *components)
{
    BOOST_ASSERT( components != 0 );
    components_ptr.reset( components );
}

/** Label the forcefield as dirty (in need of an energy recalculation) */
void FFBase::setDirty()
{
    isdirty = true;
}

/** Label the forcefield as clean (no need to recalculate the energy) */
void FFBase::setClean()
{
    isdirty = false;
}

/** Return the name of this forcefield - this is used as the root of all of
    the components of the forcefield, e.g. if the name was "Ligand CLJ", then
    the "coul" component would have the function: E_{Ligand CLJ}_{coul}(x,y,z) */
const QString& FFBase::name() const
{
    return ffname;
}

/** Set the name of this forcefield - this will change the names of all
    of the forcefield components, so beware if you are already using functions
    that are derived from components of this forcefield */
void FFBase::setName(const QString &name)
{
    ffname = name;

    BOOST_ASSERT( components_ptr.get() != 0 );
    components_ptr->setForceField(*this);
}

/** Return the energy of the component represented by the function
    'component'

    \throw SireFF::missing_component
*/
double FFBase::energy(const Function &component)
{
    components().assertContains(component);

    if (isDirty())
    {
        this->recalculateEnergy();
        this->setClean();
    }

    return nrg_components.value(component);
}

/** Return the total energy of this forcefield. This will return the cached
    value if nothing has changed, or will recalculate if something has changed. */
double FFBase::energy()
{
    return energy( components().total() );
}

/** Return the values of all of the energy components of this forcefield */
Values FFBase::energies()
{
    if (isDirty())
    {
        this->recalculateEnergy();
        this->setClean();
    }

    return nrg_components;
}

/** Change the molecule 'mol' (e.g. move it, or change its
    parameters). This does nothing if the molecule is not
    in this forcefield. Returns whether or not the forcefield
    has been changed by this change, and thus whether the
    energy needs to be recalculated. The same parameter map
    that was used when this molecule was added will be used
    to extract any necessary parameters from the molecule's
    properties 
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFBase::change(const Molecule&)
{
    throw SireError::invalid_operation( QObject::tr(
                    "The forcefield \"%1\" (class %2) does not support "
                    "the changing of molecules.")
                        .arg(this->name()).arg(this->what()), CODELOC );
}

/** Change the residue 'res' (e.g. move it, or change its
    parameters). This does nothing if the residue is not
    in this forcefield. Returns whether or not the forcefield
    has been changed by this change, and thus whether the
    energy needs to be recalculated. 
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFBase::change(const Residue &res)
{
    this->change(res.molecule());
}

/** Add the molecule 'molecule' to this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the molecule. This will replace any 
    existing copy of the molecule that already exists in
    this forcefield. This returns whether or not the 
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating. 
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
    \throw SireError::invalid_operation
*/
bool FFBase::add(const Molecule&, const ParameterMap&)
{
    throw SireError::invalid_operation( QObject::tr(
                    "The forcefield \"%1\" (class %2) does not support "
                    "the addition of molecules.")
                        .arg(this->name()).arg(this->what()), CODELOC );
}

/** Add the residue 'residue' to this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the residue. This will replace any 
    existing copy of the residue that already exists in
    this forcefield. This returns whether or not the 
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.
     
    This will throw an exception if this forcefield does not
    support partial molecules.
    
    \throw SireError::invalid_operation
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool FFBase::add(const Residue&, const ParameterMap&)
{
    throw SireError::invalid_operation( QObject::tr(
                    "The forcefield \"%1\" (class %2) does not support "
                    "the addition of residues.")
                        .arg(this->name()).arg(this->what()), CODELOC );
}

/** Remove the molecule 'molecule' from this forcefield - this
    does nothing if the molecule is not in this forcefield. This
    returns whether this has changed the forcefield (therefore 
    necessitating a recalculation of the energy) 
    
    \throw SireError::invalid_operation
*/
bool FFBase::remove(const Molecule&)
{
    throw SireError::invalid_operation( QObject::tr(
                    "The forcefield \"%1\" (class %2) does not support "
                    "the removal of molecules.")
                        .arg(this->name()).arg(this->what()), CODELOC );
}

/** Remove the residue 'residue' from this forcefield - this
    does nothing if the residue is not in this forcefield. This
    returns whether this has changed the forcefield (therefore 
    necessitating a recalculation of the energy)
    
    This will throw an exception if this forcefield does not
    support partial molecules.
    
    \throw SireError::invalid_operation
*/
bool FFBase::remove(const Residue&)
{
    throw SireError::invalid_operation( QObject::tr(
                    "The forcefield \"%1\" (class %2) does not support "
                    "the removal of residues.")
                        .arg(this->name()).arg(this->what()), CODELOC );
}

/** Replace the molecule 'oldmol' with 'newmol' (using 
    the passed parameter map to find any required parameters
    in the properties of the molecule). This is equivalent
    to 'remove(oldmol)' followed by 'add(newmol,map)', except
    that 'newmol' will only be added if 'oldmol' is contained
    in this forcefield. 
    
    This returns whether this changes the forcefield.
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool FFBase::replace(const Molecule &oldmol,
                     const Molecule &newmol,
                     const ParameterMap &map)
{
    if (this->remove(oldmol))
        this->add(newmol,map);
}

/** Return whether this forcefield contains a copy of the molecule
    'molecule' */
bool FFBase::contains(const Molecule&) const
{
    return false;
}

/** Return whether this forcefield contains a copy of the
    residue 'residue' */
bool FFBase::contains(const Residue &residue) const
{
    return this->contains(residue.molecule());
}

/** Return the copy of the molecule in this forcefield that
    has the ID == molid
    
    \throw SireMol::missing_molecule
*/
Molecule FFBase::molecule(MoleculeID molid) const
{
    throw SireMol::missing_molecule( QObject::tr(
                "There is no molecule with ID == %1 in the "
                "forcefield \"%1\"")
                    .arg(molid).arg(this->name()), CODELOC );
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue number
    'resnum'
    
    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue FFBase::residue(MoleculeID molid, ResNum resnum) const
{
    return this->molecule(molid).residue(resnum);
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue index
    'resid'
    
    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
Residue FFBase::residue(MoleculeID molid, ResID resid) const
{
    return this->molecule(molid).residue(resid);
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue 
    called 'resname'
    
    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue FFBase::residue(MoleculeID molid, const QString &resname) const
{
    return this->molecule(molid).residue(resname);
}

/** Return the copy of the molecule 'mol' that is in this forcefield 

    \throw SireMol::missing_molecule
*/
Molecule FFBase::molecule(const Molecule &mol) const
{
    return this->molecule(mol.ID());
}

/** Return the copy of the residue 'res' that is in this forcefield

    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue FFBase::residue(const Residue &res) const
{
    return this->residue(res.molecule().ID(), res.resNum());
}
