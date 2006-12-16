
#include "SireCAS/qhash_sirecas.h"

#include "ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireFF/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include <QTime>

using namespace SireFF;
using namespace SireFF::detail;
using namespace SireCAS;
using namespace SireMol;
using namespace SireDB;
using namespace SireStream;

///////////
/////////// Implementation of detail::ComponentInfo
///////////

static const RegisterMetaType<ComponentInfo> r_compinfo("SireFF::detail::ComponentInfo");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ComponentInfo &compinfo)
{
    writeHeader(ds, r_compinfo, 1) << compinfo.nme << compinfo.desc
                                   << compinfo.func;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ComponentInfo &compinfo)
{
    VersionID v = readHeader(ds, r_compinfo);

    if (v == 1)
    {
        ds >> compinfo.nme >> compinfo.desc >> compinfo.func;
    }
    else
        throw version_error(v, "1", r_compinfo, CODELOC);

    return ds;
}

/** Null constructor */
ComponentInfo::ComponentInfo()
{}

/** Construct a component with root 'root', name 'name' and description
    'description'. The function will have the form: E_{root}_{name}(x,y,z) */
ComponentInfo::ComponentInfo(const QString &root, const QString &name,
                             const QString &description)
              : nme(name), desc(description)
{
    this->setRoot(root);
}

/** Copy constructor */
ComponentInfo::ComponentInfo(const ComponentInfo &other)
              : nme(other.nme), desc(other.desc), func(other.func)
{}

/** Destructor */
ComponentInfo::~ComponentInfo()
{}

/** Set the root of the symbol - the function has the form; E_{root}_{name}(x,y,z) */
void ComponentInfo::setRoot(const QString &root)
{
    func = Function( QString("E_{%1}_{%2}").arg(root,name()) )( Symbol("x"),
                                                                Symbol("y"),
                                                                Symbol("z") );
}

///////////
/////////// Implementation of FFBase
///////////

static const RegisterMetaType<FFBase> r_ffbase("SireFF::FFBase", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const FFBase &ffbase)
{
    writeHeader(ds, r_ffbase, 1)
          << ffbase.ffname << ffbase.id_to_component
          << ffbase.nrg_components
          << ffbase.mols_in_ff // << ffbase.res_in_ff
          << ffbase.isdirty;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, FFBase &ffbase)
{
    VersionID v = readHeader(ds, r_ffbase);

    if (v == 1)
    {
        ds >> ffbase.ffname >> ffbase.id_to_component
           >> ffbase.nrg_components
           >> ffbase.mols_in_ff // >> ffbase.res_in_ff
           >> ffbase.isdirty;
    }
    else
        throw version_error(v, "1", r_ffbase, CODELOC);

    return ds;
}

/** Null constructor */
FFBase::FFBase() : QSharedData(), ffname(QObject::tr("Unnamed")), isdirty(true)
{
    this->registerComponents();
}

/** Construct a forcefield called 'name' */
FFBase::FFBase(const QString &name)
       : QSharedData(), ffname(name), isdirty(true)
{
    this->registerComponents();
}

/** Copy constructor */
FFBase::FFBase(const FFBase &other)
       : QSharedData(), ffname(other.ffname), id_to_component(other.id_to_component),
         nrg_components(other.nrg_components),
         mols_in_ff(other.mols_in_ff), //res_in_ff(other.res_in_ff),
         isdirty(other.isdirty)
{}

/** Destructor */
FFBase::~FFBase()
{}

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

/** Private function used to register the components of this forcefield */
void FFBase::registerComponents()
{
    //register the function representing the total energy
    //of this forcefield
    this->registerComponent( FFBase::TOTAL(), "",
                             QObject::tr("The total energy of the forcefield.") );
}

/** Return the name of this forcefield - this is used as the root of all of
    the components of the forcefield, e.g. if the name was "Ligand CLJ", then
    the "coul" component would have the function: E_{Ligand CLJ}_{coul}(x,y,z) */
const QString& FFBase::name() const
{
    return ffname;
}

/** Return the function represented by the component with ID 'componentid' */
const Function& FFBase::component(int componentid) const
{
    const QHash<int,detail::ComponentInfo>::const_iterator it
                                      = id_to_component.find(componentid);

    if (it == id_to_component.end())
        throw SireFF::missing_component( QObject::tr(
                "The forcefield \"%1\" does not contain a component with "
                "ID == %2").arg(name()).arg(componentid), CODELOC );

    return it.value().function();
}

/** Return a list of all of the components of this forcefield */
QList<Function> FFBase::components() const
{
    QList<Function> funcs;

    for (QHash<int,detail::ComponentInfo>::const_iterator it = id_to_component.begin();
         it != id_to_component.end();
         ++it)
    {
        funcs.append( it->function() );
    }

    return funcs;
}

/** Return a string representation of all of the components in this
    forcefield */
QString toString(const QList<Function> &components)
{
    QStringList compstrings;

    for (QList<Function>::const_iterator it = components.begin();
         it != components.end();
         ++it)
    {
        compstrings.append( it->toString() );
    }

    return compstrings.join(", ");
}

