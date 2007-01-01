
#include <QRegExp>

#include "SireCAS/qhash_sirecas.h"

#include "ffcomponent.h"
#include "ffbase.h"

#include "SireError/errors.h"

using namespace SireCAS;
using namespace SireFF;

/** Regexp used to extract the name and number of the 
    forcefield from the name of the function. 
    
    Function name is;
    
    E_{Name of the forcefield[Forcefield ID number]}_{Name of component}
    
    or;
    
    E_{Name of the forcefield[Forcefield ID number]}
    
    e.g.
    
    E_{Ligand-Protein[1]}, E_{Protein-Solvent[2]}_{Coulomb},
    E_{Protein[3]}_{Bond}
    
    The below regexp will match these strings, and returns
    five groups;
    
    Group 0  == The entire string
    Group 1  == Name of the forcefield
    Group 2  == Forcefield ID number
    Group 3  == _{Name of component} (unused)
    Group 4  == Name of component
*/
QRegExp ffname_regexp("E_\\{(.+)\\[(\\d+)\\]\\}(_\\{(.+)\\}){0,1}");

/** Null constructor */
FFComponent::FFComponent() 
            : Function(), ffid(0)
{}

/** Construct an FFComponent that represents the total energy 
    of the passed forcefield */
FFComponent::FFComponent(const FFBase &ffbase)
            : Function( QString("E_{%1[%2]}").arg(ffbase.name())
                                             .arg(ffbase.ID()) ),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the component called
    'component_name' of the passed forcefield */
FFComponent::FFComponent(const FFBase &ffbase,
                         const QString &component_name)
            : Function( QString("E_{%1[%3]}_{%2}")
                            .arg(ffbase.name(), component_name)
                            .arg(ffbase.ID()) ),
              ffid(ffbase.ID())
{}

/** Assignment operator */
FFComponent& FFComponent::operator=(const SireCAS::Function &function)
{
    //need to use a copy to maintain thread-safety
    QRegExp test_match( ffname_regexp );
    
    if ( not test_match.exactMatch(function.name()) )
        throw SireError::incompatible_error( QObject::tr(
              "Cannot assign an FFComponent from the Function \"%1\" - "
              "the name has the wrong format!")
                  .arg(function.name()), CODELOC );

    //the ID number is the match 2
    ffid = test_match.cap(2).toUInt();
    
    Function::operator=(function);
    
    return *this;
}

/** Construct from a passed function - note that the name of the
    function must match the format (specified in ffname_regexp)
    or an exception will be thrown.
    
    \throw SireError::incompatible_error
*/
FFComponent::FFComponent(const SireCAS::Function &function)
            : Function(function)
{
    *this = function;
}

/** Copy constructor */
FFComponent::FFComponent(const FFComponent &other)
            : Function(other), ffid(other.ffid)
{}

/** Destructor */
FFComponent::~FFComponent()
{}

/** Copy assignment operator */
FFComponent& FFComponent::operator=(const FFComponent &other)
{
    Function::operator=(other);
    ffid = other.ffid;
    
    return *this;
}

/** Return the name of the forcefield */
QString FFComponent::forcefieldName() const
{
    QRegExp rexp(ffname_regexp);
    
    BOOST_ASSERT( rexp.exactMatch(this->name()) );
    
    //the name is the first match
    return rexp.cap(1);
}

/** Return the name of the component that this represents */
QString FFComponent::forcefieldComponent() const
{
    QRegExp rexp(ffname_regexp);
    
    BOOST_ASSERT( rexp.exactMatch(this->name()) );
    
    //the component name is the fourth match
    return rexp.cap(4);
}

/** Return the ID number of the forcefield that this represents */
quint32 FFComponent::forcefieldID() const
{
    return ffid;
}
