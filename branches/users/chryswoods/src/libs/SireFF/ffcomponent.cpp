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

#include <QRegExp>

#include "SireCAS/qhash_sirecas.h"

#include "ffcomponent.h"
#include "ffbase.h"

#include "SireCAS/symbols.h"

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

QString getName(const FFBase &ffbase)
{
    return QString("E_{%1[%2]}").arg(ffbase.name()).arg(ffbase.ID());
}

QString getName(const FFBase &ffbase, const QString &component_name)
{
    return QString("E_{%1[%3]}_{%2}").arg(ffbase.name(), component_name)
                                     .arg(ffbase.ID());
}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield */
FFComponent::FFComponent(const FFBase &ffbase)
            : Function(getName(ffbase)),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbols &symbols)
            : Function(getName(ffbase), symbols), ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0)
            : Function(getName(ffbase),
                       sym0),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1)
            : Function(getName(ffbase),
                       sym0, sym1),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2)
            : Function(getName(ffbase),
                       sym0, sym1, sym2),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3)
            : Function(getName(ffbase),
                       sym0, sym1, sym2,
                       sym3),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4)
            : Function(getName(ffbase),
                       sym0, sym1, sym2,
                       sym3, sym4),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5)
            : Function(getName(ffbase),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6)
            : Function(getName(ffbase),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6, const Symbol &sym7)
            : Function(getName(ffbase),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6, sym7),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6, const Symbol &sym7, const Symbol &sym8)
            : Function(getName(ffbase),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6, sym7, sym8),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                         const Symbol &sym9)
            : Function(getName(ffbase),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6, sym7, sym8,
                       sym9),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the component called
    'component_name' of the passed forcefield */
FFComponent::FFComponent(const FFBase &ffbase,
                         const QString &component_name)
            : Function(getName(ffbase,component_name)),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbols &symbols)
            : Function(getName(ffbase,component_name), symbols),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the total energy
    of the passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0)
            : Function(getName(ffbase,component_name),
                       sym0),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1)
            : Function(getName(ffbase,component_name),
                       sym0, sym1),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2,
                       sym3),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2,
                       sym3, sym4),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6, const Symbol &sym7)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6, sym7),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6, const Symbol &sym7, const Symbol &sym8)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6, sym7, sym8),
              ffid(ffbase.ID())
{}

/** Construct an FFComponent that represents the energy
    of the component called 'component_name' in the
    passed forcefield based on the supplied symbols */
FFComponent::FFComponent(const FFBase &ffbase, const QString &component_name,
                         const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                         const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                         const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                         const Symbol &sym9)
            : Function(getName(ffbase,component_name),
                       sym0, sym1, sym2,
                       sym3, sym4, sym5,
                       sym6, sym7, sym8,
                       sym9),
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
QString FFComponent::forceFieldName() const
{
    QRegExp rexp(ffname_regexp);

    BOOST_ASSERT( rexp.exactMatch(this->name()) );

    //the name is the first match
    return rexp.cap(1);
}

/** Return the name of the component that this represents */
QString FFComponent::forceFieldComponent() const
{
    QRegExp rexp(ffname_regexp);

    BOOST_ASSERT( rexp.exactMatch(this->name()) );

    //the component name is the fourth match
    return rexp.cap(4);
}

/** Return the ID number of the forcefield that this represents */
ForceFieldID FFComponent::forceFieldID() const
{
    return ffid;
}