/** Assert that this forcefield contains the component 'component'

    \throw SireFF::missing_component
*/
void FFBase::assertContains(const Function &component) const
{
    if (not nrg_components.values().contains(component.ID()))
        throw SireFF::missing_component( QObject::tr(
              "The forcefield \"%1\" does not contain a component represented "
              "by the function \"%2\" (available components = %3)")
                  .arg(name()).arg(component.toString())
                  .arg(toString(components())), CODELOC );

}

/** Return the energy of the component represented by the function
    'component'

    \throw SireFF::missing_component
*/
double FFBase::energy(const Function &component)
{
    this->assertContains(component);

    if (isDirty())
    {
        this->recalculateEnergy();
        this->setClean();
    }

    return nrg_components.value(component);
}

/** Return the function representing the total energy */
const Function& FFBase::total() const
{
    return component(FFBase::TOTAL());
}

/** Return the total energy of this forcefield. This will return the cached
    value if nothing has changed, or will recalculate if something has changed. */
double FFBase::energy()
{
    return energy( total() );
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

/** Assert that this forcefield contains the molecule 'molecule'

    \throw SireMol::missing_molecule
*/
void FFBase::assertContains(const Molecule &molecule) const
{
    if ( not mols_in_ff.contains(molecule.ID()) )
        throw SireMol::missing_molecule( QObject::tr(
            "The molecule \"%1\" (%2) is not in the forcefield \"%3\"")
                .arg(molecule.name()).arg(molecule.ID())
                .arg(name()), CODELOC );
}

/** Assert that this forcefield contains the residue 'residue'

    \throw SireMol::missing_residue
*/
void FFBase::assertContains(const Residue &residue) const
{
    Molecule mol = residue.molecule();

    if ( not mols_in_ff.contains(mol.ID()) )
            // of res_in_ff.contains(residue.ID())
        throw SireMol::missing_residue( QObject::tr(
            "The residue \"%1 (%2)\" in the molecule \"%3\" (%4) "
            "is not in the forcefield \"%5\"")
                .arg(residue.name()).arg(residue.number())
                .arg(mol.name()).arg(mol.ID())
                .arg(name()), CODELOC );
}

/** Assert that the molecule stored in this forcefield has the same major
    version as 'mol'

    \throw SireMol::missing_molecule
    \throw SireStream::version_error
*/
void FFBase::assertSameMajorVersion(const Molecule &mol) const
{
    const MoleculeVersion &version = this->molecule(mol.ID()).version();

    if (mol.version().major() != version.major())
        throw SireStream::version_error( QObject::tr(
              "The major version of the molecule \"%1\" (%2) in the forcefield "
              "\"%3\" (%4) is not equal to %5.")
                  .arg(mol.name()).arg(mol.ID()).arg(name())
                  .arg(version.major()).arg(mol.version().major()), CODELOC );
}

/** Assert that the residue stored in this forcefield has the same major
    version as 'res'

    \throw SireMol::missing_residue
    \throw SireStream::version_error
*/
void FFBase::assertSameMajorVersion(const Residue &residue) const
{
    #warning TODO assertSameMajorVersion(Residue)
}

/** Assert that the molecule stored in this forcefield has the same version
    as 'mol'

    \throw SireMol::missing_molecule
    \throw SireStream::version_error
*/
void FFBase::assertSameVersion(const Molecule &mol) const
{
    const MoleculeVersion &version = this->molecule(mol.ID()).version();

    if (mol.version() != version)
        throw SireStream::version_error( QObject::tr(
              "The version of the molecule \"%1\" (%2) in the forcefield "
              "\"%3\" (%4) is not equal to %5.")
                  .arg(mol.name()).arg(mol.ID()).arg(name())
                  .arg(version.toString()).arg(mol.version().toString()), CODELOC );
}

/** Assert that the residue stored in this forcefield has the same
    version as 'res'

    \throw SireMol::missing_residue
    \throw SireStream::version_error
*/
void FFBase::assertSameVersion(const Residue &res) const
{
    #warning TODO assertSameVersion(Residue)
}

/** Register the forcefield component with ID number 'id', name 'compname'
    and with description 'description'. It is a program bug to try
    to register a component with the same name or ID as an existing
    component.

    \throw SireError::program_bug
*/
void FFBase::registerComponent(int id, const QString &compname,
                               const QString &description)
{
    if ( id_to_component.contains(id) )
        throw SireError::program_bug( QObject::tr(
            "Cannot have two energy components with the same ID number (%1). "
            "Existing component is \"%2\" (%3) - cannot replace with "
            "\"%4\".")
                .arg(id).arg(id_to_component[id].name())
                .arg(id_to_component[id].function().toString())
                .arg(compname), CODELOC );

    ComponentInfo compinfo(name(), compname, description);

    if ( nrg_components.values().contains(compinfo.function().ID()) )
        throw SireError::program_bug( QObject::tr(
            "Cannot have two components in this forcefield with the same "
            "name (%1). Existing component is \"%2\".")
                .arg(compname).arg(compinfo.function().toString()), CODELOC );

    nrg_components.add( compinfo.function() == 0.0 );
    id_to_component.insert( id, compinfo );
}

/** Register the addition of the molecule 'molecule' */
void FFBase::addToRegister(const Molecule &molecule)
{
    mols_in_ff.insert(molecule.ID());
}

/** Register the addition of the residue 'residue' */
void FFBase::addToRegister(const Residue &residue)
{
    #warning TODO addToRegister(Residue)
}
