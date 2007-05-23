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

#ifndef SIREMM_HARMONICFF_H
#define SIREMM_HARMONICFF_H

#include "SireFF/ffbase.h"
#include "SireFF/parametermap.h"

#include "SireVol/space.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class HarmonicFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::HarmonicFF&);
QDataStream& operator>>(QDataStream&, SireMM::HarmonicFF&);

namespace SireMol
{
class AtomSelection;
}

namespace SireMM
{

using SireBase::Property;
using SireBase::Version;

using SireMaths::Vector;

using SireVol::Space;

using SireMol::PartialMolecule;
using SireMol::Molecule;
using SireMol::Molecules;
using SireMol::AtomSelection;
using SireMol::MoleculeID;

using SireFF::ParameterName;
using SireFF::ParameterMap;

/** This is a forcefield that can be used to restrain a molecule to
    a single point in space, using a harmonic (spring) potential.

    This forcefield is useful as the thermodynamic equations
    of harmonic oscillators can be solved analytically, so this
    forcefield can be used to test whether free energies or
    other thermodynamic properties are being calculated correctly

    @author Christopher Woods
*/
class SIREMM_EXPORT HarmonicFF : public SireFF::FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const HarmonicFF&);
friend QDataStream& ::operator>>(QDataStream&, HarmonicFF&);

public:
    HarmonicFF();

    HarmonicFF(const Space &space);

    HarmonicFF(const HarmonicFF &other);

    ~HarmonicFF();

    class HarmonicMolecule;
    class HarmonicMoleculeData;

    class SIREMM_EXPORT Parameters : public SireFF::FFBase::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();

        const ParameterName& forceConstant() const
        {
            return _k;
        }

        const ParameterName& center() const
        {
            return _r0;
        }

        const ParameterName& k() const
        {
            return _k;
        }

        const ParameterName r0() const
        {
            return _r0;
        }

        static Parameters default_sources;

    private:
        /** The name and default source of the equilibrium
            position of the restraint (r0) */
        ParameterName _r0;

        /** The name and default source of the force constant (k)
            parameter */
        ParameterName _k;
    };

    const HarmonicFF::Parameters& parameters() const
    {
        return HarmonicFF::Parameters::default_sources;
    }

    static const char* typeName()
    {
        return "SireMM::HarmonicFF";
    }

    const char* what() const
    {
        return HarmonicFF::typeName();
    }

    HarmonicFF* clone() const
    {
        return new HarmonicFF(*this);
    }

    const Space& space() const;
    virtual bool setSpace(const Space &space);

    bool setProperty(const QString &name, const Property &value);
    Property getProperty(const QString &name) const;
    bool containsProperty(const QString &name) const;

    QHash<QString,Property> properties() const;

    void mustNowRecalculateFromScratch();

    bool change(const PartialMolecule &molecule);
    bool change(const Molecules &molecules);

    bool add(const PartialMolecule &molecule,
             const ParameterMap &map = ParameterMap());

    bool add(const Molecules &molecules,
             const ParameterMap &map = ParameterMap());

    bool remove(const PartialMolecule &molecule);
    bool remove(const Molecules &molecules);

    bool contains(const PartialMolecule &molecule) const;

    bool refersTo(MoleculeID molid) const;

    QSet<FFBase::Group> groupsReferringTo(MoleculeID molid) const;

    QSet<MoleculeID> moleculeIDs() const;

    PartialMolecule molecule(MoleculeID molid) const;

    Molecules contents() const;

protected:
    void recalculateTotalEnergy();
    void recalculateEnergy();

    void _pvt_copy(const FFBase &other);

private:
    void reindex();

    bool _pvt_change(const PartialMolecule &molecule);
    bool _pvt_add(const PartialMolecule &molecule, const ParameterMap &map);
    bool _pvt_remove(const PartialMolecule &molecule);

    /** The complete set of molecules in this forcefield */
    QVector<HarmonicMolecule> mols;

    /** Index mapping molecule ID to position in the 'mols' array */
    QHash< MoleculeID, QHash<AtomSelection,uint> > molid_to_index;

    /** The molecules which have changed since the last energy evaluation */
    QHash<MoleculeID,Molecule> changed_mols;

    /** The space in which the restraint is applied */
    Space spce;

    /** The change in energy caused by adding or removing molecules */
    double delta_nrg;

    /** Whether or not a total energy recalculation is required */
    bool need_total_recalc;
};

} // end of namespace SireMM

QDataStream& operator<<(QDataStream&, const SireMM::HarmonicFF::HarmonicMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::HarmonicFF::HarmonicMolecule&);

namespace SireMM
{

/** This class is used to represent a molecule in the harmonic forcefield */
class SIREMM_EXPORT HarmonicFF::HarmonicMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const HarmonicMolecule&);
friend QDataStream& ::operator>>(QDataStream&, HarmonicMolecule&);

public:
    HarmonicMolecule();

    HarmonicMolecule(const PartialMolecule &mol,
                     const QString &r0_property,
                     const QString &k_property);

    HarmonicMolecule(const HarmonicMolecule &other);

    ~HarmonicMolecule();

    HarmonicMolecule& operator=(const HarmonicMolecule &other);

    bool operator==(const HarmonicMolecule &other) const;
    bool operator!=(const HarmonicMolecule &other) const;

    bool isEmpty() const;
    bool isWholeMolecule() const;

    const PartialMolecule& molecule() const;

    const Version& version() const;

    const Vector& r() const;
    const Vector& r0() const;
    double k() const;

    double energy(const Space &space) const;

    void change(const PartialMolecule &molecule);

private:
    /** Implicitly shared pointer to the implementation of this class */
    QSharedDataPointer<HarmonicMoleculeData> d;
};

}

Q_DECLARE_METATYPE(SireMM::HarmonicFF);
Q_DECLARE_METATYPE(SireMM::HarmonicFF::HarmonicMolecule);

SIRE_END_HEADER

#endif
