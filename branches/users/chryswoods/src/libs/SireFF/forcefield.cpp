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

#include "forcefield.h"
#include "ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireBase;
using namespace SireFF;

static const RegisterMetaType<ForceField> r_forcefield;

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ForceField&)
{
    writeHeader(ds, r_forcefield, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ForceField&)
{
    VersionID v = readHeader(ds, r_forcefield);

    if (v != 0)
        throw version_error(v, "1", r_forcefield, CODELOC);

    return ds;
}

namespace SireFF
{
namespace detail
{
class NullFF;
}
}

QDataStream& operator<<(QDataStream&, const SireFF::detail::NullFF&);
QDataStream& operator>>(QDataStream&, SireFF::detail::NullFF&);

namespace SireFF
{
namespace detail
{

/** This is a null forcefield, which is used to provide a null object
    to prevent segfaults when using the null ForceField class.

    @author Christopher Woods
*/
class NullFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const NullFF&);
friend QDataStream& ::operator>>(QDataStream&, NullFF&);

public:
    NullFF() : FFBase()
    {}

    NullFF(const NullFF &other) : FFBase(other)
    {}

    ~NullFF()
    {}

    class SIREFF_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters()
        {}

        ~Parameters()
        {}

        static Parameters default_parameter;
    };

    const Parameters& parameters() const
    {
        return Parameters::default_parameter;
    }

    static const char* typeName()
    {
        return "SireFF::detail::NullFF";
    }

    const char* what() const
    {
        return NullFF::typeName();
    }

    NullFF* clone() const
    {
        return new NullFF(*this);
    }

protected:
    void recalculateEnergy()
    {}

    /** Null molecule returned by the null forcefield */
    static Molecule null_molecule;
};

NullFF::Parameters NullFF::Parameters::default_parameter;

} // detail
} // SireFF

////////////
//////////// Implementation of NullFF
////////////

Q_DECLARE_METATYPE(SireFF::detail::NullFF)

using namespace SireFF::detail;

static const RegisterMetaType<NullFF> r_nullff;

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const NullFF &nullff)
{
    writeHeader(ds, r_nullff, 1)
                  << static_cast<const FFBase&>(nullff);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream &operator>>(QDataStream &ds, NullFF &nullff)
{
    VersionID v = readHeader(ds, r_nullff);

    if (v == 1)
    {
        ds >> static_cast<FFBase&>(nullff);
    }
    else
        throw version_error(v, "1", r_nullff, CODELOC);

    return ds;
}

Molecule NullFF::null_molecule;

////////////
//////////// Implementation of ForceField
////////////

static const SharedPolyPointer<FFBase> shared_null( new NullFF() );

/** Constructor */
ForceField::ForceField() : SharedPolyPointer<FFBase>(shared_null)
{}

/** Construct from the passed FFBase forcefield */
ForceField::ForceField(const FFBase &ffield)
           : SharedPolyPointer<FFBase>(ffield.clone())
{}

/** Construct from a shared pointer to a forcefield */
ForceField::ForceField(const SharedPolyPointer<FFBase> &ffptr)
           : SharedPolyPointer<FFBase>(ffptr)
{
    if (not constData())
        SharedPolyPointer<FFBase>::operator=(shared_null);
}

/** Copy constructor */
ForceField::ForceField(const ForceField &other)
           : SharedPolyPointer<FFBase>(other)
{}

/** Destructor */
ForceField::~ForceField()
{}

/** Assignment operator */
ForceField& ForceField::operator=(const ForceField &other)
{
    SharedPolyPointer<FFBase>::operator=(other);
    return *this;
}

/** Assignment operator */
ForceField& ForceField::operator=(const FFBase &other)
{
    SharedPolyPointer<FFBase>::operator=(other.clone());
    return *this;
}

/** Return the copy of the molecule in this forcefield that
    has the ID == molid

    \throw SireMol::missing_molecule
*/
Molecule ForceField::molecule(MoleculeID molid) const
{
    return d().molecule(molid);
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue number
    'resnum'

    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue ForceField::residue(MoleculeID molid, ResNum resnum) const
{
    return d().residue(molid, resnum);
}

/** Return the copy of the molecule 'mol' that is in this forcefield

    \throw SireMol::missing_molecule
*/
Molecule ForceField::molecule(const Molecule &mol) const
{
    return d().molecule(mol);
}

/** Return the copy of the residue 'res' that is in this forcefield

    \throw SireMol::missing_residue
*/
Residue ForceField::residue(const Residue &res) const
{
    return d().residue(res);
}
