
#include "SireCAS/qhash_sirecas.h"

#include "ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireFF/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireFF;
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

/** Construct using the supplied base name */
FFBase::Components::Components(const QString &basename)
{
    FFBase::Components::setBaseName(basename);
}

/** Copy constructor */
FFBase::Components::Components(const FFBase::Components &other)
       : e_total(other.e_total), symbolids(other.symbolids)
{}

/** Destructor */
FFBase::Components::~Components()
{}

/** Describe the 'total' component */
QString FFBase::Components::describe_total()
{
    return QObject::tr("The total energy of the forcefield.");
}

/** Get the function from the root name and the
    component name */
Function FFBase::Components::getFunction(const QString &root,
                                       const QString &component)
{
    return Function( QString("E_{%1}_{%2}").arg(root,component) )( Symbol("x"),
                                                                   Symbol("y"),
                                                                   Symbol("z") );
}

/** Set the names of the functions from the passed base name */
void FFBase::Components::setBaseName(const QString &basename)
{
    //unregister the old function
    this->unregisterFunction(e_total);

    e_total = getFunction(basename,"total");

    //register the new function
    this->registerFunction(e_total);
}

/** Remove 'function' from the registry */
void FFBase::Components::unregisterFunction(const Function &function)
{
    symbolids.remove( function.ID() );
}

/** Register the function 'function' */
void FFBase::Components::registerFunction(const Function &function)
{
    symbolids.insert( function.ID() );
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
           >> ffbase.nrg_components
           >> ffbase.isdirty;

        //tell the forcefield about the change in name
        ffbase.setName(newname);
    }
    else
        throw version_error(v, "1", r_ffbase, CODELOC);

    return ds;
}

/** Null constructor */
FFBase::FFBase() : QSharedData(), ffname(QObject::tr("Unnamed")), isdirty(true)
{}

/** Construct a forcefield called 'name' */
FFBase::FFBase(const QString &name)
       : QSharedData(), ffname(name), isdirty(true)
{}

/** Copy constructor */
FFBase::FFBase(const FFBase &other)
       : QSharedData(),
         ffname(other.ffname),
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
    components_ptr->setBaseName(ffname);
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
